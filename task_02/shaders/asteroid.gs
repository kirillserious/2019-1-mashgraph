#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 FragPos;
} gs_in[];

out vec2 TexCoords;
out vec3 normal; 
out vec3 FragPos;

// Время с момента взрыва. Если 0 и меньше, то и взрыва нет.

uniform float explosion_time;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * (((explosion_time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
}

void main() {    
    if (explosion_time > 0) {
        vec3 normal = gs_in[1].normal;

        gl_Position = explode(gl_in[0].gl_Position, gs_in[1].normal);
        TexCoords = gs_in[0].texCoords;
        FragPos = gs_in[0].FragPos;
        normal = gs_in[0].normal;
        EmitVertex();
        gl_Position = explode(gl_in[1].gl_Position, gs_in[1].normal);
        TexCoords = gs_in[1].texCoords;
        FragPos = gs_in[1].FragPos;
        normal = gs_in[1].normal;
        EmitVertex();
        gl_Position = explode(gl_in[2].gl_Position, gs_in[1].normal);
        TexCoords = gs_in[2].texCoords;
        FragPos = gs_in[2].FragPos;
        normal = gs_in[2].normal;
        EmitVertex();
        EndPrimitive();

    } else {
        gl_Position = gl_in[0].gl_Position;
        TexCoords = gs_in[0].texCoords;
        FragPos = gs_in[0].FragPos;
        normal = gs_in[0].normal;
        EmitVertex();
        gl_Position = gl_in[1].gl_Position;
        TexCoords = gs_in[1].texCoords;
        FragPos = gs_in[1].FragPos;
        normal = gs_in[1].normal;
        EmitVertex();
        gl_Position = gl_in[2].gl_Position;
        TexCoords = gs_in[2].texCoords;
        FragPos = gs_in[2].FragPos;
        normal = gs_in[2].normal;
        EmitVertex();
        EndPrimitive();
    }
}