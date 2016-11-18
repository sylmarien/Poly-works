// Prénoms, noms et matricule des membres de l'équipe:
// - Alexandre MAO (1813566)
// - Maxime SCHMITT (1719088)
//#warning "Écrire les prénoms, noms et matricule des membres de l'équipe dans le fichier et commenter cette ligne"

#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include "chargertex.h"
#include "fenetreTP.h"
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
bool enmouvement = false;     // le modèle est en mouvement/rotation automatique ou non
bool afficheAxes = true;      // indique si on affiche les axes
GLenum modePolygone = GL_FILL; // comment afficher les polygones

////////////////////////////////////////
// déclaration des variables globales //
////////////////////////////////////////

// partie 1: illumination
GLfloat spotAngle = 15.0;        // l'angle du cône du spot
GLfloat spotExposant = 1.0;      // l'exposant du cône
int modele = 1;                  // le modèle à afficher
int typeIllumination = 1;        // 0:Gouraud, 1:Phong
bool utiliseBlinn = false;       // indique si on veut utiliser modèle spéculaire de Blinn ou Phong
bool utiliseDirect = false;      // indique si on utilise un spot style Direct3D ou OpenGL
bool localViewer = false;        // doit-on prendre en compte la position de l''observateur?

// partie 2: texture
int texnumero = 0;               // numéro de la texture appliquée
int texwrapmode = 0;             // paramètres de répétition de la texture
bool utiliseCouleur = true;      // doit-on utiliser la couleur de base de l'objet en plus de celle de la texture?
bool noirTransparent = false;    // les pixels noirs doivent-ils être transparents?
GLuint textureDE = 0;
GLuint textureECHIQUIER = 0;

// définition des lumières
glm::vec4 lumiAmbiant( 1.0, 1.0, 1.0, 1.0 );
glm::vec4 lumiDiffuse( 1.0, 1.0, 1.0, 1.0 );
glm::vec4 lumiSpeculaire( 1.0, 1.0, 1.0, 1.0 );
glm::vec4 spotPosition( 4, 1, 15, 1.0 );
glm::vec3 spotDirection( -5.0, -2.0, -10.0 );

// définition du matériau
glm::vec4 mat_ambiant( 0.1, 0.1, 0.1, 1.0 );
glm::vec4 mat_diffuse( 1.0, 0.1, 1.0, 1.0 );
glm::vec4 mat_specular( 1.0, 1.0, 1.0, 1.0 );
glm::vec4 mat_emission( 0.0, 0.0, 0.0, 1.0 );
GLfloat mat_shininess = 100.0;

void verifierAngles()
{
   if ( thetaCam > 360.0 )
      thetaCam -= 360.0;
   else if ( thetaCam < 0.0 )
      thetaCam += 360.0;

   const GLdouble MINPHI = -90.0, MAXPHI = 90.0;
   if ( phiCam > MAXPHI )
      phiCam = MAXPHI;
   else if ( phiCam < MINPHI )
      phiCam = MINPHI;
}

void calculerPhysique( )
{
   if ( enmouvement )
   {
      static int sensTheta = 1;
      static int sensPhi = 1;
      thetaCam += 0.5 * sensTheta;
      phiCam += 1.0 * sensPhi;
      //if ( thetaCam <= 0. || thetaCam >= 360.0 ) sensTheta = -sensTheta;
      if ( phiCam < -90.0 || phiCam > 90.0 ) sensPhi = -sensPhi;

      static int sensAngle = 1;
      spotAngle += sensAngle * 0.3;
      if ( spotAngle < 1.0 ) sensAngle = -sensAngle;
      if ( spotAngle > 60.0 ) sensAngle = -sensAngle;

#if 0
      static int sensExposant = 1;
      spotExposant += sensExposant * 0.3;
      if ( spotExposant < 1.0 ) sensExposant = -sensExposant;
      if ( spotExposant > 10.0 ) sensExposant = -sensExposant;
#endif
   }

   verifierAngles();
}

