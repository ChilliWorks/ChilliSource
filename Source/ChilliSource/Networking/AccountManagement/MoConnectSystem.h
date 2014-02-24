/*
 *  MoConnectSystem.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 23/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_NETWORKING_MOCONNECTSYSTEM_H_
#define _MOFLOW_NETWORKING_MOCONNECTSYSTEM_H_

#include <vector>

#include <ChilliSource/Core/Event/GenericEvent.h>
#include <ChilliSource/Core/JSON/json.h>
#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Networking/ForwardDeclarations.h>
#include <ChilliSource/Networking/Http/HttpConnectionSystem.h>
#include <ChilliSource/Networking/IAP/IAPSystem.h>
#include <ChilliSource/Core/Cryptographic/OAuthSystem.h>

namespace ChilliSource
{
	namespace Networking
    {
		class CMoConnectSystem : public Core::ISystem
        {
		public:
            //Credentials
			static const std::string kstrFacebookLoginType;
			static const std::string kstrEmailLoginType;
            
			CMoConnectSystem(IHttpConnectionSystem * inpHttpSystem, const std::string& instrMoConnectServerURL, COAuthSystem * inpOAuthSystem);
			
			DECLARE_NAMED_INTERFACE(CMoConnectSystem);
			virtual bool IsA(Core::InterfaceIDType inInterfaceID) const;
			
            COAuthSystem* GetOAuthSystem();
            
			bool HasSignedInUser() const;
			const std::string& GetCurrentUserID() const;
			const std::string& GetCurrentUserName() const;
            
			// Note this doesn't do anything server side.
            // This is a fudgy thing to get around the
            // account/setusername call not being wrapped.
			void SetCurrentUserName(const std::string& instrName);
			
			bool TryRestoreUserDetails();
			void SaveUserDetails(bool inbAnonymous);
			void ForgetSavedUserDetails(bool inbAnonymous);
            
			void SignOutCurrentUser();
			
			typedef fastdelegate::FastDelegate1<TimeIntervalSecs> ServerTimeDelegate;
			void GetServerTime(const ServerTimeDelegate& inDelegate);
			
			enum class AccountCreateResult
            {
                k_success,
                k_noServerResponse,
                k_serverRefuses
			};
			typedef fastdelegate::FastDelegate2<CMoConnectSystem*,AccountCreateResult> AccountCreateDelegate;
			void CreateNewAccount(AccountCreateDelegate inDel);
			
			enum class RegisterLoginResult
            {
				k_success,
				k_noServerResponse,              // Communication error/server tits up
				k_credentialAlreadyUsed = 1002,  // Credentials already in use
				k_authFailed = 1003,             // moConnect details are wrong
				k_unknownCredentialType = 1004,  // moConnect doesn't know or accept this kind of credentials
				k_typeAlreadyUsed = 1005,        // This account already has credentials of this type registered
				k_invalidForm = 2000,            // The form is improperly formatted
				k_invalidType = 2001,            // A param in the form is invalidly formatted
			};
			typedef fastdelegate::FastDelegate2<CMoConnectSystem*,RegisterLoginResult> RegisterLoginDelegate;
			//-----------------------------------------------------------
			/// Register Login Email
			///
			/// Will try and link the current moConnect account to a set
            /// of email / password.
			///
            /// @param String ID
            /// @param String Password
            /// @param Delegate
			//-----------------------------------------------------------
			void RegisterLoginEmail(const std::string& instrID, const std::string& instrPassword, const RegisterLoginDelegate& inDel);
			//-----------------------------------------------------------
			/// Register Login Facebook
			///
			/// Will try and link the current moConnect account to a
            /// facebook account.
			///
            /// @param String Access Token received when logged in facebook
            /// @param Delegate
			//-----------------------------------------------------------
			void RegisterLoginFacebook(const std::string& instrAccessToken, const RegisterLoginDelegate& inDel);
			
			enum class SignInResult
            {
				k_success,
				k_noServerResponse,
				k_invalidCredential = 1002,        //moConnect doesn't know or accept this kind of credentials
				k_credentialNotFound = 1003,       //moConnect doesn't know or accept this kind of credentials
				k_noAppFoundForCredential = 1006   //moConnect found the credentials however they haven't been used with this Application yet.
			};
            struct SignedInUser
            {
                std::string strUserID;
                std::string strUserName;
                std::string strRealm;
                std::string strToken;
                std::string strTokenSecret;
            };
			typedef fastdelegate::FastDelegate3<CMoConnectSystem*, SignInResult, const DYNAMIC_ARRAY<SignedInUser>& > SignInDelegate;
			//-----------------------------------------------------------
			/// Sign In Via Email
			///
			/// Will try to retrieve a moConnect account from an email address
            /// and password.
			///
            /// @param String ID
            /// @param String Password
            /// @param Delegate
            /// @param Bool whether we only want to get the list of accounts
            /// associated with this external login (true) or to try to sign in too (false)
			//-----------------------------------------------------------
			void SignInViaEmail(const std::string& instrID, const std::string& instrPassword, const SignInDelegate& inDel, bool inbRetrieveAccountsOnly = false);
			//-----------------------------------------------------------
			/// Sign In Via Email
			///
			/// Will try to retrieve a moConnect account from an email address
            /// and password.
			///
            /// @param String Access Token received when logged in facebook
            /// @param Delegate
            /// @param Bool whether we only want to get the list of accounts
            /// associated with this external login (true) or to try to sign in too (false)
			//-----------------------------------------------------------
			void SignInViaFacebook(const std::string& instrAccessToken, const SignInDelegate& inDel, bool inbRetrieveAccountsOnly = false);
            
			enum class RequestResult
            {
                k_success,
                k_cancelled,
                k_failedNoResponse,
                k_failedInternalServerError,
                k_failedClientError
			};
			typedef fastdelegate::FastDelegate3<u32, RequestResult, Json::Value&> RequestResultDelegate;
			u32 MakeRequest(const std::string& instrMethod, const RequestResultDelegate& inDelegate);
			u32 MakeRequest(const std::string& instrMethod, Json::Value& incPayload, const RequestResultDelegate& inDelegate);
			void CancelRequest(u32 inudwID);
			void CancelAllRequests();
            
            enum class PushNotificationType
            {
                k_appleAPNS,
                k_googleGCM,
                k_amazonADM,
                k_undefined
            };
            enum class PushNotificationResult
            {
                k_success,
                k_failed
            };
            typedef fastdelegate::FastDelegate1<const PushNotificationResult> PushNotificationResultDelegate;
            void RegisterForPushNotifications(const PushNotificationType ineType, const std::string& instrToken,
                                              const std::string& instrLanguage, const std::string& instrCountryCode, const PushNotificationResultDelegate& inDelegate);
			
			typedef fastdelegate::FastDelegate1<CMoConnectSystem*> EventDelegate;
			IEvent<EventDelegate>& SignedInUserChangesEvent();
			
            bool HasLoadedLoginTypes();
			bool CurrentAccountHasLogin(const std::string& instrType);
            
            struct IAPReceipt
            {
                std::string strRecordId;
                std::string strUserId;
                std::string strService;
                std::string strReceiptId;
                TimeIntervalSecs ddwTimeCreated;
                std::string strProductId;
                std::string strStatus;
                bool bRedeemed;
            };
            enum class IAPType
            {
                k_apple,
                k_google,
                k_undefined
            };
            
            typedef fastdelegate::FastDelegate3<const bool, const IHttpRequest::CompletionResult, const IAPReceipt> ValidateReceiptDelegate;
            void ValidateIAPReceipt(const IAPType ineType,
                                    const ChilliSource::Networking::IAPTransactionPtr& inpTransInfo,
                                    ValidateReceiptDelegate inDelegate);
            void RedeemIAP(const std::string& instrReceiptId);
            
			typedef fastdelegate::FastDelegate2<CMoConnectSystem*, const Json::Value&> LocalUserProfileDelegate;
            
			//-----------------------------------------------------------
			/// Request Local User Profile
			///
			/// Request the current moConnect account's profile.
			///
            /// @param Delegate
			//-----------------------------------------------------------
            void RequestLocalUserProfile(const LocalUserProfileDelegate& ineDelegate);
			//-----------------------------------------------------------
			/// Request Account Abandonment
			///
			/// Request the current moConnect account to flag the app from which
            /// this call is made as abandonned.
            ///
            /// @param Struct SignedInUser carrying which moConnect account to abandon.
            /// SignedInUser data can be retrieved from SignIn[...]() functions with
            /// inbRetrieveAccountsOnly flag on.
			//-----------------------------------------------------------
            void RequestAccountAbandonment(const SignedInUser& insSignedUser = SignedInUser());
            
        private:
            void SignIn(const Json::Value& injData, const SignInDelegate& inDel, bool inbGetAccountsOnly);
			void RegisterLogin(const Json::Value& injData, RegisterLoginDelegate inDel);
            
            void GenerateAuthenticationHeader(const std::string& instrURL, Core::ParamDictionary& outsHeader) const;
            
			void AccountCreateRequestCompletes(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult);
			AccountCreateDelegate mAccountCreateCallback;
            
			void RegisterLoginRequestCompletes(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult);
			RegisterLoginDelegate mRegisterLoginCallback;
            
			void SignInRequestCompletes(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult);
			SignInDelegate mSignInCallback;
            
			void RetrieveAccountsRequestCompletes(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult);
            SignInDelegate mRetrieveAccountsCallback;
            
            const std::string& GetPushNotificationTypeAsString(const PushNotificationType ineType);
            void PushNotificationRequestCompletes(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult);
			PushNotificationResultDelegate mPushNotificationCallback;
			
			void TimeRequestCompletes(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult);
			ServerTimeDelegate mTimeRequestCallback;
            
			void LoginsRequestCompletes(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult);
			
			void GeneralRequestCompletes(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult);
			
			void HandleRedirection(HttpRequestPtr inpRequest);
			
			void HandleSetNameResponse(const Json::Value& incValue);
			
			void OnUserChanged();
            
            void OnLocalUserProfileReceived(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult);
			
            const std::string& GetIAPTypeAsString(const IAPType ineType);
            void OnIAPRecieptValidationResponse(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult);
            void OnIAPRedeemedResponse(HttpRequestPtr inpRequest, IHttpRequest::CompletionResult ineResult);
            ValidateReceiptDelegate mValidateReceiptDelegate;
            
			std::string mstrMoConnectURL;
			
			bool mbHasSignedInUser;
			std::string mstrUserID;
			std::string mstrUserName;
			std::string mstrRealm;
            
            std::string mstrOAuthToken;
            std::string mstrOAuthTokenSecret;
			
			bool mbHasLoadedLoginTypes;
			DYNAMIC_ARRAY<std::string> mastrCurrentAccountLogins;
			HttpRequestPtr mpPendingLoginsRequest;
            
			CEvent1<EventDelegate> mSignedInUserChangesEvent;
			IHttpConnectionSystem* mpHttpConnectionSystem;
            COAuthSystem*  mpOAuthSystem;
            LocalUserProfileDelegate mLocalUserProfileDelegate;
			
			struct RequestInfo
            {
				u32 udwID;
				std::string strMethod;
				Json::Value cPayload;
				HttpRequestPtr pHttpRequest;
				RequestResultDelegate Callback;
				bool operator == (const RequestInfo& other)
                {
					return udwID == other.udwID;
				}
            };
                
            RequestInfo* FindRequestWithID(u32 inudwID);
            RequestInfo* FindRequestWithHttpRequest(HttpRequestPtr inpHttp);
                
            bool mbNoRemoveFulfilledRequests;
            DYNAMIC_ARRAY<RequestInfo> masOpenRequests;
            u32 mudwRequestIDSeed;
            
        };
    }
}
    
#endif