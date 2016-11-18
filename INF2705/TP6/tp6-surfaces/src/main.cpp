// Prénoms, noms et matricule des membres de l'équipe:
// - Alexandre MAO (1813566)
// - Maxime SCHMITT (1719088)
// #warning "Écrire les prénoms, noms et matricule des membres de l'équipe dans le fichier et commenter cette ligne"

#include <iostream>
#include "inf2705.h"

// variables pour l'utilisation des nuanceurs
ProgNuanceur progNuanceur;      // votre programme de nuanceurs
ProgNuanceur progNuanceurBase;  // le programme de nuanceurs de base

// matrices de du pipeline graphique
MatricePipeline matrModel;
MatricePipeline matrVisu;
MatricePipeline matrProj;

// variables pour définir le point de vue
double thetaCam = 0.0;        // angle de rotation de la caméra (coord. sphériques)
double phiCam = 80.0;         // angle de rotation de la caméra (coord. sphériques)
double distCam = 10.0;        // distance (coord. sphériques)

// variables d'état
bool enPerspective = false;   // indique si on est en mode Perspective (true) ou Ortho (false)
bool enmouvement = false;     // le modèle est en mouvement/rotation automatique ou non
bool afficheAxes = true;      // indique si on affiche les axes
GLenum modePolygone = GL_FILL; // comment afficher les polygones

GLfloat TessLevelInner = 40;
GLfloat TessLevelOuter = 40;

glm::vec3 posLumi( 1.0, 0.7, 1.5 ); // position de la source lumineuse
bool positionnelle = true;       // la lumière est de type positionnelle?
bool localViewer = false;        // doit-on prendre en compte la position de l'observateur?

int affichageStereo = 0;         // l'affichage est en mono

GLuint textures[9];              // les textures chargées
int indiceTexture = 0;           // indice de la texture à utiliser pour le déplacement
int indiceCouleur = 0;           // indice de la texture à utiliser pour la couleur
int indiceFonction = 0;          // indice de la fonction à afficher
float facteurZ = 1.0;            // facteur de déplacement initial en Z

struct VueStereo
{
   GLdouble dip;       // la distance interpupillaire
   GLdouble factzoom;  // le facteur de zoom
   GLdouble zavant;    // la position du plan avant du volume de visualisation
   GLdouble zarriere;  // la position du plan arrière du volume de visualisation
   GLdouble zecran;    // la position du plan de l'écran: les objets affichés en avant de ce plan «sortiront» de l'écran
   GLint modele;       // le modèle à afficher
};
struct VueStereo vue;

GLuint g_VAO[2];
GLuint g_VBO[4];

void verifierAngles()
{
   if ( thetaCam > 360.0 )
      thetaCam -= 360.0;
   else if ( thetaCam < 0.0 )
      thetaCam += 360.0;

   const GLdouble MINPHI = 0.01, MAXPHI = 180.0 - 0.01;
   if ( phiCam > MAXPHI )
      phiCam = MAXPHI;
   else if ( phiCam < MINPHI )
      phiCam = MINPHI;
}

void calculerPhysique( )
{
   if ( enmouvement )
   {
      static int sensPhi = 1;
      thetaCam += 0.4;
      phiCam += 0.7 * sensPhi;
      if ( phiCam < 0.0 || phiCam > 180.0 ) sensPhi = -sensPhi;

      verifierAngles();
   }
}

void chargerToutesLesTextures()
{
   ChargerTexture( "textures/texture1.bmp", textures[0] );
   ChargerTexture( "textures/texture2.bmp", textures[1] );
   ChargerTexture( "textures/texture3.bmp", textures[2] );
   ChargerTexture( "textures/texture4.bmp", textures[3] );
   ChargerTexture( "textures/texture5.bmp", textures[4] );
   ChargerTexture( "textures/texture6.bmp", textures[5] );
   ChargerTexture( "textures/texture7.bmp", textures[6] );
   ChargerTexture( "textures/texture8.bmp", textures[7] );
   ChargerTexture( "textures/texture9.bmp", textures[8] );
}

