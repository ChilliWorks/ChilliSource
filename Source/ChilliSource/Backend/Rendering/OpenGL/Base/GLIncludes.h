#ifndef _MOFOW_RENDERINGPLATFORM_OPENGL_GLINCLUDES_H_
#define _MOFOW_RENDERINGPLATFORM_OPENGL_GLINCLUDES_H_

#ifdef CS_TARGETPLATFORM_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <OpenGLES/EAGL.h>
#include <OpenGLES/EAGLDrawable.h>
#elif defined CS_TARGETPLATFORM_WINDOWS
#include <Platform/Windows/glew.h>
#include <Platform/Windows/glfw.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#if defined CS_TARGETPLATFORM_IOS || defined TARGET_ANDROID
#define CS_OPENGLVERSION_ES
#elif defined CS_TARGETPLATFORM_WINDOWS
#define CS_OPENGLVERSION_STANDARD
#endif

//---Missing OpenGL definitions
#define GL_GLEXT_PROTOTYPES
#ifdef TARGET_ANDROID
typedef char GLchar;
#endif

#endif