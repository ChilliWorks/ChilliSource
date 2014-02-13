/** 
 * File: XMLUtils.cpp
 * Date: 15/11/2010
 * Description:
 */

/** 
 *
 * Author: Stuart McGaw 
 * Version: 1.0 - MoFlow
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <moFlo/Core/XML/XMLUtils.h>

namespace moFlo
{
	namespace Core
	{
		bool XMLUtils::GetAttributeValue(TiXmlElement* inpElement, const std::string & inAttributeName, std::string & inAttributeValue)
		{
			const char * pAttribVal = inpElement->Attribute(inAttributeName.c_str());
			
			if (pAttribVal != NULL) {
				inAttributeValue = std::string(pAttribVal);
				return true;
			}
			return false;
		}
		
		TiXmlElement* XMLUtils::FirstChildElementWithName(TiXmlElement* inpParentElement, const std::string & inName){
			
			TiXmlElement * pChild = inpParentElement->FirstChildElement();
			
			while (pChild != NULL) {
				
				if (pChild->ValueStr() == inName) {
					return pChild;
				}
				
				pChild = pChild->NextSiblingElement();
			}
			
			return NULL;
		}
		
		TiXmlElement* XMLUtils::NextSiblingElementWithName(TiXmlElement* inpPreviousElement){
			TiXmlElement * pSibling = inpPreviousElement->NextSiblingElement();
			
			while (pSibling != NULL) {
				
				if (pSibling->ValueStr() == inpPreviousElement->ValueStr()) {
					return pSibling;
				}
				
				pSibling = pSibling->NextSiblingElement();
			}
			
			return NULL;	
		}
		
		///Template specialisations
		template <> const std::string XMLUtils::GetAttributeValueOrDefault<std::string>(TiXmlElement* inpElement, const std::string & inAttributeName, const std::string & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return Result;
			} else {
				return inDefault;
			}
		}
		template <> const f32 XMLUtils::GetAttributeValueOrDefault<f32>(TiXmlElement* inpElement, const std::string & inAttributeName, const f32 & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return CStringConverter::ParseFloat(Result);
			} else {
				return inDefault;
			}
		}
		template <> const s32 XMLUtils::GetAttributeValueOrDefault<s32>(TiXmlElement* inpElement, const std::string & inAttributeName, const s32 & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return CStringConverter::ParseInt(Result);
			} else {
				return inDefault;
			}
		}
		
		template <> const u32 XMLUtils::GetAttributeValueOrDefault<u32>(TiXmlElement* inpElement, const std::string & inAttributeName, const u32 & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return CStringConverter::ParseUnsignedInt(Result);
			} else {
				return inDefault;
			}
		}
		
		template <> const s64 XMLUtils::GetAttributeValueOrDefault<s64>(TiXmlElement* inpElement, const std::string & inAttributeName, const s64 & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return CStringConverter::ParseLong(Result);
			} else {
				return inDefault;
			}
		}
		
		template <> const u64 XMLUtils::GetAttributeValueOrDefault<u64>(TiXmlElement* inpElement, const std::string & inAttributeName, const u64 & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return CStringConverter::ParseUnsignedLong(Result);
			} else {
				return inDefault;
			}
		}
		
		template <> const bool XMLUtils::GetAttributeValueOrDefault<bool>(TiXmlElement* inpElement, const std::string & inAttributeName, const bool & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return CStringConverter::ParseBool(Result);
			} else {
				return inDefault;
			}
		}
		
		template <> const CVector2 XMLUtils::GetAttributeValueOrDefault<CVector2>(TiXmlElement* inpElement, const std::string & inAttributeName, const CVector2 & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return CStringConverter::ParseVector2(Result);
			} else {
				return inDefault;
			}
		}
		
		template <> const CVector3 XMLUtils::GetAttributeValueOrDefault<CVector3>(TiXmlElement* inpElement, const std::string & inAttributeName, const CVector3 & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return CStringConverter::ParseVector3(Result);
			} else {
				return inDefault;
			}
		}
		template <> const CVector4 XMLUtils::GetAttributeValueOrDefault<CVector4>(TiXmlElement* inpElement, const std::string & inAttributeName, const CVector4 & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return CStringConverter::ParseVector4(Result);
			} else {
				return inDefault;
			}
		}
		template <> const CMatrix4x4 XMLUtils::GetAttributeValueOrDefault<CMatrix4x4>(TiXmlElement* inpElement, const std::string & inAttributeName, const CMatrix4x4 & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return CStringConverter::ParseMatrix4(Result);
			} else {
				return inDefault;
			}
		}
		template <> const CQuaternion XMLUtils::GetAttributeValueOrDefault<CQuaternion>(TiXmlElement* inpElement, const std::string & inAttributeName, const CQuaternion & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return CStringConverter::ParseQuaternion(Result);
			} else {
				return inDefault;
			}
		}
		template <> const CColour XMLUtils::GetAttributeValueOrDefault<CColour>(TiXmlElement* inpElement, const std::string & inAttributeName, const CColour & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return CStringConverter::ParseColourValue(Result);
			} else {
				return inDefault;
			}
		}
	}
}