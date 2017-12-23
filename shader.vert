#version 320 es
in highp vec3 position;
in highp vec2 textureCoord;

out highp vec2 texCoord;

void main() {
   texCoord = textureCoord;
   gl_Position = vec4(position, 1.0);
}
