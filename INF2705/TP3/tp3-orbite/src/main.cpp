// Prénoms, noms et matricule des membres de l'équipe:
// - Maxime SCHMITT (1719088)
// - Alexandre MAO (1813566)
// #warning "Écrire les prénoms, noms et matricule des membres de l'équipe dans le fichier et commenter cette ligne"

#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include "fenetreSDL.h"
#include "shapes.h"
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
bool enmouvement = true;     // le modèle est en mouvement/rotation automatique ou non
bool afficheAxes = false;      // indique si on affiche les axes
bool coulProfondeur = false;  // indique si on veut colorer selon la profondeur
GLenum modePolygone = GL_FILL; // comment afficher les polygones

class CorpsCeleste
{
public:
   float rayon; // le rayon
   float distance; // la distance au soleil
   float rotation; // l'angle actuel de rotation en degrés
   float revolution; // l'angle actuel de révolution
   float incrRotation; // l'incrément à ajouter à chaque appel de la fonction calculerPhysique
   float incrRevolution; // l'incrément à ajouter à chaque appel de la fonction calculerPhysique
   CorpsCeleste( float r, float dist, float rot, float rev, float incrRot, float incrRev ) :
      rayon(r), distance(dist),
      rotation(rot), revolution(rev),
      incrRotation(incrRot), incrRevolution(incrRev)
   { }
};

CorpsCeleste Soleil( 4.0, 0.0, 5.0, 0.0, 0.01, 0.0 );
CorpsCeleste Terre( 0.5, 8.0, 20.0, 45.0, 0.5, 0.005 );
CorpsCeleste Lune( 0.2, 1.0, 20.0, 30.0, 0.5, 0.07 );
CorpsCeleste Jupiter( 1.2, 14.0, 10.0, 30.0, 0.03, 0.003 );
CorpsCeleste Europa( 0.2, 1.5, 5.0, 15.0, 0.7, 0.09 );
CorpsCeleste Callisto( 0.3, 2.0, 10.0, 2.0, 0.8, 0.7 );


int modele = 1; // le modèle à afficher (1-sphère, 2-cube, 3-théière). On change avec ';'.

int vueCourante = 0; // 0 pour la vue exoplanétaire; 1 pour la vue à partir du pôle Nord

// partie 1: utiliser un plan de coupe
glm::vec4 planCoupe( 0, 0, 1, 0 ); // équation du plan de coupe

// partie 3: utiliser un facteur de réchauffement
float facteurRechauffement = 0.2; // le facteur qui sert à calculer la couleur des pôles (0.0=froid, 1.0=chaud)

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
      // incrémenter rotation[] et revolution[] pour faire tourner les planètes
     Terre.rotation += Terre.incrRotation;
     Terre.revolution += Terre.incrRevolution;
     Lune.rotation += Lune.incrRotation;
     Lune.revolution += Lune.incrRevolution;
     Jupiter.rotation += Jupiter.incrRotation;
     Jupiter.revolution += Jupiter.incrRevolution;
     Europa.rotation += Europa.incrRotation;
     Europa.revolution += Europa.incrRevolution;
     Callisto.rotation += Callisto.incrRotation;
     Callisto.revolution += Callisto.incrRevolution;
   }
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
   progNuanceur.attacherSourceFragments( "nuanceurFragments.glsl" );
   progNuanceur.lier( );
}

void initialiser()
{
   enmouvement = true;

   thetaCam = 60.0;
   phiCam = 75.0;
   distCam = 26.0;

   // donner la couleur de fond
   glClearColor( 0.1, 0.1, 0.1, 1.0 );

   // activer les etats openGL
   glEnable( GL_DEPTH_TEST );

   // charger les nuanceurs
   chargerNuanceurs();
}

void conclure()
{
}

