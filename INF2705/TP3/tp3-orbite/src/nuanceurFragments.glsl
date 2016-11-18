#version 430

uniform int coulProfondeur;

in vec4 couleur;
in vec4 couleurPoleRechauffe;
in float hauteurNormalisee;

out vec4 FragColor;

void main( void )
{
   // la couleur du fragment est la couleur interpolée
   // FragColor = couleur;

   // Interpolation de la couleur en fonction de la latitude.
   FragColor = mix(couleur, couleurPoleRechauffe, hauteurNormalisee);

   if (coulProfondeur == 1) {
   		float d = gl_FragCoord.z / gl_FragCoord.w;
   		float distanceBornee = clamp(d, 20, 40);
   		float distanceNormee = (distanceBornee - 20)/20;
   		FragColor *= 1 - distanceNormee;
   		FragColor.a = couleur.a;
   }
}