void chargerToutesLesTextures()
{
   ChargerTexture( "textures/de.bmp", textureDE );
   ChargerTexture( "textures/echiquier.bmp", textureECHIQUIER );
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

// initialisation d'openGL
void initialiser()
{
   // donner l'orientation du modèle
   thetaCam = 0.0;
   phiCam = 0.0;
   distCam = 30.0;

   // couleur de l'arrière-plan
   glClearColor( 0.4, 0.2, 0.0, 1.0 );

   // activer les etats openGL
   glEnable( GL_DEPTH_TEST );

   // charger les textures
   chargerToutesLesTextures();

   // charger les nuanceurs
   chargerNuanceurs();
}

void conclure()
{
}

void afficherModele()
{
   // partie 2: paramètres de texture
   // Tableau de coordonnées de textures
   GLfloat texcoord1[2*4*6] =
   {
      0.666, 0.333,    1.0, 0.333,     1.0, 0.0,      0.666, 0.0,  // face du fond = le 6
      0.666, 0.333,    0.333, 0.333,    0.333, 0.0,     0.666, 0.0,  // face du dessous = le 3
      0.666, 0.666,    0.666, 0.333,    1.0, 0.333,     1.0, 0.666,  // face de droite = le 5
      0.333, 0.666,    0.666, 0.666,    0.666, 1.0,     0.333, 1.0,  // face du dessus = le 4
      0.333, 0.333,    0.333, 0.666,    0.0, 0.666,     0.0, 0.333,  // face de gauche = le 2
      0.333, 0.333,    0.666, 0.333,    0.666, 0.666,    0.333, 0.666  // face de devant = le 1
   };
   GLfloat texcoord2[2*4*6] =
   {
      2.0, 2.0,      -1.0, 2.0,     -1.0, -1.0,    2.0, -1.0,
      2.0, 2.0,      -1.0, 2.0,     -1.0, -1.0,    2.0, -1.0,
      2.0, -1.0,     2.0, 2.0,      -1.0, 2.0,     -1.0, -1.0,//    2.0, -1.0,
      2.0, 2.0,      -1.0, 2.0,     -1.0, -1.0,    2.0, -1.0,
      2.0, -1.0,     2.0, 2.0,      -1.0, 2.0,     -1.0, -1.0,//    2.0, -1.0,
      2.0, 2.0,      -1.0, 2.0,     -1.0, -1.0,    2.0, -1.0
   };

   GLfloat* texcoord;
   switch ( texnumero )
   {
   default:
      //std::cout << "Sans texture" << std::endl;
      glBindTexture(GL_TEXTURE_2D, 0);
      break;
   case 1:
      //std::cout << "Texture DE" << std::endl;
      glBindTexture(GL_TEXTURE_2D, textureDE);
      texcoord = texcoord1;
      break;
   case 2:
      //std::cout << "Texture ECHIQUIER" << std::endl;
      glBindTexture(GL_TEXTURE_2D, textureECHIQUIER);
      texcoord = texcoord2;
      break;
   }

   if ( texnumero) // si on utilise une texture
   {
      //std::cout << " texwrapmode=" << texwrapmode << std::endl;
      GLint locTexCoo = progNuanceur.obtenirAttribLocation( "MultiTexCoord0" );
      glVertexAttribPointer( locTexCoo, 2, GL_FLOAT, GL_FALSE, 0, texcoord );
      glEnableVertexAttribArray(locTexCoo);

      if (texnumero == 2) {
         switch (texwrapmode)
         {
            case 0:
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
               break;
            case 1:
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
               break;
            case 2:
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
               break;
            case 3:
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
               break;
         }
      }
   }

   // Dessiner le modèle
   matrModel.PushMatrix(); {

      // tourner le modèle
      matrModel.Rotate( phiCam, -1.0, 0.0, 0.0 );
      matrModel.Rotate( thetaCam, 0.0, -1.0, 0.0 );

      // mise à l'échelle
      matrModel.Scale( 5.0, 5.0, 5.0 );

      progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );

      switch ( modele )
      {
      default:
      case 1: // cube
         {
            /*         +Y                    */
            /*   3+-----------+2             */
            /*    |\          |\             */
            /*    | \         | \            */
            /*    |  \        |  \           */
            /*    |  7+-----------+6         */
            /*    |   |       |   |          */
            /*    |   |       |   |          */
            /*   0+---|-------+1  |          */
            /*     \  |        \  |     +X   */
            /*      \ |         \ |          */
            /*       \|          \|          */
            /*       4+-----------+5         */
            /*             +Z                */

            GLfloat sommets[3*4*6] =
            {
               -1.0,  1.0, -1.0,    1.0,  1.0, -1.0,    1.0, -1.0, -1.0,   -1.0, -1.0, -1.0,  // P3,P2,P1,P0
                1.0, -1.0,  1.0,   -1.0, -1.0,  1.0,   -1.0, -1.0, -1.0,    1.0, -1.0, -1.0,  // P5,P4,P0,P1
                1.0,  1.0,  1.0,    1.0, -1.0,  1.0,    1.0, -1.0, -1.0,    1.0,  1.0, -1.0,  // P6,P5,P1,P2
               -1.0,  1.0,  1.0,    1.0,  1.0,  1.0,    1.0,  1.0, -1.0,   -1.0,  1.0, -1.0,  // P7,P6,P2,P3
               -1.0, -1.0,  1.0,   -1.0,  1.0,  1.0,   -1.0,  1.0, -1.0,   -1.0, -1.0, -1.0,  // P4,P7,P3,P0
               -1.0, -1.0,  1.0,    1.0, -1.0,  1.0,    1.0,  1.0,  1.0,   -1.0,  1.0,  1.0   // P4,P5,P6,P7
            };
            // afficher le cube
            GLint locVertex = progNuanceur.obtenirAttribLocation( "Vertex" );
            glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, sommets );
            glEnableVertexAttribArray(locVertex);
            // ... "Normal"
            GLfloat normales[3*4*6] = 
            {
               0.0, 0.0, -1.0,   0.0, 0.0, -1.0,   0.0, 0.0, -1.0,   0.0, 0.0, -1.0,   // Normales face arrière
               0.0, -1.0, 0.0,   0.0, -1.0, 0.0,   0.0, -1.0, 0.0,   0.0, -1.0, 0.0,   // Normales face dessous
               1.0, 0.0, 0.0,    1.0, 0.0, 0.0,    1.0, 0.0, 0.0,    1.0, 0.0, 0.0,    // Normales face droite
               0.0, 1.0, 0.0,    0.0, 1.0, 0.0,    0.0, 1.0, 0.0,    0.0, 1.0, 0.0,    // Normales face haut
               -1.0, 0.0, 0.0,   -1.0, 0.0, 0.0,   -1.0, 0.0, 0.0,   -1.0, 0.0, 0.0,   // Normales face gauche
               0.0, 0.0, 1.0,    0.0, 0.0, 1.0,    0.0, 0.0, 1.0,    0.0, 0.0, 1.0,    // Normales face avant
            };
            GLint locNormal = progNuanceur.obtenirAttribLocation( "Normal" );
            glVertexAttribPointer( locNormal, 3, GL_FLOAT, GL_FALSE, 0, normales );
            glEnableVertexAttribArray(locNormal);

            glm::mat3 matrNormales = glm::transpose(glm::inverse(glm::mat3(matrVisu.getMatr()*matrModel.getMatr())));
            progNuanceur.assignerUniformMatrix3fv( "matrNormale", matrNormales );
            // ... "MultiTexCoord0"
            glDrawArrays( GL_QUADS, 0, 4*6 );
         }
         break;
      case 2:
         glFrontFace( GL_CW ); // les polygones de la théière ont leur face avant vers l'intérieur: il faut inverser
         matrModel.Scale( 0.5, 0.5, 0.5 );
         matrModel.Rotate( -90.0, 1.0, 0.0, 0.0 );
         matrModel.Translate( 0.0, 0.0, -1.0 );
         progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );
         shapesSolidTeapot( );  // définit automatiquement des coordonnées de texture :)
         glFrontFace( GL_CCW );
         break;
      case 3:
         shapesSolidTorus( 0.3, 0.6, 32, 32 ); // définit automatiquement des coordonnées de texture :)
         break;
      case 4:
         shapesSolidSphere( 0.6, 32, 32 ); // définit automatiquement des coordonnées de texture :)
         break;
      case 5:
         matrModel.PushMatrix(); {
            matrModel.Scale( .7, .7, .7 );
            progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );
            shapesSolidDodecahedron(); // ne définit aucune coordonnées de texture :(
         } matrModel.PopMatrix();
         break;
      case 6:
         matrModel.PushMatrix(); {
            matrModel.Scale( 1.2, 1.2, 1.2 );
            progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );
            shapesSolidIcosahedron(); // ne définit aucune coordonnées de texture :(
         } matrModel.PopMatrix();
         break;
      }
   } matrModel.PopMatrix(); progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );
}

