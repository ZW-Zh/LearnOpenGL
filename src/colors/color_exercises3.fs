#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
//摄像机观察位置 
//uniform vec3 viewPos;

void main()
{   
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
    FragColor = vec4(result, 1.0);
}