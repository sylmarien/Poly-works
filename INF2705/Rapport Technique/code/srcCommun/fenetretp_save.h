#ifndef FENETRETP_H
#define FENETRETP_H

#if defined( __APPLE__ )
#define FENETRE_LIB_GLFW 1
#else
#define FENETRE_LIB_SDL 1
#endif

#if defined(FENETRE_LIB_GLFW)
# include <GLFW/glfw3.h>
#else
# include <SDL.h>
#endif
#include <GL/glew.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>

typedef enum {
#if defined(FENETRE_LIB_GLFW)

   TP_ESCAPE       = GLFW_KEY_ESCAPE,
   TP_UP           = GLFW_KEY_UP,
   TP_DOWN         = GLFW_KEY_DOWN,
   TP_PAGEUP       = GLFW_KEY_PAGE_UP,
   TP_PAGEDOWN     = GLFW_KEY_PAGE_DOWN,
   TP_EQUALS       = '=', // GLFW_KEY_EQUALS,
   TP_GREATER      = '>', // GLFW_KEY_GREATER,
   TP_LESS         = '<', // GLFW_KEY_LESS,
   TP_RIGHT        = GLFW_KEY_RIGHT,
   TP_LEFT         = GLFW_KEY_LEFT,
   TP_PLUS         = '+', // GLFW_KEY_PLUS,
   TP_MINUS        = GLFW_KEY_MINUS,
   TP_RIGHTBRACKET = GLFW_KEY_RIGHT_BRACKET,
   TP_LEFTBRACKET  = GLFW_KEY_LEFT_BRACKET,
   TP_PERIOD       = GLFW_KEY_PERIOD,
   TP_SEMICOLON    = GLFW_KEY_SEMICOLON,
   TP_SLASH        = GLFW_KEY_SLASH,
   TP_SPACE        = GLFW_KEY_SPACE,
   TP_UNDERSCORE   = '_', // GLFW_KEY_UNDERSCORE,

   TP_0 = GLFW_KEY_0,
   TP_1 = GLFW_KEY_1,
   TP_2 = GLFW_KEY_2,
   TP_3 = GLFW_KEY_3,
   TP_4 = GLFW_KEY_4,
   TP_5 = GLFW_KEY_5,
   TP_6 = GLFW_KEY_6,
   TP_7 = GLFW_KEY_7,
   TP_8 = GLFW_KEY_8,
   TP_9 = GLFW_KEY_9,

   TP_a = GLFW_KEY_A,
   TP_b = GLFW_KEY_B,
   TP_c = GLFW_KEY_C,
   TP_d = GLFW_KEY_D,
   TP_e = GLFW_KEY_E,
   TP_f = GLFW_KEY_F,
   TP_g = GLFW_KEY_G,
   TP_h = GLFW_KEY_H,
   TP_i = GLFW_KEY_I,
   TP_j = GLFW_KEY_J,
   TP_k = GLFW_KEY_K,
   TP_l = GLFW_KEY_L,
   TP_m = GLFW_KEY_M,
   TP_n = GLFW_KEY_N,
   TP_o = GLFW_KEY_O,
   TP_p = GLFW_KEY_P,
   TP_q = GLFW_KEY_Q,
   TP_r = GLFW_KEY_R,
   TP_s = GLFW_KEY_S,
   TP_t = GLFW_KEY_T,
   TP_u = GLFW_KEY_U,
   TP_v = GLFW_KEY_V,
   TP_w = GLFW_KEY_W,
   TP_x = GLFW_KEY_X,
   TP_y = GLFW_KEY_Y,
   TP_z = GLFW_KEY_Z,

#else

   TP_ESCAPE       = SDLK_ESCAPE,
   TP_UP           = SDLK_UP,
   TP_DOWN         = SDLK_DOWN,
   TP_PAGEUP       = SDLK_PAGEUP,
   TP_PAGEDOWN     = SDLK_PAGEDOWN,
   TP_HOME         = SDLK_HOME,
   TP_END          = SDLK_END,
   TP_EQUALS       = SDLK_EQUALS,
   TP_GREATER      = SDLK_GREATER,
   TP_LESS         = SDLK_LESS,
   TP_RIGHT        = SDLK_RIGHT,
   TP_LEFT         = SDLK_LEFT,
   TP_PLUS         = SDLK_PLUS,
   TP_MINUS        = SDLK_MINUS,
   TP_RIGHTBRACKET = SDLK_RIGHTBRACKET,
   TP_LEFTBRACKET  = SDLK_LEFTBRACKET,
   TP_PERIOD       = SDLK_PERIOD,
   TP_SEMICOLON    = SDLK_SEMICOLON,
   TP_SLASH        = SDLK_SLASH,
   TP_SPACE        = SDLK_SPACE,
   TP_UNDERSCORE   = SDLK_UNDERSCORE,

   TP_0 = SDLK_0,
   TP_1 = SDLK_1,
   TP_2 = SDLK_2,
   TP_3 = SDLK_3,
   TP_4 = SDLK_4,
   TP_5 = SDLK_5,
   TP_6 = SDLK_6,
   TP_7 = SDLK_7,
   TP_8 = SDLK_8,
   TP_9 = SDLK_9,

   TP_a = SDLK_a,
   TP_b = SDLK_b,
   TP_c = SDLK_c,
   TP_d = SDLK_d,
   TP_e = SDLK_e,
   TP_f = SDLK_f,
   TP_g = SDLK_g,
   TP_h = SDLK_h,
   TP_i = SDLK_i,
   TP_j = SDLK_j,
   TP_k = SDLK_k,
   TP_l = SDLK_l,
   TP_m = SDLK_m,
   TP_n = SDLK_n,
   TP_o = SDLK_o,
   TP_p = SDLK_p,
   TP_q = SDLK_q,
   TP_r = SDLK_r,
   TP_s = SDLK_s,
   TP_t = SDLK_t,
   TP_u = SDLK_u,
   TP_v = SDLK_v,
   TP_w = SDLK_w,
   TP_x = SDLK_x,
   TP_y = SDLK_y,
   TP_z = SDLK_z,

#endif
} TP_touche;

