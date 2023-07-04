#include "Camera.h"
#include "Config.h"
#include "CreatureShader.h"
#include "Engine.h"
#include "EnvShader.h"
#include "FPSCamera.h"
#include "FreeCamera.h"
#include "Frustrum.h"
#include "glex_impl.h"
#include "glInfo.h"
#include "includes.h"
#include "KeyBoardListener.h"
#include "Light.h"
#include "main.h"
#include "ShaderLoader.h"
#include "Scene.h"
#include "Timer.h"
#include "WaveObject.h"
#include "md5.h"
#include "Light.h"
#include "ShaderDefinitions.h"


#include "LightScatteringShader.h"
#include "TRIncreaseExposure.h"
#include "TRDecreaseExposure.h"
#include "TRIncreaseDecay.h"
#include "TRDecreaseDecay.h"
#include "TRIncreaseWeight.h"
#include "TRDecreaseWeight.h"
#include "TRIncreaseDensity.h"
#include "TRDecreaseDensity.h"
#include "SwitchLightScatt.h"
#include "Pause.h"
#include "Surface.h"

Engine* Engine::engine ;


Engine::Engine(){}

void Engine::readConfiguration()
{
	//fboId = 0;
	//shadowMapId =0;
	engine = this;
	shaderSupported = false;
	drawThirdPerson = false;
	Timer::paused = false;
	gameOn = 1;
	cameraTckMvt = 0;
	//All the following parameters are usually overwritten by loadConfiguration() call
	renderWidth = 1024;
	renderHeight = 768;
	fboLightScatteringRatio = 2;
	fboShadowBuffersRatio = 0.5f;
	lightScatteringActivated = false;
	renderFullScreen = false;
	shaderFolder = ".\\shaders\\";
	modelFolder = "D:\\downloads\\doom3\\extractedModels\\";

	Config::getInstance();
	Scene::getInstance();
}

void Engine::init()
{


    glInfoObject = new glInfo();
	glInfoObject->getInfo();

	initExtensions();

	//Vertex lightPosition = Vertex(10,100,10); //560
	//Vertex lightLookAt = Vertex(0,0,0);
	//light = new Light(renderWidth,renderHeight,lightPosition,lightLookAt, 1 ,300);

	if (fboSupported)
	{
		for(unsigned int i = 0 ; i < lights.size(); i++)
			lights[i]->loadRessources();
		generateLightScatteringFBO();
	}

	if (shaderSupported)
	{
		creatureShader = new CreatureShader(CREATURE_VERTEX_SHADER,CREATURE_FRAGMENT_SHADER);
		envShader = new EnvShader(ENV_VERTEX_SHADER,ENV_FRAGMENT_SHADER);
		lightScattShader = new LightScatteringShader(LIGHT_SCATTERING_VERTEX_SHADER,LIGHT_SCATTERING_FRAGMENT_SHADER,lights[0]);
	}





	Vertex camera1Position = Vertex(112,75,-122); //560
	Vertex camera1LookAt = Vertex(88,71,-85);
	firstPerson = new FPSCamera(renderWidth,renderHeight,camera1Position,camera1LookAt, 1 ,1000);

	keyboard = new KeyBoardListener();
	keyboard->addListener(SDLK_w,new CTRIncreaseExposure(this->lightScattShader));
	keyboard->addListener(SDLK_s,new CTRDecreaseExposure(this->lightScattShader));
	keyboard->addListener(SDLK_e,new CTRIncreaseDecay(this->lightScattShader));
	keyboard->addListener(SDLK_d,new CTRDecreaseDecay(this->lightScattShader));
	keyboard->addListener(SDLK_r,new CTRIncreaseWeight(this->lightScattShader));
	keyboard->addListener(SDLK_f,new CTRDecreaseWeight(this->lightScattShader));
	keyboard->addListener(SDLK_t,new CTRIncreaseDensity(this->lightScattShader));
	keyboard->addListener(SDLK_g,new CTRDecreaseDensity(this->lightScattShader));
	//keyboard->addListener(SDLK_F1,new SwitchLightScatt());
	keyboard->addListener(SDLK_SPACE,new Pause());


	printf("Loading %d creatures.\n",creatures.size());
	for(unsigned int i = 0 ; i < creatures.size(); i++)
		creatures[i]->load();
	//printf("Loading creatures done.\n");

	printf("Loading %d surfaces.\n",surfaces.size());
	for(unsigned int i = 0 ; i < surfaces.size(); i++)
		surfaces[i]->load();
	//printf("Loading surfaces done.\n");

	/*
	weapon = new md5Object();
	weapon->tangentLoc = creatureShader->tangentLoc;
	weapon->load("models\\md5\\weapons\\shotgun_view\\viewshotgun.md5mesh","models\\md5\\weapons\\shotgun_view\\idle.md5anim");
	//weapon->load("models\\md5\\weapons\\shotgun_view\\viewshotgun.md5mesh","models\\md5\\weapons\\shotgun_view\\reload_loop.md5anim");


	*/
	/* NOT WORKING */ //pinky->load("models\\md5\\monsters\\zombies\\jumpsuitzombie.md5mesh",NULL);
	//pinky->load("models\\md5\\monsters\\zct\\zct.md5mesh",NULL);
	//pinky->load("models\\md5\\monsters\\wraith\\wraith.md5mesh",NULL);

	/* NOT WORKING */ //pinky->load("models\\md5\\monsters\\maledict\\maledict.md5mesh",NULL);
	/* NOT WORKING */ //pinky->load("models\\md5\\monsters\\skeleton\\skeleton.md5mesh",NULL);
	/* NOT WORKING */ //pinky->load("models\\md5\\monsters\\cherub\\cherub.md5mesh",NULL);

	//pinky->load("models\\md5\\monsters\\archvile\\archvile.md5mesh",NULL);
	//pinky->load("models\\md5\\monsters\\revenant\\james\\revenant.md5mesh",NULL);

	//pinky->load("models\\md5\\monsters\\guardian\\guardian.md5mesh",NULL);

	/*
	sabaoth = new md5Object();
	sabaoth->tangentLoc = creatureShader->tangentLoc;
	sabaoth->load("models\\md5\\monsters\\sabaoth\\sabaoth.md5mesh",NULL);
	*/

	// pinky->load("models\\md5\\monsters\\sentry\\sentry.md5mesh",NULL);

	//pinky->load("models\\md5\\monsters\\vagary\\vagary.md5mesh",NULL);
	//pinky->load("models\\md5\\monsters\\zcc\\zcc.md5mesh",NULL);

	/*
	md5Object* player = new md5Object();
	player->tangentLoc = creatureShader->tangentLoc;
	player->load("models\\md5\\characters\\player\\mocap\\player.md5mesh",NULL);
	player->position = float3(50,0,50);
	creatures.push_back(player);
	*/
}


