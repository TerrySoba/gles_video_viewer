#version 320 es
out highp vec4 color;
in highp vec2 texCoord;

uniform layout(location = 0) sampler2D subpixelSampler;
uniform layout(location = 1) sampler2D imageSampler;

void main() {
   color = vec4(texture( subpixelSampler, vec2(texCoord.x * 100.0,  texCoord.y * 100.0) ).rgb, 1.0) *
           vec4(texture( imageSampler, vec2(texCoord.x,  texCoord.y) ).rgb, 1.0);
}
