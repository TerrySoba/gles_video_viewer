#version 320 es
out highp vec4 color;
in highp vec2 texCoord;

uniform layout(location = 0) sampler2D subpixelSampler;
uniform layout(location = 1) sampler2D imageSampler;
uniform layout(location = 2) sampler2D scanlineSampler;
uniform layout(location = 3) ivec2 inputRes;

void main() {
   color = vec4(texture( subpixelSampler, vec2(texCoord.x * 320.0,  texCoord.y * 320.0) ).rgb, 1.0) *
           vec4(texture( imageSampler, vec2(texCoord.x,  texCoord.y) ).bgr, 1.0) *
           vec4(texture( scanlineSampler, vec2(texCoord.x,  texCoord.y * float(inputRes.y)) ).bgr, 1.0);
}
