#version 430

#define POINTS 0

uniform sampler2D laTexture;
uniform int modeSelection;

in SommetAttrib {
   vec4 couleur;
   vec2 texCoord;
} SommetIn;

out vec4 FragColor;

void main( void )
{
	// la couleur du fragment est la couleur interpolée
	FragColor = SommetIn.couleur;

#if defined(POINTS) && (POINTS == 0)
	vec4 myColor = texture( laTexture, SommetIn.texCoord );
	
	if (modeSelection == 0)
		FragColor *= myColor;
	if (myColor[0] + myColor[1] + myColor[2] < 1.0)
		discard;
#endif
}
