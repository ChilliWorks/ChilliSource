// Copyright 2013 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkEffectParam CkEffectParam */
/** @{ */

#pragma once
#include "ck/platform.h"

/** Parameters for the Biquad Filter effect */
enum
{
    /** Type of filter; should be one of the kCkBiquadFilterParam_FilterType values. */
    kCkBiquadFilterParam_FilterType,

    /** Center frequency of the filter. */
    kCkBiquadFilterParam_Freq,

    /** Q of the filter. */
    kCkBiquadFilterParam_Q,

    /** Gain in dB (for Peak, LowShelf, and HighShelf filter types) */
    kCkBiquadFilterParam_Gain,
};


/** Filter types, used for the kCkBiquadFilterParam_FilterType param. */
enum
{
    /** Low pass filter blocks high frequencies. */
    kCkBiquadFilterParam_FilterType_LowPass,

    /** High pass filter blocks low frequencies. */
    kCkBiquadFilterParam_FilterType_HighPass,
    
    /** Band pass filter blocks frequencies above and below the center frequency. */
    kCkBiquadFilterParam_FilterType_BandPass,

    /** Notch filter blocks a narrow band of frequencies. */
    kCkBiquadFilterParam_FilterType_Notch,

    /** Peak filter boosts a narrow band of frequencies. */
    kCkBiquadFilterParam_FilterType_Peak,

    /** Low shelf filter boosts low frequencies. */
    kCkBiquadFilterParam_FilterType_LowShelf,

    /** High shelf filter boosts high frequencies. */
    kCkBiquadFilterParam_FilterType_HighShelf
};


////////////////////////////////////////


/** Parameters for the Bit Crusher effect */
enum
{
    /** Number of bits of resolution to keep in the input samples, in [1..24]. */
    kCkBitCrusherParam_BitResolution,

    /** Milliseconds to hold each sample value.  Higher values result in more
      reduction in the effective sample rate.  Values around 1 ms provide a reasonable effect. */
    kCkBitCrusherParam_HoldMs,
};


////////////////////////////////////////


/** Parameters for the Ring Mod effect */
enum
{
    /** Frequency (Hz) of the modulating signal. Values around 500 Hz are typical. */
    kCkRingModParam_Freq,
};

////////////////////////////////////////


/** Parameters for the Distortion effect */
enum
{
    /** Scale factor for input; larger values cause a more pronounced effect. */
    kCkDistortionParam_Drive,

    /** Offset for input. */
    kCkDistortionParam_Offset,
};




/** @} */


