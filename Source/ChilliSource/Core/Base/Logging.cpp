/*
* File: Logging.cpp
* Date: 18/10/2010 2010 
* Description: 
*/

/*
* Author: Scott Downie
* Version: v 1.0
* Copyright ©2010 Tag Games Limited - All rights reserved 
*/

#include <ChilliSource/Core/Base/Logging.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/File/FileStream.h>

#include <iostream>

#ifdef CS_TARGETPLATFORM_ANDROID
#include <android/log.h>
#include <cstdlib>
extern "C"
{
    #define CS_ANDROID_LOG_DEBUG(...) __android_log_print(ANDROID_LOG_DEBUG, "MoFlow", "%s", __VA_ARGS__)
    #define CS_ANDROID_LOG_WARNING(...) __android_log_print(ANDROID_LOG_WARN, "MoFlow", "%s", __VA_ARGS__)
    #define CS_ANDROID_LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, "MoFlow", "%s", __VA_ARGS__)
}
#endif

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <Windows.h>
#endif

#ifdef CS_TARGETPLATFORM_IOS
#include <Foundation/NSThread.h>
#endif

#ifdef CS_ENABLE_DEBUG
#include <cassert>
#endif

#if CS_TARGETPLATFORM_IOS && __OBJC__
#import <Foundation/Foundation.h>
#endif

namespace ChilliSource
{
    namespace Core
    {
        bool Logging::mbInitialised = false;

#ifdef CS_ENABLE_LOGTOFILE
        const u32 kudwMaxLogBufferSize = 2048;
        const s8* kstrFileName = "MoFloLog.txt";
        
        std::string gstrLogBuffer;
        
        //----------------------------------------------
        /// Flush Buffer
        ///
        /// Write the contents of the log buffer to
        /// file
        //----------------------------------------------
        void FlushBuffer()
        {
            Core::FileStreamSPtr pLogFile = Core::Application::Get()->GetFileSystem()->CreateFileStream(Core::StorageLocation::k_cache, kstrFileName, Core::FileMode::FileMode::k_writeAppend);
            pLogFile->Write(gstrLogBuffer);
            gstrLogBuffer.clear();
            pLogFile->Close();
        }
#endif
    
