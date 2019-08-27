// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
// MRTs.c
//
//    This is an example to demonstrate Multiple Render Targets and framebuffer blits.
//    First, we will render a quad that outputs four colors (red, green, blue, gray)
//    per fragment using MRTs.
//    Then, we will copy the four color buffers into four screen quadrants
//    using framebuffer blits.
//
#include <stdlib.h>
#include <stdio.h>
#include "esUtil.h"

typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // Handle to a framebuffer object
   GLuint fbo;

   // Texture handle
   GLuint colorTexId[1];

   // Texture size
   GLsizei textureWidth;
   GLsizei textureHeight;

} UserData;

///
// Initialize the framebuffer object and MRTs
//
int InitFBO ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   int i;
   GLint defaultFramebuffer = 0;
   const GLenum attachments[1] = 
   { 
      GL_COLOR_ATTACHMENT0
      // GL_COLOR_ATTACHMENT1
      // GL_COLOR_ATTACHMENT2,
      // GL_COLOR_ATTACHMENT3 
   };

   glGetIntegerv ( GL_FRAMEBUFFER_BINDING, &defaultFramebuffer );

   // Setup fbo
   glGenFramebuffers ( 1, &userData->fbo );
   glBindFramebuffer ( GL_FRAMEBUFFER, userData->fbo );

   // Setup four output buffers and attach to fbo
   userData->textureHeight = userData->textureWidth = 400;
   glGenTextures ( 1, &userData->colorTexId[0] );
   for (i = 0; i < 1; ++i)
   {
      glBindTexture ( GL_TEXTURE_2D, userData->colorTexId[i] );

      glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, 
                     userData->textureWidth, userData->textureHeight, 
                     0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );

      // Set the filtering mode
      glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
      glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

      glFramebufferTexture2D ( GL_DRAW_FRAMEBUFFER, attachments[i], 
                               GL_TEXTURE_2D, userData->colorTexId[i], 0 );
   }

   glDrawBuffers ( 1, attachments );

   if ( GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus ( GL_FRAMEBUFFER ) )
   {
      return FALSE;
   }

   // Restore the original framebuffer
   glBindFramebuffer ( GL_FRAMEBUFFER, defaultFramebuffer );

   return TRUE;
}

///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   char vShaderStr[] =
      "#version 300 es                            \n"
      "layout(location = 0) in vec4 a_position;   \n"
      "layout(location = 1) in vec4 a_color;   \n"
      "out vec4 coords;   \n"
      "void main()                                \n"
      "{                                          \n"
      "   gl_Position = a_position;               \n"
      "   coords = a_color;               \n"
      "}                                          \n";

   char fShaderStr[] =
       "#version 300 es                                     \n"
       "precision mediump float;                            \n"

       "in vec4 coords;                                  \n"
       "out vec4 fragData;                                  \n"
       "void main()                                         \n"
       "{                                                   \n"
       "  fragData = coords;                  \n"
       "                                                    \n"
       "}                                                   \n";

   // Load the shaders and get a linked program object
   userData->programObject = esLoadProgram ( vShaderStr, fShaderStr );

   InitFBO ( esContext );

   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
   return TRUE;
}

///
// Draw a quad and output four colors per pixel
//
void DrawGeometry ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   GLfloat vVertices[] = { -0.50f,  0.50f, 0.0f,
                           -0.50f, -0.50f, 0.0f,
                            0.50f, -0.50f, 0.0f,
                            0.50f,  0.50f, 0.0f,
                         };

//   GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

    GLfloat pixels[esContext->width*esContext->height*3];
    GLfloat colors[esContext->width*esContext->height*3];
    for (uint32_t i = 0; i < esContext->height ; ++i) {
        for (uint32_t j = 0; j < esContext->width ; ++j) {
//            vec4 v;
//            v.x = 2.0f * (float(j + 0.5f) / float(width)) - 1.0f;
//            v.y = 2.0f * (float(i + 0.5f) / float(height)) - 1.0f;
//            v.z = 0;
//            v.w = 0;
//            pixels.push_back(v);
//
//            v.x = (float)j / width * 255;
//            v.y = (float)i / height * 255;
//            v.z = 0;
//            v.w = 0;
//            colors.push_back(v);
            pixels[3*(i * esContext->width + j) + 0] = 2.0f * (float)(j + 0.5f) / (esContext->width) - 1.0f;
            pixels[3*(i * esContext->width + j) + 1] = 2.0f * (float)(i + 0.5f) / (esContext->height) - 1.0f;
            pixels[3*(i * esContext->width + j) + 2] = 0;

            colors[3*(i * esContext->width + j) + 0] =  (float)(j ) / (esContext->width);
            colors[3*(i * esContext->width + j) + 1] =  (float)(i ) / (esContext->height);
            colors[3*(i * esContext->width + j) + 2] = 0;
        }
    }



   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex position
   glVertexAttribPointer ( 0, 3, GL_FLOAT,
                           GL_FALSE, 3 * sizeof ( GLfloat ), pixels );
   glVertexAttribPointer ( 1, 3, GL_FLOAT,
                            GL_FALSE, 3 * sizeof ( GLfloat ), colors );

   glEnableVertexAttribArray ( 0 );
   glEnableVertexAttribArray ( 1 );

   // Draw a quad
   //glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
    glDrawArrays(GL_POINTS, 0, esContext->width*esContext->height); //
}


