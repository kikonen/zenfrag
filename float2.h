#pragma once

struct float2
{
	float u;
	float v;

	float2 operator- (const float2 & other ) const
	{
		float2 out;

		out.u = this->u - other.u;
		out.v = this->v - other.v;

		return out;
	}

	float2 operator/ (const float & other ) const
	{
		float2 out;

		out.u = this->u / other;
		out.v = this->v / other;

		return out;
	}
};