// Copyright 2011 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkCustomStream CkCustomStream */
/** @{ */

#pragma once
#include "ck/platform.h"


/** Base class for custom streams.
  To create a custom stream sound (for example, for playing back audio in
  a proprietary file format), create a subclass, then register a factory
  function with CkSound::setCustomStreamHandler(). */

class CkCustomStream
{
public:
    virtual ~CkCustomStream() {}

    /** Initializes the stream (open data files, read file headers, etc).
    This is called from the file streaming thread, not the main thread,
    so synchronous file reading calls will not block audio or application logic. */
    virtual void init() = 0;

    /** Returns whether the stream has failed in a way that renders it
      unusable; for example, if an audio file could not be found. */
    virtual bool isFailed() const = 0;

    /** Returns the number of channels; should be either 1 or 2. */
    virtual int getChannels() const = 0;

    /** Returns the sample rate of the audio. */
    virtual int getSampleRate() const = 0;

    /** Returns the total duration of the sound, in sample frames. */
    virtual int getLength() const = 0;

    /** Reads audio into the buffer; returns the number of sample frames
      actually read. The audio data should be 16-bit signed PCM. */
    virtual int read(short* buf, int frames) = 0;

    /** Sets the current play position, in sample frames. */
    virtual void setPlayPosition(int frame) = 0;

    /** Returns the current play position, in sample frames. */
    virtual int getPlayPosition() const = 0;

};


/** @} */

