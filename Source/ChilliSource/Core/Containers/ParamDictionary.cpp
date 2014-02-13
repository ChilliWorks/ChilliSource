/*
 *  ParamDictionary.cpp
 *  SceneLoader
 *
 *  Created by Stuart McGaw on 15/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/ParamDictionary.h>
#include <ChilliSource/Core/XML/XMLUtils.h>
#include <ChilliSource/Core/XML/rapidxml_utils.hpp>

namespace moFlo{
	namespace Core{
		bool ParamDictionary::TryGetValue(const std::string & inKey, std::string & outValue) const{
			const_iterator pEntry = find(inKey);
			if (pEntry != end()){
				outValue = pEntry->second;
				return true;
			}else {
				return false;
			}
		}
		bool ParamDictionary::HasValue(const std::string & inKey) const
        {
			const_iterator pEntry = find(inKey);
            
            return pEntry != end();
		}
		const std::string & ParamDictionary::ValueForKey(const std::string & inKey) const
        {
			const_iterator pEntry = find(inKey);
			if (pEntry != end()){
				return pEntry->second;
			} else {
				return CStringUtils::BLANK;
			}

		}
        void ParamDictionary::SetValueForKey(const std::string& instrKey, const std::string& instrValue)
        {
            insert(std::make_pair(instrKey, instrValue));
        }
		void ParamDictionary::ToXml(TiXmlElement * inpEl) const
        {
			inpEl->SetValue("Params");
			
			const_iterator pEntry = begin();
			
			while (pEntry != end())
            {	
				TiXmlElement Entry("String");
				Entry.SetAttribute("key", pEntry->first);
				Entry.SetAttribute("value", pEntry->second);
				inpEl->InsertEndChild(Entry);
				++pEntry;
			}
			
		}
		void ParamDictionary::FromXml(TiXmlElement * inpEl){
			clear();
			
			TiXmlElement * pDictEntry = XMLUtils::FirstChildElementWithName(inpEl, "String");
			
			while (pDictEntry) {
				
				const char * pKeyStr = pDictEntry->Attribute("key");
				const char * pValueStr = pDictEntry->Attribute("value");
				
				if (pKeyStr && pValueStr) {
					insert(std::make_pair(std::string(pKeyStr),std::string(pValueStr)));
				}
				
				pDictEntry = XMLUtils::NextSiblingElementWithName(pDictEntry);
			}
			
		}

        void ParamDictionary::ToXml(rapidxml::xml_node<char> *inpEl) const
        {
            inpEl->name("Params");
            
            const_iterator pEntry = begin();
           
			while (pEntry != end())
            {
                    rapidxml::xml_node<> * pParam = rapidxml::add_new_child(inpEl, "String");
                    rapidxml::add_new_attribute(pParam, "key", pEntry->first.c_str());
                    rapidxml::add_new_attribute(pParam, "value", pEntry->second.c_str());
                    ++pEntry;
            }
        }
        
        void ParamDictionary::FromXml(rapidxml::xml_node<char> * inpEl)
        {
            clear();
            
            for(rapidxml::xml_node<> * pParam = inpEl->first_node(); pParam != NULL; pParam =pParam->next_sibling())
            {
                const char * pKey = NULL;
                const char * pValue = NULL;
                for(rapidxml::xml_attribute<> * pAttr = pParam->first_attribute(); pAttr != NULL; pAttr =pAttr->next_attribute())
                {

                    if ( pAttr->isNamed("key") )
                        pKey = pAttr->value();
                    else if ( pAttr->isNamed("value") )
                        pValue = pAttr->value();
                    
                }
                if(pKey && pValue)
                    insert(std::make_pair(pKey, pValue));
            }
        }

		std::string ParamDictionary::ToString()
		{
			std::string strResult = std::string("");
			const_iterator pEntry = begin();
			
			while (pEntry != end())
			{
				strResult.append(pEntry->first+"="+pEntry->second+";");
				++pEntry;
			}
			
			return strResult;
		}
		
		void ParamDictionary::FromString(const std::string & inString){
			clear();
            
			//Split input string into lines
            u32 udwStart=0;
            std::string strKey;
            std::string strValue;
            for(u32 i =0; i < inString.size(); i ++)
            {
                const char c = inString[i];
                // Any of these characters
                if(c == '\t' || c == '\n' || c == '\r' || c == '=' || c == ';'|| (c == ' ' && strKey.empty()))
                {
                    if(udwStart == i)
                    {
                        udwStart = i + 1;
                        continue;
                    }
                    
                    if(strKey.empty())
                    {
                        strKey.assign(inString.begin() + udwStart, inString.begin()+i);
                        
                        while(inString[udwStart] != '=' && udwStart < inString.size())
                            udwStart++;
                        
                        i = udwStart - 1;
                    }
                    else if(strValue.empty())
                    {
                        while(inString[udwStart] == ' ' && udwStart < inString.size())
                            udwStart++;
                        
                        strValue.assign(inString.begin() + udwStart, inString.begin()+i);
                        emplace(strKey, strValue);
                        
                        strKey.clear();
                        strValue.clear();
                    }
                    udwStart = i + 1;
                    continue;
                }
            }
        }
	}
}
