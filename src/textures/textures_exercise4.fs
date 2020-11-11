//使用一个uniform变量作为mix函数的第三个参数来改变两个纹理可见度，使用上和下键来改变箱子或笑脸的可见度
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float per;

void main()
{
	//FragColor = texture(texture1, TexCoord);
    //texture函数来采样纹理的颜色
    //FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);
    //两个纹理结合，根据第三个参数进行线性插值
    //如果第三个值是0.0，它会返回第一个输入；如果是1.0，会返回第二个输入值。0.2会返回80%的第一个输入颜色和20%的第二个输入颜色
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(-TexCoord.x,TexCoord.y)), per);
}