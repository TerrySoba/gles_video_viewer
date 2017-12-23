#version 320 es
in highp vec3 position;
in highp vec3 textureCoord;

out highp vec3 vecColor;

void main() {
   vecColor = vec3(textureCoord.x, textureCoord.y, 1.0);
   gl_Position = vec4(position, 1.0);
}