typedef enum {
#if defined(FENETRE_LIB_GLFW)
   TP_BUTTON_LEFT      = GLFW_MOUSE_BUTTON_1,
   TP_BUTTON_MIDDLE    = GLFW_MOUSE_BUTTON_3,
   TP_BUTTON_RIGHT     = GLFW_MOUSE_BUTTON_2,
   TP_RELEASED         = GLFW_RELEASE,
   TP_PRESSED          = GLFW_PRESS,
#else
   TP_BUTTON_LEFT      = SDL_BUTTON_LEFT,
   TP_BUTTON_MIDDLE    = SDL_BUTTON_MIDDLE,
   TP_BUTTON_RIGHT     = SDL_BUTTON_RIGHT,
   TP_RELEASED         = SDL_RELEASED,
   TP_PRESSED          = SDL_PRESSED,
#endif
} TP_bouton;


// la fenêtre graphique
class FenetreTP
{
#if defined(FENETRE_LIB_GLFW)
   static void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
   {
      FenetreTP *fen = (FenetreTP*) glfwGetWindowUserPointer( window );
      if ( action == GLFW_PRESS )
         fen->clavier( (TP_touche) key );
   };
   static void mouse_button_callback( GLFWwindow* window, int button, int action, int mods )
   {
      FenetreTP *fen = (FenetreTP*) glfwGetWindowUserPointer( window );
      double xpos, ypos; glfwGetCursorPos( window, &xpos, &ypos );
      fen->sourisClic( button, action, xpos, ypos );
   }
   static void cursor_position_callback( GLFWwindow* window, double xpos, double ypos )
   {
      FenetreTP *fen = (FenetreTP*) glfwGetWindowUserPointer( window );
      fen->sourisMouvement( xpos, ypos );
   }
   static void scroll_callback( GLFWwindow* window, double xoffset, double yoffset )
   {
      FenetreTP *fen = (FenetreTP*) glfwGetWindowUserPointer( window );
      fen->sourisWheel( xoffset, yoffset );
   }
   static void window_refresh_callback( GLFWwindow* window )
   {
      FenetreTP *fen = (FenetreTP*) glfwGetWindowUserPointer( window );
      // int left, top, right, bottom;
      // glfwGetWindowFrameSize( window, &left, &top, &right, &bottom );
      // fen->redimensionner( right-left, top-bottom );
      int width, height;
      glfwGetWindowSize( window, &width, &height );
      fen->redimensionner( width, height );
      fen->afficherScene();
      fen->swap();
   }
#else
#endif

public:
   FenetreTP( std::string nom = "INF2705 TP",
              int largeur = 900, int hauteur = 600,
              int xpos = 100, int ypos = 100 )
      : fenetre_(NULL), contexte_(NULL), largeur_(largeur), hauteur_(hauteur)
   {
#if defined(FENETRE_LIB_GLFW)
      // initialiser GLFW
      if ( !glfwInit() )
      {
         glfwdie( "ERREUR: Incapable d'initialiser GLFW3\n");
      }

      // demander certaines caractéristiques:
      glfwWindowHint( GLFW_RED_BITS, 8 );
      glfwWindowHint( GLFW_GREEN_BITS, 8 );
      glfwWindowHint( GLFW_BLUE_BITS, 8 );
      glfwWindowHint( GLFW_ALPHA_BITS, 8 );
      //glfwWindowHint( GLFW_DOUBLEBUFFER, GL_TRUE );
      glfwWindowHint( GLFW_DEPTH_BITS, 24 );
      glfwWindowHint( GLFW_STENCIL_BITS, 8 );
#if defined( __APPLE__ )
      glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
      glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
      glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
      glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
#endif

      // créer la fenêtre
      fenetre_ = glfwCreateWindow( largeur, hauteur, nom.c_str(), NULL, NULL );
      if ( !fenetre_ )
      {
         glfwdie( "ERROR: Incapable de créer la fenêtre GLFW3\n");
      }
      glfwMakeContextCurrent( fenetre_ );
      glfwSwapInterval(1);

      glfwSetWindowUserPointer( fenetre_, this );
      glfwSetKeyCallback( fenetre_, key_callback );
      glfwSetMouseButtonCallback( fenetre_, mouse_button_callback );
      glfwSetCursorPosCallback( fenetre_, cursor_position_callback );
      glfwSetScrollCallback( fenetre_, scroll_callback );
      glfwSetWindowRefreshCallback( fenetre_, window_refresh_callback );
#else
      // initialiser SDL
      const Uint32 flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
      if ( SDL_WasInit( flags ) == 0 )
      {
         if ( SDL_Init( flags ) < 0 ) sdldie( "ERREUR: Incapable d'initialiser SDL" );
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
      if ( !fenetre_ ) sdldie( "ERREUR: Incapable de créer la fenêtre SDL" );
      verifierErreurSDL(__LINE__);

      // créer le contexte OpenGL
      contexte_ = SDL_GL_CreateContext( fenetre_ );
      verifierErreurSDL(__LINE__);

      // s'assurer que les réaffichage seront synchronisés avec le rafraîchissement de l'écran
      SDL_GL_SetSwapInterval( 1 );
      verifierErreurSDL(__LINE__);
#endif

      // initiliaser GLEW
      initialiserGLEW( );

      // imprimer un peu d'information OpenGL
      imprimerInfosGL( );
   };

#if defined(FENETRE_LIB_GLFW)
   void quit( )
   {
      glfwDestroyWindow( fenetre_ );
      glfwTerminate();
      exit(0);
   }
   ~FenetreTP( )
   {
      quit();
   };
#else
   void quit( )
   {
      SDL_Event sdlevent; sdlevent.type = SDL_QUIT;
      SDL_PushEvent( &sdlevent );
   }
   ~FenetreTP( )
   {
      SDL_GL_DeleteContext( contexte_ ); contexte_ = NULL;
      SDL_DestroyWindow( fenetre_ ); fenetre_ = NULL;
   };
#endif

   // mettre à jour la fenêtre OpenGL: le tampon arrière devient le tampon avant et vice-versa
#if defined(FENETRE_LIB_GLFW)
   void swap( ) { glfwSwapBuffers( fenetre_ ); };
#else
   void swap( ) { SDL_GL_SwapWindow( fenetre_ ); };
#endif

   // fonction appelée pour tracer la scène
   void afficherScene( );
   // fonction appelée lors d'un événement de redimensionnement
   void redimensionner( GLsizei w, GLsizei h );
   // fonction appelée lors d'un événement de clavier
   void clavier( TP_touche touche );
   // fonctions appelées lors d'un événement de souris
   void sourisClic( int button, int state, int x, int y );
   void sourisWheel( int x, int y );
   void sourisMouvement( int x, int y );

   // fonction de gestion de la boucle des événements
   bool gererEvenement( )
   {
#if defined(FENETRE_LIB_GLFW)
      glfwPollEvents();
      return( !glfwWindowShouldClose( fenetre_ ) );
#else
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
            //   std::cerr << "//@FenetreTP,WINDOWEVENT;" << " e.window.event=" << e.window.event << std::endl;
            break;
         case SDL_KEYDOWN: // une touche est pressée
            clavier( (TP_touche) e.key.keysym.sym );
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
            //std::cerr << "//@FenetreTP," << __LINE__ << ";" << " e.type=" << e.type << std::endl;
            break;
         }
      }
      return( true );
#endif
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
#if defined(FENETRE_LIB_GLFW)
         int major, minor, rev; glfwGetVersion( &major, &minor, &rev );
         std::cout << "// GLFW   " << major << "." << minor << "." << rev << std::endl;
#else
         SDL_version linked; SDL_GetVersion( &linked );
         std::cout << "// SDL    " << (int) linked.major << "." << (int) linked.minor << "." << (int) linked.patch << std::endl;
#endif

