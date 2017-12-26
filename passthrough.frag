#version 320 es
out highp vec4 color;
in highp vec2 texCoord;

uniform layout(location = 0) sampler2D imageSampler;

void main() {
   color = vec4(texture( imageSampler, vec2(texCoord.x,  texCoord.y) ).rgb, 1.0);
}
