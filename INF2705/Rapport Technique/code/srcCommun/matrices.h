#ifndef MATRICES_PIPELINE_H
#define MATRICES_PIPELINE_H

//
// Classe pour les matrices du pipeline
//

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

#endif
