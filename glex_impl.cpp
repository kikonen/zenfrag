#include "glinfo.h"
#include "Engine.h"
#ifdef _WIN32
#include "glex_impl.h"
#else
#include<OpenGL/glext.h>
#endif


#ifdef _WIN32

	// VBO
	PFNGLGENBUFFERSARBPROC pglGenBuffersARB =0;                     // VBO Name Generation Procedure
	PFNGLBINDBUFFERARBPROC pglBindBufferARB =0;                     // VBO Bind Procedure
	PFNGLBUFFERDATAARBPROC pglBufferDataARB =0;                     // VBO Data Loading Procedure
	PFNGLBUFFERSUBDATAARBPROC pglBufferSubDataARB =0;               // VBO Sub Data Loading Procedure
	PFNGLDELETEBUFFERSARBPROC pglDeleteBuffersARB =0;               // VBO Deletion Procedure
	PFNGLGETBUFFERPARAMETERIVARBPROC pglGetBufferParameterivARB =0; // return various parameters of VBO
	PFNGLMAPBUFFERARBPROC pglMapBufferARB =0;                       // map VBO procedure
	PFNGLUNMAPBUFFERARBPROC pglUnmapBufferARB =0;                   // unmap VBO procedure

	//Shaders
	 PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB ;
	 PFNGLDELETEOBJECTARBPROC         glDeleteObjectARB       ;
	 PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB   ;
	 PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB ;
	 PFNGLSHADERSOURCEARBPROC         glShaderSourceARB        ;
	 PFNGLCOMPILESHADERARBPROC        glCompileShaderARB       ;
	 PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
	 PFNGLATTACHOBJECTARBPROC         glAttachObjectARB        ;
	 PFNGLGETINFOLOGARBPROC           glGetInfoLogARB          ;
	 PFNGLLINKPROGRAMARBPROC          glLinkProgramARB         ;
	 PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB  ;
	 PFNGLUNIFORM4FARBPROC            glUniform4fARB           ;
	 PFNGLUNIFORM3FARBPROC            glUniform3fARB           ;
	 PFNGLUNIFORM2FARBPROC			  glUniform2fARB           ;
	 PFNGLUNIFORM1FARBPROC            glUniform1fARB           ;
	 PFNGLUNIFORM1IARBPROC            glUniform1iARB           ;
	 PFNGLGETATTRIBLOCATIONARBPROC  glGetAttribLocationARB;
	 PFNGLVERTEXATTRIB3FARBPROC     glVertexAttrib3fARB;
	 PFNGLVERTEXATTRIBPOINTERARBPROC      glVertexAttribPointerARB;
	 PFNGLENABLEVERTEXATTRIBARRAYARBPROC  glEnableVertexAttribArrayARB ;
	 PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB;
	 PFNGLACTIVETEXTUREARBPROC		      pglActiveTextureARB;

	 //FBO
	PFNGLGENRENDERBUFFERSEXTPROC pglGenRenderbuffersEXT ;
	PFNGLBINDRENDERBUFFEREXTPROC pglBindRenderbufferEXT ;
	PFNGLRENDERBUFFERSTORAGEEXTPROC pglRenderbufferStorageEXT ;
	PFNGLGENFRAMEBUFFERSEXTPROC pglGenFramebuffersEXT ;
	PFNGLBINDFRAMEBUFFEREXTPROC pglBindFramebufferEXT ;
	PFNGLFRAMEBUFFERTEXTURE2DEXTPROC pglFramebufferTexture2DEXT ;
	PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC pglFramebufferRenderbufferEXT ;
	PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC pglCheckFramebufferStatusEXT ;

#endif



void initVBO()
{

	glInfo* glInfoObject = Engine::engine->glInfoObject;


	#ifdef _WIN32

    // check VBO is supported by your video card
    if(glInfoObject->isExtensionSupported("GL_ARB_vertex_buffer_object"))
    {
        // get pointers to GL functions
        glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
        glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
        glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
        glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
        glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
        glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
        glMapBufferARB = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
        glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");

		glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");

        // check once again VBO extension
        if(glGenBuffersARB && glBindBufferARB && glBufferDataARB && glBufferSubDataARB &&
           glMapBufferARB && glUnmapBufferARB && glDeleteBuffersARB && glGetBufferParameterivARB)
            printf("Video card supports GL_ARB_vertex_buffer_object.\n" );
        else
            printf("Video card does NOT support GL_ARB_vertex_buffer_object.\n" );

    }
    else
        printf("Video card does NOT support GL_ARB_vertex_buffer_object.\n");

	#else // for linux, do not need to get function pointers, it is up-to-date
		if(glInfoObject->isExtensionSupported("GL_ARB_vertex_buffer_object"))
			printf( "Video card supports GL_ARB_vertex_buffer_object.\n" );

		else
			printf( "Video card does NOT support GL_ARB_vertex_buffer_object.\n");

	#endif
}


