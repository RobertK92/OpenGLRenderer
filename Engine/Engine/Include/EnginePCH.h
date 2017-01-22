//////////////////////////////////////////////////////////////////////////
// PRE-COMPILED HEADER
//////////////////////////////////////////////////////////////////////////

#pragma once

#define E3D_DEBUG_LOGS_ENABLED

#define E3D_API __declspec(dllexport)
#define E3D_STRINGIFY(VAR) #VAR
#define E3D_BUFFER_OFFSET(offset) ((void*)(offset))
#define E3D_MEMBER_OFFSET(s,m) ((char*)NULL + (offsetof(s,m)))

#define E3D_UBO_BINDING_PROJECTVIEW 0

typedef char16_t char16;
typedef char32_t char32;
typedef wchar_t wchar;

typedef char int8;
typedef short int int16;
typedef long long int64;

typedef unsigned char uchar;
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint;
typedef unsigned long long uint64;

#define E3D_MAIN(gameclass) \
int main(int argc, char** argv)\
{\
	gameclass* gc = new gameclass();\
	gc->__InternalInit(gc); \
	gc->Initialise(argc, argv);\
	while(gc->IsRunning()){ \
		gc->__InternalTick(); \
	} \
	return 0;\
}\

//////////////////////////////////////////////////////////////////////////
// Config
//////////////////////////////////////////////////////////////////////////
#define E3D_VERTEX_SHADER_UNLIT_PATH "Data/Shaders/Unlit.vert"
#define E3D_FRAGMENT_SHADER_UNLIT_PATH "Data/Shaders/Unlit.frag"
#define E3D_VERTEX_SHADER_PHONG_PATH "Data/Shaders/Phong.vert"
#define E3D_FRAGMENT_SHADER_PHONG_PATH "Data/Shaders/Phong.frag"
#define E3D_GEOMETRY_SHADER_RENDER_NORMALS_PATH "Data/Shaders/Geometry.geom"

//////////////////////////////////////////////////////////////////////////
// STL headers
//////////////////////////////////////////////////////////////////////////
#include <fstream>
#include <vector>
#include <map>
#include <ctime>
#include <cassert>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <iostream>

#define E3DPrint(x) std::cout << x << std::endl;

#ifdef E3D_DEBUG_LOGS_ENABLED
#define E3DDebugPrint(x) std::cout << "--> " << x << std::endl
#else
#define E3DDebugPrint(x)
#endif // E3D_DEBUG_LOGS_ENABLED

#ifdef _WIN32
#include <windows.h>
#define E3DShowIssue(issueMsg) { \
	MessageBox(NULL, (LPCTSTR)issueMsg, LPCTSTR("E3D Issue Tracker"), MB_ICONWARNING); \
	exit(0); \
} 
#define E3DShowError(errMsg) { \
	MessageBox(NULL, (LPCTSTR)errMsg, LPCTSTR("E3D Error"), MB_ICONERROR); \
	exit(0); \
} 

#else
#define E3DShowIssue(issueMsg) assert(false && issueMsg);
#define E3DShowError(errMsg) assert(false && errMsg);
#endif // _WIN32

//////////////////////////////////////////////////////////////////////////
// GLEW
//////////////////////////////////////////////////////////////////////////
#include <GLEW/include/GL/glew.h>

//////////////////////////////////////////////////////////////////////////
// GLFW (window)
//////////////////////////////////////////////////////////////////////////
#define _GLFW_USE_DWM_SWAP_INTERVAL 1
#include <GLFW/include/glfw3.h>

//////////////////////////////////////////////////////////////////////////
// GL MATH (glm)
//////////////////////////////////////////////////////////////////////////
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>

//////////////////////////////////////////////////////////////////////////
// Open Asset Importer (assimp)
//////////////////////////////////////////////////////////////////////////
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/postprocess.h>

//////////////////////////////////////////////////////////////////////////
// Free Image (image loader)
//////////////////////////////////////////////////////////////////////////
#include <FreeImage/include/FreeImage.h>