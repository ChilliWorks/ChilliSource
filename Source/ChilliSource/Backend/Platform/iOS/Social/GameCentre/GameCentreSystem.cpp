/*
 *  GameCentre.cpp
 *  moFloTest
 *
 *  Created by Scott Downie on 01/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/iOS/Social/GameCentre/GameCentreSystem.h>
#include <ChilliSource/Backend/Platform/iOS/Social/GameCentre/GameCentreDelegates.h>

#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>
#include <GameKit/GameKit.h>
#include <ChilliSource/Backend/Platform/iOS/Social/GameCentre/GKAchievementHandler.h>

#import <sys/utsname.h>


namespace ChilliSource
{
	namespace iOS
	{
		const f32 kfGameCentreMinOSVersion          = 4.0f;
        
        const u32 kudwGameCentreRequestAttemptLimit = 5;
        const u32 kudwGameCentreMaxOpenRequests     = 25;
		
		//---Request IDs
		const GameCentreRequestID kudwGameCentrePostScoreRequestID				= 201101;
		const GameCentreRequestID kudwGameCentreGetFriendIdentifiersRequestID	= 201102;
		const GameCentreRequestID kudwGameCentreGetScoresRequestID				= 201103;
		const GameCentreRequestID kudwGameCentreGetLeaderboardsInfoRequestID	= 201104;
        const GameCentreRequestID kudwGameCentreGetNamesRequestID               = 201105;
		const GameCentreRequestID kudwGameCentreUpdateAchievementRequestID      = 201106;
		const GameCentreRequestID kudwGameCentreAchievementsStatusRequestID     = 201107;
		const GameCentreRequestID kudwGameCentreAchievementDescriptionsRequestID = 201108;
		
		DEFINE_NAMED_INTERFACE(CGameCentreSystem);
        
		//===============================================================
		/// Generic Game Centre Request
		//===============================================================
		class IGameCentreRequest
		{
		public:
            IGameCentreRequest() : mudwNumRequestAttempts(0){}
			virtual ~IGameCentreRequest(){}
			virtual bool IsA(GameCentreRequestID inTypeID) = 0;
            
            CGameCentreSystem::RequestErrorDelegate mpErrorDelegate;
            
            u32 mudwNumRequestAttempts;
            
            std::string mstrTag;
		};
		//===============================================================
		/// Post Score Game Centre Request
		//===============================================================
		class CGameCentrePostScoreRequest : public IGameCentreRequest
		{
		public:
			
			//-----------------------------------------------
			/// Is A
			///
			/// @param Type ID
			/// @return Whether the request is of given type
			//-----------------------------------------------
			bool IsA(GameCentreRequestID inTypeID) override
            {
				return inTypeID == kudwGameCentrePostScoreRequestID;
			}
			
		public:
			u64 muddwScore;
			std::string mstrLeaderboardCategoryID;
		};
		//===============================================================
		/// GameCentre UpdateAchievementRequest
		//===============================================================
		class CGameCentreUpdateAchievementRequest : public IGameCentreRequest
		{
		public:
			
			//-----------------------------------------------
			/// Is A
			///
			/// @param Type ID
			/// @return Whether the request is of given type
			//-----------------------------------------------
			bool IsA(GameCentreRequestID inTypeID) override
            {
				return inTypeID == kudwGameCentreUpdateAchievementRequestID;
			}
			
			
		public:
			bool mbAchievement;
			f64 mffProgression;
			std::string mstrAchievementID;
		};
		//===============================================================
		/// GameCentre AchievementsStatusRequest
		//===============================================================
		class CGameCentreAchievementDescriptionsRequest : public IGameCentreRequest
		{
		public:
			
			//-----------------------------------------------
			/// Is A
			///
			/// @param Type ID
			/// @return Whether the request is of given type
			//-----------------------------------------------
			bool IsA(GameCentreRequestID inTypeID) override
            {
				return inTypeID == kudwGameCentreAchievementDescriptionsRequestID;
			}
			
			
		};
		//===============================================================
		/// GameCentre AchievementsStatusRequest
		//===============================================================
		class CGameCentreAchievementsStatusRequest: public IGameCentreRequest
		{
		public:
			
			//-----------------------------------------------
			/// Is A
			///
			/// @param Type ID
			/// @return Whether the request is of given type
			//-----------------------------------------------
			bool IsA(GameCentreRequestID inTypeID) override
			{
				return inTypeID == kudwGameCentreAchievementsStatusRequestID;
			}
			
			
		};
		//===============================================================
		/// Get Scores Game Centre Request
		//===============================================================
		class CGameCentreGetScoresRequest : public IGameCentreRequest
		{
		public:
			
			//-----------------------------------------------
			/// Is A
			///
			/// @param Type ID
			/// @return Whether the request is of given type
			//-----------------------------------------------
			bool IsA(GameCentreRequestID inTypeID) override
            {
				return inTypeID == kudwGameCentreGetScoresRequestID;
			}
			
		public:
			std::string mstrLeaderboardCategoryID;
			PlayerScope mePlayerScope;
			TimeScope meTimeScope;
			u32 mudwMinRange;
			u32 mudwMaxRange;
		};
        //===============================================================
		/// Get Names Request
		//===============================================================
		class CGameCentreGetNamesRequest : public IGameCentreRequest
		{
		public:
			
			//-----------------------------------------------
			/// Is A
			///
			/// @param Type ID
			/// @return Whether the request is of given type
			//-----------------------------------------------
			bool IsA(GameCentreRequestID inTypeID) override
			{
				return inTypeID == kudwGameCentreGetNamesRequestID;
			}
			
			
			
			virtual ~CGameCentreGetNamesRequest()
			{
				[mpaPlayerIDs release];
			}
			
		public:
			NSArray* mpaPlayerIDs;
			std::string mstrKey;
		};
		//===============================================================
		/// Get Friends Game Centre Request
		//===============================================================
		class CGameCentreFriendInfoRequest : public IGameCentreRequest
		{
		public:
			
			//-----------------------------------------------
			/// Is A
			///
			/// @param Type ID
			/// @return Whether the request is of given type
			//-----------------------------------------------
			bool IsA(GameCentreRequestID inTypeID) override
            {
				return inTypeID == kudwGameCentreGetFriendIdentifiersRequestID;
			}
			
		};
		//===============================================================
		/// Get Leaderboard Info Game Centre Request
		//===============================================================
		class CGameCentreLeaderboardsInfoRequest : public IGameCentreRequest
		{
		public:
			
			//-----------------------------------------------
			/// Is A
			///
			/// @param Type ID
			/// @return Whether the request is of given type
			//-----------------------------------------------
			bool IsA(GameCentreRequestID inTypeID) override
            {
				return inTypeID == kudwGameCentreGetLeaderboardsInfoRequestID;
			}
		};
		
		//--------------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//--------------------------------------------------------------------
		CGameCentreSystem::CGameCentreSystem(bool inbUseTurnBasedMultiplayer) : mbUseTurnBasedMultiplayer(inbUseTurnBasedMultiplayer), mbProcessRequests(true), mudwNumOpenRequests(0),mpAchievementDescriptions(nil)
		{
			//Check to see whether the device supports game centre
			mbIsGameCentreSupported = IsSupported(mbUseTurnBasedMultiplayer); 
			
			if(mbIsGameCentreSupported)
			{
				//We need to register for log-out notifications
                [[GameCentreAuthenticationListener instance] BeginListeningForGKLocalAuthenticationChanged];
				[[GameCentreAuthenticationListener instance] GetGKLocalAuthenticationChangedEvent].AddListener(Core::MakeDelegate(this, &CGameCentreSystem::OnLocalAuthenticationChanged));
				
				AuthenticateClient();
				
				//Spawn a new thread to handle the request processing
				mRequestThread = std::thread(std::bind(&CGameCentreSystem::ProcessRequestQueue, this));
			}
		}
		//----------------------------------------------------------
		/// Is A
		///
		/// Returns if it is of the type given
		/// @param Comparison Type
		/// @return Whether the class matches the comparison type
		//----------------------------------------------------------
		bool CGameCentreSystem::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == CGameCentreSystem::InterfaceID;
		}
		//--------------------------------------------------------------------
		/// Get Is Client Authenticated
		///
		/// Returns true if the client is authenticated, or false if not.
		//--------------------------------------------------------------------
        bool CGameCentreSystem::GetIsClientAuthenticated() const
        {
            return mbIsGameCentreSupported;
        }
		//--------------------------------------------------------------------
		/// Authenticate Client
		///
		/// Check whether the local player is signed-in/registered and
		/// presents a user interface otherwise
		//--------------------------------------------------------------------
		void CGameCentreSystem::AuthenticateClient()
		{
            void (^setGKEventHandlerDelegate)(NSError *) = ^ (NSError *pcError)
            {
                if (pcError)
                {
                    CS_ERROR_LOG("Game Center - " + ChilliSource::Core::CStringUtils::NSStringToString([pcError localizedDescription]));
                    
                    //Handle error by disabling game centre
                    mbIsGameCentreSupported = false;
                }
                else
                {	
                    //Handle succesful authentication
                    mbIsGameCentreSupported = true;

                    if (mbUseTurnBasedMultiplayer)
                    {
                        GKTurnBasedEventHandler *pcHandler = [GKTurnBasedEventHandler sharedTurnBasedEventHandler];
                        pcHandler.delegate = [GameCentreTurnBasedEventHandlerDelegate instance];
                    }
                }                
            };
            
            if ([GKLocalPlayer localPlayer].authenticated == NO) 
            {     
                // Not authenticated yet, so authenticate
                [[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:setGKEventHandlerDelegate];        
            } 
            else 
            {
                // Already authenticated - so setup the event handler delegate
                setGKEventHandlerDelegate(nil);
            }
		}
		//--------------------------------------------------------------------
		/// Is Supported
		///
		/// @return Whether the device supports game centre
		//--------------------------------------------------------------------
		bool CGameCentreSystem::IsSupported(bool inbUseTurnBasedMultiplayer) 
		{
			//Hideously complicated test, bad Apple bad
			
			// Check for presence of GKLocalPlayer API.
			Class gcClass = (NSClassFromString(@"GKLocalPlayer"));
			// The device must be running running iOS 4.1 or later.
			NSString *reqSysVer;
            if (inbUseTurnBasedMultiplayer)
                reqSysVer = @"5.0"; // Turn-based multiplayer requires iOS 5.0
            else
                reqSysVer = @"4.1";
			NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
			bool osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
			
			if(gcClass && osVersionSupported)
			{
				struct utsname systemInfo;
				uname(&systemInfo);
				//iPhone 3G can run iOS 4.1 but does not support Game Center
				if(strcmp(systemInfo.machine, "iPhone1,2") == 0)
				{
					return false;
				}else{
					return true;
				}
			}else{
				return false;
			}
			
		}
		//--------------------------------------------------------------------
		/// Get Local Player ID
		///
		/// @return String identifier for local client
		//--------------------------------------------------------------------
		const Core::UTF8String& CGameCentreSystem::GetLocalPlayerID() const
		{
			return mstrPlayerID;
		}
		//--------------------------------------------------------------------
		/// Get Local Player Username
		///
		/// @return Username for local client
		//--------------------------------------------------------------------
		const Core::UTF8String& CGameCentreSystem::GetLocalPlayerUsername() const
		{
			return mstrPlayerName;
		}
		//--------------------------------------------------------------------
		/// ShowSystemLeaderboardUI
		///
		/// Show the system leaderboard UI
		//--------------------------------------------------------------------
		void CGameCentreSystem::ShowSystemLeaderboardUI()
		{
			UIViewController * pRootVC = [UIApplication sharedApplication].keyWindow.rootViewController;
			
			GKLeaderboardViewController * pVC = [[GKLeaderboardViewController alloc] init];
			pVC.leaderboardDelegate = [GameCentreLeaderboardDelegate instance];
			[pRootVC presentModalViewController:pVC animated:YES];
			[pVC release];
		}
		//--------------------------------------------------------------------
		/// Request Leaderboard Names and IDs
		///
		/// Will form a game center request to retreive the name and 
		/// category identifiers of this app's leaderboards
		//--------------------------------------------------------------------
		void CGameCentreSystem::RequestLeaderboardNamesandIDs(const LeaderboardsInfoRequestEventDelegate& inCallback, const std::string& instrTag, RequestErrorDelegate inpErrorDelegate)
		{
			if (!mbIsGameCentreSupported)
				return;
			
			//Create and queue a score request object
			CGameCentreLeaderboardsInfoRequest* pPostRequest = new CGameCentreLeaderboardsInfoRequest();
			pPostRequest->mstrTag = instrTag;
			pPostRequest->mpErrorDelegate = inpErrorDelegate;
			
			mLeaderboardsInfoRequestEvent.AddListener(inCallback);
			AddRequestToQueue(pPostRequest);
		}
		//--------------------------------------------------------------------
		/// Post Score To Leaderboard
		///
		/// Will attempt to send the score to Apple and will cache and
		/// resend if it fails
		///
		/// @param Category ID of the board as specified on iTunes Connect
		/// @param Score
		//--------------------------------------------------------------------
		void CGameCentreSystem::PostScoreToLeaderboard(const std::string& instrCategoryID, u64 inuddwScore, const std::string& instrTag, RequestErrorDelegate inpErrorDelegate)
		{
			if (!mbIsGameCentreSupported)
				return;
			
			//Create and queue a score request object
			CGameCentrePostScoreRequest* pPostRequest = new CGameCentrePostScoreRequest();
			pPostRequest->mstrLeaderboardCategoryID = instrCategoryID;
			pPostRequest->muddwScore = inuddwScore;
			pPostRequest->mstrTag = instrTag;
			pPostRequest->mpErrorDelegate = inpErrorDelegate;
			
			AddRequestToQueue(pPostRequest);
		}
		//---------------------------------------------------------------------
		/// Request Friends Information
		///
		/// Request that the server give us the data about the user's friends
		//---------------------------------------------------------------------
		void CGameCentreSystem::RequestFriendsInformation(const std::string& instrTag, RequestErrorDelegate inpErrorDelegate)
		{
			if (!mbIsGameCentreSupported)
				return;
			
			//Create and queue the request object
			CGameCentreFriendInfoRequest* pFriendInfoRequest = new CGameCentreFriendInfoRequest();
			pFriendInfoRequest->mstrTag = instrTag;
			pFriendInfoRequest->mpErrorDelegate = inpErrorDelegate;
			
			AddRequestToQueue(pFriendInfoRequest);
		}
		//---------------------------------------------------------------------
		///Pushes a view controller showing the system achievements UI
		//---------------------------------------------------------------------
		void CGameCentreSystem::ShowSystemAchievementsUI(){
			UIViewController * pRootVC = [UIApplication sharedApplication].keyWindow.rootViewController;
			
			GKAchievementViewController * pVC = [[GKAchievementViewController alloc] init];
			pVC.achievementDelegate = [GameCentreAchievementsDelegate instance];
			[pRootVC presentModalViewController:pVC animated:YES];
			[pVC release];
		}
		
		//---------------------------------------------------------------------
		/// Updates the current signed-in user's progression in the specified achievement
		/// @param the string Achievement ID of the achievement per iTunes Connect
		/// @param the progression from 0 - 100 towards this achievement. Calling this with 0 will reveal an achievement set to hidden
		//---------------------------------------------------------------------
		void CGameCentreSystem::SetAchievementProgression(const std::string & instrID, double inffProgression, bool inbShowToast, const std::string& instrTag, RequestErrorDelegate inpErrorDelegate){
			
			//Check we don't have this achievement
			std::vector<std::string>::iterator pAchievementID = std::find(mastrCompletedAchievements.begin(),mastrCompletedAchievements.end(),instrID);
			
			if (mbIsGameCentreSupported && pAchievementID == mastrCompletedAchievements.end()){
				
				CGameCentreUpdateAchievementRequest* pUpdateAchievementRequest = new CGameCentreUpdateAchievementRequest();
				pUpdateAchievementRequest->mstrAchievementID = instrID;
				pUpdateAchievementRequest->mffProgression = inffProgression;
				pUpdateAchievementRequest->mpErrorDelegate = inpErrorDelegate;
				pUpdateAchievementRequest->mstrTag = instrTag;
				
				AddRequestToQueue(pUpdateAchievementRequest);
				
				
				if (inbShowToast && inffProgression >= 100.0){
					
					GKAchievementDescription * pAchievement = nil;
					NSString * strAchievementName = Core::CStringUtils::StringToNSString(instrID);
					
					for (GKAchievementDescription * description in mpAchievementDescriptions) {
						if ([description.identifier compare:strAchievementName] ==  NSOrderedSame){
							pAchievement = description;
							break;
						}
					}
					
					if (pAchievement){
						[[GKAchievementHandler defaultHandler] notifyAchievement:pAchievement];
					}
					
					mastrCompletedAchievements.push_back(instrID);
				}
				
			}
			
		}
		
		//---------------------------------------------------------------------
		///Resets all achievements
		//---------------------------------------------------------------------
		void CGameCentreSystem::ResetAllAchievements()
		{
			[GKAchievement resetAchievementsWithCompletionHandler:^(NSError* inpError)
			 {
				mAchievementsResetEvent.Invoke(nil == inpError);
			 }];
		}
		//---------------------------------------------------------------------
        /// Checks if the passed in achievement ID has been unlocked
        //---------------------------------------------------------------------
        bool CGameCentreSystem::IsAchievementUnlocked(const std::string & instrID)
        {
            std::vector<std::string>::iterator it = std::find(mastrCompletedAchievements.begin(), mastrCompletedAchievements.end(), instrID);
            if(it == mastrCompletedAchievements.end())
                return false;
            else
                return true;
        }
		//---------------------------------------------------------------------
		///Subscribe to achievement reset event
		//---------------------------------------------------------------------
		Core::IEvent<CGameCentreSystem::AchievementsResetDelegate>& CGameCentreSystem::OnAchievementResetEvent()
		{
			return mAchievementsResetEvent;
		}
		
		//---------------------------------------------------------------------
		/// OnAuthenticationChangedEvent
		///
		/// Raised when the game centre user changes (new signin/signout)
		//---------------------------------------------------------------------
		Core::IEvent<CGameCentreSystem::AuthenticationChangedDelegate>& CGameCentreSystem::OnAuthenticationChangedEvent()
		{
			return mAuthenticationChangedEvent;
		}
		//--------------------------------------------------------------------
		/// Request Scores Within Range
		///
		/// Request that a set of scores from the given leaderboard are
		/// downloaded. Successful download will trigger a callback.
		///
		/// @param Category ID of the board as specified on iTunes Connect
		/// @param Player scope (i.e. global, friends)
		/// @param Time scope (i.e. all time, the last week)
		/// @param Minimum range
		/// @param Maximum range
		//---------------------------------------------------------------------
		void CGameCentreSystem::RequestScoresWithinRange(const std::string& instrCategoryID, PlayerScope inePlayerScope, TimeScope ineTimeScope, u32 inudwMinRange, u32 inudwMaxRange, 
														 CGameCentreSystem::LeaderboardScoreRequestDelegate inpDelegate, const std::string& instrTag, RequestErrorDelegate inpErrorDelegate)
		{
			if (!mbIsGameCentreSupported)
				return;
			
			//Create and queue the request object
			CGameCentreGetScoresRequest* pScoresRequest = new CGameCentreGetScoresRequest();
			pScoresRequest->mePlayerScope = inePlayerScope;
			pScoresRequest->meTimeScope = ineTimeScope;
			pScoresRequest->mstrLeaderboardCategoryID = instrCategoryID;
			pScoresRequest->mudwMinRange = inudwMinRange;
			pScoresRequest->mudwMaxRange = inudwMaxRange;
			pScoresRequest->mstrTag = instrTag;
			pScoresRequest->mpErrorDelegate = inpErrorDelegate;
			
			mMapRequestNameToRequestDelegate[instrCategoryID] = inpDelegate;
			
			AddRequestToQueue(pScoresRequest);
		}
		//--------------------------------------------------------------------
		/// Request Names For IDs
		///
		/// Request a set of player names from the array of player ID's
		///
		/// @param Array containing player IDs
		/// @param Key to marry the name data to the score data
		//---------------------------------------------------------------------
		void CGameCentreSystem::RequestNamesForIDs(NSArray* inpPlayerIDs, const std::string& instrKey, const std::string& instrTag, RequestErrorDelegate inpErrorDelegate)
		{
			if (!mbIsGameCentreSupported)
				return;
			
			//Create and queue the request object
			CGameCentreGetNamesRequest* pNamesRequest = new CGameCentreGetNamesRequest();
			pNamesRequest->mpaPlayerIDs = [inpPlayerIDs retain];
			pNamesRequest->mstrKey = instrKey;
			pNamesRequest->mstrTag = instrTag;
			pNamesRequest->mpErrorDelegate = inpErrorDelegate;
			
			AddRequestToQueue(pNamesRequest);
		}
		//--------------------------------------------------------------------
		/// Add Request To Queue
		///
		/// Push the request to the back of the queue so that the game
		/// center thread can process it. This is a thread safe method
		///
		/// @param Game Centre Request
		//--------------------------------------------------------------------
		void CGameCentreSystem::AddRequestToQueue(IGameCentreRequest* inpRequest)
		{
			//Check the number of times this request has been processed and if it
			//exceeds the limit notify the application that this request is borked
			if(++inpRequest->mudwNumRequestAttempts <= kudwGameCentreRequestAttemptLimit || !mbIsGameCentreSupported)
			{
				mRequestQueue.push(inpRequest);
			}
			else
			{
				if(inpRequest->mpErrorDelegate)
				{
					(inpRequest->mpErrorDelegate)(inpRequest->mstrTag);
				}
			}
		}
		//---------------------------------------------------------------------
		/// Process Request Queue
		///
		/// A run loop that handles the queued requests.
		//---------------------------------------------------------------------
		void CGameCentreSystem::ProcessRequestQueue()
		{
			//Create a new autorelease pool as this runs on a seperate thread
			NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];  
			
			while(mbProcessRequests)
			{
				//This method will cause the thread to sleep if the queue is empty
				IGameCentreRequest* pRequest = mRequestQueue.front();
                mRequestQueue.pop();
				
				if(mudwNumOpenRequests <= kudwGameCentreMaxOpenRequests)
				{
					//Handle a posting to leaderboard request
					if(pRequest->IsA(kudwGameCentrePostScoreRequestID))
					{
						ProcessPostScoreRequest(static_cast<CGameCentrePostScoreRequest*>(pRequest));
						mudwNumOpenRequests++;
					}
					//Handle request for friends unique identifiers
					else if(pRequest->IsA(kudwGameCentreGetFriendIdentifiersRequestID))
					{
						ProcessFriendIdentifiersRequest(static_cast<CGameCentreFriendInfoRequest*>(pRequest));
						mudwNumOpenRequests++;
					}
					//Handle request for scores in range
					else if(pRequest->IsA(kudwGameCentreGetScoresRequestID))
					{
						ProcessGetScoresRequest(static_cast<CGameCentreGetScoresRequest*>(pRequest));
						mudwNumOpenRequests++;
					}
					//Handle the leaderboard name and category ID request
					else if(pRequest->IsA(kudwGameCentreGetLeaderboardsInfoRequestID))
					{
						ProcessLeaderboardsInfoRequest(static_cast<CGameCentreLeaderboardsInfoRequest*>(pRequest));
						mudwNumOpenRequests++;
					}
					//Handle player namee requests
					else if(pRequest->IsA(kudwGameCentreGetNamesRequestID))
					{
						ProcessGetNamesRequest(static_cast<CGameCentreGetNamesRequest*>(pRequest));
						mudwNumOpenRequests++;
					} else if (pRequest->IsA(kudwGameCentreUpdateAchievementRequestID)){
						ProcessUpdateAchievementRequest(static_cast<CGameCentreUpdateAchievementRequest*>(pRequest));
						mudwNumOpenRequests++;
					} else if (pRequest->IsA(kudwGameCentreAchievementsStatusRequestID)) {
						ProcessAchievementsStatusRequest(static_cast<CGameCentreAchievementsStatusRequest*>(pRequest));
						mudwNumOpenRequests++;
					} else if (pRequest->IsA(kudwGameCentreAchievementDescriptionsRequestID)) {
						ProcessAchievementDescriptionsRequest(static_cast<CGameCentreAchievementDescriptionsRequest*>(pRequest));
						mudwNumOpenRequests++;
					}
				}
				else
				{
					//Give up our time slice we are doing nothing
                    std::this_thread::yield();
				}
			}
			
			//Purge the pool
			[pool release];
			
			mRequestThread.join();
		}
		//---------------------------------------------------------------------
		/// Process Leaderboard Info Request
		///
		/// Send a request for the server to get the names and identifiers
		/// of the apps leaderboards.
		///
		/// @param Request
		//---------------------------------------------------------------------
		void CGameCentreSystem::ProcessLeaderboardsInfoRequest(CGameCentreLeaderboardsInfoRequest* pRequest)
		{
			[GKLeaderboard loadCategoriesWithCompletionHandler:^(NSArray* pCategories, NSArray* pTitles, NSError* pError)
			 {
			 if(pError)
			 {
			 //Request failed. Let's try again later. It's already been popped from the queue
			 CS_ERROR_LOG("Game Center - " + ChilliSource::Core::CStringUtils::NSStringToString([pError localizedDescription]));
			 mudwNumOpenRequests--;
			 //let's add it back on
			 AddRequestToQueue(pRequest);
			 }
			 else if(pCategories && pTitles)
			 {
			 //Great Success
			 CS_DEBUG_LOG("Game Center - Leaderboard info downloaded successfully");
			 mudwNumOpenRequests--;
			 //Build a packet for the listener and invoke their callback
			 OnLeaderboardsInfoRequestComplete(pCategories, pTitles);
			 //Delete the request
			 delete pRequest;
			 }
			 }];
		}
		//---------------------------------------------------------------------
		/// Process Post Score Request
		///
		/// Unpack the data from the request and attempt to send to game centre
		///
		/// @param Post score request
		/// @return Whether the post was successful
		//---------------------------------------------------------------------
		void CGameCentreSystem::ProcessPostScoreRequest(CGameCentrePostScoreRequest* pRequest)
		{
			//Create a score request object
			GKScore* pScoreReporter = [[[GKScore alloc] initWithCategory:[NSString stringWithUTF8String:pRequest->mstrLeaderboardCategoryID.c_str()]] autorelease];
			//Set score
			pScoreReporter.value = pRequest->muddwScore;
			
			//Attempt to post the score
			[pScoreReporter reportScoreWithCompletionHandler:^(NSError* pError)
			 {
			 if(pError)
			 {
			 //Request failed. Let's try again later. It's already been popped from the queue
			 CS_ERROR_LOG("Game Center - " + ChilliSource::Core::CStringUtils::NSStringToString([pError localizedDescription]));
			 mudwNumOpenRequests--;
			 //let's add it back on
			 AddRequestToQueue(pRequest);
			 }
			 else
			 {
			 //Great Success
			 CS_DEBUG_LOG("Game Center - Score of " + Core::ToString(pRequest->muddwScore) + " posted successfully");
			 mudwNumOpenRequests--;
			 //Delete the request
			 delete pRequest;
			 }
			 }];
		}
		//---------------------------------------------------------------------
		/// Process Get Scores Request
		///
		/// Retrieve the scores with the parameters of the request
		///
		/// @param Get scores request
		//---------------------------------------------------------------------
		void CGameCentreSystem::ProcessGetScoresRequest(CGameCentreGetScoresRequest* pRequest)
		{
			GKLeaderboard* pLeaderboardRequest = [[[GKLeaderboard alloc] init] autorelease];
			
			//Set the leaderboard we are requesting from
			pLeaderboardRequest.category = ChilliSource::Core::CStringUtils::StringToNSString(pRequest->mstrLeaderboardCategoryID);
			
			//Set the leaderboard section we wish to use
			switch(pRequest->mePlayerScope)
			{
				default:
				case PlayerScope::k_global:
					pLeaderboardRequest.playerScope = GKLeaderboardPlayerScopeGlobal;
					break;
				case PlayerScope::k_friends:
					pLeaderboardRequest.playerScope = GKLeaderboardPlayerScopeFriendsOnly;
					break;
			};
			
			//Set the leaderboard section we wish to use
			switch(pRequest->meTimeScope)
			{
				default:
				case TimeScope::k_allTime:
					pLeaderboardRequest.timeScope = GKLeaderboardTimeScopeAllTime;
					break;
				case TimeScope::k_allToday:
					pLeaderboardRequest.timeScope = GKLeaderboardTimeScopeToday;
					break;
				case TimeScope::k_allWeek:
					pLeaderboardRequest.timeScope = GKLeaderboardTimeScopeWeek;
					break;
			};
			
			//Set the range of scores
			pLeaderboardRequest.range = NSMakeRange(pRequest->mudwMinRange, pRequest->mudwMaxRange);
			
			//Fire away!
			[pLeaderboardRequest loadScoresWithCompletionHandler: ^(NSArray* pScores, NSError* pError) 
			 {
			 if(pError)
			 {
			 //Request failed. Let's try again later. It's already been popped from the queue
			 CS_ERROR_LOG("Game Center - " + ChilliSource::Core::CStringUtils::NSStringToString([pError localizedDescription]));
			 mudwNumOpenRequests--;
			 //let's add it back on
			 AddRequestToQueue(pRequest);
			 }
			 else if(pScores)
			 {
			 //Great Success
			 CS_DEBUG_LOG("Game Center - Scores request successful. Num scores returned: " + Core::ToString([pScores count]));
			 mudwNumOpenRequests--;
			 //Now we need to store the scores and request the names before we return the data to the application
			 OnScoreRangeRequestComplete(pScores, pRequest->mstrLeaderboardCategoryID);
			 //Delete the request
			 delete pRequest;
			 }
			 }];
			
		}
		//---------------------------------------------------------------------
		/// Process Get Names Request
		///
		/// Retrieve the names from the given user IDs
		///
		/// @param Get names request
		//---------------------------------------------------------------------
		void CGameCentreSystem::ProcessGetNamesRequest(CGameCentreGetNamesRequest* pRequest)
		{
			[GKPlayer loadPlayersForIdentifiers:pRequest->mpaPlayerIDs withCompletionHandler:^(NSArray* pPlayers, NSError* pError) 
			 {
			 if(pError)
			 {
			 //Request failed. Let's try again later. It's already been popped from the queue
			 CS_ERROR_LOG("Game Center - " + ChilliSource::Core::CStringUtils::NSStringToString([pError localizedDescription]));
			 mudwNumOpenRequests--;
			 //let's add it back on
			 AddRequestToQueue(pRequest);
			 }
			 else if(pPlayers)
			 {
			 //Great Success
			 CS_DEBUG_LOG("Game Center - Names request successful. Num names returned: " + Core::ToString([pPlayers count]));
			 mudwNumOpenRequests--;
			 //Now we need to combine the names with the previously requested scores and return them to the app
			 OnNameRequestComplete(pPlayers, pRequest->mstrKey);
			 //Delete the request
			 delete pRequest;
			 }
			 }];
		}
		//---------------------------------------------------------------------
		/// Processes the given update request
		///
		/// @param Get names request
		//---------------------------------------------------------------------
		void CGameCentreSystem::ProcessUpdateAchievementRequest(CGameCentreUpdateAchievementRequest * pRequest){
			
			GKAchievement * pAchievement = [[[GKAchievement alloc] initWithIdentifier:[NSString stringWithUTF8String:pRequest->mstrAchievementID.c_str()]] autorelease];
			pAchievement.percentComplete = pRequest->mffProgression;
			
			[pAchievement reportAchievementWithCompletionHandler:^(NSError* pError)
			 {
			 if (pError){
			 CS_ERROR_LOG("Game Center - " + ChilliSource::Core::CStringUtils::NSStringToString([pError localizedDescription]));
			 mudwNumOpenRequests--;
			 AddRequestToQueue(pRequest);
			 
			 } else {
			 //Great Success
			 CS_DEBUG_LOG("Game Center - Achievement:" + pRequest->mstrAchievementID + " Updated to progress:" + Core::ToString((f32)pRequest->mffProgression));
			 mudwNumOpenRequests--;
			 //Delete the request
			 delete pRequest;
			 }
			 
			 }
			 
			 ];
		}
		void CGameCentreSystem::ProcessAchievementsStatusRequest(CGameCentreAchievementsStatusRequest * pRequest){
			
			[GKAchievement loadAchievementsWithCompletionHandler:^(NSArray *achievements, NSError *error)
			 {
				 if (error == nil){
					HandleAchievementStatusArray(achievements);	

				 } else {
					 CS_ERROR_LOG("Game Center:ProcessAchievementsStatusRequest- " + ChilliSource::Core::CStringUtils::NSStringToString([error localizedDescription]));
					 mudwNumOpenRequests--;
					 AddRequestToQueue(pRequest);
				 }
			 }
			 
			 ];
		}
		void CGameCentreSystem::HandleAchievementStatusArray(NSArray * inpAchievements){
			

			for (GKAchievement* a in inpAchievements){
				if (a.completed){
					mastrCompletedAchievements.push_back(ChilliSource::Core::CStringUtils::NSStringToString(a.identifier));
				}
			}
			
			CS_DEBUG_LOG("GameCentre:ProcessAchievementsStatusRequest - User has " + Core::ToString((u32)mastrCompletedAchievements.size()) +" achievements");

		}
		void CGameCentreSystem::ProcessAchievementDescriptionsRequest(CGameCentreAchievementDescriptionsRequest * pRequest){
			[GKAchievementDescription loadAchievementDescriptionsWithCompletionHandler:^(NSArray *descriptions, NSError *error)
			 {
			 if (error){
			 CS_ERROR_LOG("GameCentre:ProcessAchievementDescriptionsRequest - " + ChilliSource::Core::CStringUtils::NSStringToString([error localizedDescription]));
			 mudwNumOpenRequests--;
			 AddRequestToQueue(pRequest);
			 
			 } else {
			 mpAchievementDescriptions = descriptions;
			 [mpAchievementDescriptions retain];
			 CS_DEBUG_LOG("GameCentre:ProcessAchievementDescriptionsRequest - Received " + Core::ToString([descriptions count]) + " descriptions");
					   }
			 }
			];
			
		}
		//---------------------------------------------------------------------
		/// Process Friend Identifiers Request
		///
		/// Send a request for the server to populate the local players friend
		/// array
		///
		/// @param Request
		//---------------------------------------------------------------------
		void CGameCentreSystem::ProcessFriendIdentifiersRequest(CGameCentreFriendInfoRequest* pRequest)
		{
			GKLocalPlayer* pLocalPlayer = [GKLocalPlayer localPlayer];
			
			if(pLocalPlayer.authenticated)
			{
				[pLocalPlayer loadFriendsWithCompletionHandler:^(NSArray* pFriends, NSError* pError) 
				 {
				 if(pError)
				 {
				 //Request failed. Let's try again later. It's already been popped from the queue
				 CS_ERROR_LOG("Game Center - " + ChilliSource::Core::CStringUtils::NSStringToString([pError localizedDescription]));
				 mudwNumOpenRequests--;
				 //let's add it back on
				 AddRequestToQueue(pRequest);
				 }
				 else
				 {
				 //Great Success
				 CS_DEBUG_LOG("Game Center - Friend IDs request successful");
				 mudwNumOpenRequests--;
				 //Fire off a request for the actual friend data now
				 [GKPlayer loadPlayersForIdentifiers:pFriends withCompletionHandler:^(NSArray* pPlayers, NSError* pError) 
				  {
				  if(pError)
				  {
				  CS_ERROR_LOG("Game Center - " + ChilliSource::Core::CStringUtils::NSStringToString([pError localizedDescription]));
				  }
				  else if(pPlayers)
				  {
				  CS_DEBUG_LOG("Game Center - Friend Info request successful. Num friends returned: " + Core::ToString([pPlayers count]));
				  }
				  else
				  {
				  CS_DEBUG_LOG("Game Center - Nae mates");
				  }
				  
				  }];
				 
				 //Delete the request
				 delete pRequest;
				 }
				 }];
				
			}
		}
		//----------------------------------------------------------------------
		/// On Leaderboard Info Request Complete
		///
		/// The leaderboard names and ID's have been downloaded. Let's unpack 
		/// and make them available for use
		//----------------------------------------------------------------------
		void CGameCentreSystem::OnLeaderboardsInfoRequestComplete(NSArray* pCategories, NSArray* pTitles)
		{
			for(u32 i=0; i<[pCategories count]; ++i)
			{
				mLeaderboardsInfo.mCategoryIDs.push_back(ChilliSource::Core::CStringUtils::NSStringToString([pCategories objectAtIndex:i]));
				mLeaderboardsInfo.mNames.push_back(ChilliSource::Core::CStringUtils::NSStringToUTF8String([pTitles objectAtIndex:i]));
			}
			
			mLeaderboardsInfoRequestEvent.Invoke(mLeaderboardsInfo);
		}
		//---------------------------------------------------------------------
		/// On Local Authentication Changed
		///
		/// The user may have disconnected/connected or logged in or out
		//---------------------------------------------------------------------
		void CGameCentreSystem::OnLocalAuthenticationChanged()
		{
			mbIsGameCentreSupported = [GKLocalPlayer localPlayer].authenticated;
			
			if(mbIsGameCentreSupported)
			{
				//Get the players details
				if(mstrPlayerID != ChilliSource::Core::CStringUtils::NSStringToUTF8String([GKLocalPlayer localPlayer].playerID))
                {
                    mstrPlayerID = ChilliSource::Core::CStringUtils::NSStringToUTF8String([GKLocalPlayer localPlayer].playerID);
                    mstrPlayerName = ChilliSource::Core::CStringUtils::NSStringToUTF8String([GKLocalPlayer localPlayer].alias);
                    
                    CS_DEBUG_LOG("Game Center - Local Player signs in");
    
                    mAuthenticationChangedEvent.Invoke(true);
                    
                    //Load friends
                    RequestFriendsInformation("FriendInfo", nullptr);
                    
                    mastrCompletedAchievements.clear();
                    [mpAchievementDescriptions release];
					mpAchievementDescriptions = nil;
                    AddRequestToQueue(new CGameCentreAchievementsStatusRequest());
                    AddRequestToQueue(new CGameCentreAchievementDescriptionsRequest());
                }
			} 
			else 
			{
				CS_DEBUG_LOG("Game Center - Local Player signs out");
				mAuthenticationChangedEvent.Invoke(false);
			}
			
		}
		//---------------------------------------------------------------------
		/// On Score Range Request Complete
		///
		/// The score objects have been downloaded but do not contain user-
		/// names. We can use the user ID's to download the names and then 
		/// return the object as a whole
		///
		/// @param Array of score objects
		/// @param Name object which we use as the key to marry the data
		//---------------------------------------------------------------------
		void CGameCentreSystem::OnScoreRangeRequestComplete(NSArray* inpScores, const std::string& instrKey)
		{
			//Build an array of identifiers from the score object
			NSMutableArray* paIdentifier = [NSMutableArray arrayWithCapacity:[inpScores count]];
			
			//Store the scores and we will add the names later. We need to store them
			//with a key so we can map the correct names to them when the name returns
			//...fun times.
			ScoreContainer ScoreData([inpScores count]);
			LeaderboardScoreResults sResultData;
			sResultData.aScores.reserve([inpScores count]);
			
			for(GKScore* pScore in inpScores)
			{
				sResultData.strCategoryName	= ChilliSource::Core::CStringUtils::NSStringToString(pScore.category);
				
				[paIdentifier addObject:pScore.playerID];
				
				ScoreInfo Data;
				Data.strFormattedScore = ChilliSource::Core::CStringUtils::NSStringToUTF8String(pScore.formattedValue);
				Data.uddwScore = (u64)pScore.value;
				sResultData.aScores.push_back(Data);
			}
			
			mMapNameRequestToLeaderboardScoreResults.insert(std::make_pair(instrKey, sResultData));
			
			//If we need to look up names
			if (sResultData.aScores.size() > 0)
            {
				//Ping request to game centre for games
				RequestNamesForIDs(paIdentifier, instrKey, "NameRequest", nullptr);
			} 
			else 
			{
				//No results to be found
				OnNameRequestComplete(paIdentifier, instrKey);
			}
		}
		//---------------------------------------------------------------------
		/// On Name Request Complete
		///
		/// The player objects have been downloaded. We can now combine them
		/// with the score request data and return names and scores 
		/// together
		///
		/// @param Array of player objects
		/// @param Key to find the downloaded scores
		//---------------------------------------------------------------------
		void CGameCentreSystem::OnNameRequestComplete(NSArray* inpPlayers, const std::string& instrKey)
		{
			//We need to find the data we downloaded earlier with the scores
			std::unordered_map<std::string, LeaderboardScoreResults>::iterator pLeaderboardScoreResult = mMapNameRequestToLeaderboardScoreResults.find(instrKey);
			
			if(pLeaderboardScoreResult != mMapNameRequestToLeaderboardScoreResults.end())
			{
				//and add the names into them
				u32 i=0;
				for(ScoreContainer::iterator pScore = pLeaderboardScoreResult->second.aScores.begin(); pScore != pLeaderboardScoreResult->second.aScores.end(); ++pScore, ++i)
				{
					GKPlayer* pPlayer = [inpPlayers objectAtIndex:i];
					pScore->strPlayerName = ChilliSource::Core::CStringUtils::NSStringToUTF8String(pPlayer.alias);
					pScore->bIsFriend = pPlayer.isFriend;
				}
				
				std::unordered_map<std::string, LeaderboardScoreRequestDelegate>::iterator pDelegate = mMapRequestNameToRequestDelegate.find(instrKey);
				
				if (pDelegate != mMapRequestNameToRequestDelegate.end()){
					pDelegate->second(pLeaderboardScoreResult->second);	
					mMapRequestNameToRequestDelegate.erase(pDelegate);
				}
				
				//Remove 
				mMapNameRequestToLeaderboardScoreResults.erase(pLeaderboardScoreResult);
			}
		}
		//--------------------------------------------------------------------
		/// Destructor
		///
		//--------------------------------------------------------------------
		CGameCentreSystem::~CGameCentreSystem()
		{
			mbProcessRequests = false;
		}
		
	}
}