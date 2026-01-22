#version 110

attribute vec3 aPos;
attribute vec3 aNormal;
attribute vec2 aUV;

varying vec2 vUV;
varying vec3 vLighting;

uniform vec3 uLightDirection;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * vec4(aPos, 1.0);
    vUV = aUV;
    vec3 norm = normalize(gl_NormalMatrix * aNormal);
    vec3 lightDir = uLightDirection;
    float diffuse = max(dot(norm, lightDir), 0.0);
    vLighting = vec3(diffuse);
}
