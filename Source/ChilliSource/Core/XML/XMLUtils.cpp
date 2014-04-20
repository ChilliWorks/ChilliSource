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

#include <ChilliSource/Core/XML/XMLUtils.h>
#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
	namespace Core
	{
		bool XMLUtils::GetAttributeValue(TiXmlElement* inpElement, const std::string & inAttributeName, std::string & inAttributeValue)
		{
			const char * pAttribVal = inpElement->Attribute(inAttributeName.c_str());
			
			if (pAttribVal != nullptr) {
				inAttributeValue = std::string(pAttribVal);
				return true;
			}
			return false;
		}
		
		TiXmlElement* XMLUtils::FirstChildElementWithName(TiXmlElement* inpParentElement, const std::string & inName){
			
			TiXmlElement * pChild = inpParentElement->FirstChildElement();
			
			while (pChild != nullptr) {
				
				if (pChild->ValueStr() == inName) {
					return pChild;
				}
				
				pChild = pChild->NextSiblingElement();
			}
			
			return nullptr;
		}
		
		TiXmlElement* XMLUtils::NextSiblingElementWithName(TiXmlElement* inpPreviousElement){
			TiXmlElement * pSibling = inpPreviousElement->NextSiblingElement();
			
			while (pSibling != nullptr) {
				
				if (pSibling->ValueStr() == inpPreviousElement->ValueStr()) {
					return pSibling;
				}
				
				pSibling = pSibling->NextSiblingElement();
			}
			
			return nullptr;	
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
				return ParseF32(Result);
			} else {
				return inDefault;
			}
		}
		template <> const s32 XMLUtils::GetAttributeValueOrDefault<s32>(TiXmlElement* inpElement, const std::string & inAttributeName, const s32 & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return ParseS32(Result);
			} else {
				return inDefault;
			}
		}
		
		template <> const u32 XMLUtils::GetAttributeValueOrDefault<u32>(TiXmlElement* inpElement, const std::string & inAttributeName, const u32 & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return ParseU32(Result);
			} else {
				return inDefault;
			}
		}
		
		template <> const s64 XMLUtils::GetAttributeValueOrDefault<s64>(TiXmlElement* inpElement, const std::string & inAttributeName, const s64 & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return ParseS64(Result);
			} else {
				return inDefault;
			}
		}
		
		template <> const u64 XMLUtils::GetAttributeValueOrDefault<u64>(TiXmlElement* inpElement, const std::string & inAttributeName, const u64 & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return ParseU64(Result);
			} else {
				return inDefault;
			}
		}
		
		template <> const bool XMLUtils::GetAttributeValueOrDefault<bool>(TiXmlElement* inpElement, const std::string & inAttributeName, const bool & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return ParseBool(Result);
			} else {
				return inDefault;
			}
		}
		
		template <> const Vector2 XMLUtils::GetAttributeValueOrDefault<Vector2>(TiXmlElement* inpElement, const std::string & inAttributeName, const Vector2 & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return ParseVector2(Result);
			} else {
				return inDefault;
			}
		}
		
		template <> const Vector3 XMLUtils::GetAttributeValueOrDefault<Vector3>(TiXmlElement* inpElement, const std::string & inAttributeName, const Vector3 & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return ParseVector3(Result);
			} else {
				return inDefault;
			}
		}
		template <> const Vector4Old XMLUtils::GetAttributeValueOrDefault<Vector4Old>(TiXmlElement* inpElement, const std::string & inAttributeName, const Vector4Old & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return ParseVector4Old(Result);
			} else {
				return inDefault;
			}
		}
		template <> const Matrix4x4Old XMLUtils::GetAttributeValueOrDefault<Matrix4x4Old>(TiXmlElement* inpElement, const std::string & inAttributeName, const Matrix4x4Old & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return ParseMatrix4(Result);
			} else {
				return inDefault;
			}
		}
		template <> const QuaternionOld XMLUtils::GetAttributeValueOrDefault<QuaternionOld>(TiXmlElement* inpElement, const std::string & inAttributeName, const QuaternionOld & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return ParseQuaternionOld(Result);
			} else {
				return inDefault;
			}
		}
		template <> const Colour XMLUtils::GetAttributeValueOrDefault<Colour>(TiXmlElement* inpElement, const std::string & inAttributeName, const Colour & inDefault)
		{
			std::string Result;
			
			if (GetAttributeValue(inpElement, inAttributeName, Result)) {
				return ParseColour(Result);
			} else {
				return inDefault;
			}
		}
	}
}