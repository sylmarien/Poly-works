#ifndef INF2705_H
#define INF2705_H

#include <GL/glew.h>
#include <iostream>

////////////////////////////////////////////////////////////////////////////
//
// Classe pour les matrices du pipeline
//
////////////////////////////////////////////////////////////////////////////

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stack>

class MatricePipeline
{
public:
   MatricePipeline()
   { matr_.push( glm::mat4(1.0) ); };

   void LoadIdentity()
   { matr_.top() = glm::mat4(1.0); }
   void Scale( GLfloat x, GLfloat y, GLfloat z )
   { matr_.top() = glm::scale( matr_.top(), glm::vec3(x,y,z) ); }
   void Translate( GLfloat x, GLfloat y, GLfloat z )
   { matr_.top() = glm::translate( matr_.top(), glm::vec3(x,y,z) ); }
   void Rotate( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
   { matr_.top() = glm::rotate( matr_.top(), (GLfloat)glm::radians(angle), glm::vec3(x,y,z) ); }

   void LookAt( GLdouble obsX, GLdouble obsY, GLdouble obsZ, GLdouble versX, GLdouble versY, GLdouble versZ, GLdouble upX, GLdouble upY, GLdouble upZ )
   { matr_.top() = glm::lookAt( glm::vec3( obsX, obsY, obsZ ), glm::vec3( versX, versY, versZ ), glm::vec3( upX, upY, upZ ) ); }
   void Frustum( GLdouble gauche, GLdouble droite, GLdouble bas, GLdouble haut, GLdouble planAvant, GLdouble planArriere )
   { matr_.top() = glm::frustum( gauche, droite, bas, haut, planAvant, planArriere ); }
   void Perspective( GLdouble fovy, GLdouble aspect, GLdouble planAvant, GLdouble planArriere )
   { matr_.top() = glm::perspective( glm::radians(fovy), aspect, planAvant, planArriere );}
   void Ortho( GLdouble gauche, GLdouble droite, GLdouble bas, GLdouble haut, GLdouble planAvant, GLdouble planArriere )
   { matr_.top() = glm::ortho( gauche, droite, bas, haut, planAvant, planArriere ); }
   void Ortho2D( GLdouble gauche, GLdouble droite, GLdouble bas, GLdouble haut )
   { matr_.top() = glm::ortho( gauche, droite, bas, haut ); }

   void PushMatrix()
   { matr_.push( matr_.top() ); };
   void PopMatrix()
   { matr_.pop(); };

   glm::mat4 getMatr()
   { return matr_.top(); }
   glm::mat4 setMatr( glm::mat4 m )
   { return( matr_.top() = m ); }

private:
   std::stack<glm::mat4> matr_;
};

////////////////////////////////////////////////////////////////////////////
//
// Classe pour charger les nuanceurs
//
////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <errno.h>
#include <fstream>
#include <sstream>

class ProgNuanceur
{
public:
   ProgNuanceur( ) : prog_(0) { }
   ~ProgNuanceur( ) { glDeleteProgram( prog_ ); }
   GLuint obtenirId( ) { return prog_; }

   // demander à OpenGL de créer un programme de nuanceur
   void creer( )
   {
      if ( prog_ ) glDeleteProgram( prog_ );
      prog_ = glCreateProgram();
   }

   // associer le contenu du fichier au nuanceur de sommets
   bool attacherSourceSommets( const GLchar *fich = NULL )
   {
      if ( !prog_ ) creer();
      return( attacherSource( GL_VERTEX_SHADER, fich ) );
   }
   // associer le contenu du fichier au nuanceur de tessellation
   bool attacherSourceTessCtrl( const GLchar *fich = NULL )
   {
      if ( !prog_ ) creer();
      return( attacherSource( GL_TESS_CONTROL_SHADER, fich ) );
   }
   // associer le contenu du fichier au nuanceur de tessellation
   bool attacherSourceTessEval( const GLchar *fich = NULL )
   {
      if ( !prog_ ) creer();
      return( attacherSource( GL_TESS_EVALUATION_SHADER, fich ) );
   }
   // associer le contenu du fichier au nuanceur de géométrie
   bool attacherSourceGeometrie( const GLchar *fich = NULL )
   {
      if ( !prog_ ) creer();
      return( attacherSource( GL_GEOMETRY_SHADER, fich ) );
   }
   // associer le contenu du fichier au nuanceur de fragments
   bool attacherSourceFragments( const GLchar *fich = NULL )
   {
      if ( !prog_ ) creer();
      return( attacherSource( GL_FRAGMENT_SHADER, fich ) );
   }

   // faire l'édition des liens du programme
   bool lier( ) { return( lierProgramme( ) ); };

   // utiliser ce programme comme pipeline graphique
   void utiliser( ) { glUseProgram( prog_ ); }

   // assigner des variables uniformes
   void assignerUniform1f( const GLchar *nom, GLfloat v0 ) { glUniform1f( glGetUniformLocation( prog_, nom ), v0 ); }
   void assignerUniform2f( const GLchar *nom, GLfloat v0, GLfloat v1 ) { glUniform2f( glGetUniformLocation( prog_, nom ), v0, v1 ); }
   void assignerUniform3f( const GLchar *nom, GLfloat v0, GLfloat v1, GLfloat v2 ) { glUniform3f( glGetUniformLocation( prog_, nom ), v0, v1, v2 ); }
   void assignerUniform4f( const GLchar *nom, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3 ) { glUniform4f( glGetUniformLocation( prog_, nom ), v0, v1, v2, v3 ); }
   void assignerUniform1i( const GLchar *nom, GLint v0 ) { glUniform1i( glGetUniformLocation( prog_, nom ), v0 ); }
   void assignerUniform2i( const GLchar *nom, GLint v0, GLint v1 ) { glUniform2i( glGetUniformLocation( prog_, nom ), v0, v1 ); }
   void assignerUniform3i( const GLchar *nom, GLint v0, GLint v1, GLint v2 ) { glUniform3i( glGetUniformLocation( prog_, nom ), v0, v1, v2 ); }
   void assignerUniform4i( const GLchar *nom, GLint v0, GLint v1, GLint v2, GLint v3 ) { glUniform4i( glGetUniformLocation( prog_, nom ), v0, v1, v2, v3 ); }
   //void assignerUniform1ui( const GLchar *nom, GLuint v0 ) { glUniform1ui( glGetUniformLocation( prog_, nom ), v0 ); }
   //void assignerUniform2ui( const GLchar *nom, GLuint v0, GLuint v1 ) { glUniform2ui( glGetUniformLocation( prog_, nom ), v0, v1 ); }
   //void assignerUniform3ui( const GLchar *nom, GLuint v0, GLuint v1, GLuint v2 ) { glUniform3ui( glGetUniformLocation( prog_, nom ), v0, v1, v2 ); }
   //void assignerUniform4ui( const GLchar *nom, GLuint v0, GLuint v1, GLuint v2, GLuint v3 ) { glUniform4ui( glGetUniformLocation( prog_, nom ), v0, v1, v2, v3 ); }

   void assignerUniform1fv( const GLchar *nom, const GLfloat *val ) { glUniform1fv( glGetUniformLocation( prog_, nom ), 1, val ); }
   void assignerUniform2fv( const GLchar *nom, const GLfloat *val ) { glUniform2fv( glGetUniformLocation( prog_, nom ), 1, val ); }
   void assignerUniform3fv( const GLchar *nom, const glm::vec2 &val ) { assignerUniform2fv( nom, glm::value_ptr(val) ); }
   void assignerUniform3fv( const GLchar *nom, const GLfloat *val ) { glUniform3fv( glGetUniformLocation( prog_, nom ), 1, val ); }
   void assignerUniform3fv( const GLchar *nom, const glm::vec3 &val ) { assignerUniform3fv( nom, glm::value_ptr(val) ); }
   void assignerUniform4fv( const GLchar *nom, const GLfloat *val ) { glUniform4fv( glGetUniformLocation( prog_, nom ), 1, val ); }
   void assignerUniform4fv( const GLchar *nom, const glm::vec4 &val ) { assignerUniform4fv( nom, glm::value_ptr(val) ); }
   void assignerUniform1iv( const GLchar *nom, const GLint *val ) { glUniform1iv( glGetUniformLocation( prog_, nom ), 1, val ); }
   void assignerUniform2iv( const GLchar *nom, const GLint *val ) { glUniform2iv( glGetUniformLocation( prog_, nom ), 1, val ); }
   void assignerUniform3iv( const GLchar *nom, const GLint *val ) { glUniform3iv( glGetUniformLocation( prog_, nom ), 1, val ); }
   void assignerUniform4iv( const GLchar *nom, const GLint *val ) { glUniform4iv( glGetUniformLocation( prog_, nom ), 1, val ); }
   // void assignerUniform1uiv( const GLchar *nom, const GLuint *val ) { glUniform1uiv( glGetUniformLocation( prog_, nom ), 1, val ); }
   // void assignerUniform2uiv( const GLchar *nom, const GLuint *val ) { glUniform2uiv( glGetUniformLocation( prog_, nom ), 1, val ); }
   // void assignerUniform3uiv( const GLchar *nom, const GLuint *val ) { glUniform3uiv( glGetUniformLocation( prog_, nom ), 1, val ); }
   // void assignerUniform4uiv( const GLchar *nom, const GLuint *val ) { glUniform4uiv( glGetUniformLocation( prog_, nom ), 1, val ); }

