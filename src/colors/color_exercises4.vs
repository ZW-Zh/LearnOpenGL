#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

uniform vec3 objectColor;
uniform vec3 lightColor;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;
out vec4 fragColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //片段位置，顶点位置属性乘以模型矩阵变换到世界空间坐标
    FragPos = vec3(view * vec4(aPos, 1.0));
    //Normal=aNormal;
    //法线也需要转换到世界坐标，但不能进行位移，同时不等比缩放会造成法线不垂直平面
    //解决方法是专门定制一个法线矩阵，为模型矩阵左上角的逆矩阵的转置矩阵
    //逆矩阵对着色器来说开销很大，最好在cpu中计算出来，通过uniform传值。
    
    //先变换到世界在变换到观察
    Normal = mat3(transpose(inverse(view*model))) * aNormal;
    //光照原来在世界坐标也要转化到观察
    LightPos = vec3(view * vec4(lightPos, 1.0));
    

    //环境光照
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    //乘一个很小的环境常量因子
    // vec3 result = ambient * objectColor;
    // FragColor = vec4(result, 1.0);
    //FragColor = vec4(lightColor * objectColor, 1.0);
    //法线向量
    vec3 norm = normalize(Normal);
    //光的方向向量=光源位置向量-片段位置向量
    vec3 lightDir = normalize(LightPos - FragPos);
    //得到法向量和光向量夹角，防止夹角大于90变负数。
    float diff = max(dot(norm, lightDir), 0.0);
    //得到漫反射分量，夹角越大，漫反射分量越小
    vec3 diffuse = diff * lightColor;
    //环境光分量加漫反射分量
    // vec3 result = (ambient + diffuse) * objectColor;
    // FragColor = vec4(result, 1.0);
    //镜面强度
    float specularStrength = 0.5; 
    //视线方向向量  
    vec3 viewDir = normalize(- FragPos);
    //reflect函数要求第一个向量是从光源指向片段位置的向量
    //reflect求反射向量
    vec3 reflectDir = reflect(-lightDir, norm);
    //计算镜面分量
    //取32次幂，为高光的反光度，反光度越高，反射光的能力越强，散射得越少，高光点就会越小。

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    fragColor = vec4(result, 1.0);
}