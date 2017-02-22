// Copyright 2016 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkCustomFile CkCustomFile */
/** @{ */

#pragma once
#include "ck/platform.h"
#include "ck/pathtype.h"


/** Custom file base class.  
  
  If you want to override the low-level details of how 
  files are read (for example, to read files over a network, or to decrypt encrypted
  files as they are read), create a subclass and register a handler with 
  CkSetCustomFileHandler().

  This will work for bank files, and for .cks and Ogg Vorbis streams, but not for
  other stream types such as MP3, AAC, etc. */

class CkCustomFile
{
public:
    virtual ~CkCustomFile() {}

    /** Returns true if the file was successfully opened. */
    virtual bool isValid() const = 0;

    /** Read from the file.  Returns number of bytes actually read. */
    virtual int read(void* buf, int bytes) = 0;

    /** Returns the size of the file. */
    virtual int getSize() const = 0;

    /** Sets the read position in the file. */
    virtual void setPos(int pos) = 0;

    /** Returns the read position in the file. */
    virtual int getPos() const = 0;
};


/** Factory function for opening files using a CkCustomFile instance.
  The function should return CkCustomFile instance, or NULL to open 
  the file with the default handler. */
typedef CkCustomFile* (*CkCustomFileFunc)(const char* path, void* data);

/** Set the file handler. 
  Set it to NULL to use the default handler. */
void CkSetCustomFileHandler(CkCustomFileFunc, void* data);


/** @} */


