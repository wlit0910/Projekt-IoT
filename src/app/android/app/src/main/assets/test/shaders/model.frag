#version 110

uniform sampler2D uTexture;

varying vec2 vUV;
varying vec3 vLighting;

void main()
{
    vec4 texColor = texture2D(uTexture, vUV);
    gl_FragColor = vec4(texColor.rgb * vLighting, texColor.a);
}