void chargerNuanceurs()
{
   // charger le nuanceur de base
   progNuanceurBase.creer( );
   progNuanceurBase.attacherSourceSommets( "nuanceurSommetsBase.glsl" );
   progNuanceurBase.attacherSourceFragments( "nuanceurFragmentsBase.glsl" );
   progNuanceurBase.lier( );

   // charger le nuanceur de ce TP
   progNuanceur.creer( );
   progNuanceur.attacherSourceSommets( "nuanceurSommets.glsl" );
   progNuanceur.attacherSourceTessCtrl( "nuanceurTessCtrl.glsl" ); // Étape 1: ACTIVER ces nuanceurs
   progNuanceur.attacherSourceTessEval( "nuanceurTessEval.glsl" ); // Étape 1: ACTIVER ces nuanceurs
   progNuanceur.attacherSourceGeometrie( "nuanceurGeometrie.glsl" );
   progNuanceur.attacherSourceFragments( "nuanceurFragments.glsl" );
   progNuanceur.lier( );
}

void initialiser()
{
   // donner la couleur de fond
   glClearColor( 0.2, 0.2, 0.2, 1.0 );

   // activer les etats openGL
   glEnable( GL_DEPTH_TEST );

   vue.dip = 0.80;
   vue.factzoom = 1.0;
   vue.zavant = 4.0;
   vue.zarriere = 20.0;
   vue.zecran = 10.0;
   vue.modele = 1;

   // charger les nuanceurs
   chargerNuanceurs();

   // charger les textures
   chargerToutesLesTextures();

   // créer le VAO pour conserver les informations
   glGenVertexArrays( 2, g_VAO ); // deux VAOs: un pour la surface, l'autre pour les décorations
   glGenBuffers( 3, g_VBO ); // trois VBO pour différents tableaux

   // la surface
   glBindVertexArray( g_VAO[0] );
   {
      // les tableaux
      const GLfloat sommets[] = { -2.0, -2.0,
                                   2.0, -2.0,
                                   2.0,  2.0,
                                  -2.0,  2.0 };
      const GLuint connec[] = { 0, 1, 2, 2, 3, 0 };
      // créer le VBO pour les sommets
      glBindBuffer( GL_ARRAY_BUFFER, g_VBO[0] );
      glBufferData( GL_ARRAY_BUFFER, sizeof(sommets), sommets, GL_STATIC_DRAW );
      // faire le lien avec l'attribut du nuanceur de sommets
      GLint locVertex = progNuanceur.obtenirAttribLocation( "Vertex" );
      glVertexAttribPointer( locVertex, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
      glEnableVertexAttribArray(locVertex);

      // charger le VBO pour la connectivité
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, g_VBO[1] );
      glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(connec), connec, GL_STATIC_DRAW );
   }

   // les décorations
   glBindVertexArray( g_VAO[1] );
   {
      // créer le VBO pour les sommets
      GLfloat coords[] =
      {
         0., 0., 0.,
         2*posLumi[0], 2*posLumi[1], 2*posLumi[2]
      };
      glBindBuffer( GL_ARRAY_BUFFER, g_VBO[3] );
      glBufferData( GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW );
      // faire le lien avec l'attribut du nuanceur de sommets
      GLint locVertex = progNuanceurBase.obtenirAttribLocation( "Vertex" );
      glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
      glEnableVertexAttribArray(locVertex);
   }
}

void conclure()
{
   glDeleteBuffers( 4, g_VBO );
   glDeleteVertexArrays( 2, g_VAO );
}

void definirProjection( int OeilMult, int w, int h ) // 0: mono, -1: oeil gauche, +1: oeil droit
{
   // partie 2: utiliser plutôt Frustum() pour le stéréo
   if (affichageStereo == 0)
      matrProj.Perspective( 35.0, (GLdouble) w / (GLdouble) h, vue.zavant, vue.zarriere );
   else {
      const GLdouble resolution = 100.0; // pixels par pouce
      GLdouble oeilDecalage = OeilMult * vue.dip / 2.0;
      GLdouble proportionProfondeur = vue.zavant / vue.zecran;  // la profondeur du plan de parallaxe nulle
   
      matrProj.Frustum( (-0.5 * w / resolution - oeilDecalage ) * proportionProfondeur,
                        ( 0.5 * w / resolution - oeilDecalage ) * proportionProfondeur,
                        (-0.5 * h / resolution                ) * proportionProfondeur,
                        ( 0.5 * h / resolution                ) * proportionProfondeur,
                        vue.zavant, vue.zarriere );
      matrProj.Translate( -oeilDecalage, 0.0, 0.0 );
      progNuanceur.assignerUniformMatrix4fv( "matrProj", matrProj );
   }
}

