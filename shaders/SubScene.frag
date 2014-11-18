#version 330

uniform sampler2D textureData;
uniform vec3 light_position;
uniform vec3 light_color;
const vec3 eye_position = vec3(0.0f);

in vec2 tex_coord;
in vec3 vertex;

out vec4 outColor;

void main()
{
    vec3 total_light = vec3(0.0f);
    vec3 eye_dir = eye_position - vertex;
    total_light = texture(textureData, tex_coord).xyz * 0.4;
    
    vec3 light_dir = normalize(light_position - vertex);
    vec3 normal = vec3(0.0f,0.0f,1.0f);
    vec3 kd = texture(textureData, tex_coord).xyz * 0.6;
    vec3 ks = texture(textureData, tex_coord).xyz * 0.5;
    float specExp = 100.0f;
    
    // get diffuse light
    float cos_theta = dot(light_dir, normal);
	cos_theta = max(cos_theta,0.0);
    total_light += kd * light_color * cos_theta;
    
    // get specular light
    float alpha = dot(reflect(-light_dir, normal), eye_dir);
    alpha = max(alpha,0.0);
    total_light += ks * light_color * min(pow(alpha,specExp),1.0);
    outColor = vec4(total_light,texture(textureData, tex_coord).w);


}
