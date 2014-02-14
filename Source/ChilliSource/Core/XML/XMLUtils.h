/** 
 * File: XMLUtils.h
 * Date: 15/11/2010
 * Description:
 */

/** 
 *
 * Author: Stuart McGaw 
 * Version: 1.0 - MoFlow
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_CORE_XML_XMLUTILS_H_
#define _MOFLO_CORE_XML_XMLUTILS_H_

#include <ChilliSource/Core/XML/tinyxml.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/String/StringConverter.h>

namespace moFlo
{
	namespace Core
	{
		class XMLUtils
		{
		public:
			static bool GetAttributeValue(TiXmlElement* inpElement, const std::string & inAttributeName, std::string & inAttributeValue);
		
			//Default template is unspported
			template <typename T> static const T GetAttributeValueOrDefault(TiXmlElement* inpElement, const std::string & inAttributeName, const T & inDefault)
			{
				WARNING_LOG("Called GetAttributeValueOrDefault on an unsupported type");
			}
			
			static TiXmlElement* FirstChildElementWithName(TiXmlElement* inpParentElement, const std::string & inName);
			
			static TiXmlElement* NextSiblingElementWithName(TiXmlElement* inpPreviousElement);
			
		};
		
		template <> const std::string XMLUtils::GetAttributeValueOrDefault<std::string>(TiXmlElement* inpElement, const std::string & inAttributeName, const std::string & inDefault);
		template <> const f32 XMLUtils::GetAttributeValueOrDefault<f32>(TiXmlElement* inpElement, const std::string & inAttributeName, const f32 & inDefault);
		template <> const s32 XMLUtils::GetAttributeValueOrDefault<s32>(TiXmlElement* inpElement, const std::string & inAttributeName, const s32 & inDefault);
		template <> const u32 XMLUtils::GetAttributeValueOrDefault<u32>(TiXmlElement* inpElement, const std::string & inAttributeName, const u32 & inDefault);
		template <> const s64 XMLUtils::GetAttributeValueOrDefault<s64>(TiXmlElement* inpElement, const std::string & inAttributeName, const s64 & inDefault);
		template <> const u64 XMLUtils::GetAttributeValueOrDefault<u64>(TiXmlElement* inpElement, const std::string & inAttributeName, const u64 & inDefault);
		template <> const bool XMLUtils::GetAttributeValueOrDefault<bool>(TiXmlElement* inpElement, const std::string & inAttributeName, const bool & inDefault);		
		template <> const CVector2 XMLUtils::GetAttributeValueOrDefault<CVector2>(TiXmlElement* inpElement, const std::string & inAttributeName, const CVector2 & inDefault);
		template <> const CVector3 XMLUtils::GetAttributeValueOrDefault<CVector3>(TiXmlElement* inpElement, const std::string & inAttributeName, const CVector3 & inDefault);
		template <> const CVector4 XMLUtils::GetAttributeValueOrDefault<CVector4>(TiXmlElement* inpElement, const std::string & inAttributeName, const CVector4 & inDefault);
		template <> const CMatrix4x4 XMLUtils::GetAttributeValueOrDefault<CMatrix4x4>(TiXmlElement* inpElement, const std::string & inAttributeName, const CMatrix4x4 & inDefault);
		template <> const CQuaternion XMLUtils::GetAttributeValueOrDefault<CQuaternion>(TiXmlElement* inpElement, const std::string & inAttributeName, const CQuaternion & inDefault);
		template <> const CColour XMLUtils::GetAttributeValueOrDefault<CColour>(TiXmlElement* inpElement, const std::string & inAttributeName, const CColour & inDefault);
	}
}

#endif