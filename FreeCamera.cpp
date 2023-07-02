#include "FreeCamera.h"
#include "main.h"
#include "Timer.h"
#include "Engine.h"






FreeCamera::~FreeCamera(void)
{

}

FreeCamera::FreeCamera(int renderW, int renderH, Vertex& positionIn, Vertex& lookAtIn, float nearZ, float farZ) : Camera(renderW,renderH,positionIn,lookAtIn,nearZ,farZ)
{

}

void FreeCamera::update(Uint8 * tKeys)
{
	if (tKeys[SDLK_UP])
	{
		Vertex direction = lookAt - position;
		direction.normalize();
		position =  position + (direction * 0.1f * Timer::tick);
		lookAt = lookAt + (direction * 0.1f * Timer::tick);

		

		print();
	}	

	if (tKeys[SDLK_DOWN])
	{
		Vertex direction = lookAt - position;
		direction.normalize();
		position =  position - (direction * 0.1f * Timer::tick);
		lookAt = lookAt - (direction * 0.1f * Timer::tick);

		
		print();
	}	

	if (tKeys[SDLK_LEFT])
	{
		Vertex direction = lookAt - position;
		direction.normalize();
		direction.rotateY(90);
		position =  position - (direction * 0.1f * (Timer::tick));
		lookAt = lookAt - (direction *0.1f *  Timer::tick);

		
		print();
	}

	if (tKeys[SDLK_RIGHT])
	{
		Vertex direction = lookAt - position;
		direction.normalize();
		direction.rotateY(-90);
		position =  position - (direction * 0.1f * (Timer::tick));
		lookAt = lookAt - (direction * 0.1f * Timer::tick);

		
		print();
	}

	Camera::update(tKeys);
}
void FreeCamera::updateMouse(int eventX,int eventY)
{

//	printf("m.x=%d, m.y=%d\n",eventX,eventY);

	int deltax = Engine::engine->renderWidth/2 - eventX;
	int deltay = Engine::engine->renderHeight/2 - eventY;

	if (deltax == 0 && deltay==0)
		return;



	rotationX -= deltay;
	rotationY -= deltax;

	if (rotationY > 360)
		rotationY -= 360;

	if (rotationY < -360)
		rotationY += 360;

	if (rotationX > 89)
		rotationX = 89;

	if (rotationX < -89)
		rotationX = -89;

	Vertex direction = lookAtVector;
	direction.rotateX(rotationX);
	direction.rotateY(rotationY);

	lookAt = position + direction;
	
	



	Camera::updateMouse(eventX,eventY);

	SDL_WarpMouse(Engine::engine->renderWidth/2 ,Engine::engine->renderHeight/2);

	//oldMouseX = eventX;
	//oldMouseY = eventY;
}

