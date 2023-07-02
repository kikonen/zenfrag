#include "Camera.h"
#include "includes.h"
#include "Timer.h"
#include "Frustrum.h"

Camera::Camera(int renderWith, int renderHeight,Vertex& positionIn, Vertex& lookAtIn, float zNearIn, float aFarin)
{
	lookAtVector = lookAtIn - positionIn;
	printf("lookAtVector: x=%.2f, y=%.2f,z=%.2f \n",lookAtVector.x,lookAtVector.y,lookAtVector.z);
	rotationX = 0;
	rotationY=0;

	position  = positionIn;
	lookAt =lookAtIn;

	upVector = Vertex(0,1,0);

	fovy = 55;
	aspect = (float)renderWith/renderHeight;
	zNear = zNearIn;
	zFar = aFarin;

	frustrum = new Frustrum(this);
	frustrum->update();
}
Camera::~Camera() {}

void Camera::updateFrustrum()
{
	frustrum->update();
}

void Camera::apply()
{
	gluLookAt(position.x,position.y,position.z,lookAt.x,lookAt.y,lookAt.z,upVector.x,upVector.y,upVector.z);

}


int leftButtonClicked = 0;


void Camera::render()
{
	glColor4f(0,0,1,0.7f);
	glDisable(GL_TEXTURE_2D);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	// Near
	glBegin(GL_QUADS);
		glVertex3f(frustrum->viewingVolume[NBR].x,
				   frustrum->viewingVolume[NBR].y,
				   frustrum->viewingVolume[NBR].z);

		glVertex3f(frustrum->viewingVolume[NTR].x,
				   frustrum->viewingVolume[NTR].y,
				   frustrum->viewingVolume[NTR].z);

		glVertex3f(frustrum->viewingVolume[NTL].x,
				   frustrum->viewingVolume[NTL].y,
				   frustrum->viewingVolume[NTL].z);

		glVertex3f(frustrum->viewingVolume[NBL].x,
				   frustrum->viewingVolume[NBL].y,
				   frustrum->viewingVolume[NBL].z);
	glEnd();

	//Far
	glBegin(GL_QUADS);
		glVertex3f(frustrum->viewingVolume[FBR].x,
				   frustrum->viewingVolume[FBR].y,
				   frustrum->viewingVolume[FBR].z);

		glVertex3f(frustrum->viewingVolume[FTR].x,
				   frustrum->viewingVolume[FTR].y,
				   frustrum->viewingVolume[FTR].z);

		glVertex3f(frustrum->viewingVolume[FTL].x,
				   frustrum->viewingVolume[FTL].y,
				   frustrum->viewingVolume[FTL].z);

		glVertex3f(frustrum->viewingVolume[FBL].x,
				   frustrum->viewingVolume[FBL].y,
				   frustrum->viewingVolume[FBL].z);
	glEnd();



	// Right

glBegin(GL_QUADS);
		glVertex3f(frustrum->viewingVolume[FBR].x,
				   frustrum->viewingVolume[FBR].y,
				   frustrum->viewingVolume[FBR].z);

		glVertex3f(frustrum->viewingVolume[FTR].x,
				   frustrum->viewingVolume[FTR].y,
				   frustrum->viewingVolume[FTR].z);

		glVertex3f(frustrum->viewingVolume[NTR].x,
				   frustrum->viewingVolume[NTR].y,
				   frustrum->viewingVolume[NTR].z);

		glVertex3f(frustrum->viewingVolume[NBR].x,
				   frustrum->viewingVolume[NBR].y,
				   frustrum->viewingVolume[NBR].z);

glEnd();

	//Left
glBegin(GL_QUADS);
		glVertex3f(frustrum->viewingVolume[FBL].x,
				   frustrum->viewingVolume[FBL].y,
				   frustrum->viewingVolume[FBL].z);

		glVertex3f(frustrum->viewingVolume[FTL].x,
				   frustrum->viewingVolume[FTL].y,
				   frustrum->viewingVolume[FTL].z);

		glVertex3f(frustrum->viewingVolume[NTL].x,
				   frustrum->viewingVolume[NTL].y,
				   frustrum->viewingVolume[NTL].z);

		glVertex3f(frustrum->viewingVolume[NBL].x,
				   frustrum->viewingVolume[NBL].y,
				   frustrum->viewingVolume[NBL].z);

glEnd();

// Top
glBegin(GL_QUADS);
		glVertex3f(frustrum->viewingVolume[FTL].x,
				   frustrum->viewingVolume[FTL].y,
				   frustrum->viewingVolume[FTL].z);

		glVertex3f(frustrum->viewingVolume[FTR].x,
				   frustrum->viewingVolume[FTR].y,
				   frustrum->viewingVolume[FTR].z);

		glVertex3f(frustrum->viewingVolume[NTR].x,
				   frustrum->viewingVolume[NTR].y,
				   frustrum->viewingVolume[NTR].z);

		glVertex3f(frustrum->viewingVolume[NTL].x,
				   frustrum->viewingVolume[NTL].y,
				   frustrum->viewingVolume[NTL].z);

glEnd();

// Bottom
glBegin(GL_QUADS);
		glVertex3f(frustrum->viewingVolume[FBL].x,
				   frustrum->viewingVolume[FBL].y,
				   frustrum->viewingVolume[FBL].z);

		glVertex3f(frustrum->viewingVolume[FBR].x,
				   frustrum->viewingVolume[FBR].y,
				   frustrum->viewingVolume[FBR].z);

		glVertex3f(frustrum->viewingVolume[NBR].x,
				   frustrum->viewingVolume[NBR].y,
				   frustrum->viewingVolume[NBR].z);

		glVertex3f(frustrum->viewingVolume[NBL].x,
				   frustrum->viewingVolume[NBL].y,
				   frustrum->viewingVolume[NBL].z);

glEnd();

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1,1,1);
}


void Camera::setupPerspective()
{
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(fovy,aspect,zNear,zFar);
}


void Camera::print()
{
	//printf("Camera position: x=%f,y=%f,z=%f : lookat: x=%f,y=%f,z=%f\n",position.x,position.y,position.z,lookAt.x,lookAt.y,lookAt.z);
}


void Camera::update(Uint8 * tKeys)
{
	frustrum->update();	
}
void Camera::updateMouse(int eventX,int eventY)
{
	frustrum->update();
}
