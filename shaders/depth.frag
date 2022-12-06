#version 330 core

out vec4 fragColor;


void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
    fragColor = vec4(vec3(gl_FragCoord.z/gl_FragCoord.w) * 0.1f, 1.0f);

}  


/*#version 330 core

out vec4 fragColor;

void main(void){
    fragColor = vec4(vec3(gl_FragCoord.z/gl_FragCoord.w) * 0.1f, 1.0f);
}*/