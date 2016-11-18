#version 430

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

// Définition des paramètres des matériaux
struct MaterialParameters
{
   vec4 emission;
   vec4 ambient;
   vec4 diffuse;
   vec4 specular;
   float shininess;
};
uniform MaterialParameters FrontMaterial;
uniform MaterialParameters BackMaterial;

// Définition des paramètres globaux du modèle de lumière
struct LightModelParameters
{
   vec4 ambient;       // couleur ambiante
   bool localViewer;   // observateur local ou à l'infini?
   bool twoSide;       // éclairage sur les deux côtés ou un seul?
};
uniform LightModelParameters LightModel;

layout(location=0) uniform mat4 matrModel;
layout(location=4) uniform mat4 matrVisu;
layout(location=8) uniform mat4 matrProj;
layout(location=12) uniform mat3 matrNormale;

/////////////////////////////////////////////////////////////////

layout(location=0) in vec4 Vertex;
layout(location=2) in vec3 Normal;
layout(location=3) in vec4 Color;
layout(location=8) in vec4 MultiTexCoord0;
//layout(location=9) in vec4 MultiTexCoord1;

uniform int typeIllumination;
uniform int utiliseCouleur;
uniform int utiliseBlinn;

out vec4 couleur;
out vec3 lightDir;
out vec3 n;
out vec3 eyeVec;
out vec2 texCoo;

void main( void )
{
   vec4 myColor = vec4(1.0, 1.0, 1.0, 1.0);

   // transformation standard du sommet
   gl_Position = matrProj * matrVisu * matrModel * Vertex;
   // calcul de la normale normalisée
   n = matrNormale * Normal;
   // calculer la position du sommet dans l'espace de la caméra ("eye-coordinate position")
   vec3 ecPosition = vec3( matrVisu * matrModel * Vertex ).xyz;
   // Créer eyeVec et lightDir
   eyeVec = - ecPosition;
   lightDir = (matrVisu * LightSource[0].position).xyz - ecPosition;

   // Gouraud
   if (typeIllumination == 0) {
      // Normalisations
      vec3 nNormalized = normalize(n);
      vec3 eyeVecNormalized = normalize(eyeVec);
      vec3 lightDirNormalized = normalize(lightDir);

      vec4 frontMaterialDiffuse;
      if (utiliseCouleur == 1) {
         frontMaterialDiffuse = FrontMaterial.diffuse;
      } else {
         frontMaterialDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
      }

      // composante ambiante
      myColor = (FrontMaterial.emission + FrontMaterial.ambient * LightModel.ambient) + LightSource[0].ambient * FrontMaterial.ambient;

      // produit scalaire pour le calcul de la réflexion diffuse
      float NdotL = max( dot(nNormalized,lightDirNormalized), 0.0 );
   
      // composante diffuse
      myColor += abs(dot(nNormalized,lightDirNormalized)) * LightSource[0].diffuse * frontMaterialDiffuse * NdotL;

      float NdotHV;
      if (utiliseBlinn == 1) { // spéculaire Blinn
         // normaliser le demi-vecteur (Blinn)
         vec3 halfV = normalize(lightDirNormalized + eyeVecNormalized);
         // produit scalaire pour la réflexion spéculaire
         NdotHV = max( dot(nNormalized,halfV), 0.0 );
      } else { // spéculaire Phong
         vec3 R = -reflect( lightDirNormalized, nNormalized );
         NdotHV = max( dot( R, eyeVecNormalized ), 0.0 );
      }

      float specular = pow( NdotHV, FrontMaterial.shininess );
      // composante spéculaire
      myColor += LightSource[0].specular * FrontMaterial.specular * specular;
   }

   texCoo = MultiTexCoord0.st;

   // couleur du sommet
   couleur = myColor;
}
