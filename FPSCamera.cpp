#include "FPSCamera.h"
#include "main.h"
#include "Timer.h"
#include "Engine.h"

FPSCamera::FPSCamera(int renderW, int renderH, Vertex& positionIn, Vertex& lookAtIn, float nearZ, float farZ) : Camera(renderW,renderH,positionIn,lookAtIn,nearZ,farZ)
{
}

FPSCamera::~FPSCamera(void)
{
}



void FPSCamera::update(Uint8 * tKeys)
{
	if (tKeys[SDLK_UP] || tKeys[SDLK_DOWN])
	{
			float delta ;
			if (tKeys[SDLK_UP])
				delta = 0.1f * Timer::tick;
			else
				delta = - 0.1f * Timer::tick;

			float y = position.y;

			Vertex direction = lookAtVector;
			direction.rotateX(rotationX);
			direction.rotateY(rotationY);
			direction.normalize();
			
			position =  position + direction * delta ;
			position.y = y;
			lookAt = position + direction;

			

			
	}	


	if (tKeys[SDLK_LEFT] || tKeys[SDLK_RIGHT])
	{
		float delta ;
		if (tKeys[SDLK_LEFT])
			delta = -0.1f * Timer::tick;
		else
			delta =  0.1f * Timer::tick;

		float y = position.y;

		Vertex direction = lookAtVector;
		direction.rotateX(rotationX);
		direction.rotateY( rotationY);
		direction.normalize();
		Vertex savedDirection = direction;
		direction.rotateY(90);
		
		position =  position + direction * delta;
		position.y = y;
		lookAt = position + savedDirection;

		

	}

	Camera::update(tKeys);
	//printf("p: x=%.0f,y=%.0f,z=%.0f lookAt x=%.0f,y=%.0f,z=%.0f\n",position.x,position.y,position.z,lookAt.x,lookAt.y,lookAt.z);
}
void FPSCamera::updateMouse(int eventX,int eventY)
{
	

	int deltax = Engine::engine->renderWidth/2 - eventX;
	int deltay = Engine::engine->renderHeight/2 - eventY;

	if (deltax == 0 && deltay==0)
		return;

	rotationX -= deltay;
	rotationY -= deltax;

	if (rotationY > 360)		rotationY -= 360;
	if (rotationY < -360)		rotationY += 360;
	if (rotationX > 89)		rotationX = 89;
	if (rotationX < -89)		rotationX = -89;

	Vertex direction = lookAtVector;
	direction.rotateX(rotationX);
	direction.rotateY(rotationY);

	lookAt = position + direction;
	
	Camera::updateMouse(eventX,eventY);

	SDL_WarpMouse(Engine::engine->renderWidth/2 ,Engine::engine->renderHeight/2);

}

