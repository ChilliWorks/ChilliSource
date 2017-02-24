// Copyright 2016 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkEffectType CkEffectType */
/** @{ */

#pragma once
#include "ck/platform.h"

/** Types of built-in effects available */
typedef enum 
{
    /** Biquadratic filter. 
      See kCkBiquadFilterParam values for parameter IDs. */
    kCkEffectType_BiquadFilter,

    /** Bit Crusher effect reduces bit resolution and/or bit rate, producing a retro
      low-fi 8-bit sound.
      See kCkBitCrusherParam values for parameter IDs. */
    kCkEffectType_BitCrusher,

    /** Ring Mod effect modulates the audio by a sine wave. 
      See kCkRingModParam values for parameter IDs. */
    kCkEffectType_RingMod,

    /** Distortion effect.
      See kCkDistortionParam values for parameter IDs. */
    kCkEffectType_Distortion,

} CkEffectType;


/** @} */
