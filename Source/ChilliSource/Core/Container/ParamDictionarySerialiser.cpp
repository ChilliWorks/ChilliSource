//
//  ParamDictionarySerialiser.cpp
//  Chilli Source
//  Created by Scott Downie on 15/05/2014.
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

#include <ChilliSource/Core/Container/ParamDictionarySerialiser.h>

#include <ChilliSource/Core/Container/ParamDictionary.h>

namespace ChilliSource
{
	namespace Core
    {
        namespace ParamDictionarySerialiser
        {
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------
            void ToXml(const ParamDictionary& in_dict, XMLUtils::Node* out_element)
            {
                out_element->name("Params");
                
                for(auto it = in_dict.begin(); it != in_dict.end(); ++it)
                {
                    XMLUtils::Attribute* keyAttribute = out_element->document()->allocate_attribute();
                    keyAttribute->name("key");
                    keyAttribute->value(it->first.c_str());
                    
                    XMLUtils::Attribute* valueAttribute = out_element->document()->allocate_attribute();
                    valueAttribute->name("value");
                    valueAttribute->value(it->second.c_str());

                    XMLUtils::Node* node = out_element->document()->allocate_node(rapidxml::node_type::node_element);
                    node->append_attribute(keyAttribute);
                    node->append_attribute(valueAttribute);
                    out_element->append_node(node);
                }
            }
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------
            ParamDictionary FromXml(const XMLUtils::Node* in_element)
            {
                ParamDictionary result;
                
                for(auto param = in_element->first_node(); param != nullptr; param = param->next_sibling())
                {
                    const char* key = nullptr;
                    const char* value = nullptr;
                    for(auto attrib = param->first_attribute(); attrib != nullptr; attrib = attrib->next_attribute())
                    {
                        if (XMLUtils::HasName(attrib, "key") == true)
                        {
                            key = attrib->value();
                        }
                        else if (XMLUtils::HasName(attrib, "value") == true)
                        {
                            value = attrib->value();
                        }
                    }
                    
                    if(key != nullptr && value != nullptr)
                    {
                        result.SetValue(key, value);
                    }
                }
                
                return result;
            }
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------
            std::string ToString(const ParamDictionary& in_dict)
            {
                std::string result;
                
                for(auto it = in_dict.begin(); it != in_dict.end(); ++it)
                {
                    result.append(it->first+"="+it->second+";");
                }
                
                return result;
            }
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------
            ParamDictionary FromString(const std::string& in_string)
            {
                ParamDictionary result;
                
                u32 start = 0;
                std::string key;
                std::string value;
                for(u32 i=0; i<in_string.size(); ++i)
                {
                    const char c = in_string[i];
                    
                    // Any of these characters
                    if(c == '\t' || c == '\n' || c == '\r' || c == '=' || c == ';'|| (c == ' ' && key.empty()))
                    {
                        if(start == i)
                        {
                            start = i + 1;
                            continue;
                        }
                        
                        if(key.empty())
                        {
                            key.assign(in_string.begin() + start, in_string.begin()+i);
                            
                            while(in_string[start] != '=' && start < in_string.size())
                                start++;
                            
                            i = start - 1;
                        }
                        else if(value.empty())
                        {
                            while(in_string[start] == ' ' && start < in_string.size())
                                start++;
                            
                            value.assign(in_string.begin() + start, in_string.begin()+i);
                            result.SetValue(key, value);
                            
                            key.clear();
                            value.clear();
                        }
                        start = i + 1;
                        continue;
                    }
                }

                return result;
            }
        }
	}
}
