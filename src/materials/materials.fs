#version 330 core

struct Material {
    //环境光照
    vec3 ambient;
    //漫反射
    vec3 diffuse;
    //镜面光照
    vec3 specular;
    //反光度
    float shininess;
}; 
//一个光源对它的ambient、diffuse和specular光照有着不同的强度
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;


uniform Material material;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;


//摄像机观察位置 
uniform vec3 viewPos;

void main()
{   
    // 环境光
    vec3 ambient = light.ambient * material.ambient;

    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}