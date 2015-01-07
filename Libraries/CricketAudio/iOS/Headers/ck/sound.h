// Copyright 2011 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkSound CkSound */
/** @{ */

#pragma once
#include "ck/platform.h"
#include "ck/pathtype.h"
#include "ck/attenuationmode.h"
#include <stddef.h>


class CkBank;
class CkMixer;
class CkEffectBus;
class CkCustomStream;

/** A sound (either memory-resident or streamed). */

class CkSound 
{
public:
    /** Sets the mixer to which this sound is assigned.
      Setting NULL will assign it to the master mixer. */
    virtual void setMixer(CkMixer*) = 0;

    /** Gets the mixer to which this sound is assigned. */
    virtual CkMixer* getMixer() = 0;


    /** Returns true if the sound is ready.
      Bank sounds are ready immediately after creation; stream sounds may not be. */
    virtual bool isReady() const = 0;


    /** Returns true if an error occurred while creating the sound. */
    virtual bool isFailed() const = 0;


    /** Play the sound. */
    virtual void play() = 0;

    /** Stop the sound. */
    virtual void stop() = 0;

    /** Gets whether the sound is playing. */
    virtual bool isPlaying() const = 0; 


    /** Sets whether the sound is paused. 
     Note that even if false, the sound will be paused if its mixer or any of its 
     mixer's ancestors are paused.*/
    virtual void setPaused(bool) = 0;

    /** Gets whether the sound is paused.
     Note that even if false, the sound will be paused if its mixer or any of its 
     mixer's ancestors are paused.*/
    virtual bool isPaused() const = 0;

    /** Returns true if the sound is pauses, its mixer is paused, or any of its
      mixer's ancestors are paused. */
    virtual bool getMixedPauseState() const = 0;


    /** Set the loop start and end.
      Default is to loop over all sample frames. 
      EndFrame is defined as one frame after the last frame of the loop.
      Setting endFrame = -1 is equivalent to setting it to getLength(). 
     @par Android note: 
      This has no effect for streams in formats other than .cks or Ogg Vorbis. */
    virtual void setLoop(int startFrame, int endFrame) = 0;

    /** Get the loop start and end.
      Default is to loop over all sample frames. 
      EndFrame is defined as one frame after the last frame of the loop.
      Setting endFrame = -1 is equivalent to setting it to getLength(). 
     @par Android note: 
      This has no effect for streams in formats other than .cks or Ogg Vorbis. */
    virtual void getLoop(int& startFrame, int& endFrame) const = 0;

    /** Sets the number of times the sound should loop.  
      0 means no looping (play once and then stop).
      1 means it will play twice; 2 means it will play 3 times; etc. 
      -1 means it will loop indefinitely until the sound is stopped or releaseLoop() is called. */
    virtual void setLoopCount(int) = 0;

    /** Gets the number of times the sound should loop.  
      0 means no looping (play once and then stop).
      1 means it will play twice; 2 means it will play 3 times; etc. 
      -1 means it will loop indefinitely until the sound is stopped or releaseLoop() is called. */
    virtual int getLoopCount() const = 0;

    /** Gets the number of the current loop.
      For example, returns 0 if this is the first time playing through this sample;
      returns 1 if it is on its second loop through the sample; etc. */
    virtual int getCurrentLoop() const = 0;

    /** Makes the current loop the last.
      This is useful when you don't know in advance how many times you will
      want to loop; set the loop count to -1, and call releaseLoop() when
      you want it to stop looping and play to the end. */
    virtual void releaseLoop() = 0;

    /** Returns true if releaseLoop() has been called after the last call to play(). */
    virtual bool isLoopReleased() const = 0;


    /** Set the play position in the sound, in sample frames.
      This can be called before playing, to start from an offset into
      the sound; or during playback, to jump to a new location. */
    virtual void setPlayPosition(int) = 0;

    /** Set the play position in the sound, in milliseconds.
      This can be called before playing, to start from an offset into
      the sound; or during playback, to jump to a new location. */
    virtual void setPlayPositionMs(float) = 0;

    /** Get the current play position in the sound, in sample frames. */
    virtual int getPlayPosition() const = 0;

    /** Get the current play position in the sound, in milliseconds. */
    virtual float getPlayPositionMs() const = 0;


