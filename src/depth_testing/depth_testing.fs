#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{    
    FragColor = texture(texture1, TexCoords);
}
// float near = 0.1; 
// float far  = 100.0; 

// float LinearizeDepth(float depth) 
// {
//     float z = depth * 2.0 - 1.0; // back to NDC 变换到-1到1
//     return (2.0 * near * far) / (far + near - z * (far - near));    
// }

// void main()
// {             
//     float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
//     FragColor = vec4(vec3(depth), 1.0);
// }
//防止深度冲突
//1.在两个物体之间设置一个用户无法注意到的偏移值
//2.尽可能将近平面设置远一些
//3.使用更高精度的深度缓冲