#ifndef PROG_NUANCEUR_H
#define PROG_NUANCEUR_H

#include <string.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "matrices.h"

// Fonctions pour les nuanceurs

class ProgNuanceur
{
public:
   ProgNuanceur( ) : prog_(0) { }
   ~ProgNuanceur( ) { glDeleteProgram( prog_ ); }

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

   // assigner des attibuts
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
		  char buffer[255];
		  strerror_s(buffer, errno);
         std::cerr << "!!! " << fich << ": " << buffer << std::endl;
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
      strcpy_s( source, taille + 1,contenu.c_str() );
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

#endif