   void assignerUniformMatrix3fv( const GLchar *nom, const GLfloat *val ) { glUniformMatrix3fv( glGetUniformLocation( prog_, nom ), 1, GL_FALSE, val ); };
   void assignerUniformMatrix3fv( const GLchar *nom, glm::mat3 &val ) { assignerUniformMatrix3fv( nom, glm::value_ptr(val) ); }
   void assignerUniformMatrix4fv( const GLchar *nom, const GLfloat *val ) { glUniformMatrix4fv( glGetUniformLocation( prog_, nom ), 1, GL_FALSE, val ); };
   void assignerUniformMatrix4fv( const GLchar *nom, glm::mat4 &val ) { assignerUniformMatrix4fv( nom, glm::value_ptr(val) ); }
   void assignerUniformMatrix4fv( const GLchar *nom, MatricePipeline &val ) { assignerUniformMatrix4fv( nom, glm::value_ptr(val.getMatr()) ); }

   // assigner des attributs
   GLuint obtenirAttribLocation( const GLchar *nom )
   {
      GLint loc = glGetAttribLocation( prog_, nom );
      if ( loc == -1 ) std::cerr << "!!! " << nom << ": pas trouvé la \"Location\" de la variable" << std::endl;
      return( loc );
   }
   void assignerAttribLocation( GLuint index, const GLchar *nom ) { glBindAttribLocation( prog_, index, nom ); }

private:
   // La fonction textFileRead() charge en mémoire le contenu du fichier
   const GLchar *lireNuanceur( const GLchar *fich )
   {
      // Ouvrir le fichier
      std::ifstream fichier( fich );
      if ( fichier.fail() )
      {
         std::cerr << "!!! " << fich << ": " << strerror(errno) << std::endl;
         return NULL;
      }

      // Lire le fichier
      std::stringstream contenuFichier;
      contenuFichier << fichier.rdbuf();
      fichier.close();

      // Obtenir le contenu du fichier
      std::string contenu = contenuFichier.str();
      const int taille = contenu.size();

      // Retourner une chaîne pour le nuanceur
      char *source = new char[taille+1];
      strcpy( source, contenu.c_str() );
      return source;
   };

   bool attacherSource( GLuint type, const GLchar *fich )
   {
      const GLchar *source = lireNuanceur( fich );
      if ( source == NULL ) return( false );

      GLuint nuanceurObj = glCreateShader( type );
      glShaderSource( nuanceurObj, 1, &source, NULL );
      glCompileShader( nuanceurObj );
      glAttachShader( prog_, nuanceurObj );
      delete [] source;

      // afficher le message d'erreur, le cas échéant
      int infologLength = 0;
      glGetShaderiv( nuanceurObj, GL_INFO_LOG_LENGTH, &infologLength );
      if ( infologLength > 1 )
      {
         char* infoLog = new char[infologLength+1];
         int charsWritten = 0;
         glGetShaderInfoLog( nuanceurObj, infologLength, &charsWritten, infoLog );
         std::cout  << fich << std::endl << infoLog << std::endl;
         delete[] infoLog;
         return( false );
      }
      return( true );
   };

   bool lierProgramme( )
   {
      glLinkProgram( prog_ );

      // afficher le message d'erreur, le cas échéant
      int infologLength = 0;
      glGetProgramiv( prog_, GL_INFO_LOG_LENGTH, &infologLength );
      if ( infologLength > 1 )
      {
         char* infoLog = new char[infologLength+1];
         int charsWritten = 0;
         glGetProgramInfoLog( prog_, infologLength, &charsWritten, infoLog );
         std::cout << "prog_" << std::endl << infoLog << std::endl;
         delete[] infoLog;
         return( false );
      }
      return( true );
   }

