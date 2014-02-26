/*
 *  ParamDictionary.h
 *  SceneLoader
 *
 *  Created by Stuart McGaw on 15/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_CORE_PARAMDICTIONARY_H_
#define _MOFLO_CORE_PARAMDICTIONARY_H_

class TiXmlElement;

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/XML/rapidxml_forward.h>

#include <unordered_map>

namespace ChilliSource
{
	namespace Core 
    {
		typedef std::unordered_map<std::string,std::string> StringToStringMap;
		
		struct ParamDictionary : public StringToStringMap
        {
		public:
			//Returns true and assigns to outValue if key exists, leaves outvalue alone and returns false
			bool TryGetValue(const std::string & inKey, std::string & outValue) const;
			bool HasValue(const std::string & inKey) const;
			const std::string & ValueForKey(const std::string & inKey) const;
            
            void SetValueForKey(const std::string& instrKey, const std::string& instrValue);
            
			void ToXml(TiXmlElement * inpEl) const;
			void FromXml(TiXmlElement * inpEl);

            void ToXml(rapidxml::xml_node<char> * inpEl) const;
            void FromXml(rapidxml::xml_node<char> * inpEl);

			/*Fills the param dictionary based on config file style string
				the string
				blah
				cat = true
				ninja = 7
				myname = steve
				would give a param dictionary with the following value pairs
				blah = true (any key without a value gets an implict true value - treating it as a key)

			*/
			std::string ToString();
			void FromString(const std::string & inString);
		};
	}
}

#endif