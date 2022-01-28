#version 410 core


in vec4 fragColor;
in vec2 v_TexCoord;

in vec3 curPos;
in vec3 curNormal;

// Ouput data
out vec4 color;

uniform sampler2D u_NumberTexture;
uniform sampler2D u_GreenTex;
uniform sampler2D u_OrangeTex;
uniform sampler2D u_RedTex;
uniform sampler2D u_WhiteTex;
uniform sampler2D u_YellowTex;
uniform sampler2D u_BlueTex;

uniform vec4 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;

void main()
{
    vec4 texColor;

    float ambient = 0.40f;
    vec3 normal = normalize(curNormal);
    vec3 lightDirection = normalize(u_LightPos - curPos);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    float specularLight = 0.80f;
    vec3 viewDirection = normalize(u_CameraPos - curPos);
    vec3 reflectionDirection = -lightDirection - 2.0 * dot(normal, -lightDirection) * normal;
    float specAmount = pow(max(dot(reflectionDirection, viewDirection), 0.0f), 8);
    float specular = specAmount * specularLight;


    color = texture(u_NumberTexture, v_TexCoord) * u_LightColor * vec4(vec3(diffuse + ambient + specular), 1.0f);

}