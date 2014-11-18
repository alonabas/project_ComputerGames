#version 330

const vec3 eye_position = vec3(0.0f);
const vec3 ambient_light = vec3(0.24725,0.1995,0.0745);
const vec3 kd = vec3(0.85164,0.60648,0.22648); // Diffuse coefficient
const vec3 ks = vec3(0.628281,0.555802,0.366065); // Specular coefficient
const float shinisess = 51.2; // Shininess (can be changed dynamically by the user)

uniform vec3 light_position[3];
uniform vec3 light_color[3];
uniform sampler2D map;

in vec2 vertex;

out vec4 outColor;

vec3 getLight(int i){
    vec3 vertTemp = vec3(vertex/5.0f, -2.0f);
    vec3 colorLight;
    // normalize vectors
    vec3 light_dir = normalize(light_position[i] - vertTemp);
    vec3 normal = texture(map,vertex).xyz;
    normal.z = normal.z / 5.0f;
    normal = normalize(normal);

    vec3 eye_dir = normalize(eye_position - vertTemp);
    
    // get diffuse light
    float cos_theta = dot(light_dir, normal);
	cos_theta = max(cos_theta,0.0);
    colorLight = kd * light_color[i] * cos_theta;
    
    // get specular light
    float alpha = dot(reflect(-light_dir, normal), eye_dir);
    alpha = max(alpha,0.0);
    colorLight += ks * light_color[i] * min(pow(alpha,shinisess),1.0);
    return colorLight;
    
}

void main()
{
    vec3 total_light = ambient_light;
    for(int i = 0; i < 3 ;i++){
        total_light += getLight(i);
    }
    outColor = vec4(total_light,texture(map,vertex).w);
    
}