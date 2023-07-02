#pragma once

#include "float3.h"

class Entity
{
public:
	Entity(void);
	~Entity(void);

	float3 position;
	float orientation;
};
