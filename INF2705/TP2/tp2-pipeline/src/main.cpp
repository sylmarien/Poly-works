// Prénoms, noms et matricule des membres de l'équipe:
// - Alexandre MAO (1813566)
// - Maxime SCHMITT (1719088)
//#warning "Écrire les prénoms, noms et matricule des membres de l'équipe dans le fichier et commenter cette ligne"

#include <iostream>
#include <GL/glew.h>
#include "fenetreSDL.h"
#include "shapes.h"
#include "teapot_data.h"
#include "progNuanceur.h"
#include "matrices.h"

// variables pour l'utilisation des nuanceurs
ProgNuanceur progNuanceur;      // votre programme de nuanceurs
ProgNuanceur progNuanceurBase;  // le programme de nuanceurs de base

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

// partie 1:
double thetaBras = 0.0;   // angle de rotation du bras
double phiBras = 0.0;     // angle de rotation du bras
double angleTheiere = 0.0;

// partie 2:
GLuint g_VBOsommets = 0;
GLuint g_VBOconnec = 0;

bool modeLookAt = true;

GLdouble thetaInit = 0., phiInit = 60., distInit = 10.;

void chargerNuanceurs()
{
   // charger le nuanceur de base
   progNuanceurBase.creer( );
   progNuanceurBase.attacherSourceSommets( "nuanceurSommetsBase.glsl" );
   progNuanceurBase.attacherSourceFragments( "nuanceurFragmentsBase.glsl" );
   progNuanceurBase.lier( );
}

void initialiser()
{
   // positionnement de la caméra: angle et distance de la caméra à la base du bras
   thetaCam = thetaInit;
   phiCam = phiInit;
   distCam = distInit;

   // donner la couleur de fond
   glClearColor( 0.0, 0.0, 0.0, 1.0 );

   // activer les etats openGL
   glEnable( GL_DEPTH_TEST );

   // charger les nuanceurs
   chargerNuanceurs();

   
   // créer le VBO pour les sommets
   // ...
   //génération du vbo pour les sommets
   glGenBuffers(1, &g_VBOsommets);
   glBindBuffer(GL_ARRAY_BUFFER, g_VBOsommets);
   glBufferData(GL_ARRAY_BUFFER, sizeof(gTeapotSommets), gTeapotSommets, GL_STATIC_DRAW);
   
   // créer le VBO la connectivité
   // ...
   //génration du vbo pour la connectivité
   glGenBuffers(1, &g_VBOconnec);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_VBOconnec);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gTeapotConnec), gTeapotConnec, GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void conclure()
{
   glDeleteBuffers( 1, &g_VBOsommets );
   glDeleteBuffers( 1, &g_VBOconnec );
}

// (partie 1) Vous devez vous servir de ces deux fonctions (sans les modifier) pour tracer les quadriques.
void afficherCylindre( )
{
   // affiche un cylindre de rayon 1 et de hauteur 1
   const GLint slices = 16, stack = 2;
   glColor3f( 0, 0, 1 );
   shapesSolidCylinder( 1.0, 1.0, slices, stack );
}
void afficherSphere( )
{
   // affiche une sphere de rayon 1
   const GLint slices = 16, stack = 32;
   glColor3f( 1, 0, 0 );
   shapesSolidSphere( 1.0, slices, stack );
}

// (partie 2) Vous modifierez cette fonction pour utiliser les VBOs
void afficherTheiere()
{
   glColor3f( 0.0, 1.0, 0.0 );

   // voici pouvez utiliser temporairement cette fonction pour la première partie du TP, mais vous ferez mieux dans la seconde partie du TP
   //shapesSolidTeapot( );
   
   matrModel.Rotate(90, 1.0, 0.0, 0.0);
   matrModel.Scale(0.75, 0.75, 0.75);
   
   progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );
   
   glBindBuffer(GL_ARRAY_BUFFER, g_VBOsommets);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_VBOconnec);

   GLint attribLocationNuanceur = progNuanceurBase.obtenirAttribLocation("Vertex");

   glVertexAttribPointer(attribLocationNuanceur, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glEnableVertexAttribArray(attribLocationNuanceur);
   glDrawElements(GL_TRIANGLES, sizeof(gTeapotConnec)/sizeof(GLfloat), GL_UNSIGNED_INT, 0);
   glDisableVertexAttribArray(attribLocationNuanceur);
   
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   
}

