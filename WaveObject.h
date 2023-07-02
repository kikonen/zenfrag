#pragma once

#include "3DDefinitions.h"
#include <vector>
#include <map>
#include "GLUT/glut.h"
#include "BSP.h"
using namespace std;

class Parser;

class WaveObject 
{
public:
	WaveObject(float scaleFactor, Vertex offSet);
	~WaveObject(void);

	char* name;
	static string pathContext;

	void load(char* filename);
	void render();

	vector<Vertex*> vertices;
	vector<TextCoord*> textCoords;
	vector<Vertex*> normals;
	vector<Face*> faces;
	map<string,Material*> materials;
	vector<Group*> groups;

	Group* currentGroup;
	Material* currentMaterial;

	Vertex position;
	Vertex eulerRotation;

	int useDisplayList;
	GLuint displayListId;

	float scaleFactor;
	Vertex offSet;

	BSP* bsp;
	void buildBsp();
private:

	map<string,Parser*> parsers;
	void buildSetOfGroupsToRender(BSP* bsp, vector<Group*>& toRender);
	void renderDirectMode(vector<Group*>& toRender);
	void render(BSP* bsp);
	void sortGroupPerTextureId();
	void refineBSP(BSP* bsp, int depth);
	void dispatch(vector<Group*>& groups,Plan& plan, BSP* front, BSP* back);
	void splitGroup(Group* group,Plan& plan, vector<Group*>& front,vector<Group*>& back);
	void splitFace(Face* face,Plan& plan,Group* front, Group* back);

	void getAllBspGroups(BSP* bsp,vector<Group*>& toRender);
	void cutFaceViaPlan(Face* faceToCut,Group* front, Group* back, Plan& plan );
	float processIntesection(Vertex& point1, Vertex& point2, Plan& plan);
};