   GLuint prog_; // LE programme
};

////////////////////////////////////////////////////////////////////////////
//
// Classe pour créer une fenêtre
//
////////////////////////////////////////////////////////////////////////////

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
#include <string.h>
#include <stdlib.h>

typedef enum {
#if defined(FENETRE_LIB_GLFW)

   TP_ESCAPE       = GLFW_KEY_ESCAPE,
   TP_UP           = GLFW_KEY_UP,
   TP_DOWN         = GLFW_KEY_DOWN,
   TP_PAGEUP       = GLFW_KEY_PAGE_UP,
   TP_PAGEDOWN     = GLFW_KEY_PAGE_DOWN,
   TP_HOME         = GLFW_KEY_HOME,
   TP_END          = GLFW_KEY_END,
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
            //   std::cout << "//@FenetreTP,WINDOWEVENT;" << " e.window.event=" << e.window.event << std::endl;
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
   static void imprimerInfosGL( const int verbose = 1 )
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
   static int VerifierErreurGL( const std::string message )
   {
      int rc = 0;
      GLenum err;
      while ( ( err = glGetError() ) != GL_NO_ERROR )
      {
         std::cerr << "Erreur OpenGL, " << message << " " << std::endl;
         switch ( err )
         {
         case GL_INVALID_ENUM:
            std::cerr << "GL_INVALID_ENUM: Valeur d'une énumération hors limite.";
            break;
         case GL_INVALID_VALUE:
            std::cerr << "GL_INVALID_VALUE: Valeur numérique hors limite.";
            break;
         case GL_INVALID_OPERATION:
            std::cerr << "GL_INVALID_OPERATION: Opération non permise dans l'état courant.";
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

////////////////////////////////////////////////////////////////////////////
//
// Fontion pour lire un fichier de type .BMP
//
////////////////////////////////////////////////////////////////////////////

/*
 * Windows Bitmap File Loader
 * Version 1.2.5 (20120929)
 *
 * Supported Formats: 1, 4, 8, 16, 24, 32 Bit Images
 * Alpha Bitmaps are also supported.
 * Supported compression types: RLE 8, BITFIELDS
 *
 * Created by: Benjamin Kalytta, 2006 - 2012
 * Thanks for bug fixes goes to: Chris Campbell
 *
 * Licence: Free to use, URL to my source and my name is required in your source code.
 *
 * Source can be found at http://www.kalytta.com/bitmap.h
 *
 * Warning: This code should not be used in unmodified form in a production environment.
 * It should only serve as a basis for your own development.
 * There is only a minimal error handling in this code. (Notice added 20111211)
 */

#include <fstream>
#include <string>

#ifndef __LITTLE_ENDIAN__
# ifndef __BIG_ENDIAN__
#  define __LITTLE_ENDIAN__
# endif
#endif

#ifdef __LITTLE_ENDIAN__
# define BITMAP_SIGNATURE 0x4d42
#else
# define BITMAP_SIGNATURE 0x424d
#endif

#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
typedef unsigned __int32 uint32_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int8 uint8_t;
typedef __int32 int32_t;
#elif defined(__GNUC__) || defined(__CYGWIN__) || defined(__MWERKS__) || defined(__WATCOMC__) || defined(__PGI) || defined(__LCC__)
# include <stdint.h>
#else
typedef unsigned int uint32_t;
typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;
typedef int int32_t;
#endif

#pragma pack(push, 1)

typedef struct _BITMAP_FILEHEADER {
   uint16_t Signature;
   uint32_t Size;
   uint32_t Reserved;
   uint32_t BitsOffset;
} BITMAP_FILEHEADER;

#define BITMAP_FILEHEADER_SIZE 14

typedef struct _BITMAP_HEADER {
   uint32_t HeaderSize;
   int32_t Width;
   int32_t Height;
   uint16_t Planes;
   uint16_t BitCount;
   uint32_t Compression;
   uint32_t SizeImage;
   int32_t PelsPerMeterX;
   int32_t PelsPerMeterY;
   uint32_t ClrUsed;
   uint32_t ClrImportant;
   uint32_t RedMask;
   uint32_t GreenMask;
   uint32_t BlueMask;
   uint32_t AlphaMask;
   uint32_t CsType;
   uint32_t Endpoints[9]; // see http://msdn2.microsoft.com/en-us/library/ms536569.aspx
   uint32_t GammaRed;
   uint32_t GammaGreen;
   uint32_t GammaBlue;
} BITMAP_HEADER;

typedef struct _RGBA {
   uint8_t Red;
   uint8_t Green;
   uint8_t Blue;
   uint8_t Alpha;
} RGBA;

typedef struct _BGRA {
   uint8_t Blue;
   uint8_t Green;
   uint8_t Red;
   uint8_t Alpha;
} BGRA;

#pragma pack(pop)

class CBitmap {
private:
   BITMAP_FILEHEADER m_BitmapFileHeader;
   BITMAP_HEADER m_BitmapHeader;
   RGBA *m_BitmapData;
   unsigned int m_BitmapSize;

   // Masks and bit counts shouldn't exceed 32 Bits
public:
   class CColor {
   public:
      static inline unsigned int BitCountByMask(unsigned int Mask) {
         unsigned int BitCount = 0;
         while (Mask) {
            Mask &= Mask - 1;
            BitCount++;
         }
         return BitCount;
      }

      static inline unsigned int BitPositionByMask(unsigned int Mask) {
         return BitCountByMask((Mask & (~Mask + 1)) - 1);
      }

      static inline unsigned int ComponentByMask(unsigned int Color, unsigned int Mask) {
         unsigned int Component = Color & Mask;
         return Component >> BitPositionByMask(Mask);
      }

      static inline unsigned int BitCountToMask(unsigned int BitCount) {
         return (BitCount == 32) ? 0xFFFFFFFF : (1 << BitCount) - 1;
      }

      static unsigned int Convert(unsigned int Color, unsigned int FromBitCount, unsigned int ToBitCount) {
         if (ToBitCount < FromBitCount) {
            Color >>= (FromBitCount - ToBitCount);
         } else {
            Color <<= (ToBitCount - FromBitCount);
            if (Color > 0) {
               Color |= BitCountToMask(ToBitCount - FromBitCount);
            }
         }
         return Color;
      }
   };

public:

   CBitmap() : m_BitmapData(0), m_BitmapSize(0)  {
      Dispose();
   }

   CBitmap(const char* Filename) : m_BitmapData(0), m_BitmapSize(0) {
      Load(Filename);
   }

   ~CBitmap() {
      Dispose();
   }

   void Dispose() {
      if (m_BitmapData) {
         delete[] m_BitmapData;
         m_BitmapData = 0;
      }
      memset(&m_BitmapFileHeader, 0, sizeof(m_BitmapFileHeader));
      memset(&m_BitmapHeader, 0, sizeof(m_BitmapHeader));
   }

   /* Load specified Bitmap and stores it as RGBA in an internal buffer */

   bool Load(const char *Filename) {
      std::ifstream file(Filename, std::ios::binary | std::ios::in);

      if (file.bad()) {
         return false;
      }

      if (file.is_open() == false) {
         return false;
      }

      Dispose();

      file.read((char*) &m_BitmapFileHeader, BITMAP_FILEHEADER_SIZE);
      if (m_BitmapFileHeader.Signature != BITMAP_SIGNATURE) {
         return false;
      }

      file.read((char*) &m_BitmapHeader, sizeof(BITMAP_HEADER));

      /* Load Color Table */

      file.seekg(BITMAP_FILEHEADER_SIZE + m_BitmapHeader.HeaderSize, std::ios::beg);

      unsigned int ColorTableSize = 0;

      if (m_BitmapHeader.BitCount == 1) {
         ColorTableSize = 2;
      } else if (m_BitmapHeader.BitCount == 4) {
         ColorTableSize = 16;
      } else if (m_BitmapHeader.BitCount == 8) {
         ColorTableSize = 256;
      }

      // Always allocate full sized color table

      BGRA* ColorTable = new BGRA[ColorTableSize]; // std::bad_alloc exception should be thrown if memory is not available

      file.read((char*) ColorTable, sizeof(BGRA) * m_BitmapHeader.ClrUsed);

      /* ... Color Table for 16 bits images are not supported yet */

      m_BitmapSize = GetWidth() * GetHeight();
      m_BitmapData = new RGBA[m_BitmapSize];

      unsigned int LineWidth = ((GetWidth() * GetBitCount() / 8) + 3) & ~3;
      uint8_t *Line = new uint8_t[LineWidth];

      file.seekg(m_BitmapFileHeader.BitsOffset, std::ios::beg);

      int Index = 0;
      bool Result = true;

      if (m_BitmapHeader.Compression == 0) {
         for (unsigned int i = 0; i < GetHeight(); i++) {
            file.read((char*) Line, LineWidth);

            uint8_t *LinePtr = Line;

            for (unsigned int j = 0; j < GetWidth(); j++) {
               if (m_BitmapHeader.BitCount == 1) {
                  uint32_t Color = *((uint8_t*) LinePtr);
                  for (int k = 0; k < 8; k++) {
                     m_BitmapData[Index].Red = ColorTable[Color & 0x80 ? 1 : 0].Red;
                     m_BitmapData[Index].Green = ColorTable[Color & 0x80 ? 1 : 0].Green;
                     m_BitmapData[Index].Blue = ColorTable[Color & 0x80 ? 1 : 0].Blue;
                     m_BitmapData[Index].Alpha = ColorTable[Color & 0x80 ? 1 : 0].Alpha;
                     Index++;
                     Color <<= 1;
                  }
                  LinePtr++;
                  j += 7;
               } else if (m_BitmapHeader.BitCount == 4) {
                  uint32_t Color = *((uint8_t*) LinePtr);
                  m_BitmapData[Index].Red = ColorTable[(Color >> 4) & 0x0f].Red;
                  m_BitmapData[Index].Green = ColorTable[(Color >> 4) & 0x0f].Green;
                  m_BitmapData[Index].Blue = ColorTable[(Color >> 4) & 0x0f].Blue;
                  m_BitmapData[Index].Alpha = ColorTable[(Color >> 4) & 0x0f].Alpha;
                  Index++;
                  m_BitmapData[Index].Red = ColorTable[Color & 0x0f].Red;
                  m_BitmapData[Index].Green = ColorTable[Color & 0x0f].Green;
                  m_BitmapData[Index].Blue = ColorTable[Color & 0x0f].Blue;
                  m_BitmapData[Index].Alpha = ColorTable[Color & 0x0f].Alpha;
                  Index++;
                  LinePtr++;
                  j++;
               } else if (m_BitmapHeader.BitCount == 8) {
                  uint32_t Color = *((uint8_t*) LinePtr);
                  m_BitmapData[Index].Red = ColorTable[Color].Red;
                  m_BitmapData[Index].Green = ColorTable[Color].Green;
                  m_BitmapData[Index].Blue = ColorTable[Color].Blue;
                  m_BitmapData[Index].Alpha = ColorTable[Color].Alpha;
                  Index++;
                  LinePtr++;
               } else if (m_BitmapHeader.BitCount == 16) {
                  uint32_t Color = *((uint16_t*) LinePtr);
                  m_BitmapData[Index].Red = ((Color >> 10) & 0x1f) << 3;
                  m_BitmapData[Index].Green = ((Color >> 5) & 0x1f) << 3;
                  m_BitmapData[Index].Blue = (Color & 0x1f) << 3;
                  m_BitmapData[Index].Alpha = 255;
                  Index++;
                  LinePtr += 2;
               } else if (m_BitmapHeader.BitCount == 24) {
                  uint32_t Color = *((uint32_t*) LinePtr);
                  m_BitmapData[Index].Blue = Color & 0xff;
                  m_BitmapData[Index].Green = (Color >> 8) & 0xff;
                  m_BitmapData[Index].Red = (Color >> 16) & 0xff;
                  m_BitmapData[Index].Alpha = 255;
                  Index++;
                  LinePtr += 3;
               } else if (m_BitmapHeader.BitCount == 32) {
                  uint32_t Color = *((uint32_t*) LinePtr);
                  m_BitmapData[Index].Blue = Color & 0xff;
                  m_BitmapData[Index].Green = (Color >> 8) & 0xff;
                  m_BitmapData[Index].Red = (Color >> 16) & 0xff;
                  m_BitmapData[Index].Alpha = Color >> 24;
                  Index++;
                  LinePtr += 4;
               }
            }
         }
      } else if (m_BitmapHeader.Compression == 1) { // RLE 8
         uint8_t Count = 0;
         uint8_t ColorIndex = 0;
         int x = 0, y = 0;

         while (file.eof() == false) {
            file.read((char*) &Count, sizeof(uint8_t));
            file.read((char*) &ColorIndex, sizeof(uint8_t));

            if (Count > 0) {
               Index = x + y * GetWidth();
               for (int k = 0; k < Count; k++) {
                  m_BitmapData[Index + k].Red = ColorTable[ColorIndex].Red;
                  m_BitmapData[Index + k].Green = ColorTable[ColorIndex].Green;
                  m_BitmapData[Index + k].Blue = ColorTable[ColorIndex].Blue;
                  m_BitmapData[Index + k].Alpha = ColorTable[ColorIndex].Alpha;
               }
               x += Count;
            } else if (Count == 0) {
               int Flag = ColorIndex;
               if (Flag == 0) {
                  x = 0;
                  y++;
               } else if (Flag == 1) {
                  break;
               } else if (Flag == 2) {
                  char rx = 0;
                  char ry = 0;
                  file.read((char*) &rx, sizeof(char));
                  file.read((char*) &ry, sizeof(char));
                  x += rx;
                  y += ry;
               } else {
                  Count = Flag;
                  Index = x + y * GetWidth();
                  for (int k = 0; k < Count; k++) {
                     file.read((char*) &ColorIndex, sizeof(uint8_t));
                     m_BitmapData[Index + k].Red = ColorTable[ColorIndex].Red;
                     m_BitmapData[Index + k].Green = ColorTable[ColorIndex].Green;
                     m_BitmapData[Index + k].Blue = ColorTable[ColorIndex].Blue;
                     m_BitmapData[Index + k].Alpha = ColorTable[ColorIndex].Alpha;
                  }
                  x += Count;
                  // Attention: Current Microsoft STL implementation seems to be buggy, tellg() always returns 0.
                  if (file.tellg() & 1) {
                     file.seekg(1, std::ios::cur);
                  }
               }
            }
         }
      } else if (m_BitmapHeader.Compression == 2) { // RLE 4
         /* RLE 4 is not supported */
         Result = false;
      } else if (m_BitmapHeader.Compression == 3) { // BITFIELDS

         /* We assumes that mask of each color component can be in any order */

         uint32_t BitCountRed = CColor::BitCountByMask(m_BitmapHeader.RedMask);
         uint32_t BitCountGreen = CColor::BitCountByMask(m_BitmapHeader.GreenMask);
         uint32_t BitCountBlue = CColor::BitCountByMask(m_BitmapHeader.BlueMask);
         uint32_t BitCountAlpha = CColor::BitCountByMask(m_BitmapHeader.AlphaMask);

         for (unsigned int i = 0; i < GetHeight(); i++) {
            file.read((char*) Line, LineWidth);

            uint8_t *LinePtr = Line;

            for (unsigned int j = 0; j < GetWidth(); j++) {

               uint32_t Color = 0;

               if (m_BitmapHeader.BitCount == 16) {
                  Color = *((uint16_t*) LinePtr);
                  LinePtr += 2;
               } else if (m_BitmapHeader.BitCount == 32) {
                  Color = *((uint32_t*) LinePtr);
                  LinePtr += 4;
               } else {
                  // Other formats are not valid
               }
               m_BitmapData[Index].Red = CColor::Convert(CColor::ComponentByMask(Color, m_BitmapHeader.RedMask), BitCountRed, 8);
               m_BitmapData[Index].Green = CColor::Convert(CColor::ComponentByMask(Color, m_BitmapHeader.GreenMask), BitCountGreen, 8);
               m_BitmapData[Index].Blue = CColor::Convert(CColor::ComponentByMask(Color, m_BitmapHeader.BlueMask), BitCountBlue, 8);
               m_BitmapData[Index].Alpha = CColor::Convert(CColor::ComponentByMask(Color, m_BitmapHeader.AlphaMask), BitCountAlpha, 8);

               Index++;
            }
         }
      }

      delete [] ColorTable;
      delete [] Line;

      file.close();
      return Result;
   }

   bool Save(const char* Filename, unsigned int BitCount = 32) {
      bool Result = true;

      std::ofstream file(Filename, std::ios::out | std::ios::binary);

      if (file.is_open() == false) {
         return false;
      }

      BITMAP_FILEHEADER bfh;
      BITMAP_HEADER bh;
      memset(&bfh, 0, sizeof(bfh));
      memset(&bh, 0, sizeof(bh));

      bfh.Signature = BITMAP_SIGNATURE;
      bfh.BitsOffset = BITMAP_FILEHEADER_SIZE + sizeof(BITMAP_HEADER);
      bfh.Size = (GetWidth() * GetHeight() * BitCount) / 8 + bfh.BitsOffset;

      bh.HeaderSize = sizeof(BITMAP_HEADER);
      bh.BitCount = BitCount;

      if (BitCount == 32) {
         bh.Compression = 3; // BITFIELD
         bh.AlphaMask = 0xff000000;
         bh.BlueMask = 0x00ff0000;
         bh.GreenMask = 0x0000ff00;
         bh.RedMask = 0x000000ff;
      } else if (BitCount == 16) {
         bh.Compression = 3; // BITFIELD
         bh.AlphaMask = 0x00000000;
         bh.BlueMask = 0x0000001f;
         bh.GreenMask = 0x000007E0;
         bh.RedMask = 0x0000F800;
      } else {
         bh.Compression = 0; // RGB
      }

      unsigned int LineWidth = (GetWidth() + 3) & ~3;

      bh.Planes = 1;
      bh.Height = GetHeight();
      bh.Width = GetWidth();
      bh.SizeImage = (LineWidth * BitCount * GetHeight()) / 8;
      bh.PelsPerMeterX = 3780;
      bh.PelsPerMeterY = 3780;

      if (BitCount == 32) {
         file.write((char*) &bfh, sizeof(BITMAP_FILEHEADER));
         file.write((char*) &bh, sizeof(BITMAP_HEADER));
         file.write((char*) m_BitmapData, bh.SizeImage);
      } else if (BitCount < 16) {
         uint8_t* Bitmap = new uint8_t[bh.SizeImage];

         BGRA *Palette = 0;
         unsigned int PaletteSize = 0;

         if (GetBitsWithPalette(Bitmap, bh.SizeImage, BitCount, Palette, PaletteSize)) {
            bfh.BitsOffset += PaletteSize * sizeof(BGRA);

            file.write((char*) &bfh, BITMAP_FILEHEADER_SIZE);
            file.write((char*) &bh, sizeof(BITMAP_HEADER));
            file.write((char*) Palette, PaletteSize * sizeof(BGRA));
            file.write((char*) Bitmap, bh.SizeImage);
         }
         delete [] Bitmap;
         delete [] Palette;
      } else {
         uint32_t RedMask = 0;
         uint32_t GreenMask = 0;
         uint32_t BlueMask = 0;
         uint32_t AlphaMask = 0;

         if (BitCount == 16) {
            RedMask = 0x0000F800;
            GreenMask = 0x000007E0;
            BlueMask = 0x0000001F;
            AlphaMask = 0x00000000;
         } else if (BitCount == 24) {
            RedMask = 0x00FF0000;
            GreenMask = 0x0000FF00;
            BlueMask = 0x000000FF;
         } else {
            // Other color formats are not valid
            Result = false;
         }

         if (Result) {
            if (GetBits(NULL, bh.SizeImage, RedMask, GreenMask, BlueMask, AlphaMask)) {
               uint8_t* Bitmap = new uint8_t[bh.SizeImage];
               if (GetBits(Bitmap, bh.SizeImage, RedMask, GreenMask, BlueMask, AlphaMask)) {
                  file.write((char*) &bfh, sizeof(BITMAP_FILEHEADER));
                  file.write((char*) &bh, sizeof(BITMAP_HEADER));
                  file.write((char*) Bitmap, bh.SizeImage);
               }
               delete [] Bitmap;
            }
         }
      }

      file.close();
      return Result;
   }

   unsigned int GetWidth() {
      /* Add plausibility test */
      // if (abs(m_BitmapHeader.Width) > 8192) {
      //	m_BitmapHeader.Width = 8192;
      // }
      return m_BitmapHeader.Width < 0 ? -m_BitmapHeader.Width : m_BitmapHeader.Width;
   }

   unsigned int GetHeight() {
      /* Add plausibility test */
      // if (abs(m_BitmapHeader.Height) > 8192) {
      //	m_BitmapHeader.Height = 8192;
      // }
      return m_BitmapHeader.Height < 0 ? -m_BitmapHeader.Height : m_BitmapHeader.Height;
   }

   unsigned int GetBitCount() {
      /* Add plausibility test */
      // if (m_BitmapHeader.BitCount > 32) {
      //	m_BitmapHeader.BitCount = 32;
      // }
      return m_BitmapHeader.BitCount;
   }

   /* Copies internal RGBA buffer to user specified buffer */

   bool GetBits(void* Buffer, unsigned int &Size) {
      bool Result = false;
      if (Size == 0 || Buffer == 0) {
         Size = m_BitmapSize * sizeof(RGBA);
         Result = m_BitmapSize != 0;
      } else {
         memcpy(Buffer, m_BitmapData, Size);
         Result = true;
      }
      return Result;
   }

   /* Returns internal RGBA buffer */

   void* GetBits() {
      return m_BitmapData;
   }

   /* Copies internal RGBA buffer to user specified buffer and converts it into destination
    * bit format specified by component masks.
    *
    * Typical Bitmap color formats (BGR/BGRA):
    *
    * Masks for 16 bit (5-5-5): ALPHA = 0x00000000, RED = 0x00007C00, GREEN = 0x000003E0, BLUE = 0x0000001F
    * Masks for 16 bit (5-6-5): ALPHA = 0x00000000, RED = 0x0000F800, GREEN = 0x000007E0, BLUE = 0x0000001F
    * Masks for 24 bit: ALPHA = 0x00000000, RED = 0x00FF0000, GREEN = 0x0000FF00, BLUE = 0x000000FF
    * Masks for 32 bit: ALPHA = 0xFF000000, RED = 0x00FF0000, GREEN = 0x0000FF00, BLUE = 0x000000FF
    *
    * Other color formats (RGB/RGBA):
    *
    * Masks for 32 bit (RGBA): ALPHA = 0xFF000000, RED = 0x000000FF, GREEN = 0x0000FF00, BLUE = 0x00FF0000
    *
    * Bit count will be rounded to next 8 bit boundary. If IncludePadding is true, it will be ensured
    * that line width is a multiple of 4. padding bytes are included if necessary.
    *
    * NOTE: systems with big endian byte order may require masks in inversion order.
    */

   bool GetBits(void* Buffer, unsigned int &Size, unsigned int RedMask, unsigned int GreenMask, unsigned int BlueMask, unsigned int AlphaMask, bool IncludePadding = true) {
      bool Result = false;

      uint32_t BitCountRed = CColor::BitCountByMask(RedMask);
      uint32_t BitCountGreen = CColor::BitCountByMask(GreenMask);
      uint32_t BitCountBlue = CColor::BitCountByMask(BlueMask);
      uint32_t BitCountAlpha = CColor::BitCountByMask(AlphaMask);

      unsigned int BitCount = (BitCountRed + BitCountGreen + BitCountBlue + BitCountAlpha + 7) & ~7;

      if (BitCount > 32) {
         return false;
      }

      unsigned int w = GetWidth();
      //unsigned int LineWidth = (w + 3) & ~3;
      unsigned int dataBytesPerLine = (w * BitCount + 7) / 8;
      unsigned int LineWidth = (dataBytesPerLine + 3) & ~3;

      if (Size == 0 || Buffer == 0) {
         //Size = (LineWidth * GetHeight() * BitCount) / 8 + sizeof(unsigned int);
         Size = (GetWidth() * GetHeight() * BitCount) / 8 + sizeof(unsigned int);
         return true;
      }

      uint8_t* BufferPtr = (uint8_t*) Buffer;

      Result = true;

      uint32_t BitPosRed = CColor::BitPositionByMask(RedMask);
      uint32_t BitPosGreen = CColor::BitPositionByMask(GreenMask);
      uint32_t BitPosBlue = CColor::BitPositionByMask(BlueMask);
      uint32_t BitPosAlpha = CColor::BitPositionByMask(AlphaMask);

      unsigned int j = 0;

      for (unsigned int i = 0; i < m_BitmapSize; i++) {
         *(uint32_t*) BufferPtr =
         (CColor::Convert(m_BitmapData[i].Blue, 8, BitCountBlue) << BitPosBlue) |
         (CColor::Convert(m_BitmapData[i].Green, 8, BitCountGreen) << BitPosGreen) |
         (CColor::Convert(m_BitmapData[i].Red, 8, BitCountRed) << BitPosRed) |
         (CColor::Convert(m_BitmapData[i].Alpha, 8, BitCountAlpha) << BitPosAlpha);

         if (IncludePadding) {
            j++;
            if (j >= w) {
               for (unsigned int k = 0; k < LineWidth - dataBytesPerLine; k++) {
                  BufferPtr += (BitCount >> 3);
               }
               j = 0;
            }
         }

         BufferPtr += (BitCount >> 3);
      }

      Size -= sizeof(unsigned int);

      return Result;
   }

   /* See GetBits().
    * It creates a corresponding color table (palette) which have to be destroyed by the user after usage.
    *
    * Supported Bit depths are: 4, 8
    *
    * Todo: Optimize, use optimized palette, do ditehring (see my dithering class), support padding for 4 bit bitmaps
    */

   bool GetBitsWithPalette(void* Buffer, unsigned int &Size, unsigned int BitCount, BGRA* &Palette, unsigned int &PaletteSize, bool OptimalPalette = false, bool IncludePadding = true) {
      bool Result = false;

      if (BitCount > 16) {
         return false;
      }

      unsigned int w = GetWidth();
      unsigned int dataBytesPerLine = (w * BitCount + 7) / 8;
      unsigned int LineWidth = (dataBytesPerLine + 3) & ~3;

      if (Size == 0 || Buffer == 0) {
         Size = (LineWidth * GetHeight() * BitCount) / 8;
         return true;
      }


      if (OptimalPalette) {
         PaletteSize = 0;
         // Not implemented
      } else {
         if (BitCount == 1) {
            PaletteSize = 2;
            // Not implemented: Who need that?
         } else if (BitCount == 4) { // 2:2:1
            PaletteSize = 16;
            Palette = new BGRA[PaletteSize];
            for (int r = 0; r < 4; r++) {
               for (int g = 0; g < 2; g++) {
                  for (int b = 0; b < 2; b++) {
                     Palette[r | g << 2 | b << 3].Red = r ? (r << 6) | 0x3f : 0;
                     Palette[r | g << 2 | b << 3].Green = g ? (g << 7) | 0x7f : 0;
                     Palette[r | g << 2 | b << 3].Blue = b ? (b << 7) | 0x7f : 0;
                     Palette[r | g << 2 | b << 3].Alpha = 0xff;
                  }
               }
            }
         } else if (BitCount == 8) { // 3:3:2
            PaletteSize = 256;
            Palette = new BGRA[PaletteSize];
            for (int r = 0; r < 8; r++) {
               for (int g = 0; g < 8; g++) {
                  for (int b = 0; b < 4; b++) {
                     Palette[r | g << 3 | b << 6].Red = r ? (r << 5) | 0x1f : 0;
                     Palette[r | g << 3 | b << 6].Green = g ? (g << 5) | 0x1f : 0;
                     Palette[r | g << 3 | b << 6].Blue = b ? (b << 6) | 0x3f : 0;
                     Palette[r | g << 3 | b << 6].Alpha = 0xff;
                  }
               }
            }
         } else if (BitCount == 16) { // 5:5:5
            // Not implemented
         }
      }

      unsigned int j = 0;
      uint8_t* BufferPtr = (uint8_t*) Buffer;

      for (unsigned int i = 0; i < m_BitmapSize; i++) {
         if (BitCount == 1) {
            // Not implemented: Who needs that?
         } else if (BitCount == 4) {
            *BufferPtr = ((m_BitmapData[i].Red >> 6) | (m_BitmapData[i].Green >> 7) << 2 | (m_BitmapData[i].Blue >> 7) << 3) << 4;
            i++;
            *BufferPtr |= (m_BitmapData[i].Red >> 6) | (m_BitmapData[i].Green >> 7) << 2 | (m_BitmapData[i].Blue >> 7) << 3;
         } else if (BitCount == 8) {
            *BufferPtr = (m_BitmapData[i].Red >> 5) | (m_BitmapData[i].Green >> 5) << 3 | (m_BitmapData[i].Blue >> 5) << 6;
         } else if (BitCount == 16) {
            // Not implemented
         }

         if (IncludePadding) {
            j++;
            if (j >= w) {
               for (unsigned int k = 0; k < (LineWidth - dataBytesPerLine); k++) {
                  BufferPtr += BitCount / 8;
               }
               j = 0;
            }
         }

         BufferPtr++;
      }

      Result = true;

      return Result;
   }

   /* Set Bitmap Bits. Will be converted to RGBA internally */

   bool SetBits(void* Buffer, unsigned int Width, unsigned int Height, unsigned int RedMask, unsigned int GreenMask, unsigned int BlueMask, unsigned int AlphaMask = 0) {
      if (Buffer == 0) {
         return false;
      }

      uint8_t *BufferPtr = (uint8_t*) Buffer;

      Dispose();

      m_BitmapHeader.Width = Width;
      m_BitmapHeader.Height = Height;
      m_BitmapHeader.BitCount = 32;
      m_BitmapHeader.Compression = 3;

      m_BitmapSize = GetWidth() * GetHeight();
      m_BitmapData = new RGBA[m_BitmapSize];

      /* Find bit count by masks (rounded to next 8 bit boundary) */

      unsigned int BitCount = (CColor::BitCountByMask(RedMask | GreenMask | BlueMask | AlphaMask) + 7) & ~7;

      uint32_t BitCountRed = CColor::BitCountByMask(RedMask);
      uint32_t BitCountGreen = CColor::BitCountByMask(GreenMask);
      uint32_t BitCountBlue = CColor::BitCountByMask(BlueMask);
      uint32_t BitCountAlpha = CColor::BitCountByMask(AlphaMask);

      for (unsigned int i = 0; i < m_BitmapSize; i++) {
         unsigned int Color = 0;
         if (BitCount <= 8) {
            Color = *((uint8_t*) BufferPtr);
            BufferPtr += 1;
         } else if (BitCount <= 16) {
            Color = *((uint16_t*) BufferPtr);
            BufferPtr += 2;
         } else if (BitCount <= 24) {
            Color = *((uint32_t*) BufferPtr);
            BufferPtr += 3;
         } else if (BitCount <= 32) {
            Color = *((uint32_t*) BufferPtr);
            BufferPtr += 4;
         } else {
            /* unsupported */
            BufferPtr += 1;
         }
         m_BitmapData[i].Alpha = CColor::Convert(CColor::ComponentByMask(Color, AlphaMask), BitCountAlpha, 8);
         m_BitmapData[i].Red = CColor::Convert(CColor::ComponentByMask(Color, RedMask), BitCountRed, 8);
         m_BitmapData[i].Green = CColor::Convert(CColor::ComponentByMask(Color, GreenMask), BitCountGreen, 8);
         m_BitmapData[i].Blue = CColor::Convert(CColor::ComponentByMask(Color, BlueMask), BitCountBlue, 8);
      }

      return true;
   }
};

////////////////////////////////////////////////////////////////////////////
//
// Fonction pour charger une texture
//
////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
//#include <SDL_surface.h>

bool ChargerTexture( std::string fichier, GLuint &texture )
{
   // vérifier la présence du fichier BMP en essayant de l'ouvrir
   FILE *img = fopen( fichier.c_str(), "r" );
   if ( !img )
   {
      std::cerr << "Fichier de texture manquant: " << fichier << std::endl;
      return false;
   }
   fclose( img );

   // créer un objet Bitmap et obtenir la taille de l'image et les pixels RGBA
   CBitmap bitmap( fichier.c_str() );
   unsigned int size = bitmap.GetWidth() * bitmap.GetHeight() * 4; // w * h * 4 composantes
   unsigned char *pixels = new unsigned char[size];
   bitmap.GetBits( (void*) pixels, size, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000 ); // obtenir les composantes RGBA

   //SDL_Surface* surface = SDL_LoadBMP( fichier.c_str() );
   // https://wiki.libsdl.org/SDL_LoadBMP

   // création de la texture OpenGL
   glGenTextures( 1, &texture );
   glBindTexture( GL_TEXTURE_2D, texture );
   //glTexImage2D( GL_TEXTURE_2D, 0, 3, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels );

   glTexImage2D( GL_TEXTURE_2D, 0, 3, bitmap.GetWidth(), bitmap.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );// au lieu de GL_NEAREST_MIPMAP_LINEAR
   glBindTexture( GL_TEXTURE_2D, 0 );

   // faire le ménage
   delete[] pixels;

   return true;
}

#if !defined( __APPLE__ )
////////////////////////////////////////////////////////////////////////////
//
// Fonctions variées pour afficher des formes connues
//
////////////////////////////////////////////////////////////////////////////

/* Copyright (c) Mark J. Kilgard, 1994, 1997. */

/**
   (c) Copyright 1993, Silicon Graphics, Inc.

   ALL RIGHTS RESERVED

   Permission to use, copy, modify, and distribute this software
   for any purpose and without fee is hereby granted, provided
   that the above copyright notice appear in all copies and that
   both the copyright notice and this permission notice appear in
   supporting documentation, and that the name of Silicon
   Graphics, Inc. not be used in advertising or publicity
   pertaining to distribution of the software without specific,
   written prior permission.

   THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU
   "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR
   OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  IN NO
   EVENT SHALL SILICON GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE
   ELSE FOR ANY DIRECT, SPECIAL, INCIDENTAL, INDIRECT OR
   CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER,
   INCLUDING WITHOUT LIMITATION, LOSS OF PROFIT, LOSS OF USE,
   SAVINGS OR REVENUE, OR THE CLAIMS OF THIRD PARTIES, WHETHER OR
   NOT SILICON GRAPHICS, INC.  HAS BEEN ADVISED OF THE POSSIBILITY
   OF SUCH LOSS, HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   ARISING OUT OF OR IN CONNECTION WITH THE POSSESSION, USE OR
   PERFORMANCE OF THIS SOFTWARE.

   US Government Users Restricted Rights

   Use, duplication, or disclosure by the Government is subject to
   restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
   (c)(1)(ii) of the Rights in Technical Data and Computer
   Software clause at DFARS 252.227-7013 and/or in similar or
   successor clauses in the FAR or the DOD or NASA FAR
   Supplement.  Unpublished-- rights reserved under the copyright
   laws of the United States.  Contractor/manufacturer is Silicon
   Graphics, Inc., 2011 N.  Shoreline Blvd., Mountain View, CA
   94039-7311.

   OpenGL(TM) is a trademark of Silicon Graphics, Inc.
*/

#include <math.h>
#include <stdio.h>

static GLUquadricObj *quadObj;

#define QUAD_OBJ_INIT() { if(!quadObj) initQuadObj(); }

static void initQuadObj(void)
{
   quadObj = gluNewQuadric();
   if (!quadObj)
      printf("out of memory.");
}

void shapesWireSphere(GLdouble radius, GLint slices, GLint stacks)
{
   QUAD_OBJ_INIT();
   gluQuadricDrawStyle(quadObj, GLU_LINE);
   gluQuadricNormals(quadObj, GLU_SMOOTH);
   gluQuadricTexture(quadObj, GLU_TRUE);
   /* If we ever changed/used the texture or orientation state
      of quadObj, we'd need to change it to the defaults here
      with gluQuadricTexture and/or gluQuadricOrientation. */
   gluSphere(quadObj, radius, slices, stacks);
}

void shapesSolidSphere(GLdouble radius, GLint slices, GLint stacks)
{
   QUAD_OBJ_INIT();
   gluQuadricDrawStyle(quadObj, GLU_FILL);
   gluQuadricNormals(quadObj, GLU_SMOOTH);
   gluQuadricTexture(quadObj, GLU_TRUE);
   /* If we ever changed/used the texture or orientation state
      of quadObj, we'd need to change it to the defaults here
      with gluQuadricTexture and/or gluQuadricOrientation. */
   gluSphere(quadObj, radius, slices, stacks);
}

void shapesWireCylinder(GLdouble base, GLdouble height,
                        GLint slices, GLint stacks)
{
   QUAD_OBJ_INIT();
   gluQuadricDrawStyle(quadObj, GLU_LINE);
   gluQuadricNormals(quadObj, GLU_SMOOTH);
   gluQuadricTexture(quadObj, GLU_TRUE);
   /* If we ever changed/used the texture or orientation state
      of quadObj, we'd need to change it to the defaults here
      with gluQuadricTexture and/or gluQuadricOrientation. */
   gluCylinder(quadObj, base, base, height, slices, stacks);
}

void shapesSolidCylinder(GLdouble base, GLdouble height,
                         GLint slices, GLint stacks)
{
   QUAD_OBJ_INIT();
   gluQuadricDrawStyle(quadObj, GLU_FILL);
   gluQuadricNormals(quadObj, GLU_SMOOTH);
   gluQuadricTexture(quadObj, GLU_TRUE);
   /* If we ever changed/used the texture or orientation state
      of quadObj, we'd need to change it to the defaults here
      with gluQuadricTexture and/or gluQuadricOrientation. */
   gluCylinder(quadObj, base, base, height, slices, stacks);
}

void shapesWireCone(GLdouble base, GLdouble height,
                    GLint slices, GLint stacks)
{
   QUAD_OBJ_INIT();
   gluQuadricDrawStyle(quadObj, GLU_LINE);
   gluQuadricNormals(quadObj, GLU_SMOOTH);
   gluQuadricTexture(quadObj, GLU_TRUE);
   /* If we ever changed/used the texture or orientation state
      of quadObj, we'd need to change it to the defaults here
      with gluQuadricTexture and/or gluQuadricOrientation. */
   gluCylinder(quadObj, base, 0.0, height, slices, stacks);
}

void shapesSolidCone(GLdouble base, GLdouble height,
                     GLint slices, GLint stacks)
{
   QUAD_OBJ_INIT();
   gluQuadricDrawStyle(quadObj, GLU_FILL);
   gluQuadricNormals(quadObj, GLU_SMOOTH);
   gluQuadricTexture(quadObj, GLU_TRUE);
   /* If we ever changed/used the texture or orientation state
      of quadObj, we'd need to change it to the defaults here
      with gluQuadricTexture and/or gluQuadricOrientation. */
   gluCylinder(quadObj, base, 0.0, height, slices, stacks);
}

static void drawBox(GLfloat size, GLenum type)
{
   static GLfloat n[6][3] =
   {
      {-1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0},
      {0.0, 0.0, 1.0},
      {0.0, 0.0, -1.0}
   };
   static GLint faces[6][4] =
   {
      {0, 1, 2, 3},
      {3, 2, 6, 7},
      {7, 6, 5, 4},
      {4, 5, 1, 0},
      {5, 6, 2, 1},
      {7, 4, 0, 3}
   };
   GLfloat v[8][3];
   GLint i;

   v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
   v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
   v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
   v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
   v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
   v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

   for (i = 5; i >= 0; i--) {
      glBegin(type);
      glNormal3fv(&n[i][0]);
      glVertex3fv(&v[faces[i][0]][0]);
      glVertex3fv(&v[faces[i][1]][0]);
      glVertex3fv(&v[faces[i][2]][0]);
      glVertex3fv(&v[faces[i][3]][0]);
      glEnd();
   }
}

void shapesWireCube(GLdouble size)
{
   drawBox(size, GL_LINE_LOOP);
}

void shapesSolidCube(GLdouble size)
{
   drawBox(size, GL_QUADS);
}


static void doughnut(GLfloat r, GLfloat R, GLint nsides, GLint rings)
{
   int i, j;
   GLfloat theta, phi, theta1;
   GLfloat cosTheta, sinTheta;
   GLfloat cosTheta1, sinTheta1;
   GLfloat ringDelta, sideDelta;

   ringDelta = 2.0 * M_PI / rings;
   sideDelta = 2.0 * M_PI / nsides;

   theta = 0.0;
   cosTheta = 1.0;
   sinTheta = 0.0;
   for (i = rings - 1; i >= 0; i--)
   {
      theta1 = theta + ringDelta;
      cosTheta1 = cos(theta1);
      sinTheta1 = sin(theta1);
      glBegin(GL_QUAD_STRIP);
      phi = 0.0;
      for (j = nsides; j >= 0; j--)
      {
         GLfloat cosPhi, sinPhi, dist;

         phi += sideDelta;
         cosPhi = cos(phi);
         sinPhi = sin(phi);
         dist = R + r * cosPhi;

         glNormal3f(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
         glTexCoord2f((4.0*(i+1))/rings, (4.0*j)/nsides);
         glVertex3f(cosTheta1 * dist, -sinTheta1 * dist, r * sinPhi);
         glNormal3f(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
         glTexCoord2f((4.0*i)/rings, (4.0*j)/nsides);
         glVertex3f(cosTheta * dist, -sinTheta * dist,  r * sinPhi);
      }
      glEnd();
      theta = theta1;
      cosTheta = cosTheta1;
      sinTheta = sinTheta1;
   }
}

void shapesWireTorus(GLdouble innerRadius, GLdouble outerRadius,
                     GLint nsides, GLint rings)
{
   glPushAttrib(GL_POLYGON_BIT);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   doughnut(innerRadius, outerRadius, nsides, rings);
   glPopAttrib();
}

void shapesSolidTorus(GLdouble innerRadius, GLdouble outerRadius,
                      GLint nsides, GLint rings)
{
   doughnut(innerRadius, outerRadius, nsides, rings);
}

static GLfloat dodec[20][3];

static void initDodecahedron(void)
{
   GLfloat alpha, beta;

   alpha = sqrt(2.0 / (3.0 + sqrt(5.0)));
   beta = 1.0 + sqrt(6.0 / (3.0 + sqrt(5.0)) - 2.0 + 2.0 * sqrt(2.0 / (3.0 + sqrt(5.0))));
   dodec[0][0] = -alpha; dodec[0][1] = 0; dodec[0][2] = beta;
   dodec[1][0] = alpha; dodec[1][1] = 0; dodec[1][2] = beta;
   dodec[2][0] = -1; dodec[2][1] = -1; dodec[2][2] = -1;
   dodec[3][0] = -1; dodec[3][1] = -1; dodec[3][2] = 1;
   dodec[4][0] = -1; dodec[4][1] = 1; dodec[4][2] = -1;
   dodec[5][0] = -1; dodec[5][1] = 1; dodec[5][2] = 1;
   dodec[6][0] = 1; dodec[6][1] = -1; dodec[6][2] = -1;
   dodec[7][0] = 1; dodec[7][1] = -1; dodec[7][2] = 1;
   dodec[8][0] = 1; dodec[8][1] = 1; dodec[8][2] = -1;
   dodec[9][0] = 1; dodec[9][1] = 1; dodec[9][2] = 1;
   dodec[10][0] = beta; dodec[10][1] = alpha; dodec[10][2] = 0;
   dodec[11][0] = beta; dodec[11][1] = -alpha; dodec[11][2] = 0;
   dodec[12][0] = -beta; dodec[12][1] = alpha; dodec[12][2] = 0;
   dodec[13][0] = -beta; dodec[13][1] = -alpha; dodec[13][2] = 0;
   dodec[14][0] = -alpha; dodec[14][1] = 0; dodec[14][2] = -beta;
   dodec[15][0] = alpha; dodec[15][1] = 0; dodec[15][2] = -beta;
   dodec[16][0] = 0; dodec[16][1] = beta; dodec[16][2] = alpha;
   dodec[17][0] = 0; dodec[17][1] = beta; dodec[17][2] = -alpha;
   dodec[18][0] = 0; dodec[18][1] = -beta; dodec[18][2] = alpha;
   dodec[19][0] = 0; dodec[19][1] = -beta; dodec[19][2] = -alpha;

}

#define DIFF3(_a,_b,_c) {                       \
      (_c)[0] = (_a)[0] - (_b)[0];              \
      (_c)[1] = (_a)[1] - (_b)[1];              \
      (_c)[2] = (_a)[2] - (_b)[2];              \
   }

static void crossprod(GLfloat v1[3], GLfloat v2[3], GLfloat prod[3])
{
   GLfloat p[3];         /* in case prod == v1 or v2 */

   p[0] = v1[1] * v2[2] - v2[1] * v1[2];
   p[1] = v1[2] * v2[0] - v2[2] * v1[0];
   p[2] = v1[0] * v2[1] - v2[0] * v1[1];
   prod[0] = p[0];
   prod[1] = p[1];
   prod[2] = p[2];
}

static void normalize(GLfloat v[3])
{
   GLfloat d;

   d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
   if (d == 0.0)
   {
      printf("normalize: zero length vector");
      v[0] = d = 1.0;
   }
   d = 1 / d;
   v[0] *= d;
   v[1] *= d;
   v[2] *= d;
}

static void pentagon(int a, int b, int c, int d, int e, GLenum shadeType)
{
   GLfloat n0[3], d1[3], d2[3];

   DIFF3(dodec[a], dodec[b], d1);
   DIFF3(dodec[b], dodec[c], d2);
   crossprod(d1, d2, n0);
   normalize(n0);

   glBegin(shadeType);
   glNormal3fv(n0);
   glVertex3fv(&dodec[a][0]);
   glVertex3fv(&dodec[b][0]);
   glVertex3fv(&dodec[c][0]);
   glVertex3fv(&dodec[d][0]);
   glVertex3fv(&dodec[e][0]);
   glEnd();
}

static void dodecahedron(GLenum type)
{
   static int inited = 0;

   if (inited == 0)
   {
      inited = 1;
      initDodecahedron();
   }
   pentagon(0, 1, 9, 16, 5, type);
   pentagon(1, 0, 3, 18, 7, type);
   pentagon(1, 7, 11, 10, 9, type);
   pentagon(11, 7, 18, 19, 6, type);
   pentagon(8, 17, 16, 9, 10, type);
   pentagon(2, 14, 15, 6, 19, type);
   pentagon(2, 13, 12, 4, 14, type);
   pentagon(2, 19, 18, 3, 13, type);
   pentagon(3, 0, 5, 12, 13, type);
   pentagon(6, 15, 8, 10, 11, type);
   pentagon(4, 17, 8, 15, 14, type);
   pentagon(4, 12, 5, 16, 17, type);
}

void shapesWireDodecahedron(void)
{
   dodecahedron(GL_LINE_LOOP);
}

void shapesSolidDodecahedron(void)
{
   dodecahedron(GL_TRIANGLE_FAN);
}

static void recorditem(GLfloat * n1, GLfloat * n2, GLfloat * n3,
                       GLenum shadeType)
{
   GLfloat q0[3], q1[3];

   DIFF3(n1, n2, q0);
   DIFF3(n2, n3, q1);
   crossprod(q0, q1, q1);
   normalize(q1);

   glBegin(shadeType);
   glNormal3fv(q1);
   glVertex3fv(n1);
   glVertex3fv(n2);
   glVertex3fv(n3);
   glEnd();
}

static void subdivide(GLfloat * v0, GLfloat * v1, GLfloat * v2,
                      GLenum shadeType)
{
   int depth;
   GLfloat w0[3], w1[3], w2[3];
   GLfloat l;
   int i, j, k, n;

   depth = 1;
   for (i = 0; i < depth; i++)
   {
      for (j = 0; i + j < depth; j++)
      {
         k = depth - i - j;
         for (n = 0; n < 3; n++) {
            w0[n] = (i * v0[n] + j * v1[n] + k * v2[n]) / depth;
            w1[n] = ((i + 1) * v0[n] + j * v1[n] + (k - 1) * v2[n])
            / depth;
            w2[n] = (i * v0[n] + (j + 1) * v1[n] + (k - 1) * v2[n])
            / depth;
         }
         l = sqrt(w0[0] * w0[0] + w0[1] * w0[1] + w0[2] * w0[2]);
         w0[0] /= l;
         w0[1] /= l;
         w0[2] /= l;
         l = sqrt(w1[0] * w1[0] + w1[1] * w1[1] + w1[2] * w1[2]);
         w1[0] /= l;
         w1[1] /= l;
         w1[2] /= l;
         l = sqrt(w2[0] * w2[0] + w2[1] * w2[1] + w2[2] * w2[2]);
         w2[0] /= l;
         w2[1] /= l;
         w2[2] /= l;
         recorditem(w1, w0, w2, shadeType);
      }
   }
}

static void drawtriangle(int i, GLfloat data[][3], int ndx[][3],
                         GLenum shadeType)
{
   GLfloat *x0, *x1, *x2;

   x0 = data[ndx[i][0]];
   x1 = data[ndx[i][1]];
   x2 = data[ndx[i][2]];
   subdivide(x0, x1, x2, shadeType);
}

/* octahedron data: The octahedron produced is centered at the
   origin and has radius 1.0 */
static GLfloat odata[6][3] =
{
   {1.0, 0.0, 0.0},
   {-1.0, 0.0, 0.0},
   {0.0, 1.0, 0.0},
   {0.0, -1.0, 0.0},
   {0.0, 0.0, 1.0},
   {0.0, 0.0, -1.0}
};

static int ondex[8][3] =
{
   {0, 4, 2},
   {1, 2, 4},
   {0, 3, 4},
   {1, 4, 3},
   {0, 2, 5},
   {1, 5, 2},
   {0, 5, 3},
   {1, 3, 5}
};

static void octahedron(GLenum shadeType)
{
   int i;

   for (i = 7; i >= 0; i--)
   {
      drawtriangle(i, odata, ondex, shadeType);
   }
}

void shapesWireOctahedron(void)
{
   octahedron(GL_LINE_LOOP);
}

void shapesSolidOctahedron(void)
{
   octahedron(GL_TRIANGLES);
}


/* icosahedron data: These numbers are rigged to make an
   icosahedron of radius 1.0 */

#define X .525731112119133606
#define Z .850650808352039932

static GLfloat idata[12][3] =
{
   {-X, 0, Z},
   {X, 0, Z},
   {-X, 0, -Z},
   {X, 0, -Z},
   {0, Z, X},
   {0, Z, -X},
   {0, -Z, X},
   {0, -Z, -X},
   {Z, X, 0},
   {-Z, X, 0},
   {Z, -X, 0},
   {-Z, -X, 0}
};

static int connectivity[20][3] =
{
   {0, 4, 1},
   {0, 9, 4},
   {9, 5, 4},
   {4, 5, 8},
   {4, 8, 1},
   {8, 10, 1},
   {8, 3, 10},
   {5, 3, 8},
   {5, 2, 3},
   {2, 7, 3},
   {7, 10, 3},
   {7, 6, 10},
   {7, 11, 6},
   {11, 0, 6},
   {0, 1, 6},
   {6, 1, 10},
   {9, 0, 11},
   {9, 11, 2},
   {9, 2, 5},
   {7, 2, 11},
};

static void icosahedron(GLenum shadeType)
{
   int i;

   for (i = 19; i >= 0; i--)
   {
      drawtriangle(i, idata, connectivity, shadeType);
   }
}

void shapesWireIcosahedron(void)
{
   icosahedron(GL_LINE_LOOP);
}

void shapesSolidIcosahedron(void)
{
   icosahedron(GL_TRIANGLES);
}


/* tetrahedron data: */

#define T   1.73205080756887729

static GLfloat tdata[4][3] =
{
   {T, T, T},
   {T, -T, -T},
   {-T, T, -T},
   {-T, -T, T}
};
#undef T

static int tndex[4][3] =
{
   {0, 1, 3},
   {2, 1, 0},
   {3, 2, 0},
   {1, 2, 3}
};

static void tetrahedron(GLenum shadeType)
{
   int i;

   for (i = 3; i >= 0; i--)
      drawtriangle(i, tdata, tndex, shadeType);
}

void shapesWireTetrahedron(void)
{
   tetrahedron(GL_LINE_LOOP);
}

void shapesSolidTetrahedron(void)
{
   tetrahedron(GL_TRIANGLES);
}


/* Rim, body, lid, and bottom data must be reflected in x and
   y; handle and spout data across the y axis only.  */

static int patchdata[][16] =
{
   /* rim */
   {102, 103, 104, 105, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   /* body */
   {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27},
   {24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40},
   /* lid */
   {96, 96, 96, 96, 97, 98, 99, 100, 101, 101, 101, 101, 0, 1, 2, 3,},
   {0, 1, 2, 3, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117},
   /* bottom */
   {118, 118, 118, 118, 124, 122, 119, 121, 123, 126, 125, 120, 40, 39, 38, 37},
   /* handle */
   {41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56},
   {53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 28, 65, 66, 67},
   /* spout */
   {68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83},
   {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95}
};

static float cpdata[][3] =
{
   {0.2,0,2.7}, {0.2,-0.112,2.7}, {0.112,-0.2,2.7}, {0,-0.2,2.7},
   {1.3375,0,2.53125}, {1.3375,-0.749,2.53125}, {0.749,-1.3375,2.53125},
   {0,-1.3375,2.53125}, {1.4375,0,2.53125}, {1.4375,-0.805, 2.53125},
   {0.805,-1.4375,2.53125}, {0,-1.4375,2.53125}, {1.5,0,2.4}, {1.5,-0.84,2.4},
   {0.84,-1.5,2.4}, {0,-1.5,2.4}, {1.75,0, 1.875}, {1.75,-0.98,1.875},
   {0.98,-1.75,1.875}, {0,-1.75,1.875}, {2,0,1.35}, {2,-1.12,1.35},
   {1.12,-2,1.35}, {0,-2,1.35}, {2,0,0.9}, {2,-1.12,0.9}, {1.12,-2,0.9},
   {0,-2,0.9}, {-2,0,0.9}, {2, 0,0.45}, {2,-1.12,0.45}, {1.12,-2,0.45},
   {0,-2,0.45}, {1.5,0,0.225}, {1.5,-0.84,0.225}, {0.84,-1.5,0.225},
   {0,-1.5,0.225}, {1.5, 0,0.15}, {1.5,-0.84,0.15}, {0.84,-1.5,0.15},
   {0,-1.5,0.15}, {-1.6,0,2.025}, {-1.6,-0.3,2.025}, {-1.5,-0.3,2.25},
   {-1.5,0,2.25}, {-2.3,0,2.025}, {-2.3,-0.3,2.025}, {-2.5,-0.3,2.25},
   {-2.5,0,2.25}, {-2.7,0,2.025}, {-2.7,-0.3,2.025}, {-3,-0.3,2.25},
   {-3,0,2.25}, {-2.7,0,1.8}, {-2.7,-0.3,1.8}, {-3,-0.3,1.8}, {-3,0,1.8},
   {-2.7,0,1.575}, {-2.7,-0.3,1.575}, {-3,-0.3,1.35}, {-3,0,1.35},
   {-2.5,0,1.125}, {-2.5,-0.3,1.125}, {-2.65,-0.3,0.9375}, {-2.65,0, 0.9375},
   {-2,-0.3,0.9}, {-1.9,-0.3,0.6}, {-1.9,0,0.6}, {1.7,0, 1.425},
   {1.7,-0.66,1.425}, {1.7,-0.66,0.6}, {1.7,0,0.6}, {2.6,0, 1.425},
   {2.6,-0.66,1.425}, {3.1,-0.66,0.825}, {3.1,0,0.825}, {2.3, 0,2.1},
   {2.3,-0.25,2.1}, {2.4,-0.25,2.025}, {2.4,0,2.025}, {2.7, 0,2.4},
   {2.7,-0.25,2.4}, {3.3,-0.25,2.4}, {3.3,0,2.4}, {2.8,0, 2.475},
   {2.8,-0.25,2.475}, {3.525,-0.25,2.49375}, {3.525,0, 2.49375}, {2.9,0,2.475},
   {2.9,-0.15,2.475}, {3.45,-0.15,2.5125}, {3.45,0,2.5125}, {2.8,0,2.4},
   {2.8,-0.15,2.4}, {3.2,-0.15,2.4}, {3.2,0,2.4}, {0,0,3.15}, {0.8,0,3.15},
   {0.8,-0.45,3.15}, {0.45, -0.8,3.15}, {0,-0.8,3.15}, {0,0,2.85}, {1.4,0,2.4},
   {1.4,-0.784, 2.4}, {0.784,-1.4,2.4}, {0,-1.4,2.4}, {0.4,0,2.55},
   {0.4,-0.224, 2.55}, {0.224,-0.4,2.55}, {0,-0.4,2.55}, {1.3,0,2.55},
   {1.3,-0.728,2.55}, {0.728,-1.3,2.55}, {0,-1.3,2.55}, {1.3,0,2.4},
   {1.3,-0.728,2.4}, {0.728,-1.3,2.4}, {0,-1.3,2.4}, {0,0,0}, {1.425,-0.798,0},
   {1.5,0,0.075}, {1.425,0,0}, {0.798,-1.425,0}, {0,-1.5, 0.075}, {0,-1.425,0},
   {1.5,-0.84,0.075}, {0.84,-1.5,0.075}
};

static float tex[2][2][2] =
{
   { {0, 0},
     {1, 0}},
   { {0, 1},
     {1, 1}}
};


static void teapot(GLint grid, GLenum type)
{
   float p[4][4][3], q[4][4][3], r[4][4][3], s[4][4][3];
   int i, j, k, l;

   glPushAttrib(GL_ENABLE_BIT | GL_EVAL_BIT);
   glEnable(GL_AUTO_NORMAL);
   glEnable(GL_MAP2_VERTEX_3);
   glEnable(GL_MAP2_TEXTURE_COORD_2);
   for (i = 0; i < 10; i++)
   {
      for (j = 0; j < 4; j++)
      {
         for (k = 0; k < 4; k++)
         {
            for (l = 0; l < 3; l++)
            {
               p[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
               q[j][k][l] = cpdata[patchdata[i][j * 4 + (3 - k)]][l];
               if (l == 1)
                  q[j][k][l] *= -1.0;
               if (i < 6) {
                  r[j][k][l] =
                  cpdata[patchdata[i][j * 4 + (3 - k)]][l];
                  if (l == 0)
                     r[j][k][l] *= -1.0;
                  s[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
                  if (l == 0)
                     s[j][k][l] *= -1.0;
                  if (l == 1)
                     s[j][k][l] *= -1.0;
               }
            }
         }
      }
      glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &tex[0][0][0]);
      glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &p[0][0][0]);
      glMapGrid2f(grid, 0.0, 1.0, grid, 0.0, 1.0);
      glEvalMesh2(type, 0, grid, 0, grid);
      glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &q[0][0][0]);
      glEvalMesh2(type, 0, grid, 0, grid);
      if (i < 6)
      {
         glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &r[0][0][0]);
         glEvalMesh2(type, 0, grid, 0, grid);
         glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &s[0][0][0]);
         glEvalMesh2(type, 0, grid, 0, grid);
      }
   }
   glPopAttrib();
}

void shapesSolidTeapot()
{
   teapot(14, GL_FILL);
}

void shapesWireTeapot()
{
   teapot(10, GL_LINE);
}
#endif

#endif