    /** Sets the volume of this sound.
      The volume can range from 0 to 1. The default value is 1. 
      Note that the actual volume used for mixing will also depend on the volume of the
      mixer to which this sound is assigned. */
    virtual void setVolume(float) = 0;

    /** Gets the volume of this sound.
      The volume can range from 0 to 1. The default value is 1. 
      Note that the actual volume used for mixing will also depend on the volume of the 
      mixer to which this sound is assigned. */
    virtual float getVolume() const = 0;


    /** Gets the volume value used for mixing. 
      This is the product of the volume set with setVolume() and the mixed
      volume value of the mixer to which the sound is assigned. */
    virtual float getMixedVolume() const = 0;


    /** Sets the pan.
      The pan can range from -1 (left) to +1 (right). The default is 0 (center). */
    virtual void setPan(float) = 0;

    /** Gets the pan.
      The pan can range from -1 (left) to +1 (right). The default is 0 (center). */
    virtual float getPan() const = 0;

    /** Sets the pan matrix explicitly.  
      The pan matrix determines how the left and right channels of an input are weighted during mixing.
      Usually it is set by setPan(), but you can use setPanMatrix() if you need more control.
      For a mono input, the left channel of the output is the input scaled by ll, and the right channel of the output is the input scaled by rr.  The off-diagonal lr and rl terms are ignored.
      For a stereo input, the left channel of the output is the left channel of the input scaled by ll plus the right channel of the input scaled by lr.  The right channel of the output is the left channel of the input scaled by rl plus the right channel of the input scaled by rr.
      */
    virtual void setPanMatrix(float ll, float lr, float rl, float rr) = 0;

    /** Gets the pan matrix.
      The pan matrix determines how the left and right channels of an input are weighted during mixing.
      Usually it is set by setPan(), but you can use setPanMatrix() if you need more control.
      For a mono input, the left channel of the output is the input scaled by ll, and the right channel of the output is the input scaled by rr.  The off-diagonal lr and rl terms are ignored.
      For a stereo input, the left channel of the output is the left channel of the input scaled by ll plus the right channel of the input scaled by lr.  The right channel of the output is the left channel of the input scaled by rl plus the right channel of the input scaled by rr.
      */
    virtual void getPanMatrix(float& ll, float& lr, float& rl, float& rr) const = 0;


    /** Sets the pitch shift value, in half-steps.
      This also changes the playback speed of the sound.  
      Positive values mean higher pitch (and faster playback); lower values mean lower pitch 
      (and slower playback).  The default value is 0. 
     @par Android note: 
      This does not work for streams in formats other than .cks or Ogg Vorbis. */
    virtual void setPitchShift(float halfSteps) = 0;

    /** Gets the pitch shift value, in half-steps.
      Positive values mean higher pitch (and faster playback); lower values mean lower pitch 
      (and slower playback).  The default value is 0. 
     @par Android note: 
      This does not work for streams in formats other than .cks or Ogg Vorbis. */
    virtual float getPitchShift() const = 0;


    /** Sets the playback speed.
      Values greater than 1 mean faster playback; values less than 1 mean
      slower playback.  The default value is 1.
     @par Android note: 
      This does not work for streams in formats other than .cks or Ogg Vorbis. */
    virtual void setSpeed(float) = 0;

    /** Gets the playback speed.
      Values greater than 1 mean faster playback; values less than 1 mean
      slower playback.  The default value is 1. 
     @par Android note: 
      This does not work for streams in formats other than .cks or Ogg Vorbis. */
    virtual float getSpeed() const = 0;


    /** Sets the sound to be played when this sound finishes playing.
      The next sound will play immediately, with no gaps. 
     @par Android note:
      This does not work for streams in formats other than .cks or Ogg Vorbis. */
    virtual void setNextSound(CkSound*) = 0;

    /** Gets the sound to be played when this sound finishes playing.
      The next sound will play immediately, with no gaps.
     @par Android note:
      This does not work for streams in formats other than .cks or Ogg Vorbis. */
    virtual CkSound* getNextSound() const = 0;


    /** Gets the total duration of the sound, in sample frames.
      Stream sounds will return -1 if they are not ready or if the length is unknown. */
    virtual int getLength() const = 0;

    /** Gets the total duration of the sound, in milliseconds.
      Stream sounds will return -1 if they are not ready or if the length is unknown. */
    virtual float getLengthMs() const = 0;

    /** Gets the sample rate of the sound, in Hz.
      Stream sounds will return -1 if they are not ready. */
    virtual int getSampleRate() const = 0;

