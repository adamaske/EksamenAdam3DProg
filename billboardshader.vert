#version 410 core
layout(location = 0) in vec3 positionIn;
layout(location = 2) in vec2 vertexUV;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 billboardCenter;
uniform bool bSpherical = false;
uniform int bisScreenSpaceOnly = 0;
out vec2 UV;


void main(void)
{
    vec3 cameraRight = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    vec3 cameraUp = vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

    vec3 vertexPosition = billboardCenter + (cameraRight * positionIn.x) + ( vec3(0,1,0) * positionIn.y);

    if(bSpherical)
    {
        vertexPosition = billboardCenter + (cameraRight * positionIn.x) + ( cameraUp * positionIn.y);
    }

    gl_Position = projectionMatrix * viewMatrix * vec4(vertexPosition, 1.0) * (1-bisScreenSpaceOnly) +
                    (vec4(billboardCenter,1) + vec4(positionIn, 1.0)) * bisScreenSpaceOnly;

    UV = vertexUV;
}
