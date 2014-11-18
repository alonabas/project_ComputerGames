#version 330

// Based on http://glslsandbox.com/e#19869.0


uniform sampler2D textureData;
uniform sampler2D textureDataDepth;
uniform sampler2D textureDataSky;
uniform sampler2D textureDataNormal;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPosition;
uniform vec3 cameraDir;
uniform float time;
uniform vec2 resolution;

const float specExp = 100.0f;

in vec2 tex_coord;
in vec3 vert;

out vec4 outColor;


mat3 m = mat3( 0.00,  0.80,  0.60,
              -0.80,  0.36, -0.48,
              -0.60, -0.48,  0.64 );
float hash( float n )
{
    return fract(sin(n)*43758.5453);
}

float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
    
    f = f*f*(3.0-2.0*f);
    
    float n = p.x + p.y*57.0 + 113.0*p.z;
    
    float res = mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                        mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y),
                    mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                        mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
    return res;
}

float fbm( vec3 p )
{
    float f;
    f  = 0.5000*noise( p ); p = m*p*2.02;
    f += 0.2500*noise( p ); p = m*p*2.03;
    f += 0.1250*noise( p );
    return f;
}


//-----------------------------------------------------------------------------
// Main functions
//-----------------------------------------------------------------------------
float scene(vec3 p)
{
	return .1-length(p)*.04+fbm(p*.3);
}

vec4 getSky()
{
    vec2 v = 1.0f - tex_coord*2.0f;
    
	vec2 mo = vec2(time*.1,cos(time*.25)*3.);
    // camera by iq
    vec3 org = 25.0*normalize(vec3(cos(2.75-3.0*mo.x), 0.7-1.0*(mo.y-1.0), sin(2.75-3.0*mo.x)));
	vec3 ta = vec3(0.0, 1.0, 0.0);
    vec3 ww = normalize( ta - org);
    vec3 uu = normalize(cross( vec3(0.0,1.0,0.0), ww ));
    vec3 vv = normalize(cross(ww,uu));
    vec3 dir = normalize( v.x*uu + v.y*vv + 1.5*ww );
	vec4 color=vec4(0.05,0.05,0.15,1.0f);
	
	const int nbSample = 6;
	const int nbSampleLight = 3;
	
	float zMax         = 40.;
	float step         = zMax/float(nbSample);
	float zMaxl         = 20.;
	float stepl         = zMaxl/float(nbSampleLight);
    vec3 p             = org;
    float T            = 1.;
    float absorption   = 100.;
	vec3 sun_direction = normalize( vec3(1.,.0,.0) );
    
	for(int i=0; i<nbSample; i++)
	{
		float density = scene(p);
		if(density>0.)
		{
			float tmp = density / float(nbSample);
			T *= 1. -tmp * absorption;
			if( T <= 0.01)
				break;
            
            
            //Light scattering
			float Tl = 1.0;
			for(int j=0; j<nbSampleLight; j++)
			{
				float densityLight = scene( p + normalize(sun_direction)*float(j)*stepl);
				if(densityLight>0.)
                	Tl *= 1. - densityLight * absorption/float(nbSample);
                if (Tl <= 0.01)
                    break;
			}
			
			//Add ambiant + light scattering color
			color += vec4(0.5,0.5,0.9,1.0)*50.*tmp*T +  vec4(1.,.7,.4,1.)*80.*tmp*T*Tl;
		}
		p += dir*step;
	}
    
    return color;
    
}
float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    vec3 vertex = vec3(1.0f - 2.0f*tex_coord, -3.0f);
    float z_comp = texture(textureDataDepth, tex_coord).x;
    vertex -= z_comp * cameraDir;
    vertex.x = vertex.x * 7.0f;
    
    vec3 total_light = vec3(0.0f);
    vec3 eye_dir = normalize(cameraPosition - vertex);
    total_light = texture(textureData, tex_coord).xyz * 0.6;
    vec3 light_dir = normalize(lightPos - vertex);
//    outColor = vec4(light_dir,1.0f);
//    return;

    vec3 normal = normalize(texture(textureDataNormal, tex_coord).xyz);
    vec3 kd = texture(textureData, tex_coord).xyz * 0.8;
    vec3 ks = texture(textureData, tex_coord).xyz * 0.8;
    
    // get diffuse light
    float cos_theta = dot(light_dir, normal);
	cos_theta = max(cos_theta,0.0);
    total_light += kd * lightColor * cos_theta;
    
    // get specular light
    float alpha = dot(reflect(-light_dir, normal), eye_dir);
    alpha = max(alpha,0.0);
    total_light += ks * lightColor * min(pow(alpha,specExp),1.0);
    
    vec4 scene_light = vec4(total_light,1.0f);
    // outColor = scene_light;
    vec4 sky_light = getSky();
    outColor = scene_light * (1.0f - texture(textureDataSky, tex_coord)) + sky_light*texture(textureDataSky, tex_coord);
}