void observerDeLaTerre( )
{
	// placer le point de vue pour voir de la Terre
	matrModel.PushMatrix(); {
		matrModel.Rotate(Terre.revolution, 0.0, 0.0, 1.0);
		matrModel.Translate(Terre.distance, 0.0, 0.0);
		matrModel.Rotate(Terre.rotation, 0.0, 0.0, 1.0);
		matrModel.Translate(0.0, 0.0, Terre.rayon + 0.05);
		matrModel.Rotate(90, 1.0, 0.0, 0.0);
		matrVisu.setMatr(glm::inverse(matrModel.getMatr()));
	} matrModel.PopMatrix();
}

void definirCamera()
{
   if ( vueCourante == 0 )
   {
      // La souris influence le point de vue
      matrVisu.LookAt( distCam*cos(glm::radians(thetaCam))*sin(glm::radians(phiCam)),
                       distCam*sin(glm::radians(thetaCam))*sin(glm::radians(phiCam)),
                       distCam*cos(glm::radians(phiCam)),
                       0, 0, 0,
                       0, 0, 1 );
   }
   else
   {
      // La caméra est sur la Terre et voir passer les autres objets célestes en utilisant l'inverse de la matrice mm
      // partie 2: modifs ici ...
      observerDeLaTerre( );
   }
}

void afficherCorpsCeleste( GLfloat rayon )
{
   switch ( modele )
   {
   case 1:
      shapesSolidSphere( rayon, 16, 16 );
      break;
   case 2:
      shapesSolidCube( 2*rayon );
      break;
   case 3:
   default:
      matrModel.PushMatrix(); {
         matrModel.Scale( 0.4*rayon, 0.4*rayon, 0.4*rayon );
         progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );
         shapesSolidTeapot( );
      } matrModel.PopMatrix(); progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );
      break;
   }
}

void afficherDisque( GLfloat distance, GLfloat r, GLfloat g, GLfloat b )
{
   glColor3f( r, g, b );
   shapesSolidTorus( 0.1, distance, 4, 64 );
}

void afficherQuad( GLfloat a ) // le plan qui ferme les solides
{
   glColor4f( 0.0, 1.0, 0.0, a );
   GLfloat coo[ 4*3 ] = { -20, -20, -planCoupe[3],
                           -20, 20, -planCoupe[3],
                           20, 20, -planCoupe[3],
                           20, -20, -planCoupe[3]
                        };

   GLint loc = progNuanceurBase.obtenirAttribLocation( "Vertex" );
   glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, coo );
   glEnableVertexAttribArray(loc);

   glDrawArrays(GL_QUADS, 0, 4);

   glDisableVertexAttribArray(loc);
}