    /** Gets the number of channels in the sound (1 for mono, 2 for stereo).
      Stream sounds will return -1 if they are not ready. */
    virtual int getChannels() const = 0;


    /** Sets the effect bus to which this sound's audio is sent to be processed, or
      NULL for dry output (no effects).
     @par Android note: 
      Effects cannot be applied to streams in formats other than .cks or Ogg Vorbis. */
    virtual void setEffectBus(CkEffectBus*) = 0;

    /** Gets the effect bus to which this sound's audio is sent to be processed, or
      NULL for dry output (no effects).
     @par Android note: 
      Effects cannot be applied to streams in formats other than .cks or Ogg Vorbis. */
    virtual CkEffectBus* getEffectBus() = 0;


    /** Sets whether 3D positioning is enabled.
      If enabled, pan will be determined by the sound and listener positions,
      ignoring the value set by setPan().  Volume will be attenuated based
      on the sound and listener positions and the attenuation settings. */
    virtual void set3dEnabled(bool) = 0;

    /** Gets whether 3D positioning is enabled. 
      If enabled, pan will be determined by the sound and listener positions,
      ignoring the value set by setPan().  Volume will be attenuated based
      on the sound and listener positions and the attenuation settings. */
    virtual bool is3dEnabled() const = 0;

    /** Gets whether a 3D sound is virtual.
      If, due to distance attenuation, a 3D sound's volume becomes near enough to 0
      to be inaudible, it becomes "virtual".  A virtual sound is effectively paused
      and is not processed until it would become audible again; this reduces the 
      amount of audio processing required. */
    virtual bool isVirtual() const = 0;

    /** Sets the position of the sound emitter in 3D space.
      This is used for pan and volume calculations when 3D positioning is enabled. */
    virtual void set3dPosition(float x, float y, float z) = 0;

    /** Gets the position of the sound emitter in 3D space.
      This is used for pan and volume calculations when 3D positioning is enabled. */
    virtual void get3dPosition(float& x, float& y, float& z) const = 0;

    /** Sets the velocity of the sound emitter in 3D space.
      This is used for doppler shift calculations when 3D positioning is enabled.
      The units should be the same as those used for set3dSoundSpeed(). */
    virtual void set3dVelocity(float vx, float vy, float vz) = 0;

    /** Gets the velocity of the sound emitter in 3D space.
      This is used for doppler shift calculations when 3D positioning is enabled.
      The units should be the same as those used for set3dSoundSpeed(). */
    virtual void get3dVelocity(float& vx, float& vy, float& vz) const = 0;

    /** Sets the listener position and rotation in 3D space. 
      This is used for pan and volume calculations when 3D positioning is enabled.
      @param eyeX,eyeY,eyeZ           The listener's position
      @param lookAtX,lookAtY,lookAtZ  The listener's look-at point
      @param upX,upY,upZ              The listener's up vector
      */
    static void set3dListenerPosition(float eyeX, float eyeY, float eyeZ,
                                      float lookAtX, float lookAtY, float lookAtZ,
                                      float upX, float upY, float upZ);

    /** Gets the listener position and rotation in 3D space. 
      This is used for pan and volume calculations when 3D positioning is enabled.
      @param eyeX,eyeY,eyeZ           The listener's position
      @param lookAtX,lookAtY,lookAtZ  The listener's look-at point
      @param upX,upY,upZ              The listener's up vector
      */
    static void get3dListenerPosition(float& eyeX, float& eyeY, float& eyeZ,
                                      float& lookAtX, float& lookAtY, float& lookAtZ,
                                      float& upX, float& upY, float& upZ);

    /** Sets the listener velocity in 3D space.
      This is used for doppler shift calculations when 3D positioning is enabled. 
      The units should be the same as those used for set3dSoundSpeed(). */
    static void set3dListenerVelocity(float vx, float vy, float vz);

    /** Gets the listener velocity in 3D space.
      This is used for doppler shift calculations when 3D positioning is enabled.
      The units should be the same as those used for set3dSoundSpeed(). */
    static void get3dListenerVelocity(float& vx, float& vy, float& vz);

