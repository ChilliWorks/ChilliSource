// Copyright 2011 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkMixer CkMixer */
/** @{ */

#pragma once
#include "ck/platform.h"
#include <stddef.h>


/** A mixer represents a hierarchical grouping of sounds for controlling volumes. */

class CkMixer
{
public:
    /** Set the name of the mixer.
      The name should be 31 characters or less. */
    virtual void setName(const char*) = 0;

    /** Get the name of the mixer. */
    virtual const char* getName() const = 0;


    /** Set the volume of this mixer.
      (The actual volume used during mixing will also depend on ancestors.) */
    virtual void setVolume(float) = 0;

    /** Get the volume of this mixer.
      (The actual volume used during mixing will also depend on ancestors.) */
    virtual float getVolume() const = 0;


    /** Get the volume used for mixing (affected by ancestors). */
    virtual float getMixedVolume() const = 0;


    /** Set whether this mixer is paused.
      A sound will be paused if the sound, its mixer, or any of its mixer's ancestors
      are paused. */
    virtual void setPaused(bool) = 0;

    /** Get whether this mixer is paused.
      A sound will be paused if the sound, its mixer, or any of its mixer's ancestors
      are paused. */
    virtual bool isPaused() const = 0;


    /** Returns true if this mixer, or any of its ancestors, is paused. */
    virtual bool getMixedPauseState() const = 0;


    /** Set the parent of the mixer.
      Setting to NULL sets the master mixer as the parent. */
    virtual void setParent(CkMixer*) = 0;

    /** Get the parent of the mixer. */
    virtual CkMixer* getParent() = 0;


    /** Returns the master mixer, which is the root of the mixer hierarchy. */
    static CkMixer* getMaster();

    /** Creates a new mixer. */
    static CkMixer* newMixer(const char* name, CkMixer* parent = NULL);

    /** Finds a mixer by name; returns NULL if not found. */
    static CkMixer* find(const char* name);


    /** Destroys the mixer. */
    virtual void destroy() = 0;

protected:
    CkMixer() {}
    virtual ~CkMixer() {}

private:
    CkMixer(const CkMixer&);
    CkMixer& operator=(const CkMixer&);
};

/** @} */
