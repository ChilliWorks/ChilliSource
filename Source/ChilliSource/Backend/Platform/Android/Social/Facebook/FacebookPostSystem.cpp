/**
 * CFacebookPostSystem.cpp
 * moFlow
 *
 * Created by Robert Henning on 03/05/2012
 * Copyright �2012 Tag Games Limited - All rights reserved
 */

#include <ChilliSource/Backend/Platform/Android/Social/Facebook/FacebookJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Social/Facebook/FacebookPostSystem.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>

namespace ChilliSource
{
	using namespace ChilliSource::Social;

	namespace Android
	{
		CFacebookPostSystem::CFacebookPostSystem(Social::IFacebookAuthenticationSystem* inpAuthSystem) : mpAuthSystem(inpAuthSystem)
		{
			mpJavaInterface = static_cast<CFacebookAuthenticationSystem*>(mpAuthSystem)->GetJavaInterface();
			mpJavaInterface->SetPostSystem(this);
		}

		bool CFacebookPostSystem::IsA(Core::InterfaceIDType inID) const
		{
			return IFacebookPostSystem::InterfaceID == inID;
		}

		void CFacebookPostSystem::TryPost(const FacebookPostDesc& insDesc, const IFacebookPostSystem::PostResultDelegate& insResultCallback)
		{
			mCompletionDelegate = insResultCallback;

            if(mpAuthSystem->IsSignedIn())
            {
                if(mpAuthSystem->HasPermission("publish_actions"))
                {
                    Post(insDesc);
                }
                else
                {
                    msPostDesc = insDesc;
                    std::vector<std::string> aWritePerms;
                    aWritePerms.push_back("publish_actions");
                    mpAuthSystem->AuthoriseWritePermissions(aWritePerms, Core::MakeDelegate(this, &CFacebookPostSystem::OnPublishPermissionAuthorised));
                }
            }
            else
            {
                CS_LOG_ERROR("Facebook Post: User must be authenticated");
            }
		}

		void CFacebookPostSystem::TrySendRequest(const Social::FacebookPostDesc& insDesc, const PostResultDelegate& insResultCallback, std::vector<std::string>& inastrRecommendedFriends)
		{
			mRequestCompleteDelegate = insResultCallback;

			if(mpAuthSystem->IsSignedIn())
			{
                if(mpAuthSystem->HasPermission("publish_actions"))
                {
                    PostRequest(insDesc);
                }
                else
                {
                    msPostDesc = insDesc;
                    std::vector<std::string> aWritePerms;
                    aWritePerms.push_back("publish_actions");
                    mpAuthSystem->AuthoriseWritePermissions(aWritePerms, Core::MakeDelegate(this, &CFacebookPostSystem::OnPublishPermissionAuthorised));
                }
            }
            else
            {
                CS_LOG_ERROR("Facebook Post: User must be authenticated");
            }
		}

		void CreateKeyValueArrayFromPostDesc(const FacebookPostDesc& insDesc, std::vector<std::string>& outaKeyValues)
		{
			outaKeyValues.push_back("link");
			outaKeyValues.push_back(insDesc.strURL);

			outaKeyValues.push_back("picture");
			outaKeyValues.push_back(insDesc.strPictureURL);

			outaKeyValues.push_back("name");
			outaKeyValues.push_back(insDesc.strName);

			outaKeyValues.push_back("caption");
			outaKeyValues.push_back(insDesc.strCaption);

			outaKeyValues.push_back("description");
			outaKeyValues.push_back(insDesc.strDescription);
        }

		void CreateKeyValueArrayFromRequestPostDesc(const FacebookPostDesc& insDesc, std::vector<std::string>& outaKeyValues)
		{
			outaKeyValues.push_back("picture");
			outaKeyValues.push_back(insDesc.strPictureURL);

			outaKeyValues.push_back("name");
			outaKeyValues.push_back(insDesc.strName);

			outaKeyValues.push_back("caption");
			outaKeyValues.push_back(insDesc.strCaption);

			outaKeyValues.push_back("message");
			outaKeyValues.push_back(insDesc.strDescription);

			outaKeyValues.push_back("to");
			outaKeyValues.push_back(insDesc.strTo);
        }

		void CFacebookPostSystem::Post(const FacebookPostDesc& insDesc)
		{
            std::string strGraphPath = "me/feed";
            if(!insDesc.strTo.empty())
            {
            	strGraphPath = insDesc.strTo + "/feed";
            }

            std::vector<std::string> aPostParamsKeyValue;
            CreateKeyValueArrayFromPostDesc(insDesc, aPostParamsKeyValue);
            mpJavaInterface->TryPostToFeed(strGraphPath, aPostParamsKeyValue);
		}

		void CFacebookPostSystem::PostRequest(const Social::FacebookPostDesc& insDesc)
		{
            std::vector<std::string> aPostParamsKeyValue;
            CreateKeyValueArrayFromRequestPostDesc(insDesc, aPostParamsKeyValue);
            mpJavaInterface->TryPostRequest(aPostParamsKeyValue);
		}

		void CFacebookPostSystem::OnPublishPermissionAuthorised(const Social::IFacebookAuthenticationSystem::AuthenticateResponse& insResponse)
		{
            switch(insResponse.eResult)
            {
                case IFacebookAuthenticationSystem::AR_SUCCESS:
                    Post(msPostDesc);
                    break;
                case IFacebookAuthenticationSystem::AR_FAILED:
                    if(mCompletionDelegate)
                    {
                        mCompletionDelegate(Social::IFacebookPostSystem::PR_FAILED);
                    }
                    break;
            }
		}

		void CFacebookPostSystem::OnPostToFeedComplete(bool inbSuccess)
		{
			if(!mCompletionDelegate)
			{
				return;
			}

			if(inbSuccess)
			{
				mCompletionDelegate(Social::IFacebookPostSystem::PR_SUCCESS);
			}
			else
			{
				mCompletionDelegate(Social::IFacebookPostSystem::PR_FAILED);
            }
		}

		void CFacebookPostSystem::OnPostRequestComplete(bool inbSuccess)
		{
			if(!mRequestCompleteDelegate)
			{
				return;
			}

			if(inbSuccess)
			{
				mRequestCompleteDelegate(Social::IFacebookPostSystem::PR_SUCCESS);
			}
			else
			{
				mRequestCompleteDelegate(Social::IFacebookPostSystem::PR_FAILED);
            }
		}
	}
}