void afficherBras()
{
   const GLfloat cylhauteur = 2.0;
   const GLfloat sphererayon = 0.25;

   matrModel.PushMatrix();
   {
     // Afficher le premier cylindre
     matrModel.Scale(sphererayon, sphererayon, cylhauteur);
     progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );
     afficherCylindre();
   }
   matrModel.PopMatrix();
   
   // Se positionner pour la première sphère
   matrModel.Translate( 0.0, 0.0, cylhauteur + 0.4*sphererayon);
   matrModel.PushMatrix();
   {
     // Afficher la première sphère
     matrModel.Scale(sphererayon, sphererayon, sphererayon);
     progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );
     afficherSphere();
   }
   matrModel.PopMatrix();

   // Rotation du deuxième cylindre
   matrModel.Rotate(phiBras, 0.0, 0.0, 1.0);
   
   // Afficher le deuxième cylindre
   matrModel.Rotate(90, 0.0, 1.0, 0.0);
   matrModel.Translate(0.0, 0.0, 0.4*sphererayon);
   matrModel.PushMatrix();
   {
     matrModel.Scale(sphererayon, sphererayon, cylhauteur);
     progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );
     afficherCylindre();
   }
   matrModel.PopMatrix();
   
   // Afficher la deuxième sphère
   matrModel.Translate(0.0, 0.0, cylhauteur + 0.4*sphererayon);
   matrModel.PushMatrix();
   {
     matrModel.Scale(sphererayon, sphererayon, sphererayon);
     progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );
     afficherSphere();
   }
   matrModel.PopMatrix();
   matrModel.Translate(0.0, 0.0, 0.4*sphererayon);
   
   // Rotation du troisième cylindre
   matrModel.Rotate(-thetaBras, 1.0, 0.0, 0.0);
   matrModel.PushMatrix();
   {
     // Afficher troisième cylindre
     matrModel.Scale(sphererayon, sphererayon, cylhauteur);
     progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );
     afficherCylindre();
   }
   // Afficher la troisème et dernière sphère
   matrModel.PopMatrix();
   matrModel.Translate(0.0, 0.0, cylhauteur + 0.4*sphererayon);
   matrModel.PushMatrix();
   {
     matrModel.Scale(sphererayon, sphererayon, sphererayon);
     progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );
     afficherSphere();
   }
   matrModel.PopMatrix();
   
   // Remise du repère dans l'orientation initiale pour l'affichage de la théière
   matrModel.Rotate(-90, 0.0, 1.0, 0.0);

   // Rotation de la théière
   matrModel.Rotate(angleTheiere, 0.0, 1.0, 0.0);
   
   // afficher la théière
   matrModel.Scale(sphererayon, sphererayon, sphererayon);
   matrModel.Translate(3.35, 0.0, -1.5);
   progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );
   afficherTheiere();
}

void definirCamera()
{
   if ( modeLookAt )
   {
      matrVisu.LookAt( distCam*cos(glm::radians(thetaCam))*sin(glm::radians(phiCam)), distCam*sin(glm::radians(thetaCam))*sin(glm::radians(phiCam)), cos(glm::radians(phiCam))*distCam, 0, 0, 0,  0, 0, 1 );
   }
   else
   {
      matrVisu.LoadIdentity();
      matrVisu.Translate(0.0, 0.0, -distCam);
      matrVisu.Rotate(-90, 0.0, 0.0, 1.0);
      matrVisu.Rotate(-phiCam, 0.0, 1.0, 0.0);
      matrVisu.Rotate(-thetaCam, 0.0, 0.0, 1.0);
      // matrModel.Translate(), matrModel.Rotate(), ...
   }
}

