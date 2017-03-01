// Copyright 2016 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup Ck Ck */
/** @{ */

#pragma once
#include "ck/config.h"
#include "ck/pathtype.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** Initializes Cricket Audio. Returns nonzero if successful. */
int CkInit(CkConfig*);

/** Updates Cricket Audio.
  Should be called once per frame. */
void CkUpdate();

/** Shuts down Cricket Audio. */
void CkShutdown();

/** Suspends Cricket Audio; typically used when an app is made inactive. */
void CkSuspend();

/** Resumes Cricket Audio; typically used when an app is made active after having been inactive. */
void CkResume();


/** Returns a rough estimate of the load on the audio processing thread.
  This is the fraction of the interval between audio callbacks that was actually spent
  rendering audio; it will range between 0 and 1. */
float CkGetRenderLoad();


/** Returns the value of the clip flag.
  The clip flag is set to true whenever the final audio output "clips", i.e. 
  exceeds the maximum value. */
bool CkGetClipFlag();

/** Resets the value of the clip flag to false.  
  The clip flag is set to true whenever the final audio output "clips", i.e. 
  exceeds the maximum value. */
void CkResetClipFlag();


/** Sets the maximum rate at which volumes change on playing sounds, in ms per full volume scale. 
  Default value is 40 ms. */
void CkSetVolumeRampTime(float ms);

/** Gets the maximum rate at which volumes change on playing sounds, in ms per full volume scale. */
float CkGetVolumeRampTime();

/** Default value of volume ramp time. */
extern const float Ck_volumeRampTimeMsDefault;


/** Lock the audio processing thread to prevent it from processing any API calls.
  The lock should be held only for a short time.  This is useful to ensure that
  certain calls are processed together.  For example, if you want to play multiple
  sounds together exactly in sync, call CkLockAudio(), call play() on each sound,
  then call CkUnlockAudio(). */
void CkLockAudio();

/** Unlock the audio processing thread after a call to CkLockAudio(). */
void CkUnlockAudio();


/** Start capturing the final audio output to a file.  The filename must end with ".wav" 
  (for a Microsoft WAVE file) or ".raw" (for a headerless file containing 32-bit floating-point 
  interleaved stereo samples). */
void CkStartCapture(const char* path, CkPathType pathType);

/** Stop capturing audio output. */
void CkStopCapture();


#ifdef __cplusplus
} // extern "C"
#endif

/** @} */
