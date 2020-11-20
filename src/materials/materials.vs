#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //片段位置，顶点位置属性乘以模型矩阵变换到世界空间坐标
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal=aNormal;
    //法线也需要转换到世界坐标，但不能进行位移，同时不等比缩放会造成法线不垂直平面
    //解决方法是专门定制一个法线矩阵，为模型矩阵左上角的逆矩阵的转置矩阵
    //逆矩阵对着色器来说开销很大，最好在cpu中计算出来，通过uniform传值。
    Normal = mat3(transpose(inverse(model))) * aNormal;
}