///////////////////////////////////////////////////////////////////////////////
// write 2d text using GLUT
// The projection matrix must be set to orthogonal before call this function.
///////////////////////////////////////////////////////////////////////////////
void drawString(const char *str, int x, int y)
{
	static const float color[4] = {1, 1, 1, 1};
    glColor4fv(color);          // set text color
    glRasterPos2i(x, y);        // place text position

    // loop all characters in the string
    while(*str)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *str);
        ++str;
    }
}




char* fps = (char*)malloc(10 * sizeof(char));
char* polyCount = (char*)malloc(30 * sizeof(char));
char* s_textSwitchs = (char*)malloc(30 * sizeof(char));
char* s_extensions = (char*)malloc(40 * sizeof(char));


void Engine::renderEntity(md5Object* entity)
{
	glPushMatrix();
		glTranslated(entity->position.x,entity->position.y,entity->position.z);
		glRotatef (-90.0f, 1.0, 0.0, 0.0);
		glRotatef (entity->orientation, 0.0, 0.0, 1.0);

		// Keep
		glMatrixMode(GL_TEXTURE);
			glActiveTextureARB(GL_TEXTURE0);
				glPushMatrix();
					glTranslated(entity->position.x,entity->position.y,entity->position.z);
					glRotatef (-90.0f, 1.0, 0.0, 0.0);
					glRotatef (entity->orientation, 0.0, 0.0, 1.0);

					glActiveTextureARB(GL_TEXTURE1);
					glPushMatrix();
						glTranslated(entity->position.x,entity->position.y,entity->position.z);
						glRotatef (-90.0f, 1.0, 0.0, 0.0);
						glRotatef (entity->orientation, 0.0, 0.0, 1.0);


					entity->render();



					glPopMatrix();
			glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Engine::renderScene(bool onlyOcluders, bool useShaders)
{

	if (useShaders)
		creatureShader->enable();
	else
		creatureShader->disable();

	for ( unsigned int i = 0 ; i < creatures.size();  i++)
			renderEntity(creatures.at(i));

	if (onlyOcluders)
		return;

	//if (useShaders)
	//	envShader->enable();

	renderRoom();


}

void Engine::renderRoom()
{
	for(unsigned int i = 0 ; i < surfaces.size(); i++)
		surfaces[i]->render();
}

void Engine::grabMatrices()
{
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
}

void Engine::setTextureMatrix(unsigned int textureId)
{
	grabMatrices();

	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(textureId);

	const GLdouble bias[] = {	0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0};

	glLoadIdentity();
	glLoadMatrixd(bias);

	glMultMatrixd (projection);
	glMultMatrixd (modelView);

	glMatrixMode(GL_MODELVIEW);
}




void Engine::render()
{



	//fflush(stdout);
	//cout << "salut" << std::endl;
	//printf("\nStarting rendition-----------------------------\n");

	polygonRendered = 0;
	textureSwitchs = 0;

	// Perform prepass for lightScattering effect




	//glDisable(GL_DEPTH_TEST);


	if (lightScatteringActivated)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,lightScattFBOId);
		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		//glUseProgramObjectARB(0);
		glClearColor(0.1f,0.1f,0.1f,1);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);



		glViewport(0,0,renderWidth/Engine::engine->fboLightScatteringRatio,renderHeight/Engine::engine->fboLightScatteringRatio);
		firstPerson->setupPerspective();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		firstPerson->apply();
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glPushMatrix();
				glTranslatef(lights[0]->position.x,lights[0]->position.y,lights[0]->position.z);
				lights[0]->render();
		glPopMatrix();

		glColor4f(0.0f,0.0f,0.0f,1.0f);
		renderScene(true,false);
		//glColor4f(1.0f,1.0f,1.0f,0.4f);
		//renderRoom();

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	//return;
	//glClear(GL_DEPTH_BUFFER_BIT );


	//return;




	//Render shadow map for every light
		// No shaders
		for(unsigned int i=0 ; i < lights.size(); i ++)
		{
			Light* currentLight = lights[i];
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glCullFace(GL_BACK);

			// Render offscreen to the depth fbo
			//If light is a pointLight
			if (currentLight->type == Light::POINT_LIGHT)
			{

				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, currentLight->backShadowFBOId);
				glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

				// Apply light modelview transformation
				currentLight->setupPerspective();
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				currentLight->applyBackView();

				setTextureMatrix(GL_TEXTURE1);
				renderScene(true,false);
			}

			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, currentLight->frontShadowFBOId);
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			glViewport(0,0,renderWidth/Engine::engine->fboShadowBuffersRatio,renderHeight/Engine::engine->fboShadowBuffersRatio);

			// Apply light modelview transformation
			currentLight->setupPerspective();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			currentLight->apply();

			setTextureMatrix(GL_TEXTURE0);
			renderScene(true,false);


		}

	// End Render shadow map




	// Rendering with shadow
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glViewport(0,0,renderWidth,renderHeight);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		//glCullFace(GL_FRONT);

		glActiveTextureARB(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,lights[0]->frontShadowTextureId);

		//printf("shadow texture id = %d\n",lights[0]->frontShadowFBOId);

		if (lights[0]->type == Light::POINT_LIGHT)
		{
			glActiveTextureARB(GL_TEXTURE7);
			glBindTexture(GL_TEXTURE_2D,lights[0]->backShadowFBOId);
		}

		// Render
		firstPerson->setupPerspective();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		firstPerson->apply();

		GLfloat  light_position[4];
		light_position[0] = lights[0]->position.x;
		light_position[1] = lights[0]->position.y;
		light_position[2] = lights[0]->position.z;
		light_position[3] = 1;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lights[0]->ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lights[0]->diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lights[0]->specular);
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, lights[0]->constantAttenuation);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, lights[0]->linearAttenuation);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, lights[0]->quadraticAttenuation);

		glDisable(GL_LIGHTING);

		glPushMatrix();
			glTranslatef(lights[0]->position.x,lights[0]->position.y,lights[0]->position.z);
			lights[0]->render();
		glPopMatrix();

		glCullFace(GL_FRONT);
		glEnable(GL_DEPTH_TEST);
		renderScene(false,true);

		if (lightScatteringActivated)
			lightScattShader->grabLightCoo() ;