         const GLubyte *glVersion  = glGetString( GL_VERSION );
         const GLubyte *glVendor   = glGetString( GL_VENDOR );
         const GLubyte *glRenderer = glGetString( GL_RENDERER );
         const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
         std::cout << "// OpenGL " << PBYTE(glVersion) << PBYTE(glVendor) << std::endl;
         std::cout << "// GPU    " << PBYTE(glRenderer) << std::endl;
         std::cout << "// GLSL   " << PBYTE(glslVersion) << std::endl;

         if ( verbose >= 2 )
         {
            const GLubyte *glExtensions  = glGetString( GL_EXTENSIONS );
            std::cout << "// extensions = " << PBYTE(glExtensions) << std::endl;
         }
      }
#undef PBYTE
#undef PCHAR
      return;
   }

#if defined(FENETRE_LIB_GLFW)
   // donner une message et mourir...
   static void glfwdie( const char *msg )
   {
      //const char *sdlerror = SDL_GetError();
      //std::cout << "glfwdie " << msg << " " << sdlerror << std::endl;
      std::cout << "glfwdie " << msg << " " << std::endl;
      //glfwTerminate();
      exit(1);
   }
#else
   // donner une message et mourir...
   static void sdldie( const char *msg )
   {
      const char *sdlerror = SDL_GetError();
      std::cout << "sdldie " << msg << " " << sdlerror << std::endl;
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
#endif
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
      //GLint locVertex = progNuanceur.obtenirAttribLocation( "Vertex" );
      GLint locVertex = 0;
      glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, coo );
      glEnableVertexAttribArray(locVertex);
      //GLint locColor = progNuanceur.obtenirAttribLocation( "Color" );
      GLint locColor = 3;
      glVertexAttribPointer( locColor, 3, GL_FLOAT, GL_FALSE, 0, couleur );
      glEnableVertexAttribArray(locColor);
      glDrawElements( GL_LINES, sizeof(connec)/sizeof(GLuint), GL_UNSIGNED_INT, connec );
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

#if defined(FENETRE_LIB_GLFW)
   GLFWwindow *fenetre_;
   int *contexte_;
#else
   SDL_Window *fenetre_;
   SDL_GLContext contexte_;
#endif
   GLsizei largeur_; // la largeur de la fenêtre
   GLsizei hauteur_; // la hauteur de la fenêtre
};

#endif