void afficherLumiere()
{
   // Dessiner la lumiere

   // tracer une ligne vers la source lumineuse
   const GLfloat fact = 5.;
   GLfloat coords[] =
   {
      spotPosition[0]                      , spotPosition[1]                      , spotPosition[2],
      spotPosition[0]+spotDirection[0]/fact, spotPosition[1]+spotDirection[1]/fact, spotPosition[2]+spotDirection[2]/fact
   };
   glLineWidth( 3.0 );
   glColor3f( 1.0, 1.0, 0.5 ); // jaune
   GLint locVertex = progNuanceur.obtenirAttribLocation( "Vertex" );
   glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, coords );
   glEnableVertexAttribArray(locVertex);
   glDrawArrays( GL_LINES, 0, 2 );
   glLineWidth( 1.0 );

   // tracer la source lumineuse
   matrModel.PushMatrix(); {
      matrModel.Translate( spotPosition[0], spotPosition[1], spotPosition[2] );
      progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );
      shapesSolidSphere( 0.5, 10, 10 );
   } matrModel.PopMatrix(); progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );
}

// fonction d'affichage
void FenetreTP::afficherScene()
{
   // effacer l'ecran et le tampon de profondeur
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glPolygonMode( GL_FRONT_AND_BACK, modePolygone );

   progNuanceurBase.utiliser( );

   // définir le pipeline graphique
   if ( enPerspective )
   {
      matrProj.Perspective( 40.0, (GLdouble)largeur_ / (GLdouble)hauteur_,
                            0.1, 60.0 );
   }
   else
   {
      const GLfloat d = 10.0;
      if ( largeur_ <= hauteur_ )
      {
         matrProj.Ortho( -d, d,
                         -d*(GLdouble)hauteur_ / (GLdouble)largeur_,
                         d*(GLdouble)hauteur_ / (GLdouble)largeur_,
                         0.1, 60.0 );
      }
      else
      {
         matrProj.Ortho( -d*(GLdouble)largeur_ / (GLdouble)hauteur_,
                         d*(GLdouble)largeur_ / (GLdouble)hauteur_,
                         -d, d,
                         0.1, 60.0 );
      }
   }
   progNuanceurBase.assignerUniformMatrix4fv( "matrProj", matrProj );

   matrVisu.LookAt( 0.0, 0.0, distCam,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0 );
   progNuanceurBase.assignerUniformMatrix4fv( "matrVisu", matrVisu );

   matrModel.LoadIdentity();
   progNuanceurBase.assignerUniformMatrix4fv( "matrModel", matrModel );

   // afficher les axes
   if ( afficheAxes ) FenetreTP::afficherAxes( 8.0 );

   // dessiner la scène
   afficherLumiere();

   progNuanceur.utiliser( );
   progNuanceur.assignerUniformMatrix4fv( "matrProj", matrProj );
   progNuanceur.assignerUniformMatrix4fv( "matrVisu", matrVisu );
   progNuanceur.assignerUniformMatrix4fv( "matrModel", matrModel );
   //progNuanceur.assignerUniform3f( progNuanceur, "eyePos", x, y, z );
   progNuanceur.assignerUniform1i( "typeIllumination", typeIllumination );
   progNuanceur.assignerUniform1i( "utiliseBlinn", utiliseBlinn );
   progNuanceur.assignerUniform1i( "utiliseDirect", utiliseDirect );
   glActiveTexture( GL_TEXTURE0 ); // activer la texture '0' (valeur de défaut)
   progNuanceur.assignerUniform1i( "laTexture", 0 ); // '0' => utilisation de GL_TEXTURE0
   progNuanceur.assignerUniform1i( "utiliseCouleur", utiliseCouleur );
   progNuanceur.assignerUniform1i( "texnumero", texnumero );
   progNuanceur.assignerUniform1i( "noirTransparent", noirTransparent );

   // mettre à jour l'éclairage
   progNuanceur.assignerUniform1i( "LightModel.localViewer", localViewer );
   progNuanceur.assignerUniform4fv( "LightSource[0].ambient", lumiAmbiant );
   progNuanceur.assignerUniform4fv( "LightSource[0].diffuse", lumiDiffuse );
   progNuanceur.assignerUniform4fv( "LightSource[0].specular", lumiSpeculaire );
   progNuanceur.assignerUniform4fv( "LightSource[0].position", spotPosition );
   progNuanceur.assignerUniform3fv( "LightSource[0].spotDirection", spotDirection );
   progNuanceur.assignerUniform1f( "LightSource[0].spotExponent", spotExposant );
   progNuanceur.assignerUniform1f( "LightSource[0].spotCutoff", spotAngle );

   // mettre à jour les propriétés
   progNuanceur.assignerUniform4fv( "FrontMaterial.ambient", mat_ambiant );
   progNuanceur.assignerUniform4fv( "FrontMaterial.diffuse", mat_diffuse );
   progNuanceur.assignerUniform4fv( "FrontMaterial.specular", mat_specular );
   progNuanceur.assignerUniform4fv( "FrontMaterial.emission", mat_emission );
   progNuanceur.assignerUniform1f( "FrontMaterial.shininess", mat_shininess );

   afficherModele();
}

