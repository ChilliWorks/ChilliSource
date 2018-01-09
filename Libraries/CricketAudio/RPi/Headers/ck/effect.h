// Copyright 2016 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkEffect CkEffect */
/** @{ */

#pragma once
#include "ck/platform.h"
#include "ck/effecttype.h"
#include <stddef.h>

class CkEffectProcessor;


/** An audio effect. */
class CkEffect
{
public:
    /** Set an effect parameter. 
      Parameter IDs are found in ck/effectparam.h. */
    virtual void setParam(int paramId, float value) = 0;


    /** Reset the state of this effect. */
    virtual void reset() = 0;


    /** Set whether this effect is bypassed.
      A bypassed effect is not applied to the audio. */
    virtual void setBypassed(bool) = 0;

    /** Gets whether this effect is bypassed.
      A bypassed effect is not applied to the audio. */
    virtual bool isBypassed() const = 0;


    /** Sets the wet/dry ratio for this effect
      The ratio can range from 0 to 1.
      A value of 0 means the audio is left completely "dry", that is, with no
      effects applied; it is equivalent to bypassing the ffect.
      A value of 1 means the audio is completely "wet", that is, the original
      "dry" signal is not mixed back into the processed audio. */
    virtual void setWetDryRatio(float) = 0;

    /** Gets the wet/dry ratio for this effect.
      The ratio can range from 0 to 1.
      A value of 0 means the audio is left completely "dry", that is, with no
      effects applied; it is equivalent to bypassing the effect.
      A value of 1 means the audio is completely "wet", that is, the original
      "dry" signal is not mixed back into the processed audio. */
    virtual float getWetDryRatio() const = 0;


    /** Create a built-in effect. 
      Effect types are found in ck/effecttype.h. */
    static CkEffect* newEffect(CkEffectType);


    /** Factory function for creating effect processors for custom effects. */
    typedef CkEffectProcessor* (*CustomEffectFunc)(void* arg);

    /** Register a custom effect. 
     To unregister an effect, pass in NULL for the CustomEffectFunc. */
    static void registerCustomEffect(int id, CustomEffectFunc);

    /** Create a custom effect.
      A factory function must have been previously registered with the given id,
      or this will return NULL. */
    static CkEffect* newCustomEffect(int id, void* arg = NULL);


    /** Destroy the effect. */
    virtual void destroy() = 0;

protected:
    CkEffect() {}
    virtual ~CkEffect() {}

private:
    CkEffect(const CkEffect&);
    CkEffect& operator=(const CkEffect&);
};


/** @} */
