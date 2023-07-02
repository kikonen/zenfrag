#include "Frustrum.h"
#include "includes.h"
#include "Camera.h"

Frustrum::Frustrum(Camera* camera)
{
	this->camera = camera;
	viewingPlanNames[BOTTOM] = "BOTTOM";
	viewingPlanNames[TOP] = "TOP";
	viewingPlanNames[LEFT] = "LEFT";
	viewingPlanNames[RIGHT] = "RIGHT";
	viewingPlanNames[NEAR] = "NEAR";
	viewingPlanNames[FAR] = "FAR";
}

Frustrum::~Frustrum(void)
{
}

void Frustrum::update(void)
{


	float tan_tmp = tan(camera->fovy * ANG2RAD / 2);

	float hNear = tan_tmp * camera->zNear;
	float wNear = hNear * camera->aspect;

	float hFar =  tan_tmp * camera->zFar;
	float wFar = hFar * camera->aspect;


	Vertex directionZ = camera->lookAt - camera->position;
	directionZ.normalize();

	Vertex directionX =  directionZ.crossProduct(camera->upVector);
	directionX.normalize();

	Vertex directionY = directionX.crossProduct(directionZ);
	directionY.normalize();


	

	Vertex nc = camera->position + (directionZ * camera->zNear);
	Vertex fc = camera->position + (directionZ * camera->zFar);

	
	// Update every 8 viewing frustrum
	viewingVolume[NTR] = nc + (directionX * wNear) + (directionY * hNear);
	viewingVolume[NBR] = nc + (directionX * wNear) - (directionY * hNear);
	viewingVolume[NTL] = nc - (directionX * wNear) + (directionY * hNear);
	viewingVolume[NBL] = nc - (directionX * wNear) - (directionY * hNear);
	viewingVolume[FTR] = fc + (directionX * wFar) + (directionY * hFar);
	viewingVolume[FBR] = fc + (directionX * wFar) - (directionY * hFar);
	viewingVolume[FTL] = fc - (directionX * wFar) + (directionY * hFar);
	viewingVolume[FBL] = fc - (directionX * wFar) - (directionY * hFar);


	// Extract plans
	Vertex leftToRight		=   viewingVolume[NBR] - viewingVolume[NBL] ;
	Vertex bottomToTop = viewingVolume[NTR] - viewingVolume[NBR] ;
	viewingPlans[NEAR] = Plan(bottomToTop,leftToRight,viewingVolume[NBR]);
	viewingPlans[FAR] = Plan(leftToRight,bottomToTop,viewingVolume[FBR]);
	Vertex leftNearToFar	=   viewingVolume[FBL] - viewingVolume[NBL] ;
	viewingPlans[BOTTOM] = Plan(leftToRight,leftNearToFar,viewingVolume[NBL]);
	viewingPlans[LEFT] = Plan(leftNearToFar,bottomToTop,viewingVolume[NBL]);
	Vertex rightNearToFar	=   viewingVolume[FBR] - viewingVolume[NBR] ;
	viewingPlans[RIGHT] = Plan(bottomToTop,rightNearToFar,viewingVolume[NBR]);
	Vertex upperLeftNearToFar	=   viewingVolume[FTL] - viewingVolume[NTL] ;
	viewingPlans[TOP] = Plan(upperLeftNearToFar,leftToRight,viewingVolume[NTL]);

	
}

bool Frustrum::isPointInFront(Vertex& point,ViewingPlan planIndice)
{
	Plan* plan = &viewingPlans[planIndice];
	float distanceFromPlan = plan->distanceFromPoint(point);
	return distanceFromPlan >= 0;
}

// Return OUT, IN or INTERSECT
CollisionDetection Frustrum::checkAABBCollision(AABB& box)
{
	CollisionDetection result ;
	bool intersecting = false;
	for (int i=0; i<6 ;i++)
	{
		result = planeAABBCollision(box,i);
		if (result == COLL_OUT) return COLL_OUT;
		if (result == COLL_INTERSECT) 
			intersecting = true;
	}
	if (intersecting)
		return COLL_INTERSECT;
	return COLL_IN;
}

CollisionDetection Frustrum::planeAABBCollision(AABB& box, int  planIndice)
{
	float distance = 0;
	bool foundNegative = false;
	bool foundPositive = false;
	for (int i=0; i < 8 ; i++)
	{
		distance =  viewingPlans[planIndice].distanceFromPoint(box.boundaries[i]);
		if (distance < 0)
			foundNegative =true;
		else 
			foundPositive = true;
	}

	if (foundNegative && foundPositive)
		return COLL_INTERSECT;

	if (foundPositive)
		return COLL_IN;
	else
		return COLL_OUT;
}
