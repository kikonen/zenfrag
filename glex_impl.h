#pragma once

#define GL_GLEXT_PROTOTYPES

#include "includes.h"
#ifdef _WIN32
	#include "glext.h"
#else
	#include <OpenGL/glext.h>
#endif

#ifdef _WIN32

// Ugly shit: Thanks microsoft !!!


// VBOS extension
extern PFNGLGENBUFFERSARBPROC pglGenBuffersARB ;                     // VBO Name Generation Procedure
extern PFNGLBINDBUFFERARBPROC pglBindBufferARB ;                     // VBO Bind Procedure
extern PFNGLBUFFERDATAARBPROC pglBufferDataARB ;                     // VBO Data Loading Procedure
extern PFNGLBUFFERSUBDATAARBPROC pglBufferSubDataARB ;               // VBO Sub Data Loading Procedure
extern PFNGLDELETEBUFFERSARBPROC pglDeleteBuffersARB ;               // VBO Deletion Procedure
extern PFNGLGETBUFFERPARAMETERIVARBPROC pglGetBufferParameterivARB ; // return various parameters of VBO
extern PFNGLMAPBUFFERARBPROC pglMapBufferARB ;                       // map VBO procedure
extern PFNGLUNMAPBUFFERARBPROC pglUnmapBufferARB ;                   // unmap VBO procedure
#define glGenBuffersARB           pglGenBuffersARB
#define glBindBufferARB           pglBindBufferARB
#define glBufferDataARB           pglBufferDataARB
#define glBufferSubDataARB        pglBufferSubDataARB
#define glDeleteBuffersARB        pglDeleteBuffersARB
#define glGetBufferParameterivARB pglGetBufferParameterivARB
#define glMapBufferARB            pglMapBufferARB
#define glUnmapBufferARB          pglUnmapBufferARB

//Shaders extensions
extern 	 PFNGLCREATEPROGRAMOBJECTARBPROC  pglCreateProgramObjectARB ;
extern	 PFNGLDELETEOBJECTARBPROC         pglDeleteObjectARB       ;
extern 	 PFNGLUSEPROGRAMOBJECTARBPROC     pglUseProgramObjectARB   ;
extern 	 PFNGLCREATESHADEROBJECTARBPROC   pglCreateShaderObjectARB ;
extern 	 PFNGLSHADERSOURCEARBPROC         pglShaderSourceARB        ;
extern 	 PFNGLCOMPILESHADERARBPROC        pglCompileShaderARB       ;
extern 	 PFNGLGETOBJECTPARAMETERIVARBPROC pglGetObjectParameterivARB;
extern 	 PFNGLATTACHOBJECTARBPROC         pglAttachObjectARB        ;
extern 	 PFNGLGETINFOLOGARBPROC           pglGetInfoLogARB          ;
extern 	 PFNGLLINKPROGRAMARBPROC          pglLinkProgramARB         ;
extern 	 PFNGLGETUNIFORMLOCATIONARBPROC   pglGetUniformLocationARB  ;
extern 	 PFNGLUNIFORM4FARBPROC            pglUniform4fARB           ;
extern 	 PFNGLUNIFORM3FARBPROC            pglUniform3fARB           ;     
extern 	 PFNGLUNIFORM2FARBPROC            pglUniform2fARB           ;     
extern 	 PFNGLUNIFORM1FARBPROC            pglUniform1fARB           ;
extern 	 PFNGLUNIFORM1IARBPROC            pglUniform1iARB           ;
extern 	 PFNGLGETATTRIBLOCATIONARBPROC  pglGetAttribLocationARB;
extern 	 PFNGLVERTEXATTRIB3FARBPROC     pglVertexAttrib3fARB;
extern 	 PFNGLVERTEXATTRIBPOINTERARBPROC      pglVertexAttribPointerARB;
extern 	 PFNGLENABLEVERTEXATTRIBARRAYARBPROC  pglEnableVertexAttribArrayARB ;
extern 	 PFNGLDISABLEVERTEXATTRIBARRAYARBPROC pglDisableVertexAttribArrayARB;
extern	PFNGLACTIVETEXTUREARBPROC		      pglActiveTextureARB;
#define  glCreateProgramObjectARB	pglCreateProgramObjectARB
#define  glDeleteObjectARB			pglDeleteObjectARB
#define  glUseProgramObjectARB		pglUseProgramObjectARB
#define  glCreateShaderObjectARB	pglCreateShaderObjectARB
#define  glShaderSourceARB          pglShaderSourceARB
#define  glCompileShaderARB			pglCompileShaderARB
#define  glGetObjectParameterivARB	pglGetObjectParameterivARB				
#define  glAttachObjectARB			pglAttachObjectARB        
#define  glGetInfoLogARB			pglGetInfoLogARB          
#define  glLinkProgramARB			pglLinkProgramARB         
#define  glGetUniformLocationARB	pglGetUniformLocationARB  
#define  glUniform4fARB				pglUniform4fARB           
#define  glUniform3fARB				pglUniform3fARB      
#define  glUniform2fARB				pglUniform2fARB          
#define  glUniform1fARB				pglUniform1fARB          
#define  glUniform1iARB				pglUniform1iARB
#define  glGetAttribLocationARB		pglGetAttribLocationARB
#define  glVertexAttrib3fARB		pglVertexAttrib3fARB
#define  glVertexAttribPointerARB		pglVertexAttribPointerARB
#define  glEnableVertexAttribArrayARB	pglEnableVertexAttribArrayARB
#define  glDisableVertexAttribArrayARB	pglDisableVertexAttribArrayARB
#define  glActiveTextureARB			pglActiveTextureARB


//FBO and PBO extensions

extern PFNGLGENRENDERBUFFERSEXTPROC pglGenRenderbuffersEXT ;
extern PFNGLBINDRENDERBUFFEREXTPROC pglBindRenderbufferEXT ;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC pglRenderbufferStorageEXT ;
extern PFNGLGENFRAMEBUFFERSEXTPROC pglGenFramebuffersEXT ;
extern PFNGLBINDFRAMEBUFFEREXTPROC pglBindFramebufferEXT ;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC pglFramebufferTexture2DEXT ;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC pglFramebufferRenderbufferEXT ;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC pglCheckFramebufferStatusEXT ;
#define glGenRenderbuffersEXT pglGenRenderbuffersEXT 
#define glBindRenderbufferEXT pglBindRenderbufferEXT 
#define glRenderbufferStorageEXT pglRenderbufferStorageEXT 
#define glGenFramebuffersEXT pglGenFramebuffersEXT 
#define glBindFramebufferEXT pglBindFramebufferEXT 
#define glFramebufferTexture2DEXT pglFramebufferTexture2DEXT 
#define glFramebufferRenderbufferEXT pglFramebufferRenderbufferEXT
#define glCheckFramebufferStatusEXT pglCheckFramebufferStatusEXT 

#endif
void	initVBO();
void	initShaders();
void	initFBO();
void initExtensions();