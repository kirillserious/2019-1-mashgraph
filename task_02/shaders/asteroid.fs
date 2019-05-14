#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;

vec3 lightColor = vec3(1.0, 1.0,  1.0);
vec3 lightPos   = vec3(1.0, 1.0, -3.0);
vec3 viewPos    = vec3(0.0, 0.0,  0.0);

uniform float explosion_time;

void main()
{

        if (explosion_time <= 0) {

        // Ambient
        float ambientStrength = 0.1f;
        vec3 ambient = ambientStrength * lightColor;
        
        // Diffuse 
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
    
        // Specular
        float specularStrength = 0.5f;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;  
        
        vec4 result = vec4((diffuse + ambient + specular), 1.0) * texture(texture_diffuse1, TexCoords);

        FragColor = result;
        }

        else {
                FragColor = texture(texture_diffuse1, TexCoords);
        }
}