void afficherDecoration()
{
   glBindVertexArray( g_VAO[1] );

   // remettre le programme de base pour le reste des décorations
   progNuanceurBase.utiliser( );
   progNuanceurBase.assignerUniformMatrix4fv( "matrProj", matrProj ); // donner la projection courante

   // dessiner le cube englobant
   glColor3f( 1.0, 1.0, 1.0 );
   shapesWireCube( 4.0 );

   // dessiner une sphère à la position de la lumière
   glColor3f( 1.0, 1.0, 0.5 ); // jaune
   if ( !positionnelle )
   {
      GLfloat coords[] =
      {
         0., 0., 0.,
         2*posLumi[0], 2*posLumi[1], 2*posLumi[2]
      };
      glBegin( GL_LINES );
      glVertex3fv( &(coords[0]) );
      glVertex3fv( &(coords[3]) );
      glEnd( );
   }
   matrModel.PushMatrix();{
      matrModel.Translate( posLumi[0], posLumi[1], posLumi[2] );
      progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );
      shapesSolidSphere( 0.1, 10, 10 );
   }matrModel.PopMatrix();
   progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );
}

void afficherModele()
{
   progNuanceur.utiliser();
   progNuanceur.assignerUniformMatrix4fv( "matrProj", matrProj );

   // Appliquer le matériau
   glm::vec4 mat_ambiant( 0.2, 0.2, 0.2, 1.0 );
   glm::vec4 mat_diffuse( 1.0, 1.0, 1.0, 1.0 );
   glm::vec4 mat_specular( 1.0, 1.0, 1.0, 1.0 );
   glm::vec4 mat_emission( 0.0, 0.0, 0.0, 1.0 );
   GLfloat mat_shininess = 100.0;

   progNuanceur.assignerUniform4fv( "FrontMaterial.ambient", mat_ambiant );
   progNuanceur.assignerUniform4fv( "FrontMaterial.diffuse", mat_diffuse );
   progNuanceur.assignerUniform4fv( "FrontMaterial.specular", mat_specular );
   progNuanceur.assignerUniform4fv( "FrontMaterial.emission", mat_emission );
   progNuanceur.assignerUniform1f( "FrontMaterial.shininess", mat_shininess );

   glm::mat3 matrNormale = glm::transpose( glm::inverse( glm::mat3( matrVisu.getMatr() * matrModel.getMatr() ) ) );
   progNuanceur.assignerUniformMatrix3fv( "matrNormale", matrNormale );

   glm::vec4 lumiAmbiant( 0.5, 0.5, 0.5, 1.0 );
   glm::vec4 lumiDiffuse( 0.7, 0.7, 0.7, 1.0 );
   glm::vec4 lumiSpeculaire( 0.5, 0.5, 0.5, 1.0 );
   progNuanceur.assignerUniform1i( "LightModel.localViewer", localViewer );
   progNuanceur.assignerUniform4fv( "LightSource[0].ambient", lumiAmbiant );
   progNuanceur.assignerUniform4fv( "LightSource[0].diffuse", lumiDiffuse );
   progNuanceur.assignerUniform4fv( "LightSource[0].specular", lumiSpeculaire );

   // partie 1: définir la lumière
   GLfloat posH = positionnelle ? 1.0 : 0.0;
   glm::vec4 lumiPosition( posLumi[0], posLumi[1], posLumi[2], posH );
   progNuanceur.assignerUniform4fv( "LightSource[0].position", lumiPosition );

   // partie 1: activer les deux glClipPane limitant le z vers le haut et vers le bas
   glEnable( GL_CLIP_PLANE0 );
   glEnable( GL_CLIP_PLANE1 );

   // afficher la surface (plane)
   glBindVertexArray( g_VAO[0] );
   glPatchParameteri( GL_PATCH_VERTICES, 3 );
   //glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
   glDrawElements( GL_PATCHES, 6, GL_UNSIGNED_INT, 0 ); // Étape 1: UTILISER des GL_PATCHES plutôt que des GL_TRIANGLES

   glDisable( GL_CLIP_PLANE0 );
   glDisable( GL_CLIP_PLANE1 );

   // afficher les décorations
   afficherDecoration();
}

