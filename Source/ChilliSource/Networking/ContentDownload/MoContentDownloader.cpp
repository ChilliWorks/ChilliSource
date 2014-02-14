//
//  MoContentDownloader.cpp
//  MoFlow
//
//  Created by Scott Downie on 23/01/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Networking/ContentDownload/MoContentDownloader.h>

#include <ChilliSource/Core/Main/Device.h>
#include <ChilliSource/Core/JSON/json.h>

namespace moFlo
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
        CMoContentDownloader::CMoContentDownloader(IHttpConnectionSystem* inpRequestSystem, const std::string& instrAssetServerURL, const DYNAMIC_ARRAY<std::string>& inastrTags)
        : mpHttpConnectionSystem(inpRequestSystem), mstrAssetServerURL(instrAssetServerURL), mastrTags(inastrTags)
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
        bool CMoContentDownloader::DownloadContentManifest(const ContentDownloader::Delegate& inDelegate)
        {
            mOnContentManifestDownloadCompleteDelegate = inDelegate;
            
            //Request the content manifest
            if(mpHttpConnectionSystem->CheckReachability())
            {
                //Build the JSON request with the device info so the server can decide what
                //assets are suitable for us
                Json::Value JDeviceData(Json::objectValue);
                JDeviceData["Type"] = Core::CDevice::GetManufacturerName() + Core::CDevice::GetModelName() + Core::CDevice::GetModelTypeName();
                JDeviceData["OS"] = Core::CDevice::GetOSVersion();
                JDeviceData["Locale"] = Core::CDevice::GetLocale().GetCountryCode();
                JDeviceData["Language"] = Core::CDevice::GetLanguage().GetLocaleCode();
                
                //The server uses the tags to determine which content to serve
                Json::Value JTags(Json::arrayValue);
                for(DYNAMIC_ARRAY<std::string>::const_iterator it = mastrTags.begin(); it != mastrTags.end(); ++it)
                {
                    JTags.append(*it);
                }
                JDeviceData["Tags"] = JTags;
                
                Json::FastWriter JWriter;
                
                HttpRequestDetails RequestDetails;
				RequestDetails.strURL = mstrAssetServerURL;
				RequestDetails.eType = moFlo::Networking::HttpRequestDetails::POST;
                RequestDetails.strBody = JWriter.write(JDeviceData);
                
                mpHttpConnectionSystem->MakeRequest(RequestDetails, IHttpRequest::CompletionDelegate(this, &CMoContentDownloader::OnContentManifestDownloadComplete));
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
        void CMoContentDownloader::DownloadPackage(const std::string& instrURL, const ContentDownloader::Delegate& inDelegate)
        {
            mOnContentDownloadCompleteDelegate = inDelegate;
            
            HttpRequestDetails RequestDetails;
            RequestDetails.strURL = instrURL;
            RequestDetails.eType = moFlo::Networking::HttpRequestDetails::GET;
            mpCurrentRequest = mpHttpConnectionSystem->MakeRequest(RequestDetails, IHttpRequest::CompletionDelegate(this, &CMoContentDownloader::OnContentDownloadComplete));
        }
        //----------------------------------------------------------------
        /// On Content Manifest Download Complete
        ///
        /// Triggered when the manifest download has completed
        ///
        /// @param Request response
        /// @param Request result
        //----------------------------------------------------------------
        void CMoContentDownloader::OnContentManifestDownloadComplete(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult)
        {
            switch(ineResult)
            {
                case IHttpRequest::COMPLETED:
                {
                    //Check the response code for errors
                    switch(inpRequest->GetResponseCode())
                    {
                        default:   //OK 
                        case Networking::kHTTPResponseOK:
                        {
                            mOnContentManifestDownloadCompleteDelegate(ContentDownloader::SUCCEEDED, inpRequest->GetResponseString());
                            break;
                        }
                        case Networking::kHTTPError:      //Error
                        case Networking::kHTTPBusy:       //Temporary error try again later
                        case Networking::kHTTPNotFound:   //End point doesn't exist
                        {
                            mOnContentManifestDownloadCompleteDelegate(ContentDownloader::FAILED, inpRequest->GetResponseString());
                            break;
                        }
                    }
                    break;
                }
                case IHttpRequest::TIMEOUT:
                case IHttpRequest::FAILED:
                case IHttpRequest::CANCELLED:
                {
                    mOnContentManifestDownloadCompleteDelegate(ContentDownloader::FAILED, inpRequest->GetResponseString());
                    break;
                }
                case IHttpRequest::FLUSHED:
                {
                    //Check the response code for errors
                    switch(inpRequest->GetResponseCode())
                    {
                        default:   //OK 
                        case Networking::kHTTPResponseOK:
                        {
                            mOnContentManifestDownloadCompleteDelegate(ContentDownloader::FLUSHED, inpRequest->GetResponseString());
                            break;
                        }
                        case Networking::kHTTPError:      //Error
                        case Networking::kHTTPBusy:       //Temporary error try again later
                        case Networking::kHTTPNotFound:   //End point doesn't exist
                        {
                            mOnContentManifestDownloadCompleteDelegate(ContentDownloader::FAILED, inpRequest->GetResponseString());
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
        void CMoContentDownloader::OnContentDownloadComplete(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult)
        {
            if(mpCurrentRequest == inpRequest)
                mpCurrentRequest = HttpRequestPtr();
            
            switch(ineResult)
            {
                case IHttpRequest::COMPLETED:
                {
                    // Check the response code for errors
                    switch(inpRequest->GetResponseCode())
                    {
                        default:   //OK
                        case Networking::kHTTPResponseOK:
                        {
                            mOnContentDownloadCompleteDelegate(ContentDownloader::SUCCEEDED, inpRequest->GetResponseString());
                            break;
                        }
                        case Networking::kHTTPMovedTemporarily:      // Redirected
                        case Networking::kHTTPRedirectTemporarily:   // Redirected
                        {
                            // Redirect here
                            if(!inpRequest->GetDetails().strRedirectionURL.empty())
                            {
                                HttpRequestDetails RequestDetails = inpRequest->GetDetails();
                                RequestDetails.strURL = RequestDetails.strRedirectionURL;
                                RequestDetails.strRedirectionURL = "";
                                RequestDetails.eType = moFlo::Networking::HttpRequestDetails::GET;
                                mpCurrentRequest = mpHttpConnectionSystem->MakeRequest(RequestDetails, IHttpRequest::CompletionDelegate(this, &CMoContentDownloader::OnContentDownloadComplete));
                                break;
                            }
                        }
                    }
                    break;
                }
                case IHttpRequest::TIMEOUT:
                case IHttpRequest::FAILED:
                case IHttpRequest::CANCELLED:
                {
                    mOnContentDownloadCompleteDelegate(ContentDownloader::FAILED, inpRequest->GetResponseString());
                    break;
                }
                case IHttpRequest::FLUSHED:
                {
                    mOnContentDownloadCompleteDelegate(ContentDownloader::FLUSHED, inpRequest->GetResponseString());
                    break;
                }
            }
        }
        
        //----------------------------------------------------------------
        /// Get CurrentDownloaded Bytes
        ///
        /// @return The amount of bytes read by the current request, if any
        //----------------------------------------------------------------
        u32 CMoContentDownloader::GetCurrentDownloadedBytes() const
        {
            if(mpCurrentRequest)
            {
                return mpCurrentRequest->GetBytesRead();
            }
            return 0;
        }
    }
}