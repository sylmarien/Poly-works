#ifndef _CHARGERTEXTURE_H_
#define _CHARGERTEXTURE_H_

#include <string.h>
#include <GL/glew.h>
#include <stdio.h>
#include <iostream>
#include "chargertex.h"
#include "bitmap.h"
//#include <SDL_surface.h>

bool ChargerTexture( std::string fichier, GLuint &texture )
{
   // vérifier la présence du fichier BMP en essayant de l'ouvrir
	FILE *img;
	fopen_s(&img, fichier.c_str(), "r" );//handle error !
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

#endif
