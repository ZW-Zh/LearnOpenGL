#version 330 core

//加入漫反射贴图
struct Material {
    //移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色
    //sampler2D不透明类型(Opaque Type)，也就是说我们不能将它实例化，只能通过uniform来定义它
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
//一个光源对它的ambient、diffuse和specular光照有着不同的强度
struct Light {
    //聚光位置
    vec3 position;
    //聚光方向 
    vec3 direction;
    //切光角
    float cutOff;
    //外切光角
    float outerCutOff;
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
     vec3 lightDir = normalize(light.position - FragPos);
    // // check if lighting is inside the spotlight cone
    // float theta = dot(lightDir, normalize(-light.direction)); 

    // if(theta > light.cutOff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
    // {    
        // ambient
        vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
        // attenuation
        float distance    = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

        // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
        
        ambient  *= attenuation; 
        diffuse   *= attenuation;
        specular *= attenuation;   

        float theta     = dot(lightDir, normalize(-light.direction));
        float epsilon   = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  
        
        diffuse  *= intensity;
        specular *= intensity;  
            
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    //}
    // else 
    // {
    //     // else, use ambient light so scene isn't completely dark outside the spotlight.
    //     FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);
    // }
}