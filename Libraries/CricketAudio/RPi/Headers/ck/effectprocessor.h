// Copyright 2016 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkEffectProcessor CkEffectProcessor */
/** @{ */

#pragma once
#include "ck/platform.h"

/** Performs audio processing for effects.

  To implement your own custom audio effects, create a subclass, then register a factory
  function that creates an instance of it with CkEffect::registerCustomEffect(). */

class CkEffectProcessor
{
public:
    virtual ~CkEffectProcessor() {}

    /** Set an effect parameter.
      Note that this should not be called directly; it is called by CkEffect::setParam(). */
    virtual void setParam(int paramId, float value) = 0;

    /** Reset this effect's state. */
    virtual void reset() = 0;

    /** If true, the effect is processed in-place; the input and output will be the
      same buffer.  If false, input and output will be separate buffers. */
    virtual bool isInPlace() const = 0;

    /** Process audio.
      The buffer contains interleaved stereo data; each sample is a signed 8.24 fixed-point value.
      If isInPlace() returns true, inBuf and outBuf will be pointers to the same buffer. 
      It is OK to modify the data in inBuf, even if the effect is not in-place.
      (Remember that because the data is stereo, the number of samples is 2*frames.) 

      @par This will only be called on older Android armv5 or armv6 devices, or if CkConfig.sampleType is set to kCkSampleType_Fixed.
      If you are not supporting those devices, you may leave this with its default implementation, which passes the audio 
      through unchanged. */
    virtual void process(int* inBuf, int* outBuf, int frames);

    /** Process audio.
      The buffer contains interleaved stereo data; each sample is a 32-bit floating-point value.
      If isInPlace() returns true, inBuf and outBuf will be pointers to the same buffer. 
      It is OK to modify the data in inBuf, even if the effect is not in-place.
      (Remember that because the data is stereo, the number of samples is 2*frames.) */
    virtual void process(float* inBuf, float* outBuf, int frames) = 0;

    /** Get the sample rate of the audio data.  This value may be different on different
      devices, but will not change during app execution. */
    static int getSampleRate();
};


/** @} */

