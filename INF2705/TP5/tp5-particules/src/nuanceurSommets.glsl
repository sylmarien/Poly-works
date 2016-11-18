#version 430

layout(location=0) uniform mat4 matrModel;
layout(location=4) uniform mat4 matrVisu;
layout(location=8) uniform mat4 matrProj;

layout(location=0) in vec4 Vertex;
layout(location=3) in vec4 Color;

out SommetAttrib {
   vec4 couleur;
} SommetOut;

void main( void )
{
   // transformation standard du sommet
   // gl_Position = matrProj * matrVisu * matrModel * Vertex;
   gl_Position = matrVisu * matrModel * Vertex;

   gl_PointSize = 0.25;

   // couleur du sommet
   SommetOut.couleur = Color;
}
