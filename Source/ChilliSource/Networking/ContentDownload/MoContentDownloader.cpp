//
//  MoContentDownloader.cpp
//  Chilli Source
//  Created by Scott Downie on 23/01/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#include <ChilliSource/Networking/ContentDownload/MoContentDownloader.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Device.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Networking/Http/HttpResponse.h>

#include <json/json.h>

namespace ChilliSource
{
    namespace Networking
    {
        //----------------------------------------------------------------
        /// Constructor
        ///
        /// @param HTTP request system
        /// @param Asset server URL
        /// @param Dynamic array of tags that determine content
        //----------------------------------------------------------------
        MoContentDownloader::MoContentDownloader(HttpRequestSystem* inpRequestSystem, const std::string& instrAssetServerURL, const std::vector<std::string>& inastrTags)
        : mpHttpRequestSystem(inpRequestSystem), mstrAssetServerURL(instrAssetServerURL), mastrTags(inastrTags)
        {
            
        }
        //----------------------------------------------------------------
        /// Download Content Manifest
        ///
        /// Pull the .moman file from the server and callback when
        /// the download is complete
        ///
        /// @param Delegate
        /// @return Whether the manifest download has begun
        //----------------------------------------------------------------
        bool MoContentDownloader::DownloadContentManifest(const Delegate& inDelegate)
        {
            mOnContentManifestDownloadCompleteDelegate = inDelegate;
            
            //Request the content manifest
            if(mpHttpRequestSystem->CheckReachability())
            {
                Core::Device* device = Core::Application::Get()->GetSystem<Core::Device>();
                
                //Build the JSON request with the device info so the server can decide what
                //assets are suitable for us
                Json::Value JDeviceData(Json::objectValue);
                JDeviceData["Type"] = device->GetManufacturer() + device->GetModel() + device->GetModelType();
                JDeviceData["OS"] = device->GetOSVersion();
                JDeviceData["Locale"] = device->GetLocale();
                JDeviceData["Language"] = device->GetLanguage();
                
                //The server uses the tags to determine which content to serve
                Json::Value JTags(Json::arrayValue);
                for(std::vector<std::string>::const_iterator it = mastrTags.begin(); it != mastrTags.end(); ++it)
                {
                    JTags.append(*it);
                }
                JDeviceData["Tags"] = JTags;
                
                Json::FastWriter JWriter;
                mpHttpRequestSystem->MakePostRequest(mstrAssetServerURL, JWriter.write(JDeviceData), Core::MakeDelegate(this, &MoContentDownloader::OnContentManifestDownloadComplete));
                return true;
            }
            else
            {
                return false;
            }
        }
        //----------------------------------------------------------------
        /// Download Package
        ///
        /// Download the package file from the given URL
        ///
        /// @param URL string
        /// @param Delegate
        //----------------------------------------------------------------
        void MoContentDownloader::DownloadPackage(const std::string& instrURL, const Delegate& inDelegate)
        {
            mOnContentDownloadCompleteDelegate = inDelegate;
            mpCurrentRequest = mpHttpRequestSystem->MakeGetRequest(instrURL, Core::MakeDelegate(this, &MoContentDownloader::OnContentDownloadComplete));
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        void MoContentDownloader::OnContentManifestDownloadComplete(const HttpRequest* in_request, const HttpResponse& in_response)
        {
            switch(in_response.GetResult())
            {
                case HttpResponse::Result::k_completed:
                {
                    //Check the response code for errors
                    switch(in_response.GetCode())
                    {
                        default:   //OK 
                        case HttpResponseCode::k_ok:
                        {
                            mOnContentManifestDownloadCompleteDelegate(Result::k_succeeded, in_response.GetDataAsString());
                            break;
                        }
                        case HttpResponseCode::k_error:      //Error
                        case HttpResponseCode::k_unavailable://Temporary error try again later
                        case HttpResponseCode::k_notFound:   //End point doesn't exist
                        {
                            mOnContentManifestDownloadCompleteDelegate(Result::k_failed, in_response.GetDataAsString());
                            break;
                        }
                    }
                    break;
                }
                case HttpResponse::Result::k_timeout:
                case HttpResponse::Result::k_failed:
                {
                    mOnContentManifestDownloadCompleteDelegate(Result::k_failed, in_response.GetDataAsString());
                    break;
                }
                case HttpResponse::Result::k_flushed:
                {
                    //Check the response code for errors
                    switch(in_response.GetCode())
                    {
                        default:   //OK 
                        case HttpResponseCode::k_ok:
                        {
                            mOnContentManifestDownloadCompleteDelegate(Result::k_flushed, in_response.GetDataAsString());
                            break;
                        }
                        case HttpResponseCode::k_error:      //Error
                        case HttpResponseCode::k_unavailable://Temporary error try again later
                        case HttpResponseCode::k_notFound:   //End point doesn't exist
                        {
                            mOnContentManifestDownloadCompleteDelegate(Result::k_failed, in_response.GetDataAsString());
                            break;
                        }
                    }
                    break;
                }
            }
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        void MoContentDownloader::OnContentDownloadComplete(const HttpRequest* in_request, const HttpResponse& in_response)
        {
            if(mpCurrentRequest == in_request)
                mpCurrentRequest = nullptr;
            
            switch(in_response.GetResult())
            {
                case HttpResponse::Result::k_completed:
                {
                    // Check the response code for errors
                    switch(in_response.GetCode())
                    {
                        default:   //OK
                        case HttpResponseCode::k_ok:
                            mOnContentDownloadCompleteDelegate(Result::k_succeeded, in_response.GetDataAsString());
                            break;
                    }
                    break;
                }
                case HttpResponse::Result::k_timeout:
                case HttpResponse::Result::k_failed:
                {
                    mOnContentDownloadCompleteDelegate(Result::k_failed, in_response.GetDataAsString());
                    break;
                }
                case HttpResponse::Result::k_flushed:
                {
                    mOnContentDownloadCompleteDelegate(Result::k_flushed, in_response.GetDataAsString());
                    break;
                }
            }
        }
    }
}