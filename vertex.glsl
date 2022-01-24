#version 410 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 texUV;

out vec4 fragColor;
//
uniform vec4 ourColor;
uniform mat4 u_ModelMatrix;
//uniform sampler2D u_Texture;

out vec2 v_TexCoord;

void main(){
    v_TexCoord = texUV;
    gl_Position = u_ModelMatrix * vec4(vertexPosition_modelspace, 1.0f);
    fragColor = ourColor;

}