// #version 110
// varying vec4 couleur

#version 430

in vec4 couleur;

out vec4 FragColor;

void main( void )
{
   // la couleur du fragment est la couleur interpolée
   // glFragColor = couleur;
   FragColor = couleur;
}
