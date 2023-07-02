#pragma once

#include "SDL.h"

#include "GLUT/glut.h"
#include "glext.h"
#include "includes.h"

//#include "md5MaterialsLibrary.h"

class md5Object;
class CreatureShader;
class LightScatteringShader;
class EnvShader;
class Camera;
class Light;
class glInfo;
class KeyBoardListener;
class Surface;


class Engine
{
	public:

		Engine();

		// First method to call after creating the object
		void readConfiguration();

		void init();
		void mainLoop ();
		void render();
		void run();
		
		static Engine* engine ;

		Camera* firstPerson;
		Camera* observer;

		vector<Light*> lights;

		bool drawThirdPerson;

		int polygonRendered;
		int textureSwitchs;

		glInfo* glInfoObject;

		bool shaderSupported;
		//GLhandleARB shadowShader;

		// FBO flags
		bool fboSupported;
		bool fboUsed;

		//Shaders
		CreatureShader* creatureShader;
		EnvShader* envShader;
		LightScatteringShader* lightScattShader;

		//Parameters read from config.txt
		int renderWidth;
		int renderHeight;
		int fboLightScatteringRatio;
		float fboShadowBuffersRatio;
		bool lightScatteringActivated ;
		bool renderFullScreen;
		string shaderFolder;
		string modelFolder;

		vector<md5Object*> creatures;
		vector<Surface*> surfaces;

	private:

		short gameOn ;
		void update();
		void renderScene(bool setMatrix, bool useShaders);
		void readSystemEntries(Uint8 * tKeys);

		//WaveObject* tenso;
		//WaveObject* sky1;
		//WaveObject* sky2;
		//WaveObject* ikal;

		KeyBoardListener* keyboard ;

		void grabMatrices(void);
		double modelView[16];
		double projection[16];
		void setTextureMatrix(unsigned int textureId);

		// FBO shadow map
		//void generateShadowMap();
		//GLuint shadowMapId;
		//GLuint fboTextureId;
		//GLuint fboId;
		
		// FBO Light scatteting
		void generateLightScatteringFBO();
		GLuint lightScattTextureId;
		GLuint lightScattFBOId;

		

		


		// Weapons
		md5Object* shotgun;

		void renderRoom();		
		void renderEntity(md5Object* entity);

		

		md5Object* weapon;

		unsigned int cameraTckMvt;
} ;