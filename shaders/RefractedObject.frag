#version 330

const vec3 ambientLight = vec3(0.1, 0.1, 0.4);
const vec3 ka = vec3(0.1f);
const vec3 kd = vec3(0.6, 0.6, 1.0f);
const vec3 ks = vec3(0.7, 0.7, 1.0f);
const float shinisess = 500.0f;
const float refractiveIndex = 1.5;

uniform vec3 light_position[3];
uniform vec3 light_color[3];
uniform samplerCube textureData;

in vec3 normal_dir;
in vec3 eye_dir;
in vec3 vertex;
in vec3 v_reflection;
in vec3 v_refraction;
in float v_fresnel;

out vec4 outColor;


vec3 getLight(int i, vec3 my_kd){
    
    vec3 colorLight;
    vec3 light_dir = normalize(light_position[i] - vertex);
    
    // get diffuse light
    float cos_theta = dot(light_dir, normal_dir);
	cos_theta = max(cos_theta,0.0);
    colorLight = my_kd * light_color[i] * cos_theta;
    
    // get specular light
    float alpha = dot(reflect(-light_dir, normal_dir), eye_dir);
    alpha = max(alpha,0.0);
    colorLight += ks * light_color[i] * min(pow(alpha,shinisess),1.0);
    return colorLight;
    
}

void main()
{
    vec3 refractionColor = vec3(texture(textureData, normalize(v_refraction)));
    vec3 reflectionColor = vec3(texture(textureData, normalize(v_reflection)));
    vec3 my_kd = mix(refractionColor, reflectionColor, v_fresnel);
    my_kd.b = my_kd.b * 1.5f;
    vec3 total_light = vec3(0.0f);
    for(int i = 0; i<3; i++){
        total_light += getLight(i, my_kd);
    }
    outColor = vec4(total_light,1.0f);
}