// fonction de redimensionnement de la fenêtre graphique
void FenetreTP::redimensionner( GLsizei w, GLsizei h )
{
   glViewport( 0, 0, w, h );
}

// fonction de gestion du clavier
void FenetreTP::clavier( TP_touche touche )
{
   // traitement des touches q et echap
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

   case TP_p:
      enPerspective = !enPerspective;
      break;

   case TP_i: // illumination de Gouraud ou Phong
      if ( ++typeIllumination > 1 ) typeIllumination = 0;
      break;

   case TP_r: // réflexion spéculaire de Blinn ou Phong
      utiliseBlinn = !utiliseBlinn;
      break;

   case TP_s: // permutation du modèle de spot Direct3D
      utiliseDirect = !utiliseDirect;
      break;

   case TP_l: // local viewer
      localViewer = !localViewer;
      std::cout << " localViewer=" << localViewer << std::endl;
      break;

   case TP_MINUS:
   case TP_UNDERSCORE:
      spotAngle -= 2.0;
      if ( spotAngle < 0.0 ) spotAngle = 0.0;
      std::cout <<  " spotAngle=" << spotAngle << std::endl;
      break;
   case TP_EQUALS:
   case TP_PLUS:
      spotAngle += 2.0;
      if ( spotAngle > 90.0 ) spotAngle = 90.0;
      std::cout <<  " spotAngle=" << spotAngle << std::endl;
      break;

   case TP_PERIOD:
      spotExposant -= 0.3;
      if ( spotExposant < 0.0 ) spotExposant = 0.0;
      std::cout <<  " spotExposant=" << spotExposant << std::endl;
      break;
   case TP_SLASH:
      spotExposant += 0.3;
      if ( spotExposant > 89.0 ) spotExposant = 89.0;
      std::cout <<  " spotExposant=" << spotExposant << std::endl;
      break;

   case TP_RIGHTBRACKET:
      mat_shininess *= 1.1;
      std::cout << " mat_shininess=" << mat_shininess << std::endl;
      break;
   case TP_LEFTBRACKET:
      mat_shininess /= 1.1; if ( mat_shininess < 0.0 ) mat_shininess = 0.0;
      std::cout << " mat_shininess=" << mat_shininess << std::endl;
      break;

   case TP_RIGHT:
      spotPosition[0] += 0.3;
      break;
   case TP_LEFT:
      spotPosition[0] -= 0.3;
      break;
   case TP_UP:
      spotPosition[1] += 0.3;
      break;
   case TP_DOWN:
      spotPosition[1] -= 0.3;
      break;

   case TP_END:
      spotDirection[0] += 0.6;
      break;
   case TP_HOME:
      spotDirection[0] -= 0.6;
      break;
   case TP_PAGEUP:
      spotDirection[1] += 0.6;
      break;
   case TP_PAGEDOWN:
      spotDirection[1] -= 0.6;
      break;

   case TP_SEMICOLON:
      if ( ++modele > 6 ) modele = 1;
      std::cout << " modele=" << modele << std::endl;
      break;

   case TP_0: // placer les choses afin d'avoir une belle vue
      thetaCam = 0.0; phiCam = 0.0; distCam = 30.0;
      break;

   case TP_t:
      texnumero++;
      if ( texnumero > 2 ) texnumero = 0;
      std::cout << " texnumero=" << texnumero << std::endl;
      break;

   case TP_w:
      texwrapmode++;
      texwrapmode = texwrapmode%4;
      std::cout << " texwrapmode=" << texwrapmode << std::endl;
      break;

   case TP_c:
      utiliseCouleur = !utiliseCouleur;
      std::cout << " utiliseCouleur=" << utiliseCouleur << std::endl;
      break;

   case TP_n:
      noirTransparent = !noirTransparent;
      std::cout << " noirTransparent=" << noirTransparent << std::endl;
      break;

   case TP_g:
      modePolygone = ( modePolygone == GL_FILL ) ? GL_LINE : GL_FILL;
      break;

   case TP_SPACE :
      // permuter la rotation automatique du modèle
      enmouvement = !enmouvement;
      break;

   default:
      std::cout << " touche inconnue : " << (char) touche << std::endl;
      break;
   }

   static std::string illuminationStr[] = { "0:Gouraud", "1:Phong" };
   static std::string reflexionStr[] = { "0:Phong", "1:Blinn" };
   static std::string spotStr[] = { "0:OpenGL", "1:Direct3D" };
   std::cout << " illumination= " << illuminationStr[typeIllumination]
             << ", refléxion spéculaire= " << reflexionStr[utiliseBlinn]
             << ", spot= " << spotStr[utiliseDirect]
             << std::endl;
   
}

// fonction callback pour un clic de souris
int dernierX = 0; // la dernière valeur en X de position de la souris
int dernierY = 0; // la derniere valeur en Y de position de la souris
static enum { deplaceCam, deplaceSpotDirection, deplaceSpotPosition } deplace = deplaceCam;
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
         deplace = deplaceSpotDirection;
         break;
      case TP_BUTTON_RIGHT:
         deplace = deplaceSpotPosition;
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
   spotAngle += sens*y;
   if ( spotAngle > 90.0 ) spotAngle = 90.0;
   std::cout <<  " spotAngle=" << spotAngle << std::endl;
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
      case deplaceSpotDirection:
         spotDirection[0] += 0.06 * dx;
         spotDirection[1] -= 0.06 * dy;
         break;
      case deplaceSpotPosition:
         spotPosition[0] += 0.03 * dx;
         spotPosition[1] -= 0.03 * dy;
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