void afficherModele()
{
    // afficher le système solaire
    matrModel.PushMatrix( ); {
    
    // affichage de base
     
    // afficher les deux disques pour les orbites de la Terre et de Jupiter
	 afficherDisque( Terre.distance, 0.0, 0.0, 1.0 );
	 afficherDisque( Jupiter.distance, 1.0, 0.0, 0.0 );

    // afficher les CorpsCeleste

    // La Terre et la Lune.
	 matrModel.PushMatrix(); {
		matrModel.Rotate(Terre.revolution, 0.0, 0.0, 1.0);
	 	matrModel.Translate(Terre.distance, 0, 0);
	 	matrModel.PushMatrix(); {
	 		matrModel.Rotate(Terre.rotation, 0.0, 0.0, 1.0);
	 		progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );
         		glColor3f( 0.5, 0.5, 1.0 );
         		afficherCorpsCeleste(Terre.rayon);
	 	} matrModel.PopMatrix();

      // La Lune.
		matrModel.Rotate(Lune.revolution, 0.0, 0.0, 1.0);
	 	matrModel.Translate(Lune.distance, 0.0, 0.0);
		matrModel.Rotate(Lune.rotation, 0.0, 0.0, 1.0);
	 	progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );
         	glColor3f( 0.6, 0.6, 0.6 );
         	afficherCorpsCeleste( Lune.rayon );
	 } matrModel.PopMatrix();


    // Jupiter et ses satellites.
	 matrModel.PushMatrix(); {
	        matrModel.Rotate(Jupiter.revolution, 0.0, 0.0, 1.0 );
	 	matrModel.Translate( Jupiter.distance, 0, 0 ); 
		matrModel.PushMatrix(); {
			matrModel.Rotate(Jupiter.rotation, 0.0, 0.0, 1.0);
	 		progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );
         		glColor3f( 1.0, 0.5, 0.5 );
         		afficherCorpsCeleste( Jupiter.rayon );
		} matrModel.PopMatrix();

      // Europa.
	 	matrModel.PushMatrix(); {
		  	matrModel.Rotate(Europa.revolution, 0.0, 0.0, 1.0 );
		  	matrModel.Translate(Europa.distance, 0, 0 );
			matrModel.Rotate(Europa.rotation, 0.0, 0.0, 1.0 );
		  	progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );
		  	glColor3f( 0.4, 0.4, 0.8 );
		  	afficherCorpsCeleste( Europa.rayon );
	 	} matrModel.PopMatrix();

      // Callisto
		matrModel.Rotate(Callisto.revolution, 0.0, 0.0, 1.0 );
	 	matrModel.Translate( Callisto.distance, 0, 0 );
		matrModel.Rotate(Callisto.rotation, 0.0, 0.0, 1.0 );
	 	progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );
         	glColor3f( 0.5, 0.5, 0.1 );
         	afficherCorpsCeleste( Callisto.rayon );
	 } matrModel.PopMatrix();
	 
   } matrModel.PopMatrix();
   progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );

    // Le Soleil, en dernier pour la transparence.
    progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );
    glDisable( GL_DEPTH_TEST );
    glEnable( GL_BLEND );
    glColor4f( 1.0, 1.0, 0.0, 0.5 );
    // On fixe le facteur de réchauffement pour que la variation de température n'ait pas d'influence sur le Soleil.
    progNuanceur.assignerUniform1f( "facteurRechauffement", 1.0);
    afficherCorpsCeleste( Soleil.rayon );
    progNuanceur.assignerUniform1f( "facteurRechauffement", facteurRechauffement );
    glDisable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );
}

