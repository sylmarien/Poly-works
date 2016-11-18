#version 430

#define POINTS 0

#if defined(POINTS) && (POINTS == 1)
layout(points) in;
layout(points, max_vertices = 1) out;
#else
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
#endif

in SommetAttrib {
   vec4 couleur;
   //vec2 texCoord;
} SommetIn[];

out SommetAttrib {
   vec4 couleur;
   vec2 texCoord;
} SommetOut;

uniform mat4 matrProj;

void main()
{
#if defined(POINTS) && (POINTS == 1)
   gl_Position = matrProj * gl_in[0].gl_Position;
   SommetOut.couleur = SommetIn[0].couleur;
   EmitVertex();
#else
   const vec2 coins[4] = { vec2(-0.5, 0.5), vec2(-0.5, -0.5), vec2(0.5, 0.5), vec2(0.5, -0.5) };
   for ( int i = 0 ; i < 4 ; ++i )
   {
      vec2 decal = gl_in[0].gl_PointSize * coins[i]; // on positionne successivement aux quatre coins
      vec4 pos = vec4( gl_in[0].gl_Position.xy + decal, gl_in[0].gl_Position.zw );
      gl_Position = matrProj * pos;    // on termine la transformation débutée dans le nuanceur de sommets

      // gl_Position.xy = 0.8 * gl_Position.xy;
      // gl_Position.xyz = 0.8 * gl_Position.xyz;
      // gl_Position.xyzw = 0.8 * gl_Position.xyzw;

      SommetOut.couleur = SommetIn[0].couleur;
      //SommetOut.texCoord = SommetIn[i].texCoord;
      SommetOut.texCoord = coins[i] + vec2(0.5,0.5); // on utilise coins[] pour définir des coordonnées de texture
      EmitVertex();
   }
#endif
}
