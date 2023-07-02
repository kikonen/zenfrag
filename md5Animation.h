#pragma once

#include "md5definitions.h"
#include "md5.h"
#include "includes.h"


struct hierarchyElement
{
	char name[64] ;
	int parent    ;
	int flags     ;
	int startIndex;
};

struct boundElement
{
	float3 min;
	float3 max;
};


class md5Animation
{
	public:
		md5Animation(void);
		void load(const char* animationFile);
		void generateAllSkeletons(void);
		~md5Animation(void);

		// Here are stored all the skeletons (one per frame)
		int numJoints ;
		md5joint** joints;
		bool* jointsGenerated;

		md5joint* baseFrame;
		int numFrames ;
		int frameRate ;
		void update(int frameIndex, float percentage, md5joint* joint);

		float** frames;
		bool loadingErrors;
	private:
	
		//md5Object* md5object;
		
		void generateSkeletonsForFrame(int frameIndex);
		
	
		
		
		

		hierarchyElement* hierarchy;
		int hierarchyCount;

		boundElement* bounds;
		int boundsCount;

		
		int baseFrameCount;

		
		
		int numAnimatedComponents ;

		

		

		// Loading variables;

		char buff[READ_BUFF_SIZE]; 
		int loadingStatus ;
		int version;
		char commandline[READ_BUFF_SIZE];
		void readVersion(FILE* fp);
		void readCommandline(FILE* fp);
		void readNumFrames(FILE* fp);
		void readNumJoints(FILE* fp);
		void readFrameRate(FILE* fp);
		void readNumAnimatedComponents(FILE* fp);
		void readHierarchy(FILE* fp);
		void readBounds(FILE* fp);
		void readBaseFrame(FILE* fp);
		void readFrames(FILE* fp);
};
