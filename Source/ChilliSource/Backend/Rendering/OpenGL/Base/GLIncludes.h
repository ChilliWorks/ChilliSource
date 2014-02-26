#ifndef _MOFOW_RENDERINGPLATFORM_OPENGL_GLINCLUDES_H_
#define _MOFOW_RENDERINGPLATFORM_OPENGL_GLINCLUDES_H_

#ifdef TARGET_OS_IPHONE
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <OpenGLES/EAGL.h>
#include <OpenGLES/EAGLDrawable.h>
#elif TARGET_WINDOWS
#include <ChilliSource/Platform/Windows/OpenGL/glew.h>
#include <ChilliSource/Platform/Windows/OpenGL/glfw.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#if defined TARGET_OS_IPHONE || defined TARGET_ANDROID
#define MOFLOW_OPENGLES2
#elif defined TARGET_WINDOWS
#define MOFLOW_OPENGL
#endif

//---Missing OpenGL definitions
#define GL_GLEXT_PROTOTYPES
#ifdef TARGET_ANDROID
typedef char GLchar;
#endif

#endif