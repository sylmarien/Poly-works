// Prénoms, noms et matricule des membres de l'équipe:
// - Alexandre MAO (1813566)
// - Maxime SCHMITT (1719088)
// #warning "Écrire les prénoms, noms et matricule des membres de l'équipe dans le fichier et commenter cette ligne"

#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include "inf2705.h"
//#include "chargertex.h"
//#include "fenetreTP.h"
//#include "progNuanceur.h"
//#include "matrices.h"

// variables pour l'utilisation des nuanceurs
ProgNuanceur progNuanceur;      // votre programme de nuanceurs
ProgNuanceur progNuanceurBase;  // le programme de nuanceurs de base
GLuint vao[2];
GLuint vbo[2];
GLint locVertex;
GLint locColor;

// matrices de du pipeline graphique
MatricePipeline matrModel;
MatricePipeline matrVisu;
MatricePipeline matrProj;

// variables pour définir le point de vue
double thetaCam = 0.0;        // angle de rotation de la caméra (coord. sphériques)
double phiCam = 0.0;          // angle de rotation de la caméra (coord. sphériques)
double distCam = 0.0;         // distance (coord. sphériques)

// variables d'état
bool enPerspective = false;   // indique si on est en mode Perspective (true) ou Ortho (false)
bool enmouvement = false;     // le modèle est en mouvement/rotation automatique ou non
bool afficheAxes = true;      // indique si on affiche les axes
GLenum modePolygone = GL_FILL; // comment afficher les polygones

GLuint texturePOINT = 0;

struct Particule
{
   GLfloat position[3];          // en unités
   GLfloat vitesse[3];           // en unités/seconde
   GLfloat tempsDeVieRestant;    // en secondes
   GLubyte couleurUtilisee[4];   // Couleur utilisée dans le vao
   GLubyte couleur[4];           // couleur actuelle de la particule
   GLubyte couleurSelection[4];  // Couleur unique de la particule
   bool    estSelectionnee;      // la particule est actuellement sélectionnée
   // (vous pouvez ajouter d'autres éléments)
};
const unsigned int MAXNPARTICULES = 1000000;
Particule particules[MAXNPARTICULES]; // tableau de particules

unsigned int nparticules = 300;  // nombre de particules utilisées (actuellement affichées)
float tempsVieMax = 5;           // temps de vie maximal (en secondes)

float gravite = 0.5; // gravité utilisée dans le calcul de la position de la particule

const GLubyte COULMIN =  50; // valeur minimale d'une composante de couleur lorsque la particule (re)naît
const GLubyte COULMAX = 250; // valeur maximale d'une composante de couleur lorsque la particule (re)naît

const float positionPuits[3] = { 0, 0, 0.5 }; // position du puits de particules
const float bMin[] = { -1.25, -1.0, -0.5 };   // les valeurs minimales de la boite en x,y,z
const float bMax[] = {  1.25,  1.0,  2.0 };   // les valeurs maximales de la boite en x,y,z

bool modeSelection = false; // on est en mode sélection
int dernierX=0, dernierY=0;

bool transparenceSelonAge = false; // l'âge de la particule est utilisée pour la rendre transparente
bool couleurSelonNCollisions = false; // la couleur de la particules dépend du nombre de collisions

const float deltaT = 1./60.0;

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

// Valeur aléatoire entre 0.0 et 1.0
float myrandom()
{
   return (float) rand() / (float) RAND_MAX;
}

void initPart( Particule &part )
{
   part.position[0] = positionPuits[0];
   part.position[1] = positionPuits[1];
   part.position[2] = positionPuits[2];
   // ... autres valeurs à initialiser ...
   part.vitesse[0] = 2.0 * myrandom() - 1.0;
   part.vitesse[1] = 2.0 * myrandom() - 1.0;
   part.vitesse[2] = 2.0 * myrandom() - 1.0;

   part.tempsDeVieRestant = myrandom() * tempsVieMax;

   part.couleur[0] = COULMIN + myrandom() * (COULMAX - COULMIN);
   part.couleur[1] = COULMIN + myrandom() * (COULMAX - COULMIN);
   part.couleur[2] = COULMIN + myrandom() * (COULMAX - COULMIN);
   part.couleur[3] = COULMIN + myrandom() * (COULMAX - COULMIN);

   part.estSelectionnee = false;
}

