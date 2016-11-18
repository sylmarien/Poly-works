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

uniform int typeIllumination;
uniform int utiliseBlinn;
uniform int utiliseDirect;

uniform sampler2D laTexture;
uniform int utiliseCouleur;
uniform int texnumero;
uniform int noirTransparent;

in vec4 couleur;
in vec3 lightDir;
in vec3 n;
in vec3 eyeVec;
in vec2 texCoo;

out vec4 FragColor;

void main( void )
{   
   vec4 myColor = couleur;

   // Normalisations
   vec3 nNormalized = normalize(n);
   vec3 eyeVecNormalized = normalize(eyeVec);
   vec3 lightDirNormalized = normalize(lightDir);

   // Gestion de la suppression de la couleur
   vec4 frontMaterialDiffuse;
   if (utiliseCouleur == 1) {
      frontMaterialDiffuse = FrontMaterial.diffuse;
   } else {
      frontMaterialDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
   }

   // direction du spot
   vec3 D = normalize(LightSource[0].spotDirection);
   // produit scalaire pour le calcul de la réflexion diffuse
   float NdotL = max( dot(nNormalized,lightDirNormalized), 0.0 );

   float cosGamma = dot(-lightDirNormalized, D);
   float cosDelta = cos(radians(LightSource[0].spotCutoff));
   float cosInner = cosDelta;
   float cosOuter = pow(cosDelta, 1.01 + LightSource[0].spotExponent / 2);

   // Facteur multiplicatif du cône
   float facteurCone;
   if (utiliseDirect == 0) { // Spot OpenGL  
      if ( cosGamma > cosDelta ) {
         facteurCone = pow(cosGamma, LightSource[0].spotExponent);
      } else {
         facteurCone = 0.0;
      }
   } else { // Spot Direct3D
      if ( cosGamma >= cosInner ) {
         facteurCone = 1.0;
      } else if ( cosGamma >= cosOuter ) {
         facteurCone = ( cosGamma - cosOuter ) / ( cosInner - cosOuter );
      } else {
         facteurCone = 0;
      }
   }

   // Phong
   if (typeIllumination == 1) {

      // composante ambiante
      myColor = (FrontMaterial.emission + FrontMaterial.ambient * LightModel.ambient) + LightSource[0].ambient * FrontMaterial.ambient;
   
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

   // Prise en compte du cône
   myColor *= facteurCone;

   // Prise en compte des textures
   if (texnumero != 0) {
      vec4 couleurTexture = texture(laTexture, texCoo);
      if (couleurTexture.rgb == vec3(0.0, 0.0, 0.0) && noirTransparent == 1 && facteurCone != 0.0) {
         discard;
      } else {
         myColor *= couleurTexture;
      }
   }

   FragColor = clamp(myColor, 0.0, 1.0);
}
