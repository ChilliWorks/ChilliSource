/*
 *  BuildProperties.h
 *  moFlo
 *
 *  Created by Scott Downie on 21/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

//================================================
/// Description:
///
/// Set the target platform for the current build
/// Set the logging level
//================================================

#ifndef _MO_FLO_BUILD_PROPERTIES_
#define _MO_FLO_BUILD_PROPERTIES_

#ifdef __APPLE__
	#include "TargetConditionals.h"
#endif

//---Logging Levels
#define LOG_VERBOSE		(0)
#define LOG_WARNING		(1)
#define LOG_ERROR		(2)

#ifdef DEBUG
    //---Active Log Level
    #define LOG_LEVEL		(LOG_VERBOSE)
    //---Debug Flags
    #define DEBUG_DRAWING	(1)
#else
    //---Active Log Level
    #define LOG_LEVEL		(LOG_ERROR)
    //---Debug Flags
    #define DEBUG_DRAWING	(0)
#endif

#endif