void avancerParticules(unsigned int i)
{
   particules[i].tempsDeVieRestant -= deltaT;

   for (unsigned int j = 0 ; j < 3; j++)
   {
      // avancer la particule
      particules[i].position[j] += particules[i].vitesse[j] * deltaT;
   }
   // Mettre à jour la vitesse
   particules[i].vitesse[2] -= gravite * deltaT;

   for (unsigned int j = 0 ; j < 3; j++)
   {
      // vérifier les collisions
      if (particules[i].position[j] <= bMin[j] || particules[i].position[j] >= bMax[j])
      {
         particules[i].vitesse[j] = - particules[i].vitesse[j];
      }
   }

   if (particules[i].tempsDeVieRestant <= 0)
      initPart(particules[i]);
}

void donnerCouleur(unsigned int i)
{
   if (modeSelection)
   {
      particules[i].couleurUtilisee[0] = particules[i].couleurSelection[0];
      particules[i].couleurUtilisee[1] = particules[i].couleurSelection[1];
      particules[i].couleurUtilisee[2] = particules[i].couleurSelection[2];
   } else if (particules[i].estSelectionnee)
   {
      particules[i].couleurUtilisee[0] = 0;
      particules[i].couleurUtilisee[1] = 0;
      particules[i].couleurUtilisee[2] = 0;
   } else
   {
      particules[i].couleurUtilisee[0] = particules[i].couleur[0];
      particules[i].couleurUtilisee[1] = particules[i].couleur[1];
      particules[i].couleurUtilisee[2] = particules[i].couleur[2];
   }
}

void calculerPhysique( )
{
   for ( unsigned int i = 0 ; i < nparticules ; i++ )
   {
      if ( enmouvement && !modeSelection)
      {
         // déplacer les particules
         avancerParticules(i);
         donnerCouleur(i);
      } else
      {
         donnerCouleur(i);
      }
   }
}

void chargerNuanceurs()
{
   // charger le nuanceur de base
   progNuanceurBase.creer( );
   progNuanceurBase.attacherSourceSommets( "nuanceurSommetsBase.glsl" );
   progNuanceurBase.attacherSourceFragments( "nuanceurFragmentsBase.glsl" );
   progNuanceurBase.lier( );

   // charger le nuanceur
   progNuanceur.creer( );
   progNuanceur.attacherSourceSommets( "nuanceurSommets.glsl" );
   progNuanceur.attacherSourceGeometrie( "nuanceurGeometrie.glsl" );
   progNuanceur.attacherSourceFragments( "nuanceurFragments.glsl" );
   progNuanceur.lier( );
}

void initialiser()
{
   enmouvement = true;

   // donner la position de la caméra
   thetaCam = 270.0;
   phiCam = 80.0;
   distCam = 5.0;

   // donner la couleur de fond
   glClearColor( 0.0, 0.0, 0.0, 1.0 );

   // activer les etats openGL
   glEnable( GL_DEPTH_TEST );
   glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
   glPointSize( 5. );
   glEnable( GL_POINT_SMOOTH );

   // charger la texture du point
   ChargerTexture( "textures/spark.bmp", texturePOINT );

   // Initialisation des particules
   for ( unsigned int i = 0 ; i < MAXNPARTICULES ; i++ )
   {
      initPart( particules[i] );
      particules[i].couleurSelection[0] = i % 255;
      particules[i].couleurSelection[1] = (i/255) % 255;
      particules[i].couleurSelection[2] = (i/(255*255)) % 255;
   }

   // charger les nuanceurs
   chargerNuanceurs();

   glGenVertexArrays( 2, vao ); // générer deux VAOs
   glGenBuffers( 2, vbo );      // générer deux VBOs

   // Initialiser le vao pour le cube
   progNuanceurBase.utiliser( );
   glBindVertexArray( vao[0] );
   const GLfloat coo[] = { bMin[0], bMax[1], bMin[2],  bMax[0], bMax[1], bMin[2],  bMax[0], bMin[1], bMin[2],  bMin[0], bMin[1], bMin[2],
                           bMax[0], bMax[1], bMax[2],  bMin[0], bMax[1], bMax[2],  bMin[0], bMax[1], bMin[2],  bMax[0], bMax[1], bMin[2],
                           bMin[0], bMax[1], bMax[2],  bMin[0], bMin[1], bMax[2],  bMin[0], bMin[1], bMin[2],  bMin[0], bMax[1], bMin[2],
                           bMax[0], bMax[1], bMax[2],  bMax[0], bMin[1], bMax[2],  bMax[0], bMin[1], bMin[2],  bMax[0], bMax[1], bMin[2] };
   const GLubyte couleur[] = { 255, 255, 255,  255, 255, 255,  255, 255, 255,  255, 255, 255,
                               255, 255,   0,  255, 255,   0,  255, 255,   0,  255, 255,   0,
                               255,   0, 255,  255,   0, 255,  255,   0, 255,  255,   0, 255,
                               0, 255, 255,    0, 255, 255,    0, 255, 255,    0, 255, 255 };

   // remplir le VBO
   glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
   glBufferData( GL_ARRAY_BUFFER, sizeof(coo)+sizeof(couleur), NULL, GL_STATIC_DRAW );
   glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(coo), coo );
   glBufferSubData( GL_ARRAY_BUFFER, sizeof(coo), sizeof(couleur), couleur );
   // faire le lien avec les attributs du nuanceur de sommets
   GLint locVertexBase = progNuanceurBase.obtenirAttribLocation( "Vertex" );
   glVertexAttribPointer( locVertexBase, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0 );
   glEnableVertexAttribArray(locVertexBase);
   GLint locColorBase = progNuanceurBase.obtenirAttribLocation( "Color" );
   glVertexAttribPointer( locColorBase, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, (GLvoid *)sizeof(coo) );
   glEnableVertexAttribArray(locColorBase);
   glBindVertexArray( 0 );

   // Initialiser le vao pour les particules
   progNuanceur.utiliser( );
   glBindVertexArray( vao[1] );
   // remplir le VBO
   glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
   glBufferData( GL_ARRAY_BUFFER, sizeof(particules), particules, GL_STREAM_DRAW );
   // faire le lien avec les attributs du nuanceur de sommets
   GLint locVertex = progNuanceur.obtenirAttribLocation( "Vertex" );
   glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, sizeof(Particule), (GLvoid*)0 );
   glEnableVertexAttribArray(locVertex);
   GLint locColor = progNuanceur.obtenirAttribLocation( "Color" );
   glVertexAttribPointer( locColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Particule), (GLvoid *)(7 * sizeof(GLfloat)) );
   glEnableVertexAttribArray(locColor);
   glBindVertexArray( 0 );

   glBindBuffer( GL_ARRAY_BUFFER, 0 );

   FenetreTP::VerifierErreurGL("fin de initialiser");
}

