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

/////////////////////////////////////////////////////////////////

uniform sampler2D textureCouleur;
uniform int indiceCouleur;

// bool gl_FrontFacing  // variable standard d'OpenGL: c'est la face avant qui est actuellement tracée?

in Attribs {
   vec3 normale, lumiDir, eyeVec;
   vec2 texCoord;
} AttribsIn;

out vec4 FragColor;

bool utiliseBlinn = true;

void main( void )
{
   // Normalisations
   vec3 nNormalized = normalize(AttribsIn.normale);
   vec3 eyeVecNormalized;
   if (LightModel.localViewer)
      eyeVecNormalized = normalize(AttribsIn.eyeVec);
   else
      eyeVecNormalized = vec3(0.0, 0.0, 1.0);

   vec3 lightDirNormalized = normalize(AttribsIn.lumiDir);

   if (gl_FrontFacing)
      nNormalized = - nNormalized;

   // produit scalaire pour le calcul de la réflexion diffuse
   float NdotL = max( dot(nNormalized,lightDirNormalized), 0.0 );

   // Phong
   // composante ambiante
   vec4 myColor = (FrontMaterial.emission + FrontMaterial.ambient * LightModel.ambient) + LightSource[0].ambient * FrontMaterial.ambient;

   // composante diffuse
   myColor += abs(dot(nNormalized,lightDirNormalized)) * LightSource[0].diffuse * FrontMaterial.diffuse * NdotL;

   float NdotHV;
   if (utiliseBlinn) { // spéculaire Blinn
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

   // Prise en compte des textures
   if (indiceCouleur != 0) {
      vec4 couleurTexture = texture(textureCouleur, AttribsIn.texCoord);
      myColor *= couleurTexture;
   }

   FragColor = myColor; // blanc
}