        //----------------------------------------------
        /// Init
        ///
        /// Initialise the logging system
        //----------------------------------------------
        void Logging::Init()
        {
#ifdef CS_ENABLE_LOGTOFILE
            //Clear the old file
            Core::FileStreamSPtr pLogFile = Core::Application::Get()->GetFileSystem()->CreateFileStream(Core::StorageLocation::k_cache, kstrFileName, Core::FileMode::k_write);
            pLogFile->Write("MoFlow Log File");
            pLogFile->Close();
#endif
            mbInitialised = true;
        }
        //----------------------------------------------
        /// Log Verbose
        ///
        /// Used to output helpful but not neccessary 
        /// messages.
        /// @param Message
        /// Logging Level: VERBOSE
        //----------------------------------------------
        void Logging::LogVerbose(const std::string &instrMessage)
        {
#if defined CS_LOGLEVEL_VERBOSE
#ifdef CS_TARGETPLATFORM_ANDROID
            CS_ANDROID_LOG_DEBUG(instrMessage.c_str());
#elif CS_TARGETPLATFORM_IOS
            NSString* pLog = [[NSString alloc] initWithUTF8String:instrMessage.c_str()];
            NSLog(@"moFlo: %@ ", pLog);
            [pLog release];
#else
            std::cout << "moFlo: " + instrMessage << std::endl;
#endif

#ifdef CS_ENABLE_LOGTOFILE
            gstrLogBuffer += ("\nmoFlo: " + instrMessage);
            
            //Check whether to flush the buffer
            if(gstrLogBuffer.length() > kudwMaxLogBufferSize)
            {
                if (mbInitialised == true)
                    FlushBuffer();
            }
#endif
#endif
        }
        //----------------------------------------------
        /// Log Warning
        ///
        /// Used to output warning messages; that the 
        /// user may have handled (i.e. cannot find 
        /// resource provider)
        /// @param Message
        /// Logging Level: WARNING
        //----------------------------------------------
        void Logging::LogWarning(const std::string &instrMessage)
        {
#if defined CS_LOGLEVEL_VERBOSE || defined CS_LOGLEVEL_WARNING
#ifdef CS_TARGETPLATFORM_ANDROID
            CS_ANDROID_LOG_WARNING(("WARNING: " + instrMessage).c_str());
#elif CS_TARGETPLATFORM_IOS
            NSString* pLog = [[NSString alloc] initWithUTF8String:instrMessage.c_str()];
            NSLog(@"moFlo: WARNING %@ ", pLog);
            [pLog release];
#else
            std::cout << "moFlo WARNING: " + instrMessage << std::endl;
#endif

#ifdef CS_ENABLE_LOGTOFILE
            gstrLogBuffer += ("\nmoFlo WARNING: " + instrMessage);
            
            //Check whether to flush the buffer
            if(gstrLogBuffer.length() > kudwMaxLogBufferSize)
            {
                if (mbInitialised == true)
                    FlushBuffer();
            }
#endif
#endif
        }
        //----------------------------------------------
        /// Log Error
        ///
        /// Used to output error messages (i.e. trying 
        /// to create a component that the render system
        /// cannot instantiate)
        /// @param Message
        /// Logging Level: ERROR
        //----------------------------------------------
        void Logging::LogError(const std::string &instrMessage)
        {
#if defined CS_LOGLEVEL_VERBOSE || defined CS_LOGLEVEL_WARNING || defined CS_LOGLEVEL_ERROR
#ifdef CS_TARGETPLATFORM_ANDROID
            CS_ANDROID_LOG_ERROR(("ERROR: " + instrMessage).c_str());
#elif CS_TARGETPLATFORM_IOS
            NSString* pLog = [[NSString alloc] initWithUTF8String:instrMessage.c_str()];
            NSLog(@"moFlo: ERROR %@ ", pLog);
            [pLog release];
#else
            std::cout << "moFlo ERROR: " + instrMessage << std::endl;
#endif
   
#ifdef CS_ENABLE_LOGTOFILE
            gstrLogBuffer += ("\nmoFlo ERROR: " + instrMessage);
            
            //Force flush the buffer on an error
            if (mbInitialised == true)
                FlushBuffer();
            
#endif
#endif
        }
        //----------------------------------------------
        /// Log Fatal
        ///
        /// Used to output fatal messages before exiting
        /// the application
        /// @param Message
        /// Logging Level: FATAL
        //----------------------------------------------
        void Logging::LogFatal(const std::string &instrMessage)
        {
#ifdef CS_TARGETPLATFORM_ANDROID
            CS_ANDROID_LOG_ERROR(("FATAL: " + instrMessage).c_str());
            CS_ANDROID_LOG_ERROR("moFlo is exiting...");
#endif
        
#ifdef CS_TARGETPLATFORM_IOS
            NSLog(@"%@", [NSThread callStackSymbols]);

            NSString* pLog = [[NSString alloc] initWithUTF8String:instrMessage.c_str()];
            NSLog(@"moFlo: FATAL %@ ", pLog);

            NSLog(@"moFlo is exiting ...");    
            [pLog release];
#endif
        
#ifdef CS_TARGETPLATFORM_WINDOWS 
            MessageBoxA(nullptr, instrMessage.c_str(), "moFlow fatal error!", MB_OK);
#endif

#ifdef CS_ENABLE_LOGTOFILE
            gstrLogBuffer += ("\nmoFlo FATAL: " + instrMessage);
            gstrLogBuffer += ("\nmoFlo is exiting...");
            
            //Force flush the buffer on an error
            if (mbInitialised == true)
                FlushBuffer();
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
            exit(1);
#else
            assert(false);
#endif
        }
    
#ifdef CS_ENABLE_LOGTOFILE
        //----------------------------------------------
        /// Get Log Data
        ///
        /// Fetch all the log data as a string stream
        ///
        /// @param Out: Populated string stream
        //----------------------------------------------
        void Logging::GetLogData(std::stringstream& outStream)
        {
            //Clear the old file
            Core::FileStreamSPtr pLogFile = Core::Application::Get()->GetFileSystem()->CreateFileStream(Core::StorageLocation::k_cache, kstrFileName, Core::FM_READ);
            pLogFile->Get(outStream);
            outStream << gstrLogBuffer;
            pLogFile->Close();
        }
#endif
    }
}