void conclure()
{
   glUseProgram( 0 );
   glDeleteVertexArrays( 2, vao );
   glDeleteBuffers( 2, vbo );
}

void FenetreTP::afficherScene()
{
   // effacer l'ecran et le tampon de profondeur
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glPolygonMode( GL_FRONT_AND_BACK, modePolygone );

   progNuanceurBase.utiliser( );

   // définir le pipeline graphique
   matrProj.Perspective( 45.0, (GLdouble) largeur_ / (GLdouble) hauteur_, 0.1, 10.0 );
   progNuanceurBase.assignerUniformMatrix4fv( "matrProj", matrProj );

   matrVisu.LookAt( positionPuits[0]+distCam*cos(glm::radians(thetaCam))*sin(glm::radians(phiCam)),
                    positionPuits[1]+distCam*sin(glm::radians(thetaCam))*sin(glm::radians(phiCam)),
                    positionPuits[2]+distCam*cos(glm::radians(phiCam)),
                    positionPuits[0], positionPuits[1], positionPuits[2],
                    0, 0, 1 );
   progNuanceurBase.assignerUniformMatrix4fv( "matrVisu", matrVisu );

   matrModel.LoadIdentity();
   progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );

   // afficher les axes
   if ( afficheAxes ) FenetreTP::afficherAxes( 0.2 );

   // afficher la boîte
   glBindVertexArray( vao[0] );
   glDrawArrays( GL_QUADS, 0, 16 );

   // afficher les particules
   glActiveTexture( GL_TEXTURE0 ); // activer la texture '0' (valeur de défaut)
   glBindTexture(GL_TEXTURE_2D, texturePOINT);
   progNuanceur.utiliser( );
   progNuanceur.assignerUniformMatrix4fv( "matrProj", matrProj );
   progNuanceur.assignerUniformMatrix4fv( "matrVisu", matrVisu );
   progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );
   progNuanceur.assignerUniform1i( "laTexture", 0 ); // '0' => utilisation de GL_TEXTURE0
   progNuanceur.assignerUniform1i( "modeSelection", modeSelection );

   glBindVertexArray( vao[1] );
   // modifier le VBO pour donner les valeurs courantes des particules
   glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
   glBufferData( GL_ARRAY_BUFFER, sizeof(particules), particules, GL_STREAM_DRAW );
   glDrawArrays( GL_POINTS, 0, nparticules );

   // Sélection
   if (modeSelection) 
   {
      // s'assurer que toutes les opérations sont terminées
      glFinish();

      // obtenir la clôture et calculer la position demandée
      GLint cloture[4]; glGetIntegerv( GL_VIEWPORT, cloture );
      GLint posX = dernierX, posY = cloture[3]-dernierY;

      // dire de lire le tampon arrière où l'on vient tout juste de dessiner
      glReadBuffer( GL_BACK );

      // obtenir la couleur
      GLubyte couleur[3];
      glReadPixels( posX, posY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, couleur );

      GLubyte R = couleur[0];
      GLubyte G = couleur[1];
      GLubyte B = couleur[2];

      unsigned int i = R + G*255 + B*255*255;

      if (R != 255 && G != 255 && B != 255 && i < nparticules)
         particules[i].estSelectionnee = !particules[i].estSelectionnee;
   }
}

