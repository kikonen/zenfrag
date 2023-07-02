#pragma once
#include "Camera.h"

class FreeCamera : public Camera
{
	public:


		FreeCamera(int renderWith, int renderHeight, Vertex& positionIn, Vertex& lookAtIn, float nearZ, float farZ);

		~FreeCamera(void);

		virtual void update(Uint8 * tKeys);
		virtual void updateMouse(int deltax,int deltay);




};
