// Copyright 2016 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkBank CkBank */
/** @{ */

#pragma once
#include "ck/platform.h"
#include "ck/pathtype.h"


/** A bank of memory-resident sounds. */

class CkBank
{
public:
    /** Returns true when an asynchronously loaded bank has finished loading. */
    virtual bool isLoaded() const = 0;

    /** Returns true if an asynchronously loaded bank had an error during loading. */
    virtual bool isFailed() const = 0;

    /** Returns the name of the bank. */
    virtual const char* getName() const = 0;

    /** Returns the number of sounds in the bank. */
    virtual int getNumSounds() const = 0;

    /** Returns the name of a sound in this bank.
      The name will be 31 characters or less. */
    virtual const char* getSoundName(int index) const = 0;


    ////////////////////////////////////////

    /** Loads a bank. 

      This will return NULL if the bank cannot be loaded.

      If the bank file is embedded in a larger file, specify the offset and 
      length of the embedded data; otherwise, leave them both as 0.
     */
    static CkBank* newBank(const char* path, CkPathType = kCkPathType_Default, int offset = 0, int length = 0);

    /** Loads a bank asynchronously. 

      This will return NULL if the bank cannot be loaded.
      When loading a bank asynchronously, you should also check isFailed()
      after loading to make sure no errors occurred.

      If the bank file is embedded in a larger file, specify the offset and 
      length of the embedded data; otherwise, leave them both as 0.
      */
    static CkBank* newBankAsync(const char* path, CkPathType = kCkPathType_Default, int offset = 0, int length = 0);

    /** Loads a bank from a memory buffer. 

      The buffer should contain entire bank file; you are responsible for
      freeing the memory after the bank is deleted. 

      This will return NULL if the bank cannot be loaded.
      */
    static CkBank* newBankFromMemory(void*, int bytes);


    /** Finds a loaded bank by name; returns NULL if not found. */
    static CkBank* find(const char* bankName);

    /** Destroys the bank. */
    virtual void destroy() = 0;

protected:
    CkBank() {}
    virtual ~CkBank() {}

private:
    CkBank(const CkBank&);
    CkBank& operator=(const CkBank&);
};

/** @} */
