#pragma once 

#include "SDL.h"
#include "Vertex.h"


class Frustrum;



class Camera
{
	public:

		Camera(int renderWith, int renderHeight, Vertex& positionIn, Vertex& lookAtIn, float nearZ, float farZ);
		~Camera();
		
		Vertex lookAtVector;
		float rotationX;
		float rotationY;

		void setupPerspective();

		void apply();
		virtual void update(Uint8 * tKeys) ;
		virtual void updateMouse(int deltax,int deltay) ;
		void render();
		void print();

		Vertex position;
		Vertex lookAt;

		Vertex upVector ;

		// Frustrum definition
		float fovy;
		float aspect;
		float zNear;
		float zFar;

		void updateFrustrum();
		Frustrum* frustrum;

} ;
