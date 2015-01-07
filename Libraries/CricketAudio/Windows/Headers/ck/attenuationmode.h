// Copyright 2011 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkAttenuationMode CkAttenuationMode */
/** @{ */

#pragma once
#include "ck/platform.h"


/** Indicates how volume on 3D sounds is attenuated with distance. */
typedef enum 
{

    /** No attenuation; volume is 1.0 when closer than the far distance, and far volume outside of that. */
    kCkAttenuationMode_None = 0,

    /** Linear attenuation of volume between near distance and far distance. */
    kCkAttenuationMode_Linear,

    /** Attenuation proportional to the reciprocal of the distance. */
    kCkAttenuationMode_InvDistance,

    /** Attenuation proportional to the square of the reciprocal of the distance. 
     This is a good choice for realistically simulating point sound sources. */
    kCkAttenuationMode_InvDistanceSquared,

} CkAttenuationMode;

/** @} */

