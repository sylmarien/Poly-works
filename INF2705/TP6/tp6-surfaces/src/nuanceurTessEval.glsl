#version 430

layout(triangles) in;

// Définition des paramètres des sources de lumière
struct LightSourceParameters
{
   vec4 ambient;
   vec4 diffuse;
   vec4 specular;
   vec4 position;
   vec3 spotDirection;
   float spotExponent;
   float spotCutoff;            // ([0.0,90.0] ou 180.0)
   float constantAttenuation;
   float linearAttenuation;
   float quadraticAttenuation;
};
uniform LightSourceParameters LightSource[3];

// Définition des paramètres globaux du modèle de lumière
struct LightModelParameters
{
   vec4 ambient;       // couleur ambiante
   bool localViewer;   // observateur local ou à l'infini?
   bool twoSide;       // éclairage sur les deux côtés ou un seul?
};
uniform LightModelParameters LightModel;

// in vec3 gl_TessCoord;
// in int gl_PatchVerticesIn;
// in int gl_PrimitiveID;
// patch in float gl_TessLevelOuter[4];
// patch in float gl_TessLevelInner[2];
// in gl_PerVertex
// {
//   vec4 gl_Position;
//   float gl_PointSize;
//   float gl_ClipDistance[];
// } gl_in[gl_MaxPatchVertices];

// out gl_PerVertex {
//   vec4 gl_Position;
//   float gl_PointSize;
//   float gl_ClipDistance[];
// };

layout(location=0) uniform mat4 matrModel;
layout(location=4) uniform mat4 matrVisu;
layout(location=8) uniform mat4 matrProj;
layout(location=12) uniform mat3 matrNormale;

/////////////////////////////////////////////////////////////////

uniform int indiceFonction;
uniform int indiceTexture;
uniform float facteurZ;
uniform sampler2D textureDeplacement;

out Attribs {
   vec3 normale, lumiDir, eyeVec;
   vec2 texCoord;
} AttribsOut;

float interpole( float v0, float v1, float v2 )
{
   return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}
vec2 interpole( vec2 v0, vec2 v1, vec2 v2 )
{
   return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}
vec3 interpole( vec3 v0, vec3 v1, vec3 v2 )
{
   return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}
vec4 interpole( vec4 v0, vec4 v1, vec4 v2 )
{
   return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}

float Fonc( float x, float y )
{
   float z = 0.0;
   if ( indiceFonction == 0 )
      z = ( y*y - x*x );
   else if ( indiceFonction == 1 )
      z = 2.0 * x*y;
   else if ( indiceFonction == 2 )
      z = ( y*sin(2.0*x) * x*cos(2.0*y) );
   else if ( indiceFonction == 3 )
      z = 5.0 * (x*y) / exp(x*x + y*y);
   else if ( indiceFonction == 4 )
   {
      float t = length(vec2(x, y)) * 2.0;
      z = (1.0 - t*t) * exp(-t*t / 4.0);
   }
   return facteurZ * z;
}

uniform vec4 planCoupeHaut = vec4( 0.0, 0.0, -1.0, 2.0 );
uniform vec4 planCoupeBas = vec4( 0.0, 0.0, 1.0, 2.0 );

void main( void )
{
   // interpoler la position selon la tessellation
   vec4 sommet = interpole( gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position );

   // générer (en utilisant sommet.xy) les coordonnées de texture plutôt que les interpoler
   AttribsOut.texCoord = vec2((sommet.x + 2)/4, (sommet.y +2)/4);

   float z;
   if (indiceTexture == 0) {
      z = Fonc(sommet.x, sommet.y);
   } else {
      vec4 couleurTexture = texture(textureDeplacement, AttribsOut.texCoord);
      z = (couleurTexture.r + couleurTexture.g + couleurTexture.b) / 3;
   }
   sommet.z = z;

   // déplacer le sommet selon la fonction (possiblement en utilisant la texture)
   gl_Position = matrProj * matrVisu * matrModel * vec4(sommet.x, sommet.y, z, sommet.w); // à modifier

   // initialiser la variable gl_ClipDistance[] pour que le découpage soit fait par OpenGL
   vec4 posOeil = matrModel * sommet;
   gl_ClipDistance[0] = dot( planCoupeHaut, posOeil );
   gl_ClipDistance[1] = dot( planCoupeBas, posOeil );

   // calculer la normale (possiblement en utilisant la texture)
   float epsilon = 0.01;
   AttribsOut.normale = matrNormale * vec3((Fonc(sommet.x + epsilon, sommet.y) - Fonc(sommet.x - epsilon, sommet.y)) / (2*epsilon), (Fonc(sommet.x, sommet.y + epsilon) - Fonc(sommet.x, sommet.y - epsilon)) / (2*epsilon), -1);

   // vecteur de la direction vers la lumière
   AttribsOut.lumiDir = (matrVisu * (LightSource[0].position - posOeil)).xyz;

   // vecteur de la direction vers l'observateur
   AttribsOut.eyeVec = - (matrVisu * matrModel * sommet).xyz;
}
