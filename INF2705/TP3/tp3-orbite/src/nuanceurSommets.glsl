#version 430

const float M_PI = 3.14159265358979323846;	// pi
const float M_PI_2 = 1.57079632679489661923;	// pi/2
const vec4 couleurPole = vec4( 1.0, 1.0, 1.0, 1.0 ); // couleur du pôle sans réchauffement

layout(location=0) uniform mat4 matrModel;
layout(location=4) uniform mat4 matrVisu;
layout(location=8) uniform mat4 matrProj;

uniform float facteurRechauffement; // facteur qui sert à calculer la couleur des pôles (0.0=froid, 1.0=chaud)
uniform vec4 planCoupe; // équation du plan de coupe

layout(location=0) in vec4 Vertex;
layout(location=3) in vec4 Color;

out vec4 couleur;
out vec4 couleurPoleRechauffe;
out float hauteurNormalisee;

void main( void )
{
   // transformation standard du sommet
   gl_Position = matrProj * matrVisu * matrModel * Vertex;

   // Couleur au pole
   couleurPoleRechauffe = mix(couleurPole, Color, facteurRechauffement);

   hauteurNormalisee = abs(Vertex.z/length(Vertex.xyz));
   // hauteurNormalisee = asin(Vertex.z/Vertex.length())/M_PI_2;

   // couleur du sommet
   couleur = Color;

   // Plan de coupe
   vec4 posOeil = matrModel * Vertex;
   gl_ClipDistance[0] = dot( planCoupe, posOeil );
}