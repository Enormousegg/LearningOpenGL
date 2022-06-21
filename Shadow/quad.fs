#version 330 core

in vec2 TexCoords;

uniform samplerCube depthMap;
uniform mat4 far_plane;

void main()
{             
    float depthValue = texture(depthMap, TexCoords).r;
    FragColor = vec4(vec3(closestDepth / far_plane), 1.0);
}