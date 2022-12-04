#version 330 core
out vec4 fragColor;
void main(void){
    fragColor = vec4(gl_FragCoord.z/gl_FragCoord.w);
}