void FenetreSDL::afficherScene()
{
   // effacer l'ecran et le tampon de profondeur
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glPolygonMode( GL_FRONT_AND_BACK, modePolygone );

   progNuanceurBase.utiliser( );

   // définir le pipeline graphique
   matrProj.Perspective( 45.0, (GLdouble) largeur_ / (GLdouble) hauteur_, 0.1, 300.0 );
   progNuanceurBase.assignerUniformMatrix4fv( "matrProj", matrProj );

   definirCamera();
   progNuanceurBase.assignerUniformMatrix4fv( "matrVisu", matrVisu );

   matrModel.LoadIdentity();
   progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );

   // afficher les axes
   if ( afficheAxes ) FenetreSDL::afficherAxes();

   glColor3f( 1., .5, .5 );
   glBegin( GL_QUADS );
   glVertex3f( -4.0,  4.0, 0.0 );
   glVertex3f(  4.0,  4.0, 0.0 );
   glVertex3f(  4.0, -4.0, 0.0 );
   glVertex3f( -4.0, -4.0, 0.0 );
   glEnd();

   afficherBras();
}

void FenetreSDL::redimensionner( GLsizei w, GLsizei h )
{
   glViewport( 0, 0, w, h );
}

void FenetreSDL::clavier( SDL_Keycode touche )
{
   switch ( touche )
   {
   case SDLK_ESCAPE:
   case SDLK_q:
      {
         SDL_Event sdlevent; sdlevent.type = SDL_QUIT;
         SDL_PushEvent( &sdlevent );
      }
      break;

   case SDLK_x: // permutation de l'affichage des axes
      afficheAxes = !afficheAxes;
      std::cout << "// Affichage des axes ? " << ( afficheAxes ? "OUI" : "NON" ) << std::endl;
      break;

   case SDLK_v: // Recharger les nuanceurs
      chargerNuanceurs();
      std::cout << "// Recharger nuanceurs" << std::endl;
      break;

   case SDLK_MINUS:
   case SDLK_UNDERSCORE:
      distCam += 0.1;
      break;
   case SDLK_PLUS:
   case SDLK_EQUALS:
      if ( distCam > 1.0 )
         distCam -= 0.1;
      break;
   case SDLK_r:
      phiCam = phiInit; thetaCam = thetaInit; distCam = distInit;
      break;
   case SDLK_l:
      modeLookAt = !modeLookAt;
      std::cout << " modeLookAt=" << modeLookAt << std::endl;
      break;
   case SDLK_g:
      modePolygone = ( modePolygone == GL_FILL ) ? GL_LINE : GL_FILL;
      break;
   case SDLK_LEFT:
      phiBras -= 2.0;
      break;
   case SDLK_RIGHT:
      phiBras += 2.0;
      break;
   case SDLK_DOWN:
      if (thetaBras > 0)
	thetaBras -= 2.0;
      break;
   case SDLK_UP:
      if (thetaBras < 90)
	thetaBras += 2.0;
      break;
   case SDLK_LEFTBRACKET:
      if (angleTheiere > 0)
	angleTheiere -= 1.0;
      break;
   case SDLK_RIGHTBRACKET:
      if (angleTheiere < 45)
	angleTheiere += 1.0;
      break;
   }
}

int dernierX, dernierY;
static bool pressed = false;
void FenetreSDL::sourisClic( int button, int state, int x, int y )
{
   // button est un parmi { SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, SDL_BUTTON_RIGHT }
   // state  est un parmi { SDL_PRESSED, DL_RELEASED }
   pressed = ( state == SDL_PRESSED );
   if ( pressed )
   {
      dernierX = x;
      dernierY = y;
   }
}

void FenetreSDL::sourisWheel( int x, int y )
{
   //const int sens = +1;
}

void FenetreSDL::sourisMouvement( int x, int y )
{
   if (pressed)
   {
      if (dernierX - x != 0)
         thetaCam += (dernierX - x);
      if (dernierY - y != 0)
         phiCam += (dernierY - y);
      thetaCam = thetaCam < -90 ? -90 : thetaCam;
      thetaCam = thetaCam > 90 ? 90 : thetaCam;
      phiCam = phiCam < 1 ? 1 : phiCam;
      phiCam = phiCam > 179 ? 179 : phiCam;
      dernierX = x;
      dernierY = y;
   }
}

int main( int argc, char *argv[] )
{
   // créer une fenêtre SDL
   FenetreSDL fenetre( "TP2" );

   // allouer des ressources et définir le contexte OpenGL
   initialiser();

   bool boucler = true;
   while ( boucler )
   {
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