void FenetreTP::afficherScene()
{
   // effacer l'ecran et le tampon de profondeur
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glPolygonMode( GL_FRONT_AND_BACK, modePolygone );

   progNuanceurBase.utiliser( );

   // définir le pipeline graphique
   definirProjection( 0, largeur_, hauteur_ );
   progNuanceurBase.assignerUniformMatrix4fv( "matrProj", matrProj );

   matrVisu.LookAt( distCam*cos(glm::radians(thetaCam))*sin(glm::radians(phiCam)),
                    distCam*sin(glm::radians(thetaCam))*sin(glm::radians(phiCam)),
                    distCam*cos(glm::radians(phiCam)),
                    0, 0, 0,
                    0, 0, 1 );
   progNuanceurBase.assignerUniformMatrix4fv( "matrVisu", matrVisu );

   matrModel.LoadIdentity();
   progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );

   progNuanceur.utiliser( );
   progNuanceur.assignerUniformMatrix4fv( "matrProj", matrProj );
   progNuanceur.assignerUniformMatrix4fv( "matrVisu", matrVisu );
   progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );
   progNuanceur.assignerUniform1i( "indiceFonction", indiceFonction );
   //progNuanceur.assignerUniform3f( "vecteurQuelconque", comp1, comp2, comp3 );
   progNuanceur.assignerUniform1i( "indiceTexture", indiceTexture );
   progNuanceur.assignerUniform1i( "indiceCouleur", indiceCouleur );
   progNuanceur.assignerUniform1f( "facteurZ", facteurZ );
   progNuanceur.assignerUniform1i( "textureDeplacement", 0 );    // '0' => utilisation de GL_TEXTURE0
   progNuanceur.assignerUniform1i( "textureCouleur", 1 );        // '1' => utilisation de GL_TEXTURE1
   progNuanceur.assignerUniform1f( "TessLevelInner", TessLevelInner ); // Étape 1: ACTIVER ces variables
   progNuanceur.assignerUniform1f( "TessLevelOuter", TessLevelOuter ); // Étape 1: ACTIVER ces variables

   // partie 2: afficher la surface en mono ou en stéréo
   if (affichageStereo == 0) {
      definirProjection( 0, largeur_, hauteur_ );
      afficherModele();
   } else if (affichageStereo == 1) {
      definirProjection( -1, largeur_, hauteur_ );
      glColorMask( GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE );
      afficherModele();

      glClear( GL_DEPTH_BUFFER_BIT );
      definirProjection( +1, largeur_, hauteur_ );
      glColorMask( GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE );
      afficherModele();
      
      glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
   } else {
      glViewport(0, 0, largeur_/2, hauteur_ );
      definirProjection( -1, largeur_, hauteur_ );
      afficherModele();

      glClear( GL_DEPTH_BUFFER_BIT );
      glViewport(largeur_/2, 0, largeur_/2, hauteur_);
      definirProjection( +1, largeur_, hauteur_ );
      afficherModele();
   }

   glViewport( 0, 0, largeur_, hauteur_ );
   // afficher les axes
   if ( afficheAxes ) FenetreTP::afficherAxes( );
}

// fonction de redimensionnement de la fenêtre graphique
void FenetreTP::redimensionner( GLsizei w, GLsizei h )
{
   glViewport( 0, 0, w, h );
}