    /** Sets parameters used for 3D volume attenuation. 
      @param mode      The attenuation mode (default is kCkAttenuationMode_InvDistanceSquared)
      @param nearDist  The near distance; sounds closer than this distance will not be attenuated (default is 1.0)
      @param farDist   The far distance; sounds further than this will be at the far volume (default is 100.0)
      @param farVol    The far volume; sounds further than farDist will be at this volume (default is 0.0)
      */
    static void set3dAttenuation(CkAttenuationMode mode, float nearDist, float farDist, float farVol);

    /** Gets parameters used for 3D volume attenuation. 
      @param mode      The attenuation mode
      @param nearDist  The near distance; sounds closer than this distance will not be attenuated.
      @param farDist   The far distance; sounds further than this will be at the far volume.
      @param farVol    The far volume; sounds further than farDist will be at this volume.
      */
    static void get3dAttenuation(CkAttenuationMode& mode, float& nearDist, float& farDist, float& farVol);

    /** Sets the speed of sound.
      This is used for doppler shift calculations when 3D positioning is enabled.  If no doppler
      shift is desired, this can be set to 0 (which is the default value).
      You may want to use the predefined constants for the speed of sound (such as 
      k_soundSpeed_MetersPerSecond); use the constant that corresponds to
      the unit system you are using for your velocity and position values.
      You can also exaggerate the doppler effect by using a smaller value. */
    static void set3dSoundSpeed(float);

    /** Gets the speed of sound.
      This is used for doppler shift calculations when 3D positioning is enabled.  */
    static float get3dSoundSpeed();

    /** The speed of sound in dry air at 20 degrees C, in centimeters per second.
      This value is provided as a convenience for CkSound::set3dSoundSpeed(). */
    static const float k_soundSpeed_CentimetersPerSecond;

    /** The speed of sound in dry air at 20 degrees C, in meters per second.
      This value is provided as a convenience for CkSound::set3dSoundSpeed(). */
    static const float k_soundSpeed_MetersPerSecond;

    /** The speed of sound in dry air at 20 degrees C, in inches per second.
      This value is provided as a convenience for CkSound::set3dSoundSpeed(). */
    static const float k_soundSpeed_InchesPerSecond;

    /** The speed of sound in dry air at 20 degrees C, in feet per second.
      This value is provided as a convenience for CkSound::set3dSoundSpeed(). */
    static const float k_soundSpeed_FeetPerSecond;

    ////////////////////////////////////////
    // bank sounds

    /** Creates a sound from a bank by index. 
      This will return NULL if the index is invalid. */
    static CkSound* newBankSound(CkBank*, int index);

    /** Creates a sound from a bank by name. 
      If bank is NULL, all loaded banks are searched for the first matching sound. 
      This will return NULL if no sound with that name can be found. */
    static CkSound* newBankSound(CkBank*, const char* name);


    ////////////////////////////////////////
    // streams

    /** Creates a stream from a file path. 
      This will return NULL if the file could not be opened.  If a non-NULL value
      is returned, you should also check isFailed() to find out if any errors occurred
      during initialization.

      If the stream file is embedded in a larger file, specify the offset and length of 
      the embedded data in bytes, and provide a string ending with a file extension that indicates
      the file format of the embedded file (for example, ".mp3" or "music.mp3").
      Otherwise, leave the offset, length, and extension as 0.
     */
    static CkSound* newStreamSound(const char* path, CkPathType = kCkPathType_Default, int offset = 0, int length = 0, const char* extension = NULL);


    /** Factory function for creating custom streams. 
     The function should return a CkCustomStream instance, or NULL to create a stream
     with the default handler. */
    typedef CkCustomStream* (*CustomStreamFunc)(const char* path, void* data);

    /** Set the stream handler.
      Set it to NULL to use the default handler. */
    static void setCustomStreamHandler(CustomStreamFunc, void* data);



#if CK_PLATFORM_IOS
    /** Creates a stream from a URL for an asset in the iTunes library. 
      The URL should be determined using an MPMediaQuery object; see the
      "iPod Library Access Programming Guide" document in the iOS Developer Library.
      Note that this will only work for music files without DRM; for files with DRM, the URL will be returned as nil.
      @par Only available on iOS version 4.3 or later; will return NULL on earlier iOS versions.
     */
    static CkSound* newAssetStreamSound(const char* url);
#endif


    /** Destroys the sound. */
    virtual void destroy() = 0;

protected:
    CkSound() {}
    virtual ~CkSound() {}

private:
    CkSound(const CkSound&);
    CkSound& operator=(const CkSound&);
};


/** @} */
