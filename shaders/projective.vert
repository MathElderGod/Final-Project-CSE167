# version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 tex_coords;

uniform mat4 modelview;
uniform mat4 projection;

out vec4 position;
out vec3 normal;
out vec2 coords;

void main(){
    gl_Position = projection * modelview * vec4( vertex_position, 1.0f );
    // forward the raw position and normal in the model coord to frag shader
    position = vec4(vertex_position, 1.0f );
    normal = vertex_normal;
    coords = tex_coords;
}