void FenetreTP::clavier( TP_touche touche )
{
   switch ( touche )
   {
   case TP_ESCAPE:
   case TP_q:
      quit();
      break;

   case TP_x: // permutation de l'affichage des axes
      afficheAxes = !afficheAxes;
      std::cout << "// Affichage des axes ? " << ( afficheAxes ? "OUI" : "NON" ) << std::endl;
      break;

   case TP_v: // Recharger les nuanceurs
      chargerNuanceurs();
      std::cout << "// Recharger nuanceurs" << std::endl;
      break;

   case TP_g:
      modePolygone = ( modePolygone == GL_FILL ) ? GL_LINE : GL_FILL;
      break;

   case TP_i:
      ++TessLevelInner;
      std::cout << " TessLevelInner=" << TessLevelInner << " TessLevelOuter=" << TessLevelOuter << std::endl;
      glPatchParameteri( GL_PATCH_DEFAULT_INNER_LEVEL, TessLevelInner );
      break;
   case TP_k:
      if ( --TessLevelInner < 1 ) TessLevelInner = 1;
      std::cout << " TessLevelInner=" << TessLevelInner << " TessLevelOuter=" << TessLevelOuter << std::endl;
      glPatchParameteri( GL_PATCH_DEFAULT_INNER_LEVEL, TessLevelInner );
      break;

   case TP_o:
      ++TessLevelOuter;
      std::cout << " TessLevelInner=" << TessLevelInner << " TessLevelOuter=" << TessLevelOuter << std::endl;
      glPatchParameteri( GL_PATCH_DEFAULT_OUTER_LEVEL, TessLevelOuter );
      break;
   case TP_l:
      if ( --TessLevelOuter < 1 ) TessLevelOuter = 1;
      std::cout << " TessLevelInner=" << TessLevelInner << " TessLevelOuter=" << TessLevelOuter << std::endl;
      glPatchParameteri( GL_PATCH_DEFAULT_OUTER_LEVEL, TessLevelOuter );
      break;

   case TP_u:
      ++TessLevelOuter;
      TessLevelInner = TessLevelOuter;
      std::cout << " TessLevelInner=" << TessLevelInner << " TessLevelOuter=" << TessLevelOuter << std::endl;
      glPatchParameteri( GL_PATCH_DEFAULT_OUTER_LEVEL, TessLevelOuter );
      glPatchParameteri( GL_PATCH_DEFAULT_INNER_LEVEL, TessLevelInner );
      break;
   case TP_j:
      if ( --TessLevelOuter < 1 ) TessLevelOuter = 1;
      TessLevelInner = TessLevelOuter;
      std::cout << " TessLevelInner=" << TessLevelInner << " TessLevelOuter=" << TessLevelOuter << std::endl;
      glPatchParameteri( GL_PATCH_DEFAULT_OUTER_LEVEL, TessLevelOuter );
      glPatchParameteri( GL_PATCH_DEFAULT_INNER_LEVEL, TessLevelInner );
      break;

   case TP_f: // fonction à afficher
      indiceFonction++;
      if ( indiceFonction > 4 ) indiceFonction = 0;
      std::cout << " indiceFonction=" << indiceFonction << std::endl;
      break;

   case TP_MINUS:
      facteurZ -= 0.01;
      std::cout << " facteurZ=" << facteurZ << std::endl;
      break;

   case TP_PLUS:
   case TP_EQUALS:
      facteurZ += 0.01;
      std::cout << " facteurZ=" << facteurZ << std::endl;
      break;

   case TP_UP:
      posLumi[0] -= 0.1;
      break;
   case TP_DOWN:
      posLumi[0] += 0.1;
      break;
   case TP_LEFT:
      posLumi[1] -= 0.1;
      break;
   case TP_RIGHT:
      posLumi[1] += 0.1;
      break;
   case TP_PAGEUP:
      posLumi[2] += 0.1;
      break;
   case TP_PAGEDOWN:
      posLumi[2] -= 0.1;
      break;

   case TP_p:
      positionnelle = !positionnelle;
      std::cout << " positionnelle=" << positionnelle << std::endl;
      break;

   case TP_w: // local viewer
      localViewer = !localViewer;
      std::cout << " localViewer=" << localViewer << std::endl;
      break;

   case TP_s: // partie 2: affichage stereo
      affichageStereo++;
      if ( affichageStereo > 2 ) affichageStereo = 0;
      std::cout << " affichageStereo=" << affichageStereo << std::endl;
      break;

   // partie 2: assigner la texture à utiliser
   case TP_0:  // réinitialiser indiceCouleur et indiceTexture
      indiceTexture = -1; // sera remis à 0 par les prochaines instructions
      indiceCouleur = -1; // sera remis à 0 par les prochaines instructions
      /* break */

   case TP_c: // indice de la texture à utiliser pour la couleur
      indiceCouleur++;
      if ( indiceCouleur > 9 ) indiceCouleur = 0;
      std::cout << " indiceTexture=" << indiceTexture << " indiceCouleur=" << indiceCouleur << std::endl;
      // assigner la texture à l'unité de texture
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, indiceCouleur);
      if ( touche != TP_0 ) break;

   case TP_d: // indice de la texture à utiliser pour le déplacement
      indiceTexture++;
      if ( indiceTexture > 9 ) indiceTexture = 0;
      std::cout << " indiceTexture=" << indiceTexture << " indiceCouleur=" << indiceCouleur << std::endl;
      // assigner la texture à l'unité de texture
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, indiceTexture);
      break;

   case TP_e: // indice de la texture à utiliser pour la couleur ET le déplacement
      indiceTexture++;
      if ( indiceTexture > 9 ) indiceTexture = 0;
      indiceCouleur = indiceTexture;
      std::cout << " indiceTexture=" << indiceTexture << " indiceCouleur=" << indiceCouleur << std::endl;
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, indiceCouleur);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, indiceTexture);
      break;

   case TP_SPACE :
      // permuter la rotation automatique du modèle
      enmouvement = !enmouvement;
      break;

   default:
      std::cout << " touche inconnue : " << (char) touche << std::endl;
      break;
   }
}

