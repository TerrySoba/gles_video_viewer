#version 320 es
out highp vec4 color;
in highp vec2 texCoord;

uniform sampler2D myTextureSampler;

void main() {
   // color = vec4(texCoord.r, texCoord.g, 1.0, 1.0);
   color = vec4(texture( myTextureSampler, texCoord ).rgb, 1.0);
}
