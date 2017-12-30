#version 320 es
out highp vec4 color;
in highp vec2 texCoord;

uniform layout(location = 0) sampler2D imageSampler;

void main() {
   color = vec4(0.0, 0.0, 0.0, 0.0);

   for (int i = 0; i < 5; ++i)
   {
       color += vec4(texture( imageSampler, vec2(texCoord.x + float(i) / (1920.0 * 2.0),  texCoord.y) ).rgb, 1.0);
   }

   color /= 5.0;

   color *= 3.0;

}
