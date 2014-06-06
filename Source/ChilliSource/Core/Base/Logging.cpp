//
//  Logging.cpp
//  Chilli Source
//  Created by Scott Downie on 18/10/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

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
#define CS_ANDROID_LOG_VERBOSE(...) __android_log_print(ANDROID_LOG_DEBUG, "Chilli Source", "%s", __VA_ARGS__)
#define CS_ANDROID_LOG_WARNING(...) __android_log_print(ANDROID_LOG_WARN, "Chilli Source", "%s", __VA_ARGS__)
#define CS_ANDROID_LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, "Chilli Source", "%s", __VA_ARGS__)
}
#endif

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <CSBackend/Platform/Windows/Core/String/WindowsStringUtils.h>
#include <Windows.h>
#endif

#ifdef CS_TARGETPLATFORM_IOS
#include <Foundation/NSThread.h>
#endif

#ifdef CS_ENABLE_DEBUG
#include <cassert>
#endif

#if CS_TARGETPLATFORM_IOS
#import <Foundation/Foundation.h>
#include <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>
#endif

namespace ChilliSource
{
    namespace Core
    {
        namespace
        {
#ifdef CS_ENABLE_LOGTOFILE
            const u32 k_maxLogBufferSize = 2048;
            const std::string k_logFileName = "ChilliSourceLog.txt";
#endif
        }
        
        Logging* Logging::s_logging = nullptr;
        //-----------------------------------------------
        //-----------------------------------------------
        void Logging::Create()
        {
#ifdef CS_ENABLE_DEBUG
            assert(s_logging == nullptr);
#endif
            s_logging = new Logging();
        }
        //-----------------------------------------------
        //-----------------------------------------------
        Logging* Logging::Get()
        {
#ifdef CS_ENABLE_DEBUG
            assert(s_logging != nullptr);
#endif
            return s_logging;
        }
        //----------------------------------------------
        //----------------------------------------------
        Logging::Logging()
#ifdef CS_ENABLE_LOGTOFILE
            : m_isFirstLog(true)
#endif
        {
        }
        //----------------------------------------------
        //----------------------------------------------
        void Logging::LogVerbose(const std::string &in_message)
        {
#if defined CS_LOGLEVEL_VERBOSE
            LogMessage(LogLevel::k_verbose, in_message);
#endif
        }
        //----------------------------------------------
        //----------------------------------------------
        void Logging::LogWarning(const std::string &in_message)
        {
#if defined CS_LOGLEVEL_VERBOSE || defined CS_LOGLEVEL_WARNING
            LogMessage(LogLevel::k_warning, "WARNING: " + in_message);
#endif
        }
        //----------------------------------------------
        //----------------------------------------------
        void Logging::LogError(const std::string &in_message)
        {
#if defined CS_LOGLEVEL_VERBOSE || defined CS_LOGLEVEL_WARNING || defined CS_LOGLEVEL_ERROR
            LogMessage(LogLevel::k_error, "ERROR: " + in_message);
#endif
        }
        //----------------------------------------------
        //----------------------------------------------
        void Logging::LogFatal(const std::string &in_message)
        {
#if defined CS_LOGLEVEL_VERBOSE || defined CS_LOGLEVEL_WARNING || defined CS_LOGLEVEL_ERROR || defined CS_LOGLEVEL_FATAL
            LogMessage(LogLevel::k_error, "FATAL: " + in_message);
            LogMessage(LogLevel::k_error, "Chilli Source is exiting...");
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
            exit(1);
#else
#ifdef CS_ENABLE_DEBUG
            assert(false);
#endif
#endif
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void Logging::Destroy()
        {
            CS_SAFEDELETE(s_logging);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void Logging::LogMessage(LogLevel in_logLevel, const std::string& in_message)
        {
#ifdef CS_TARGETPLATFORM_ANDROID
            switch (in_logLevel)
            {
                case LogLevel::k_verbose:
                	CS_ANDROID_LOG_VERBOSE(in_message.c_str());
                    break;
                case LogLevel::k_warning:
                    CS_ANDROID_LOG_WARNING(in_message.c_str());
                    break;
                case LogLevel::k_error:
                    CS_ANDROID_LOG_ERROR(in_message.c_str());
                    break;
                
            }
#elif defined (CS_TARGETPLATFORM_IOS)
            NSString* message = [NSStringUtils newNSStringWithUTF8String:in_message];
            NSLog(@"[Chilli Source] %@", message);
            [message release];
#elif defined (CS_TARGETPLATFORM_WINDOWS)
			OutputDebugString(CSBackend::Windows::WindowsStringUtils::UTF8ToUTF16("[Chilli Source] " + in_message + "\n").c_str());
#endif
            
#ifdef CS_ENABLE_LOGTOFILE
            LogToFile(in_message);
#endif
        }
        
#ifdef CS_ENABLE_LOGTOFILE
        //-----------------------------------------------
        //-----------------------------------------------
        void Logging::CreateLogFile()
        {
            FileStreamUPtr stream = Application::Get()->GetFileSystem()->CreateFileStream(StorageLocation::k_cache, k_logFileName, Core::FileMode::k_write);
            stream->Write("Chilli Source Log");
            stream->Close();
        }
        //-----------------------------------------------
        //-----------------------------------------------
        void Logging::LogToFile(const std::string& in_message)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_logBuffer += "\n" + in_message;
            
            FileSystem* fileSystem = Application::Get()->GetFileSystem();
            if(fileSystem != nullptr && m_logBuffer.length() > k_maxLogBufferSize)
            {
                if (m_isFirstLog == true)
                {
                    FileStreamUPtr stream = Application::Get()->GetFileSystem()->CreateFileStream(StorageLocation::k_cache, k_logFileName, Core::FileMode::k_write);
                    stream->Write("Chilli Source Log");
                    stream->Write(m_logBuffer);
                    stream->Close();
                    m_logBuffer.clear();
                    m_isFirstLog = false;
                }
                else
                {
                    FileStreamUPtr stream = Application::Get()->GetFileSystem()->CreateFileStream(StorageLocation::k_cache, k_logFileName, Core::FileMode::k_writeAppend);
                    stream->Write(m_logBuffer);
                    stream->Close();
                    m_logBuffer.clear();
                }
            }
        }
#endif
    }
}