/*
		glClear(GL_DEPTH_BUFFER_BIT);

		//Render weapon

		creatureShader->enable();  // Optimization possible here, this shader is re-activated
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(55,(float)renderWidth/renderHeight,1,200);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		Vertex lightPosForWeapon = light->position - firstPerson->position;
		lightPosForWeapon.rotateY(firstPerson->rotationY);
		lightPosForWeapon.rotateX(-firstPerson->rotationX);

		light_position[0] = lightPosForWeapon.x ;
		light_position[1] = lightPosForWeapon.y ;
		light_position[2] = lightPosForWeapon.z ;
		light_position[3] = 0;
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		glPushMatrix();
			glTranslated(0,0,-10);
			glRotated(90,0,0,1);
			glRotated(90,0,1,0);
			weapon->render();
		glPopMatrix();
*/


		glUseProgramObjectARB(0);
	// End Render scene with shadow

		if (lightScatteringActivated)
			lightScattShader->enable();

		glDisable(GL_LIGHTING);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();


		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-renderWidth/2,renderWidth/2,-renderHeight/2,renderHeight/2,0,500);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if (lightScatteringActivated)
		{

			//Post pass for lightScattering effect

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			glActiveTextureARB(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,lightScattTextureId);
			glBegin(GL_QUADS);
				glTexCoord2f(0,0);	glVertex2f(-renderWidth/2,-renderHeight/2);
				glTexCoord2f(0,1);	glVertex2f(-renderWidth/2, renderHeight/2);
				glTexCoord2f(1,1);	glVertex2f( renderWidth/2, renderHeight/2);
				glTexCoord2f(1,0);	glVertex2f( renderWidth/2,-renderHeight/2);
			glEnd();
			lightScattShader->disable();
			glDisable(GL_BLEND);
		}





	glDisable(GL_LIGHTING);
	//============ DEBUG==========================
	/*
	// Draw shadow map
	glEnable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);

	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//
	//glColor4f(1,1,1,1);
	glActiveTextureARB(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,lights[0]->frontShadowTextureId);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);	glVertex2f(renderWidth/4,renderHeight/4);
		glTexCoord2f(0,1);	glVertex2f(renderWidth/4, renderHeight/2);
		glTexCoord2f(1,1);	glVertex2f( renderWidth/2, renderHeight/2);
		glTexCoord2f(1,0);	glVertex2f( renderWidth/2,renderHeight/4);
	glEnd();
	*/
	//============ DEBUG==========================


	//glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
		sprintf(fps,"fps: %.0f",Timer::fps );
		sprintf(polyCount,"Polygones rendered: %d",polygonRendered );
		//sprintf(s_textSwitchs,"textures switches: %d",textureSwitchs );
		//drawString(fps,renderWidth/2-250,-renderHeight/2+60);
		//drawString(polyCount,renderWidth/2-250,-renderHeight/2+40);
		//drawString(s_textSwitchs,renderWidth/2-250,-renderHeight/2+20);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}


