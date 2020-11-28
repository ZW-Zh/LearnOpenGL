#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

// void main()
// {    
//     FragColor = texture(texture1, TexCoords);
// }
float near = 0.1; 
float far  = 100.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 变换到-1到1
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main()
{             
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
    FragColor = vec4(vec3(depth), 1.0);
}