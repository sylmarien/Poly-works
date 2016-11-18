// #version 110
// varying vec4 couleur

#version 430

layout(location=0) uniform mat4 matrModel;
layout(location=4) uniform mat4 matrVisu;
layout(location=8) uniform mat4 matrProj;

layout(location=0) in vec4 Vertex;
layout(location=3) in vec4 Color;

out vec4 couleur;

void main( void )
{
   // transformation standard du sommet
   // gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
   gl_Position = matrProj * matrVisu * matrModel * Vertex;

   // couleur du sommet
   // couleur = gl_Color;
   couleur = Color;
}
