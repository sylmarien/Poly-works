#ifndef FENETERSDL_H
#define FENETERSDL_H

#include <SDL.h>
#include <GL/glew.h>
#include <string>
#include <iostream>

// la fenêtre graphique
class FenetreSDL
{
public:
   FenetreSDL( std::string nom = "INF2705",
               int largeur = 900, int hauteur = 600,
               int xpos = 100, int ypos = 100 )
      : fenetre_(NULL), contexte_(NULL), largeur_(largeur), hauteur_(hauteur)
   {
      // initialiser SDL
      const Uint32 flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
      if ( SDL_WasInit( flags ) == 0 )
      {
         if ( SDL_Init( flags ) < 0 ) sdldie( "Incapable d'initialiser SDL" );
         atexit( SDL_Quit );
      }

      // demander certaines caractéristiques: https://wiki.libsdl.org/SDL_GL_SetAttribute
      SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
      SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
      SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
      SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
      //SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
      SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
      SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
      //SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4 );
      SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
      //SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
      //SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );

      // créer la fenêtre
      fenetre_ = SDL_CreateWindow( nom.c_str(), xpos, ypos, largeur, hauteur,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
      if ( !fenetre_ ) sdldie( "Incapable de créer la fenêtre" );
      verifierErreurSDL(__LINE__);

      // créer le contexte OpenGL
      contexte_ = SDL_GL_CreateContext( fenetre_ );
      verifierErreurSDL(__LINE__);

      // s'assurer que les réaffichage seront synchronisés avec le rafraîchissement de l'écran
      SDL_GL_SetSwapInterval( 1 );
      verifierErreurSDL(__LINE__);

      // initiliaser GLEW
      initialiserGLEW( );

      // imprimer un peu d'information OpenGL
      imprimerInfosGL( );
   };

   ~FenetreSDL( )
   {
      SDL_GL_DeleteContext( contexte_ ); contexte_ = NULL;
      SDL_DestroyWindow( fenetre_ ); fenetre_ = NULL;
   };

   // fonction appelée pour tracer la scène
   void afficherScene( );
   // mettre à jour la fenêtre OpenGL: le tampon arrière devient le tampon avant et vice-versa
   void swap( ) { SDL_GL_SwapWindow( fenetre_ ); };

   // fonction appelée lors d'un événement de redimensionnement
   void redimensionner( GLsizei w, GLsizei h );
   // fonction appelée lors d'un événement de clavier
   void clavier( SDL_Keycode touche );
   // fonctions appelées lors d'un événement de souris
   void sourisClic( int button, int state, int x, int y );
   void sourisWheel( int x, int y );
   void sourisMouvement( int x, int y );

   // fonction de gestion de la boucle des événements
   bool gererEvenement( )
   {
      SDL_Event e;
      while ( SDL_PollEvent( &e ) )
      {
         switch ( e.type )
         {
         case SDL_QUIT: // c'est la fin
            return( false );
            break;
         case SDL_WINDOWEVENT:
            if ( e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED ) // redimensionnement
            {
               largeur_ = e.window.data1;
               hauteur_ = e.window.data2;
               redimensionner( largeur_, hauteur_ );
            }
            else if ( e.window.event == SDL_WINDOWEVENT_SHOWN ) // affichage
            {
               SDL_GetWindowSize( fenetre_, &largeur_, &hauteur_ );
               redimensionner( largeur_, hauteur_ );
            }
            //else
            //   std::cerr << "//@FenetreSDL,WINDOWEVENT;" << " e.window.event=" << e.window.event << std::endl;
            break;
         case SDL_KEYDOWN: // une touche est pressée
            clavier( e.key.keysym.sym );
            break;
         case SDL_KEYUP: // une touche est relâchée
            break;
         case SDL_MOUSEBUTTONDOWN: // un bouton de la souris est pressé
         case SDL_MOUSEBUTTONUP: // un bouton de la souris est relâché
            sourisClic( e.button.button, e.button.state, e.button.x, e.button.y );
            break;
         case SDL_MOUSEMOTION: // la souris est déplacée
            sourisMouvement( e.motion.x, e.motion.y );
            break;
         case SDL_MOUSEWHEEL: // la molette de la souris est tournée
            sourisWheel( e.wheel.x, e.wheel.y );
            break;
         default:
            //std::cerr << "//@FenetreSDL," << __LINE__ << ";" << " e.type=" << e.type << std::endl;
            break;
         }
      }
      return( true );
   };

   //
   // Quelques fonctions utilitaires
   //

   // afficher les versions des éléments du pipeline OpenGL
   static void imprimerInfosGL( const int verbose  = 1 )
   {
#define PBYTE(CHAINE) ( (CHAINE) != NULL ? (CHAINE) : (const GLubyte *) "????" )
#define PCHAR(CHAINE) ( (CHAINE) != NULL ? (CHAINE) : (const char *) "????" )

      if ( verbose >= 1 )
      {
         const GLubyte *glVersion  = glGetString( GL_VERSION );
         const GLubyte *glVendor   = glGetString( GL_VENDOR );
         const GLubyte *glRenderer = glGetString( GL_RENDERER );
         const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
         printf( "// OpenGL %s, %s\n", PBYTE(glVersion), PBYTE(glVendor) );
         printf( "// GPU    %s\n", PBYTE(glRenderer) );
         printf( "// GLSL   %s\n", PBYTE(glslVersion) );

         if ( verbose >= 2 )
         {
            //const GLubyte *gluVersion = gluGetString( (GLenum) GLU_VERSION );
            //printf( "// GLU    %s\n", PBYTE(gluVersion) );

            if ( verbose >= 3 )
            {
               const GLubyte *glExtensions  = glGetString( GL_EXTENSIONS );
               //const GLubyte *gluExtensions = gluGetString( (GLenum) GLU_EXTENSIONS );
               printf( "// GL      ext = %s\n", PBYTE(glExtensions) );
               //printf( "// GLU     ext = %s\n", PBYTE(gluExtensions) );
            }
         }
      }
#undef PBYTE
#undef PCHAR
      return;
   }

   // donner une message et mourir...
   static void sdldie( const char *msg )
   {
      const char *sdlerror = SDL_GetError();
      std::cout << "sdldie" << msg << " " << sdlerror << std::endl;
      SDL_Quit();
      exit(1);
   }
   // vérifier les erreurs
   static void verifierErreurSDL( int line = -1 )
   {
      const char *sdlerror = SDL_GetError();
      if ( *sdlerror != '\0' )
      {
         std::cout << "SDL Error: " << sdlerror << std::endl;
         if ( line != -1 )
            std::cout << "line: " << line;
         std::cout << std::endl;
         SDL_ClearError();
      }
   }
   // La fonction glGetError() permet de savoir si une erreur est survenue depuis le dernier appel à cette fonction.
   static int VerifierErreurGL( const std::string endroit )
   {
      int rc = 0;
      GLenum err;
      while ( ( err = glGetError() ) != GL_NO_ERROR )
      {
         std::cerr << "Erreur OpenGL en cet endroit: " << endroit << std::endl;
         switch ( err )
         {
         case GL_INVALID_ENUM:
            std::cerr << "GL_INVALID_ENUM: Valeur d'une énuméraiton hors limite.";
            break;
         case GL_INVALID_VALUE:
            std::cerr << "GL_INVALID_VALUE: Valeur numérique hors limite.";
            break;
         case GL_INVALID_OPERATION:
            std::cerr << "GL_INVALID_OPERATION: Opéraiotn non permise dans l'état courant.";
            break;
         case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION: L'objet est incomplet.";
            break;
         case GL_OUT_OF_MEMORY:
            std::cerr << "GL_OUT_OF_MEMORY: Pas assez de mémoire pour exécuter la commande.";
            break;
         case GL_STACK_UNDERFLOW:
            std::cerr << "GL_STACK_UNDERFLOW: Une opération entraînerait un débordement de pile interne.";
            break;
         case GL_STACK_OVERFLOW:
            std::cerr << "GL_STACK_OVERFLOW: Une opération entraînerait un débordement de pile interne.";
            break;
         default:
            std::cerr << "err = " << err << ": Erreur inconnue!";
            break;
         }
         std::cerr << std::endl;
         ++rc;
      }
      return( rc );
   };

   // La fonction afficherAxes affiche des axes qui représentent l'orientation courante du repère
   // Les axes sont colorés ainsi: X = Rouge, Y = Vert, Z = Bleu
   static void afficherAxes( const GLfloat longueurAxe = 1.0, const GLfloat largeurLigne = 2.0 )
   {
      glPushAttrib( GL_ENABLE_BIT | GL_LINE_BIT );
      glLineWidth( largeurLigne );

      const GLfloat coo[] = { 0., 0., 0.,
                              longueurAxe, 0., 0.,
                              0., longueurAxe, 0.,
                              0., 0., longueurAxe };
      const GLfloat couleur[] = { 1., 1., 1.,
                                  1., 0., 0.,
                                  0., 1., 0.,
                                  0., 0., 1. };

#if 0
      const GLuint connec[] = { 0, 1, 0, 2, 0, 3 };
      glEnableClientState( GL_VERTEX_ARRAY );
      glEnableClientState( GL_COLOR_ARRAY );
      glVertexPointer( 3, GL_FLOAT, 0, coo );
      glColorPointer( 3, GL_FLOAT, 0, couleur );
      glDrawElements( GL_LINES, sizeof(connec)/sizeof(GLuint), GL_UNSIGNED_INT, connec );
      glDisableClientState( GL_VERTEX_ARRAY );
      glDisableClientState( GL_COLOR_ARRAY );
#else
      glBegin( GL_LINES );{
         glColor3fv( &(couleur[3]) ); glVertex3fv( &(coo[0]) ); glVertex3fv( &(coo[3]) );
         glColor3fv( &(couleur[6]) ); glVertex3fv( &(coo[0]) ); glVertex3fv( &(coo[6]) );
         glColor3fv( &(couleur[9]) ); glVertex3fv( &(coo[0]) ); glVertex3fv( &(coo[9]) );
      }glEnd( );
#endif

      glPopAttrib( );
      return;
   };

private:
   void initialiserGLEW( )
   {
      glewExperimental = GL_TRUE;
      GLenum rev = glewInit();
      if ( rev != GLEW_OK )
      {
         std::cout << "Error: " << glewGetErrorString(rev) << std::endl;
         exit( 1 );
      }
   };

   SDL_Window *fenetre_;
   SDL_GLContext contexte_;
   int largeur_;    // la largeur de la fenêtre
   int hauteur_;    // la hauteur de la fenêtre
};

#endif
