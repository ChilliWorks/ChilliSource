//
//  StandardMacros.h
//  Chilli Source
//
//  Created by Ian Copland on 25/02/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_BASE_STANDARDMACROS_H_
#define _CHILLISOURCE_CORE_BASE_STANDARDMACROS_H_

//------------------------------------------------------------
/// Memory macros
//------------------------------------------------------------
#define CS_SAFEDELETE(in_object)                   {if(in_object) delete(in_object); in_object = nullptr;}
#define CS_SAFEDELETE_ARRAY(in_object)				{if(in_object) delete[] (in_object); in_object = nullptr;}
//------------------------------------------------------------
/// Logging macros
//------------------------------------------------------------
#define CS_LOG_DEBUG(in_object)						(ChilliSource::Core::Logging::LogVerbose(in_object))
#define CS_LOG_WARNING(in_object)                   (ChilliSource::Core::Logging::LogWarning(in_object))
#define CS_LOG_ERROR(in_object)						(ChilliSource::Core::Logging::LogError(in_object))
#define CS_LOG_FATAL(in_object)						(ChilliSource::Core::Logging::LogFatal(in_object))
//------------------------------------------------------------
/// Assertion macros
//------------------------------------------------------------
#ifdef CS_ENABLE_DEBUG
#define CS_ASSERT(in_query, in_message) if((in_query) == 0){ChilliSource::Core::Logging::LogFatal(in_message);}
#else
#define CS_ASSERT(in_query, in_message)
#endif
//------------------------------------------------------------
/// Casting macros
//------------------------------------------------------------
#ifdef CS_ENABLE_DEBUG
#define CS_SMARTCAST(in_to, in_from) dynamic_cast<in_to>(in_from); assert(dynamic_cast<in_to>(in_from) != nullptr);
#else
#define CS_SMARTCAST(in_to, in_from) static_cast<in_to>(in_from);
#endif
//------------------------------------------------------------
/// Forward declaration macros.
//------------------------------------------------------------
#define CS_FORWARDDECLARE_CLASS(in_className)							\
	class in_className;													\
	typedef std::unique_ptr<in_className> in_className##UPtr;           \
	typedef std::shared_ptr<in_className> in_className##SPtr;           \
	typedef std::weak_ptr<in_className> in_className##WPtr;             \
	typedef std::unique_ptr<const in_className> in_className##CUPtr;    \
	typedef std::shared_ptr<const in_className> in_className##CSPtr;    \
	typedef std::weak_ptr<const in_className> in_className##CWPtr;

#define CS_FORWARDDECLARE_STRUCT(in_structName)							\
	struct in_structName;												\
	typedef std::unique_ptr<in_structName> in_structName##UPtr;			\
	typedef std::shared_ptr<in_structName> in_structName##SPtr;			\
	typedef std::weak_ptr<in_structName> in_structName##WPtr;			\
	typedef std::unique_ptr<const in_structName> in_structName##CUPtr;	\
	typedef std::shared_ptr<const in_structName> in_structName##CSPtr;	\
	typedef std::weak_ptr<const in_structName> in_structName##CWPtr;
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