void writeImage(int w, int h, GLubyte* ptr){
    FILE *f;
    unsigned char *img = NULL;

    int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int

    img = (unsigned char *)malloc(3*w*h);
    memset(img,0,3*w*h);

    for(int i=0; i<w; i++)
    {
        for(int j=0; j<h; j++)
        {
            int x=i; int y= j;

            int r = ptr[(x+y*w)*4+0];
            int g = ptr[(x+y*w)*4+1];
            int b = ptr[(x+y*w)*4+2];
//
            // bmp is bgr
            img[(x+y*w)*3+2] = (unsigned char)(r);
            img[(x+y*w)*3+1] = (unsigned char)(g);
            img[(x+y*w)*3+0] = (unsigned char)(b);
        }
    }

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[3] = {0,0,0};

    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(       w    );
    bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       w>>16);
    bmpinfoheader[ 7] = (unsigned char)(       w>>24);
    bmpinfoheader[ 8] = (unsigned char)(       h    );
    bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
    bmpinfoheader[10] = (unsigned char)(       h>>16);
    bmpinfoheader[11] = (unsigned char)(       h>>24);

    f = fopen("/home/pang/img.bmp","wb");
    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);
    for(int i=0; i<h; i++)
    {
        fwrite(img+(w*(h-i-1)*3),3,w,f);
        fwrite(bmppad,1,(4-(w*3)%4)%4,f);
    }

    free(img);
    fclose(f);
}


void saveFrameBuffer(ESContext *esContext) {
    UserData *userData = esContext->userData;

    // set the fbo for reading
    glBindFramebuffer ( GL_READ_FRAMEBUFFER, userData->fbo );

    GLubyte* pixels = (GLubyte*) malloc(userData->textureWidth * userData->textureHeight * sizeof(GLubyte) * 4);
    glReadPixels(0, 0, userData->textureWidth, userData->textureHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    writeImage(userData->textureWidth, userData->textureHeight, pixels);
    free(pixels);
}
///
// Copy MRT output buffers to screen
//
void BlitTextures ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   // set the fbo for reading
   glBindFramebuffer ( GL_READ_FRAMEBUFFER, userData->fbo );

   GLubyte* pixels = (GLubyte*) malloc(userData->textureWidth * userData->textureHeight * sizeof(GLubyte) * 4);
   glReadPixels(0, 0, userData->textureWidth, userData->textureHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

   writeImage(userData->textureWidth, userData->textureHeight, pixels);
   free(pixels);


//   // Copy the output red buffer to lower left quadrant
//   glReadBuffer ( GL_COLOR_ATTACHMENT0 );
//   glBlitFramebuffer ( 0, 0, userData->textureWidth, userData->textureHeight,
//                       0, 0, esContext->width/1, esContext->height/1,
//                       GL_COLOR_BUFFER_BIT, GL_LINEAR );

   // // Copy the output green buffer to lower right quadrant
   // glReadBuffer ( GL_COLOR_ATTACHMENT1 );
   // glBlitFramebuffer ( 0, 0, userData->textureWidth, userData->textureHeight,
   //                     esContext->width/2, 0, esContext->width, esContext->height/2, 
   //                     GL_COLOR_BUFFER_BIT, GL_LINEAR );

   // // Copy the output blue buffer to upper left quadrant
   // glReadBuffer ( GL_COLOR_ATTACHMENT2 );
   // glBlitFramebuffer ( 0, 0, userData->textureWidth, userData->textureHeight,
   //                     0, esContext->height/2, esContext->width/2, esContext->height, 
   //                     GL_COLOR_BUFFER_BIT, GL_LINEAR );

   // // Copy the output gray buffer to upper right quadrant
   // glReadBuffer ( GL_COLOR_ATTACHMENT3 );
   // glBlitFramebuffer ( 0, 0, userData->textureWidth, userData->textureHeight,
   //                     esContext->width/2, esContext->height/2, esContext->width, esContext->height, 
   //                     GL_COLOR_BUFFER_BIT, GL_LINEAR );
}

///
// Render to MRTs and screen
//
void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   GLint defaultFramebuffer = 0;
   const GLenum attachments[4] = 
   { 
      GL_COLOR_ATTACHMENT0,
      GL_COLOR_ATTACHMENT1,
      GL_COLOR_ATTACHMENT2,
      GL_COLOR_ATTACHMENT3 
   };
   
   glGetIntegerv ( GL_FRAMEBUFFER_BINDING, &defaultFramebuffer );

   // FIRST: use MRTs to output four colors to four buffers
   glBindFramebuffer ( GL_FRAMEBUFFER, userData->fbo );
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glDrawBuffers ( 4, attachments );
   DrawGeometry ( esContext );

   // save to image
   saveFrameBuffer( esContext );

//   // SECOND: copy the four output buffers into four window quadrants
//   // with framebuffer blits
//
//   // Restore the default framebuffer
//   glBindFramebuffer ( GL_DRAW_FRAMEBUFFER, defaultFramebuffer );
//   BlitTextures ( esContext );
}


///
// Cleanup
//
void ShutDown ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   // Delete texture objects
   glDeleteTextures ( 1, userData->colorTexId );

   // Delete fbo
   glDeleteFramebuffers ( 1, &userData->fbo );

   // Delete program object
   glDeleteProgram ( userData->programObject );
}

int esMain ( ESContext *esContext )
{
   esContext->userData = malloc ( sizeof ( UserData ) );

   esCreateWindow ( esContext, "Multiple Render Targets", 400, 400, ES_WINDOW_RGB | ES_WINDOW_ALPHA );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterDrawFunc ( esContext, Draw );
   esRegisterShutdownFunc ( esContext, ShutDown );

   return GL_TRUE;
}
