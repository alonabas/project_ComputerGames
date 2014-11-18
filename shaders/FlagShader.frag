#version 330

uniform vec3 light_position[3];
uniform vec3 light_color[3];
uniform sampler2D textureData;
uniform int type;
const vec3 eye_position = vec3(0.0f);

vec3 my_kd;
vec3 my_ks;
float my_shinisess;

in vec3 ambient_light;
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
    colorLight = my_kd * light_color[i] * cos_theta;
    
    // get specular light
    float alpha = dot(reflect(-light_dir, normal), eye_dir);
    alpha = max(alpha,0.0);
    colorLight += my_ks * light_color[i] * min(pow(alpha,my_shinisess),1.0);
    return colorLight;
}


void main()
{
    vec3 total_light;
    total_light = ambient_light;
    if (type == 0){
        my_kd = texture(textureData,tex_coord).xyz;
        my_ks = vec3(0.1, 0.1, 0.1);
        my_shinisess = 20.0f;
        for(int i = 0; i<3; i++){
            total_light += getLight(i);
        }
        outColor = vec4(total_light,1.0f);
    }
    else{
        my_kd = vec3(0.4,0.4,0.4);
        my_ks = vec3(0.774597,0.774597,0.774597);
        my_shinisess = 76.8;
        for(int i = 0; i<3; i++){
            total_light += getLight(i);
        }
        outColor = vec4(total_light,1.0f);
    }

}
