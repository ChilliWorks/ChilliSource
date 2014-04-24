//
//  MoContentDownloader.cpp
//  MoFlow
//
//  Created by Scott Downie on 23/01/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Networking/ContentDownload/MoContentDownloader.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Device.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/JSON/json.h>

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
                JDeviceData["Locale"] = device->GetLocale().GetCountryCode();
                JDeviceData["Language"] = device->GetLanguage().GetLocaleCode();
                
                //The server uses the tags to determine which content to serve
                Json::Value JTags(Json::arrayValue);
                for(std::vector<std::string>::const_iterator it = mastrTags.begin(); it != mastrTags.end(); ++it)
                {
                    JTags.append(*it);
                }
                JDeviceData["Tags"] = JTags;
                
                Json::FastWriter JWriter;
                
                HttpRequest::Desc requestDesc;
				requestDesc.m_url = mstrAssetServerURL;
				requestDesc.m_type = HttpRequest::Type::k_post;
                requestDesc.m_body = JWriter.write(JDeviceData);
                
                mpHttpRequestSystem->MakeRequest(requestDesc, Core::MakeDelegate(this, &MoContentDownloader::OnContentManifestDownloadComplete));
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
            
            HttpRequest::Desc requestDesc;
            requestDesc.m_url = instrURL;
            requestDesc.m_type = HttpRequest::Type::k_get;
            mpCurrentRequest = mpHttpRequestSystem->MakeRequest(requestDesc, Core::MakeDelegate(this, &MoContentDownloader::OnContentDownloadComplete));
        }
        //----------------------------------------------------------------
        /// On Content Manifest Download Complete
        ///
        /// Triggered when the manifest download has completed
        ///
        /// @param Request response
        /// @param Request result
        //----------------------------------------------------------------
        void MoContentDownloader::OnContentManifestDownloadComplete(HttpRequest* inpRequest, HttpRequest::Result ineResult)
        {
            switch(ineResult)
            {
                case HttpRequest::Result::k_completed:
                {
                    //Check the response code for errors
                    switch(inpRequest->GetResponseCode())
                    {
                        default:   //OK 
                        case HttpResponseCode::k_ok:
                        {
                            mOnContentManifestDownloadCompleteDelegate(Result::k_succeeded, inpRequest->GetResponse());
                            break;
                        }
                        case HttpResponseCode::k_error:      //Error
                        case HttpResponseCode::k_busy:       //Temporary error try again later
                        case HttpResponseCode::k_notFound:   //End point doesn't exist
                        {
                            mOnContentManifestDownloadCompleteDelegate(Result::k_failed, inpRequest->GetResponse());
                            break;
                        }
                    }
                    break;
                }
                case HttpRequest::Result::k_timeout:
                case HttpRequest::Result::k_failed:
                case HttpRequest::Result::k_cancelled:
                {
                    mOnContentManifestDownloadCompleteDelegate(Result::k_failed, inpRequest->GetResponse());
                    break;
                }
                case HttpRequest::Result::k_flushed:
                {
                    //Check the response code for errors
                    switch(inpRequest->GetResponseCode())
                    {
                        default:   //OK 
                        case HttpResponseCode::k_ok:
                        {
                            mOnContentManifestDownloadCompleteDelegate(Result::k_flushed, inpRequest->GetResponse());
                            break;
                        }
                        case HttpResponseCode::k_error:      //Error
                        case HttpResponseCode::k_busy:       //Temporary error try again later
                        case HttpResponseCode::k_notFound:   //End point doesn't exist
                        {
                            mOnContentManifestDownloadCompleteDelegate(Result::k_failed, inpRequest->GetResponse());
                            break;
                        }
                    }
                    break;
                }
            }
        }
        //----------------------------------------------------------------
        /// On Content Download Complete
        ///
        /// Triggered when a package download has completed
        ///
        /// @param Request response
        /// @param Request result
        //----------------------------------------------------------------
        void MoContentDownloader::OnContentDownloadComplete(HttpRequest* inpRequest, HttpRequest::Result ineResult)
        {
            if(mpCurrentRequest == inpRequest)
                mpCurrentRequest = nullptr;
            
            switch(ineResult)
            {
                case HttpRequest::Result::k_completed:
                {
                    // Check the response code for errors
                    switch(inpRequest->GetResponseCode())
                    {
                        default:   //OK
                        case HttpResponseCode::k_ok:
                        {
                            mOnContentDownloadCompleteDelegate(Result::k_succeeded, inpRequest->GetResponse());
                            break;
                        }
                        case HttpResponseCode::k_movedTemporarily:      // Redirected
                        case HttpResponseCode::k_redirectTemporarily:   // Redirected
                        {
                            // Redirect here
                            if(!inpRequest->GetDescription().m_redirectionUrl.empty())
                            {
                                HttpRequest::Desc requestDetails = inpRequest->GetDescription();
                                requestDetails.m_url = requestDetails.m_redirectionUrl;
                                requestDetails.m_redirectionUrl = "";
                                requestDetails.m_type = HttpRequest::Type::k_get;
                                mpCurrentRequest = mpHttpRequestSystem->MakeRequest(requestDetails, Core::MakeDelegate(this, &MoContentDownloader::OnContentDownloadComplete));
                                break;
                            }
                        }
                    }
                    break;
                }
                case HttpRequest::Result::k_timeout:
                case HttpRequest::Result::k_failed:
                case HttpRequest::Result::k_cancelled:
                {
                    mOnContentDownloadCompleteDelegate(Result::k_failed, inpRequest->GetResponse());
                    break;
                }
                case HttpRequest::Result::k_flushed:
                {
                    mOnContentDownloadCompleteDelegate(Result::k_flushed, inpRequest->GetResponse());
                    break;
                }
            }
        }
        
        //----------------------------------------------------------------
        /// Get CurrentDownloaded Bytes
        ///
        /// @return The amount of bytes read by the current request, if any
        //----------------------------------------------------------------
        u32 MoContentDownloader::GetCurrentDownloadedBytes() const
        {
            if(mpCurrentRequest)
            {
                return mpCurrentRequest->GetBytesRead();
            }
            return 0;
        }
    }
}