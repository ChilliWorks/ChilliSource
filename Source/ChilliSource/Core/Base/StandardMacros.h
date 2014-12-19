//
//  StandardMacros.h
//  Chilli Source
//  Created by Ian Copland on 25/02/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_BASE_STANDARDMACROS_H_
#define _CHILLISOURCE_CORE_BASE_STANDARDMACROS_H_

//------------------------------------------------------------
/// Memory macros
//------------------------------------------------------------
#define CS_SAFEDELETE(in_object)        {if(in_object) delete(in_object); in_object = nullptr;}
#define CS_SAFEDELETE_ARRAY(in_object)  {if(in_object) delete[] (in_object); in_object = nullptr;}
//------------------------------------------------------------
/// Logging macros
//------------------------------------------------------------
#define CS_LOG_VERBOSE(in_message)      (CSCore::Logging::Get()->LogVerbose(in_message))
#define CS_LOG_WARNING(in_message)      (CSCore::Logging::Get()->LogWarning(in_message))
#define CS_LOG_ERROR(in_message)        (CSCore::Logging::Get()->LogError(in_message))
#define CS_LOG_FATAL(in_message)        (CSCore::Logging::Get()->LogFatal(in_message))
//------------------------------------------------------------
/// Assertion macros
//------------------------------------------------------------
#ifdef CS_ENABLE_DEBUG
#define CS_ASSERT(in_query, in_message) if((in_query) == 0){CSCore::Logging::Get()->LogFatal(in_message);}
#else
#define CS_ASSERT(in_query, in_message)
#endif
//------------------------------------------------------------
/// Casting macros
//------------------------------------------------------------
#ifdef CS_ENABLE_DEBUG
#define CS_SMARTCAST(in_to, in_from, in_failureMessage) dynamic_cast<in_to>(in_from); CS_ASSERT(dynamic_cast<in_to>(in_from) != nullptr, in_failureMessage);
#else
#define CS_SMARTCAST(in_to, in_from, in_failureMessage) static_cast<in_to>(in_from);
#endif
//------------------------------------------------------------
/// Forward declaration macros.
//------------------------------------------------------------
#define CS_FORWARDDECLARE_CLASS(in_className)							\
	class in_className;													\
    using in_className##UPtr = std::unique_ptr<in_className>;           \
    using in_className##SPtr = std::shared_ptr<in_className>;           \
    using in_className##WPtr = std::weak_ptr<in_className>;             \
    using in_className##CUPtr = std::unique_ptr<const in_className>;    \
    using in_className##CSPtr = std::shared_ptr<const in_className>;    \
    using in_className##CWPtr = std::weak_ptr<const in_className>;

#define CS_FORWARDDECLARE_STRUCT(in_structName)							\
    struct in_structName;                                               \
    using in_structName##UPtr = std::unique_ptr<in_structName>;         \
    using in_structName##SPtr = std::shared_ptr<in_structName>;         \
    using in_structName##WPtr = std::weak_ptr<in_structName>;           \
    using in_structName##CUPtr = std::unique_ptr<const in_structName>;  \
    using in_structName##CSPtr = std::shared_ptr<const in_structName>;  \
    using in_structName##CWPtr = std::weak_ptr<const in_structName>;
//------------------------------------------------------------
/// A macro for declaring a class non copyable. This is acheived
/// by deleting the default copy and assignment constructor.
/// If this is used the default constructor must manually be
/// specified.
//------------------------------------------------------------
#define CS_DECLARE_NOCOPY(in_className)                            \
    in_className(const in_className& in_toCopy) = delete;               \
    in_className& operator=(const in_className& in_toCopy) = delete;
//------------------------------------------------------------
/// Standard input and output macros. As windows has its own
/// "safe" versions of some standard io functions, these macros 
/// should be used to ensure the correct version is used for the 
/// current platform.
//------------------------------------------------------------
#ifdef CS_TARGETPLATFORM_WINDOWS
#	define CS_SSCANF sscanf_s
#	define CS_SPRINTF sprintf_s
#else
#	define CS_SSCANF sscanf
#	define CS_SPRINTF sprintf
#endif

#endif
