#pragma once

#include "Vertex.h"
#include "GLUT\glut.h"
#include "glext.h"

class Light
{
public:

	enum Type{ SPOT_LIGHT , POINT_LIGHT};

	Light(int renderWith, int renderHeight,Vertex& positionIn, Vertex& lookAtIn, float zNearIn, float aFarin);
	~Light(void);

	void loadRessources();
	void generateFBO(GLuint &fbo, GLuint &fboTextureId);
	

	int id;
	Type type;

	void apply(void);
	void applyBackView(void);
	void render(void);
	void update(void);

	void setupPerspective();

	// Frustrum definition
	float fovy;
	float aspect;
	float zNear;
	float zFar;

	Vertex position;
	Vertex lookAt;
	Vertex upVector;
	

	unsigned int dList;
	
	GLfloat diffuse[4] ;
	GLfloat ambient[4] ;
	GLfloat specular[4] ;
	GLfloat constantAttenuation ;
	GLfloat linearAttenuation ;
	GLfloat quadraticAttenuation ;


	GLuint frontShadowFBOId;
	GLuint frontShadowTextureId;

	GLuint backShadowFBOId;
	GLuint backShadowTextureId;
};
