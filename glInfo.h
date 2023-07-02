#pragma once

///////////////////////////////////////////////////////////////////////////////
// glInfo.h
// ========
// get GL vendor, version, supported extensions and other states using glGet*
// functions and store them glInfo struct variable
//
// To get valid OpenGL infos, OpenGL rendering context (RC) must be opened
// before calling glInfo::getInfo(). Otherwise it returns false.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2005-10-04
// UPDATED: 2008-06-11
//
// Copyright (c) 2005 Song Ho Ahn
///////////////////////////////////////////////////////////////////////////////


#include "includes.h"

// struct variable to store OpenGL info
class glInfo
{
public:
    std::string vendor;
    std::string renderer;
    std::string version;
    std::vector <std::string> extensions;
    GLint redBits;
    GLint greenBits;
    GLint blueBits;
    GLint alphaBits;
    GLint depthBits;
    GLint stencilBits;
    GLint maxTextureSize;
    GLint maxLights;
    GLint maxAttribStacks;
    GLint maxModelViewStacks;
    GLint maxProjectionStacks;
    GLint maxClipPlanes;
    GLint maxTextureStacks;

    // ctor, init all members
    glInfo() : redBits(0), greenBits(0), blueBits(0), alphaBits(0), depthBits(0),
               stencilBits(0), maxTextureSize(0), maxLights(0), maxAttribStacks(0),
               maxModelViewStacks(0), maxClipPlanes(0), maxTextureStacks(0) {}

    bool getInfo();                             // extract info
    void printSelf();                           // print itself
    bool isExtensionSupported(const char* ext); // check if a extension is supported
};

