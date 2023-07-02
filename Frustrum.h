#pragma once


#include "3DDefinitions.h"

class Camera;

class Frustrum
{
public:
	Frustrum(Camera* camera);
	~Frustrum(void);

	void update(void);

	CollisionDetection checkAABBCollision(AABB& box);

	Camera* camera;
	Vertex viewingVolume[8];
	Plan viewingPlans[6];
	string viewingPlanNames[6];
private:
	
	bool isPointInFront(Vertex& point,ViewingPlan planIndice);
	CollisionDetection planeAABBCollision(AABB& box, int  viewingPlan);
};
