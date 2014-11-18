#version 330

#define MY_PI 3.1415926

const vec3 ambient_light = vec3(0.19225,0.19225,0.19225);
const vec3 kd = vec3(0.50754f);
const vec3 ks = vec3(0.508273);
const float shinisess = 51.2f;
const vec3 eye_position = vec3(0.0f);

uniform vec3 light_position[3];
uniform vec3 light_color[3];

in vec3 normal_vertex;
in vec3 vertex;
in vec2 tex_coord;

out vec4 outColor;

vec3 getLight(int i){
    
    vec3 colorLight;
    // normalize vectors
    vec3 light_dir = normalize(light_position[i] - vertex);
    vec3 eye_dir = normalize(eye_position - vertex);
    vec3 normal = normalize(normal_vertex);
    
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
    for(int i = 0; i<3; i++){
        total_light += getLight(i);
    }
    outColor = vec4(total_light,1.0f);
}
