#version 330

in vec2 UV;
uniform float size_x;
uniform float size_y;
uniform float time_animation;
const mat3 YUV2RGB = mat3(0.299f,0.587f,0.114f,-0.14713f,-0.28886f, 0.436f, 0.615f, -0.51499f, -0.10001);

out vec3 color;

uniform sampler2D fbo_texture;
uniform sampler2D depth_texture;

vec3 average(vec2 textCoord, int size){ // basic smooth
    vec3 filteredColor = vec3(0.0f);
    // convolution
    int rc = size/2;
    for(int i = 0; i<size; i++){
        for(int j=0; j<size; j++){
            float x = float(i-rc)/size_x;
            float y = float(j-rc)/size_y;
            filteredColor += texture(fbo_texture, vec2(textCoord.x + x, textCoord.y + y)).xyz ;
        }
    }
    return filteredColor / pow(float(size),2.0);
    
}

vec3 addGlow(vec2 textCoord,int kernelSize){
    vec3 col;
    vec3 YUV = YUV2RGB * average(textCoord,kernelSize); // look at neighbours of given pixel to add glow, this make glow more sparse
    if(YUV.x > 0.2f){ //add glow
        col = texture(fbo_texture, textCoord).xyz;
        col += average(textCoord,5);
    }
    else {
        col = texture(fbo_texture, textCoord).xyz;
    }
    col = texture(fbo_texture, textCoord).xyz;
    col += average(textCoord,5);

    return col;
}

vec3 addBasicSharpen(vec2 textCoord){
    vec3 filteredColor = vec3(0.0f);
    filteredColor = 5.0 * texture(fbo_texture, vec2(textCoord.x, textCoord.y)).xyz;
    filteredColor -= texture(fbo_texture, vec2(textCoord.x - 1.0f/size_x, textCoord.y)).xyz;
    filteredColor -= texture(fbo_texture, vec2(textCoord.x + 1.0f/size_x, textCoord.y)).xyz;
    filteredColor -= texture(fbo_texture, vec2(textCoord.x, textCoord.y + 1.0f/size_y)).xyz;
    filteredColor -= texture(fbo_texture, vec2(textCoord.x, textCoord.y - 1.0f/size_y)).xyz;
    return filteredColor;
}

vec3 gammaCorrection(vec3 col){
    
    color.x = pow(col.x,0.5f);
    color.y = pow(col.y,0.5f);
    color.z = pow(col.z,0.5f);
    return color;
}

void main(){
    vec3 colBlur = addGlow(UV, 9);
    vec3 colSharpen = addBasicSharpen(UV);
    color =mix(colBlur,colSharpen, 0.5);
    color = mix(colBlur,colSharpen,0.3f);
    color = colBlur;
}