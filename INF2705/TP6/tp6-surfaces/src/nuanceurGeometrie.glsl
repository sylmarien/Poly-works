#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 4) out;

layout(location=0) uniform mat4 matrModel;
layout(location=4) uniform mat4 matrVisu;
layout(location=8) uniform mat4 matrProj;

in Attribs {
   vec3 normale, lumiDir, eyeVec;
   vec2 texCoord;
} AttribsIn[];

out Attribs {
   vec3 normale, lumiDir, eyeVec;
   vec2 texCoord;
} AttribsOut;

void main()
{
   // émettre les sommets
   for ( int i = 0 ; i < gl_in.length() ; ++i )
   {
      gl_Position = gl_in[i].gl_Position;
      gl_ClipDistance[0] = gl_in[i].gl_ClipDistance[0];
      gl_ClipDistance[1] = gl_in[i].gl_ClipDistance[1];
      AttribsOut.normale = AttribsIn[i].normale;
      AttribsOut.lumiDir = AttribsIn[i].lumiDir;
      AttribsOut.eyeVec = AttribsIn[i].eyeVec;
      AttribsOut.texCoord = AttribsIn[i].texCoord;
      EmitVertex();
   }
}
