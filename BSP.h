#pragma once

#include "3DDefinitions.h"
#include "Group.h"

class BSP
{
public:
	BSP(void);


	BSP* front;
	BSP* back;
	Plan plan;
	AABB boudaryVolume;
	vector<Group*> groups;


	~BSP(void);
};
