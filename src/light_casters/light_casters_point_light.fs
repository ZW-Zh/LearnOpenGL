#version 330 core

//加入漫反射贴图
struct Material {
    //移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色
    //sampler2D不透明类型(Opaque Type)，也就是说我们不能将它实例化，只能通过uniform来定义它
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
}; 
//一个光源对它的ambient、diffuse和specular光照有着不同的强度
struct Light {
    vec3 position; 
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //公式中的系数
    float constant;
    float linear;
    float quadratic;
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
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float distance = length(light.position - FragPos);
    //衰减公式
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    //一个从片段至光源的光线方向
    //vec3 lightDir = normalize(-light.direction);
    //我们一直将光的位置和位置向量定义为vec3，但一些人会喜欢将所有的向量都定义为vec4。当我们将位置向量定义为一个vec4时，很重要的一点是要将w分量设置为1.0，这样变换和投影才能正确应用。然而，当我们定义一个方向向量为vec4的时候，我们不想让位移有任何的效果（因为它仅仅代表的是方向），所以我们将w分量设置为0.0。
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //取样对应的颜色值并将它乘以光源的镜面强度。一个像素越「白」，乘积就会越大，物体的镜面光分量就会越亮
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));  

    ambient *= attenuation; 
    diffuse *= attenuation;
    specular *= attenuation;
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}