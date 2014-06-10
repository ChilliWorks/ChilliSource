/*
 *  Main.h
 *  PreMultipliedAlphaPNGTool
 *
 *  Created by Ian Copland on 28/03/2012.
 *  Copyright 2012 Tag Games Ltd. All rights reserved.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif 
#ifndef _PREMULTIPLIEDALPHAPNGTOOL_MAIN_H_
#define _PREMULTIPLIEDALPHAPNGTOOL_MAIN_H_

//-------------------------------------------------
/// Print Fatal Error
///
/// Displays a fatal error message. This can be called
/// from C.
//-------------------------------------------------
void PrintFatalError(char* inFatalError);
//-------------------------------------------------
/// Print Message
///
/// Displays a message. This can be called
/// from C.
//-------------------------------------------------
void PrintMessage(char* inMessage);
//-------------------------------------------------
/// Print Warning
///
/// Displays a warning. This can be called
/// from C.
//-------------------------------------------------
void PrintWarning(char* inMessage);

#endif
#ifdef __cplusplus
}
#endif