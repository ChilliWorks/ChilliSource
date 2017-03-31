//
//  JsonUtils.cpp
//  ChilliSource
//  Created by Ian Copland on 24/11/2014.
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

#include <ChilliSource/Core/Json/JsonUtils.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/FileSystem.h>


#include <json/json.h>

namespace ChilliSource
{
    namespace JsonUtils
    {
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        Json::Value ParseJson(const std::string& in_jsonString) noexcept
        {
            Json::Value output;
            
            Json::Reader jsonReader;
            if (jsonReader.parse(in_jsonString, output) == false)
            {
                CS_LOG_FATAL("Could not parse json from string due to errors: \n" + jsonReader.getFormattedErrorMessages());
            }
            
            if (output.isNull())
            {
                CS_LOG_FATAL("Could not parse json from string.");
            }
            
            return output;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        bool ReadJson(StorageLocation in_storageLocation, const std::string& in_filePath, Json::Value& out_jsonValue) noexcept
        {
            auto fileStream = Application::Get()->GetFileSystem()->CreateTextInputStream(in_storageLocation, in_filePath);
            
            if (fileStream == nullptr)
            {
                CS_LOG_ERROR("Could not open json file: " + in_filePath);
                return false;
            }
            
            std::string fileContents = fileStream->ReadAll();
            fileStream.reset();
            
            Json::Reader jsonReader;
            if (jsonReader.parse(fileContents, out_jsonValue) == false)
            {
                CS_LOG_FATAL("Could not parse json file '" + in_filePath + "' due to errors: \n" + jsonReader.getFormattedErrorMessages());
            }
            
            if (out_jsonValue.isNull())
            {
                CS_LOG_FATAL("Could not parse json file: " + in_filePath);
            }
            
            return true;
        }
        
        //---------------------------------------------------------------
        bool WriteJson(StorageLocation storageLocation, const std::string& filePath, const Json::Value& jsonValue) noexcept
        {
            auto fileStream = Application::Get()->GetFileSystem()->CreateTextOutputStream(storageLocation, filePath);
            
            if (fileStream == nullptr || fileStream->IsValid() == false)
            {
                CS_LOG_ERROR("Could not open/create json file: " + filePath);
                return false;
            }
            
            fileStream->Write(jsonValue.toStyledString());

            return true;
        }
    }
}
