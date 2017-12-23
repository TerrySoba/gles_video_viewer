#version 320 es
out highp vec4 color;

in highp vec3 vecColor;

void main() {
   color = vec4(vecColor.r, vecColor.g, vecColor.b, 1.0);
}
