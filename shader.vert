#version 320 es
in layout(location = 0) highp vec3 position;
in layout(location = 1) highp vec2 textureCoord;

out highp vec2 texCoord;

void main() {
   texCoord = textureCoord;
   gl_Position = vec4(position, 1.0);
}