void FenetreSDL::afficherScene( )
{
   // effacer l'ecran et le tampon de profondeur et le stencil
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

   progNuanceurBase.utiliser( );

   // définir le pipeline graphique
   matrProj.Perspective( 70.0, (GLdouble) largeur_ / (GLdouble) hauteur_, 0.1, 100.0 );
   progNuanceurBase.assignerUniformMatrix4fv( "matrProj", matrProj );

   definirCamera( );
   progNuanceurBase.assignerUniformMatrix4fv( "matrVisu", matrVisu );

   matrModel.LoadIdentity();
   progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );

   // afficher les axes
   if ( afficheAxes ) FenetreSDL::afficherAxes();

   // dessiner la scène
   progNuanceur.utiliser( );
   progNuanceur.assignerUniformMatrix4fv( "matrProj", matrProj );
   progNuanceur.assignerUniformMatrix4fv( "matrVisu", matrVisu );
   progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );
   progNuanceur.assignerUniform1f( "facteurRechauffement", facteurRechauffement );
   progNuanceur.assignerUniform4fv( "planCoupe", planCoupe );


   // afficher le modèle et tenir compte du stencil et du plan de coupe
   glEnable(GL_STENCIL_TEST);

   // Plan de coupe
   glEnable( GL_CLIP_PLANE0 );

   // Paramétrer le stencil
   glStencilFunc(GL_ALWAYS, 0, 1);
   glStencilOp(GL_KEEP, GL_INCR, GL_INCR);

   afficherModele();

   glStencilFunc(GL_EQUAL, 1, 1); // TESTE seulement les pairs pas les 1, donc artefacts quand on fait avec des theieres.
   glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

   glDisable( GL_CLIP_PLANE0 );
   // On fixe le facteur de réchauffement pour que la variation de température n'ait pas d'influence sur le plan.
   progNuanceur.assignerUniform1f( "facteurRechauffement", 1.0);
   afficherQuad( 1.0 );
   progNuanceur.assignerUniform1f( "facteurRechauffement", facteurRechauffement );
   glDisable(GL_STENCIL_TEST);

   // en plus, dessiner le plan en transparence pour bien voir son étendue
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   // On fixe le facteur de réchauffement pour que la variation de température n'ait pas d'influence sur le plan.
   progNuanceur.assignerUniform1f( "facteurRechauffement", 1.0);
   afficherQuad( 0.1 );
   progNuanceur.assignerUniform1f( "facteurRechauffement", facteurRechauffement );
   glDisable(GL_BLEND);
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

   case SDLK_SPACE:
      enmouvement = !enmouvement;
      break;

   case SDLK_c:
      facteurRechauffement += 0.05; if ( facteurRechauffement > 1.0 ) facteurRechauffement = 1.0;
      std::cout << " facteurRechauffement=" << facteurRechauffement << " " << std::endl;
      break;
   case SDLK_f:
      facteurRechauffement -= 0.05; if ( facteurRechauffement < 0.0 ) facteurRechauffement = 0.0;
      std::cout << " facteurRechauffement=" << facteurRechauffement << " " << std::endl;
      break;

   case SDLK_0: // point de vue globale/externe
   case SDLK_1: // point de vue situé au pôle Nord
   case SDLK_2: // ... autres point de vue
   case SDLK_3:
   case SDLK_4:
   case SDLK_5:
   case SDLK_6:
   case SDLK_7:
   case SDLK_8:
   case SDLK_9:
      vueCourante = touche - SDLK_0;
      break;

   case SDLK_g:
      modePolygone = ( modePolygone == GL_FILL ) ? GL_LINE : GL_FILL;
      glPolygonMode( GL_FRONT_AND_BACK, modePolygone );
      break;

   case SDLK_PLUS:
   case SDLK_EQUALS:
      distCam--;
      std::cout << " distCam=" << distCam << std::endl;
      break;

   case SDLK_UNDERSCORE:
   case SDLK_MINUS:
      distCam++;
      std::cout << " distCam=" << distCam << std::endl;
      break;

   case SDLK_RIGHTBRACKET:
   case SDLK_GREATER:
      planCoupe[3] += 0.1;
      std::cout << " planCoupe[3]=" << planCoupe[3] << std::endl;
      break;
   case SDLK_LEFTBRACKET:
   case SDLK_LESS:
      planCoupe[3] -= 0.1;
      std::cout << " planCoupe[3]=" << planCoupe[3] << std::endl;
      break;

   case SDLK_SEMICOLON:
      if ( ++modele > 3 ) modele = 1;
      std::cout << " modele=" << modele << std::endl;
      break;

   case SDLK_p:
      coulProfondeur = !coulProfondeur;
      progNuanceur.assignerUniform1i( "coulProfondeur", coulProfondeur );
      break;

   }
}

int dernierX = 0, dernierY = 0;
static bool pressed = false;
void FenetreSDL::sourisClic( int button, int state, int x, int y )
{
   pressed = ( state == SDL_PRESSED );
   if ( pressed )
      if ( button == SDL_BUTTON_LEFT )
      {
         dernierX = x;
         dernierY = y;
      }
}

void FenetreSDL::sourisWheel( int x, int y )
{
   const int sens = +1;
   planCoupe[3] += 0.02 * sens * y;
   std::cout << " planCoupe[3]=" << planCoupe[3] << std::endl;
}

void FenetreSDL::sourisMouvement( int x, int y )
{
   if ( pressed )
   {
      int dx = x - dernierX;
      int dy = y - dernierY;
      thetaCam -= dx / 3.0;
      phiCam   -= dy / 3.0;

      dernierX = x;
      dernierY = y;

      verifierAngles();
   }
}

int main( int argc, char *argv[] )
{
   // créer une fenêtre SDL
   FenetreSDL fenetre( "TP3" );

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