// fonction callback pour un clic de souris
int dernierX = 0; // la dernière valeur en X de position de la souris
int dernierY = 0; // la derniere valeur en Y de position de la souris
static enum { deplaceCam, deplaceLumHauteur, deplaceLum } deplace = deplaceCam;
static bool pressed = false;
void FenetreTP::sourisClic( int button, int state, int x, int y )
{
   pressed = ( state == TP_PRESSED );
   if ( pressed )
   {
      // on vient de presser la souris
      dernierX = x;
      dernierY = y;
      switch ( button )
      {
      case TP_BUTTON_LEFT:
         deplace = deplaceCam;
         break;
      case TP_BUTTON_MIDDLE:
         deplace = deplaceLumHauteur;
         break;
      case TP_BUTTON_RIGHT:
         deplace = deplaceLum;
         break;
      }
   }
   else
   {
      // on vient de relacher la souris
   }
}

void FenetreTP::sourisWheel( int x, int y )
{
   const int sens = +1;
   facteurZ += 0.01 * sens * y;
   std::cout << " facteurZ=" << facteurZ << std::endl;
}

// fonction de mouvement de la souris
void FenetreTP::sourisMouvement( int x, int y )
{
   if ( pressed )
   {
      int dx = x - dernierX;
      int dy = y - dernierY;
      switch ( deplace )
      {
      case deplaceCam:
         thetaCam -= dx / 3.0;
         phiCam   -= dy / 3.0;
         break;
      case deplaceLumHauteur:
         posLumi[2] -= dy * 0.01;
         break;
      case deplaceLum:
         posLumi[1] += dx * 0.01;
         posLumi[0] += dy * 0.01;
         break;
      }

      dernierX = x;
      dernierY = y;

      verifierAngles();
   }
}

int main( int argc, char *argv[] )
{
   // créer une fenêtre
   FenetreTP fenetre( "INF2705 TP" );

   // allouer des ressources et définir le contexte OpenGL
   initialiser();

   bool boucler = true;
   while ( boucler )
   {
      // mettre à jour la physique
      calculerPhysique( );

      // affichage
      fenetre.afficherScene();
      fenetre.swap();

      // récupérer les événements et appeler la fonction de rappel
      boucler = fenetre.gererEvenement();
   }

   // détruire les ressources OpenGL allouées
   conclure();

   return 0;
}
