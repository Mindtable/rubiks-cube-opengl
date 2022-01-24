#version 410 core


in vec4 fragColor;
in vec2 v_TexCoord;
// Ouput data
out vec4 color;

uniform sampler2D u_NumberTexture;
uniform sampler2D u_GreenTex;
uniform sampler2D u_OrangeTex;
uniform sampler2D u_RedTex;
uniform sampler2D u_WhiteTex;
uniform sampler2D u_YellowTex;
uniform sampler2D u_BlueTex;

void main()
{
    vec4 texColor;
//    if (u_NumberTexture == 0)
//        texColor = texture(u_GreenTex, v_TexCoord);
//    if (u_NumberTexture == 1)
//        texColor = texture(u_OrangeTex, v_TexCoord);
//    if (u_NumberTexture == 2)
//        texColor = texture(u_RedTex, v_TexCoord);
//    if (u_NumberTexture == 3)
//        texColor = texture(u_WhiteTex, v_TexCoord);
//    if (u_NumberTexture == 4)
//        texColor = texture(u_YellowTex, v_TexCoord);
//    if (u_NumberTexture == 5)
//        texColor = texture(u_BlueTex, v_TexCoord);
//    if (u_NumberTexture == 5)
//        texColor = vec4(1.0, 1.0, 1.0, 1.0);
//    texelFetch(screenTextureMS, TexCoords, 3);
//    color = texelFetch(u_NumberTexture, v_TexCoord, 3);
    color = texture(u_NumberTexture, v_TexCoord);

}