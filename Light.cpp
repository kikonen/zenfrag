#include "Light.h"
#include "includes.h"
#include "Engine.h"
#include "Config.h"
#include "glex_impl.h"

Light::Light(int renderWith, int renderHeight,Vertex& positionIn, Vertex& lookAtIn, float zNearIn, float aFarin)
{
	position.x = positionIn.x;
	position.y = positionIn.y;
	position.z = positionIn.z;

	lookAt.x=lookAtIn.x;
	lookAt.y=lookAtIn.y;
	lookAt.z=lookAtIn.z;

	upVector = Vertex(0,1,0);

	fovy = 150;
	aspect = (float)renderWith/renderHeight;
	zNear = zNearIn;
	zFar = aFarin;

	upVector.x=0;
	upVector.y=1;
	upVector.z=0;



	dList = glGenLists(1);
	glNewList(dList,GL_COMPILE);
		glColor4f(1,1,1,1);
		//glutSolidSphere(5,10,10);
	glEndList();

	//type =  SPOT_LIGHT;
	type = POINT_LIGHT;
}

Light::~Light(void)
{


}


void Light::apply(void)
{
	gluLookAt(position.x,position.y,position.z,lookAt.x,lookAt.y,lookAt.z,upVector.x,upVector.y,upVector.z);
}


void Light::applyBackView(void)
{
	Vertex reverseLookAt = position + (position - lookAt);
	gluLookAt(position.x,position.y,position.z,reverseLookAt.x,reverseLookAt.y,reverseLookAt.z,upVector.x,upVector.y,upVector.z);
}
void Light::render()
{
	glColor4fv(diffuse);
	glCallList(dList);

}

void Light::setupPerspective()
{
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(fovy,aspect,zNear,zFar);
}


void Light::loadRessources()
{
	if (this->type == SPOT_LIGHT)
		generateFBO(frontShadowFBOId,frontShadowTextureId);

	if (this->type == POINT_LIGHT)
	{
		generateFBO(frontShadowFBOId,frontShadowTextureId);
		generateFBO(backShadowFBOId,backShadowTextureId);
	}
}


void Light::generateFBO(GLuint &fboId, GLuint &shadowMapId)
{
	//return ;
	int shadowMapWidth = Engine::engine->renderWidth/Engine::engine->fboShadowBuffersRatio;
	int shadowMapHeight = Engine::engine->renderHeight/Engine::engine->fboShadowBuffersRatio;


	GLuint fboTextureId;
	glGenTextures(1, &fboTextureId);
	glBindTexture(GL_TEXTURE_2D, fboTextureId);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, shadowMapWidth, shadowMapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);


	// Try to use a texture depth component
	glGenTextures(1, &shadowMapId);
	glBindTexture(GL_TEXTURE_2D, shadowMapId);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	// Normally filtering on depth texture is done bia GL_NEAREST, but Nvidia has a built-in support for Hardware filtering: use GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



	// Two next lines are necessary if we wan to use the convenient shadow2DProj function in the shader.
	// Otherwise we have to rely on texture2DProj
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);


	// create a framebuffer object
	glGenFramebuffersEXT(1, &fboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D, fboTextureId, 0);
	// attach the renderbuffer to depth attachment point
	//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT, shadowMapId);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, shadowMapId, 0);

	// check FBO status
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");
	}
	else
	{
		printf("Light ShadowMap texId =%d\n",shadowMapId);
		printf("Light ShadowMap fBOid =%d\n",fboId);
		printf("ShadowMap Width =%d\n",shadowMapWidth);
		printf("ShadowMap Height =%d\n",shadowMapHeight);
	}
	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}