void FenetreTP::redimensionner( GLsizei w, GLsizei h )
{
   glViewport( 0, 0, w, h );
}

void ramenerSelectionnes()
{
   for (unsigned int i = 0; i < nparticules; i++)
   {
      if (particules[i].estSelectionnee)
         initPart(particules[i]);
   }
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

   case TP_SPACE:
      enmouvement = !enmouvement;
      break;

   case TP_g:
      modePolygone = ( modePolygone == GL_FILL ) ? GL_LINE : GL_FILL;
      break;

   case TP_PLUS:
   case TP_EQUALS:
      distCam -= 0.2;
      if ( distCam < 0.4 ) distCam = 0.4;
      std::cout << " distCam=" << distCam << std::endl;
      break;

   case TP_UNDERSCORE:
   case TP_MINUS:
      distCam += 0.2;
      std::cout << " distCam=" << distCam << std::endl;
      break;

   case TP_LEFTBRACKET:
      {
         unsigned int nparticulesPrec = nparticules;
         nparticules /= 1.2;
         if ( nparticules < 5 ) nparticules = 5;
         // on remet au puits les particules inutilisées
         for ( unsigned int i = nparticules ; i < nparticulesPrec ; ++i ) initPart( particules[i] );
      }
      std::cout << " nparticules=" << nparticules << std::endl;
      break;
   case TP_RIGHTBRACKET:
   case TP_m:
      nparticules *= 1.2;
      if ( nparticules > MAXNPARTICULES ) nparticules = MAXNPARTICULES;
      std::cout << " nparticules=" << nparticules << std::endl;
      break;
   case TP_b:
      gravite += 0.05;
      std::cout << " gravite=" << gravite << std::endl;
      break;
   case TP_h:
      gravite -= 0.05;
      if ( gravite < 0.0 ) gravite = 0.0;
      std::cout << " gravite=" << gravite << std::endl;
      break;
   case TP_l:
      tempsVieMax += 1.0;
      std::cout << " tempsVieMax=" << tempsVieMax << std::endl;
      break;
   case TP_k:
      tempsVieMax -= 1.0;
      if ( tempsVieMax < 1.0 ) tempsVieMax = 1.0;
      std::cout << " tempsVieMax=" << tempsVieMax << std::endl;
      break;
   case TP_p:
      ramenerSelectionnes();
      break;
   case TP_t:
      transparenceSelonAge = !transparenceSelonAge;
      break;
   case TP_c:
      couleurSelonNCollisions = !couleurSelonNCollisions;
      break;

   default:
      std::cout << " touche inconnue : " << (char) touche << std::endl;
      break;
   }
}

static bool pressed = false;
void FenetreTP::sourisClic( int button, int state, int x, int y )
{
   pressed = ( state == TP_PRESSED );
   if ( pressed )
   {
      modeSelection = ( button != TP_BUTTON_LEFT );
      dernierX = x;
      dernierY = y;
   }
   else
   {
      modeSelection = false;
   }
}

void FenetreTP::sourisWheel( int x, int y )
{
   //const int sens = +1;
}

void FenetreTP::sourisMouvement( int x, int y )
{
   if ( pressed )
   {
      if ( !modeSelection )
      {
         int dx = x - dernierX;
         int dy = y - dernierY;
         thetaCam -= dx / 3.0;
         phiCam   -= dy / 3.0;
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
      if ( modeSelection )
      {
         // la sélection a été faite
         modeSelection = pressed = false;
         // (pas d'appel à swap(): il n'est pas pertinent de montrer ce qu'on vient de tracer pour la sélection)
      }
      else
         fenetre.swap();

      // récupérer les événements et appeler la fonction de rappel
      boucler = fenetre.gererEvenement();
   }

   // détruire les ressources OpenGL allouées
   conclure();

   return 0;
}
