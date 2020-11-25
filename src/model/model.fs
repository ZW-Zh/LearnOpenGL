#version 330 core

//加入漫反射贴图
struct Material {
    //移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色
    //sampler2D不透明类型(Opaque Type)，也就是说我们不能将它实例化，只能通过uniform来定义它
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float     shininess;
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
//纹理坐标，通过纹理坐标对应漫反射贴图
in vec2 TexCoords;

//摄像机观察位置 
uniform vec3 viewPos;

void main()
{   
    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));

    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //取样对应的颜色值并将它乘以光源的镜面强度。一个像素越「白」，乘积就会越大，物体的镜面光分量就会越亮
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));  

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}