void Engine::update()
{
	Timer::update();

	SDL_PumpEvents();

	SDL_Event event;
	// Check for events



	while ( SDL_PollEvent (&event) ) {
		switch (event.type) {
			case SDL_QUIT:	gameOn = 0;	break;
			case SDL_MOUSEMOTION:
								//if (event.button.button == SDL_BUTTON_LEFT)
								//{
									firstPerson->updateMouse(event.button.x,event.button.y);

								//}


								break;
			default:		    break;
		}
	}

	{
		int keyCount;
		const Uint8* tKeys = SDL_GetKeyboardState(&keyCount);

		if (keyCount > 0) {
			readSystemEntries(tKeys);

			keyboard->update(tKeys);

			// Update camera according to keys
			firstPerson->update(tKeys);
		}
	}

	firstPerson->position = Vertex(112,75,-122);
	firstPerson->lookAt = Vertex(88,71,-85);


	if (!Timer::paused)
	{
		cameraTckMvt += Timer::tick;
		//lights[0]->position.x = 255 * cos((float)cameraTckMvt/1000.0f);
		//lights[0]->position.z = 255 * sin((float)cameraTckMvt/1000.0f);

		for ( unsigned int i = 0 ; i < creatures.size();  i++)
			(creatures.at(i))->update();

		//weapon->update();
	}


 }





void Engine::run ()
{
	init();
    while (gameOn )
	{
		update();
		render ();

        SDL_GL_SwapWindow(window);
	}
}

void Engine::readSystemEntries(const Uint8 * tKeys)
{
	if (tKeys[SDLK_ESCAPE] || tKeys[SDLK_q])
			gameOn = false;


}



void Engine::generateLightScatteringFBO()
{
	int offScreenWidth = renderWidth/Engine::engine->fboLightScatteringRatio;
	int offScreenHeight = renderHeight/Engine::engine->fboLightScatteringRatio;

	glGenTextures(1, &lightScattTextureId);
	glBindTexture(GL_TEXTURE_2D, lightScattTextureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, offScreenWidth, offScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a renderbuffer object to store depth info
	/*
	GLuint lightScattDepthBufferId;
	glGenRenderbuffersEXT(1, &lightScattDepthBufferId);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, lightScattDepthBufferId);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,offScreenWidth, offScreenHeight);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
*/
	// create a framebuffer object
	glGenFramebuffersEXT(1, &lightScattFBOId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, lightScattFBOId);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D, lightScattTextureId, 0);
	// attach the renderbuffer to depth attachment point
	//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT, lightScattDepthBufferId);

	// check FBO status
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		fboUsed = false;
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO for light scattering\n");
	}
	else
		fboUsed = true;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);




}