void initShaders()
{

	glInfo* glInfoObject = Engine::engine->glInfoObject;

	#ifdef _WIN32




  //Create all the functions using wglGetProcAddress
  glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
  glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)wglGetProcAddress("glDeleteObjectARB");
  glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
  glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
  glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB");
  glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB");
  glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB");
  glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB");
  glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetInfoLogARB");
  glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");

  glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
  glUniform4fARB = (PFNGLUNIFORM4FARBPROC)wglGetProcAddress("glUniform4fARB");
  glUniform3fARB = (PFNGLUNIFORM3FARBPROC)wglGetProcAddress("glUniform3fARB");
  glUniform2fARB = (PFNGLUNIFORM2FARBPROC)wglGetProcAddress("glUniform2fARB");
  glUniform1fARB = (PFNGLUNIFORM1FARBPROC)wglGetProcAddress("glUniform1fARB");
  glUniform1iARB = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
  glGetAttribLocationARB = (PFNGLGETATTRIBLOCATIONARBPROC)wglGetProcAddress("glGetAttribLocationARB");
  glVertexAttrib3fARB = (PFNGLVERTEXATTRIB3FARBPROC)wglGetProcAddress("glVertexAttrib3fARB");

  glVertexAttribPointerARB=(PFNGLVERTEXATTRIBPOINTERARBPROC)wglGetProcAddress("glVertexAttribPointerARB");
  glEnableVertexAttribArrayARB=(PFNGLENABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress("glEnableVertexAttribArrayARB");
  glDisableVertexAttribArrayARB=( PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress("glDisableVertexAttribArrayARB");

	// check once again VBO extension
    if(glCreateProgramObjectARB &&
		glDeleteObjectARB &&
		glUseProgramObjectARB &&
		glCreateShaderObjectARB &&
       glShaderSourceARB &&
	   glCompileShaderARB &&
	   glGetObjectParameterivARB &&
	   glAttachObjectARB &&
	   	glGetInfoLogARB &&
       glLinkProgramARB &&
	   glGetUniformLocationARB &&
	   glUniform4fARB &&
	   glUniform3fARB &&
	   glUniform2fARB &&
	   glUniform1fARB &&
	glUniform1iARB &&
	glGetAttribLocationARB &&
		glVertexAttrib3fARB &&
		glVertexAttribPointerARB &&
		glEnableVertexAttribArrayARB &&
		glDisableVertexAttribArrayARB
	   )
	{
        printf("Video card supports GL_ARB_shading_language.\n" );
		Engine::engine->shaderSupported= true;
	}
    else
        printf("Video card does NOT support GL_ARB_shading_language.\n" );

#else
	if(glInfoObject->isExtensionSupported("GL_ARB_shading_language_100"))
	{
			printf( "Video card supports GL_ARB_shading_language_100.\n" );
			Engine::engine->shaderSupported= true;
	}
		else
			printf( "Video card does NOT support GL_ARB_shading_language_100.\n");

#endif
}

void initFBO()
{
	#ifdef _WIN32

		glGenRenderbuffersEXT		= (PFNGLGENRENDERBUFFERSEXTPROC)	wglGetProcAddress("glGenRenderbuffersEXT");
		glBindRenderbufferEXT		= (PFNGLBINDRENDERBUFFEREXTPROC)	wglGetProcAddress("glBindRenderbufferEXT");
		glRenderbufferStorageEXT	= (PFNGLRENDERBUFFERSTORAGEEXTPROC)	wglGetProcAddress("glRenderbufferStorageEXT");

		glGenFramebuffersEXT		= (PFNGLGENFRAMEBUFFERSEXTPROC)		wglGetProcAddress("glGenFramebuffersEXT");
		glBindFramebufferEXT		= (PFNGLBINDFRAMEBUFFEREXTPROC)		wglGetProcAddress("glBindFramebufferEXT");
		glFramebufferTexture2DEXT	= (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
		glFramebufferRenderbufferEXT= (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
		glCheckFramebufferStatusEXT	= (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");

	if (glGenRenderbuffersEXT &&
		glBindRenderbufferEXT &&
		glRenderbufferStorageEXT &&
		glGenFramebuffersEXT &&
		glBindFramebufferEXT &&
		glFramebufferTexture2DEXT &&
		glFramebufferRenderbufferEXT &&
		glCheckFramebufferStatusEXT)
	{
		printf("Video card supports GL_EXT_framebuffer_object.\n" );
		Engine::engine-> fboSupported = true;
	}
	else
	{
		Engine::engine-> fboSupported = false;
		printf("Video card DOES NOT supports FBO.\n" );
	}

	#else
	printf("Video card supports GL_EXT_framebuffer_object.\n" );
	Engine::engine-> fboSupported = true;

	#endif



}

void initExtensions()
{
	initVBO();
	initShaders();
	initFBO();

}
