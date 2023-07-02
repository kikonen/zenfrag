#pragma once
#include "Camera.h"


class FPSCamera : public Camera
{
public:
	FPSCamera(int renderWith, int renderHeight, Vertex& positionIn, Vertex& lookAtIn, float nearZ, float farZ);
	~FPSCamera(void);

	virtual void update(Uint8 * tKeys);
	virtual void updateMouse(int deltax,int deltay);
};
