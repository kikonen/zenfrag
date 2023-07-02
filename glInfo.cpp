///////////////////////////////////////////////////////////////////////////////
// glInfo.cpp
// ==========
// get GL vendor, version, supported extensions and other states using glGet*
// functions and store them glInfo struct variable
//
// To get valid OpenGL infos, OpenGL rendering context (RC) must be opened 
// before calling glInfo::getInfo(). Otherwise it returns false.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2005-10-04
// UPDATED: 2006-12-14
//
// Copyright (c) 2005 Song Ho Ahn
///////////////////////////////////////////////////////////////////////////////

#include "glInfo.h"
#include "includes.h"




///////////////////////////////////////////////////////////////////////////////
// extract openGL info
// This function must be called after GL rendering context opened.
///////////////////////////////////////////////////////////////////////////////
bool glInfo::getInfo()
{
    char* str = 0;
    char* tok = 0;

    // get vendor string
    str = (char*)glGetString(GL_VENDOR);
    if(str) 
		this->vendor = str;                  // check NULL return value
    else 
		return false;

    // get renderer string
    str = (char*)glGetString(GL_RENDERER);
    if(str) 
		this->renderer = str;                // check NULL return value
    else 
		return false;

    // get version string
    str = (char*)glGetString(GL_VERSION);
    if(str) 
		this->version = str;                 // check NULL return value
    else 
		return false;

    // get all extensions as a string
    str = (char*)glGetString(GL_EXTENSIONS);

    // split extensions
    if(str)
    {
        tok = strtok((char*)str, " ");
        while(tok)
        {
            this->extensions.push_back(tok);    // put a extension into struct
            tok = strtok(0, " ");               // next token
        }
    }
    else
    {
		return false;
	}
		
    // sort extension by alphabetical order
    std::sort(this->extensions.begin(), this->extensions.end());

    // get number of color bits
    glGetIntegerv(GL_RED_BITS, &this->redBits);
    glGetIntegerv(GL_GREEN_BITS, &this->greenBits);
    glGetIntegerv(GL_BLUE_BITS, &this->blueBits);
    glGetIntegerv(GL_ALPHA_BITS, &this->alphaBits);

    // get depth bits
    glGetIntegerv(GL_DEPTH_BITS, &this->depthBits);

    // get stecil bits
    glGetIntegerv(GL_STENCIL_BITS, &this->stencilBits);

    // get max number of lights allowed
    glGetIntegerv(GL_MAX_LIGHTS, &this->maxLights);

    // get max texture resolution
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &this->maxTextureSize);

    // get max number of clipping planes
    glGetIntegerv(GL_MAX_CLIP_PLANES, &this->maxClipPlanes);

    // get max modelview and projection matrix stacks
    glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &this->maxModelViewStacks);
    glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &this->maxProjectionStacks);
    glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, &this->maxAttribStacks);

    // get max texture stacks
    glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, &this->maxTextureStacks);

    return true;
}



///////////////////////////////////////////////////////////////////////////////
// check if the video card support a certain extension
///////////////////////////////////////////////////////////////////////////////
bool glInfo::isExtensionSupported(const char* ext)
{
    // search corresponding extension
    std::vector< string >::const_iterator iter = this->extensions.begin();
    std::vector< string >::const_iterator endIter = this->extensions.end();

    while(iter != endIter)
    {
        if(ext == *iter)
            return true;
        else
            ++iter;
    }
    return false;
}



///////////////////////////////////////////////////////////////////////////////
// print OpenGL info to screen and save to a file
///////////////////////////////////////////////////////////////////////////////
void glInfo::printSelf()
{
    

    
    printf("OpenGL Driver Info");					printf("\n");
    printf("==================");					printf("\n");
	printf("Vendor: %s",this->vendor.c_str());		printf("\n");
    printf("Version: %s",this->version.c_str() );	printf("\n");
    printf("Renderer: %s",this->renderer.c_str() );	printf("\n");

    printf("\n");
    printf("Color Bits(R,G,B,A): (%d,%d,%d,%d)",this->redBits,this->greenBits,this->blueBits,this->alphaBits); printf("\n");
    printf("Depth Bits: %d",this->depthBits );		printf("\n");
    printf("Stencil Bits: %d",this->stencilBits  );	printf("\n");

    printf("\n");
    printf("Max Texture Size: %dx%d",this->maxTextureSize,this->maxTextureSize  );	printf("\n");
    printf("Max Lights: %d",this->maxLights  );										printf("\n");
    printf("Max Clip Planes: %d",this->maxClipPlanes  );							printf("\n");
    printf("Max Modelview Matrix Stacks: %d",this->maxModelViewStacks  );			printf("\n");
    printf("Max Projection Matrix Stacks: %d",this->maxProjectionStacks  );		printf("\n");
    printf("Max Attribute Stacks: %d",this->maxAttribStacks  );					printf("\n");
    printf("Max Texture Stacks: %d",this->maxTextureStacks  );						printf("\n");

    printf("\n");
    printf("Total Number of Extensions: %d",this->extensions.size() );	printf("\n");
    printf("=============================="); printf("\n");
    for(unsigned int i = 0; i < this->extensions.size(); ++i)
	{
		printf("%s",extensions.at(i).c_str()); printf("\n");
	}
    printf("======================================================================");

    printf("\n");
}
