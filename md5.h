#pragma once

#include "includes.h"
#include "md5definitions.h"
#include "Quaternion.h"
#include "Entity.h"

class md5Animation;

#define READ_BUFF_SIZE 1024

class md5Object  : public Entity{
	public:
		md5Object();
		~md5Object();
		void set(const char* md5mesh, const char* md5anim);
		void load();
		void interolate(float tick);
		void render();
		void update();
		
		GLint tangentLoc;
		md5Animation* animation ;
		float animCursor;
		

		float** tangentArraySkin;

		int noDraw;
		
	private:
		
		//Model data
		md5joint* joints;
		int jointsCount;

		md5Mesh* meshes;
		int meshesCount;
	
		//int lastNormalTextureId;

		static int lastSpecId;
		static int lastDiffId;
		static int lastBumpId;

		
		void md5Object::renderJoints();
		void md5Object::renderSkin(int meshIndex);
		void md5Object::renderNormals(int meshIndex);
		void md5Object::renderTangents(int meshIndex);
		void md5Object::prepareVertexArray(int meshIndex);
		
	
		//Methods used to read the mesh
		int loadingStatus ;
		int version;
		char commandline[READ_BUFF_SIZE];
		void loadMesh(FILE* fp);
		char buff[READ_BUFF_SIZE];    //Reading buffer (md5 model are read line by line, a line is not expected to be more than 512bytes)
		void readVersion(FILE* fp);
		void getJointsCount(FILE* fp);
		void getMeshesCount(FILE* fp);
		void readCommandLine(FILE* fp);
		void readJoints(FILE* fp);
		void readMeshes(FILE* fp);
	
		//Post parsing
		void loadShaders();

		void generateNormalsAndTangents();
		

		//Anim part
		
		
				// Used for rendition in openGL, double pointer because each mesh has it's own vertex/element array
		int** elementsSkin;
		float** vertexArraySkin;
		float** normalArraySkin;
		
		float** texturArraySkin;

		//OpenGL VBO optimisation
		GLuint  *vboVertexId;
		GLuint  *vboNormalId;
		GLuint  *vboTexturId;
		GLuint  *vboElementsId;

		string md5mesh;
		string md5anim;
};


