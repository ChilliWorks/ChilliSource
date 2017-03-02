// Copyright 2016 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkEffectBus CkEffectBus */
/** @{ */

#pragma once
#include "ck/platform.h"


class CkEffect;

/** Manages a set of audio effects applied to sounds. */
class CkEffectBus
{
public:

    /** Add an effect to this bus.
      An effect can only be on one bus at a time. */
    virtual void addEffect(CkEffect*) = 0;

    /** Remove an effect from this bus. */
    virtual void removeEffect(CkEffect*) = 0;

    /** Remove all effects from this bus. */
    virtual void removeAllEffects() = 0;


    /** Set the bus to which this bus's output is sent.
      If NULL (the default), the audio from this bus is mixed into the final output. */
    virtual void setOutputBus(CkEffectBus*) = 0;

    /** Get the bus to which this bus's output is sent.
      If NULL (the default), the audio from this bus is mixed into the final output. */
    virtual CkEffectBus* getOutputBus() = 0;


    /** Reset the state of all the effects on this bus. */
    virtual void reset() = 0;


    /** Set whether this bus is bypassed.
      A bypassed effect is not applied to the audio. */
    virtual void setBypassed(bool) = 0;

    /** Gets whether this bus is bypassed.
      Effects in a bypassed bus are not applied to the audio. */
    virtual bool isBypassed() const = 0;


    /** Sets the wet/dry ratio for this bus.
      The ratio can range from 0 to 1.
      A value of 0 means the audio is left completely "dry", that is, with no
      effects applied; it is equivalent to bypassing the bus.
      A value of 1 means the audio is completely "wet", that is, the original
      "dry" signal is not mixed back into the processed audio. */
    virtual void setWetDryRatio(float) = 0;

    /** Gets the wet/dry ratio for this bus.
      The ratio can range from 0 to 1.
      A value of 0 means the audio is left completely "dry", that is, with no
      effects applied; it is equivalent to bypassing the bus.
      A value of 1 means the audio is completely "wet", that is, the original
      "dry" signal is not mixed back into the processed audio. */
    virtual float getWetDryRatio() const = 0;

    ////////////////////////////////////////

    /** Create a new effect bus. */
    static CkEffectBus* newEffectBus();

    /** Get the global effect bus.
      Effects on this bus are applied to the final audio before it is output. */
    static CkEffectBus* getGlobalEffectBus();

    /** Destroy this effect bus. */
    virtual void destroy() = 0;

protected:
    CkEffectBus() {}
    virtual ~CkEffectBus() {}

private:
    CkEffectBus(const CkEffectBus&);
    CkEffectBus& operator=(const CkEffectBus&);

};

/** @} */

