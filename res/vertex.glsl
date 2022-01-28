#version 410 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 texUV;
layout(location = 2) in vec3 Normals;

out vec4 fragColor;
out vec3 curPos;
out vec3 curNormal;
//
uniform vec4 ourColor;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ProjectionViewMatrix;
uniform mat4 u_Floating;

uniform int u_AnimationNeeded;
uniform mat4 u_Animation;
//uniform sampler2D u_Texture;

out vec2 v_TexCoord;

void main(){
    v_TexCoord = texUV;
    vec4 tempVector = vec4(vertexPosition_modelspace, 1.0f);
    if (u_AnimationNeeded == 1) {
        tempVector = u_Animation * tempVector;
    }

    //Transfer curNormal to fragment
    curNormal = vec3(u_ModelMatrix * u_Floating * vec4(Normals, 0.0f));

    //Transfer curPos to fragment
    curPos = vec3(u_ModelMatrix * u_Floating * tempVector);

    gl_Position = u_ProjectionViewMatrix * u_ModelMatrix * u_Floating * tempVector;
    fragColor = ourColor;

}