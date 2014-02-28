//
//  LaunchingActions.cpp
//  MoFlow
//
//  Created by Scott Downie on 26/11/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/LaunchingActions/LaunchingActions.h>

#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#include <ChilliSource/Core/JSON/json.h>
#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace LaunchingActions
        {
            namespace
            {
                struct LaunchAction
                {
                    std::string strAction;
                    std::string strData;
                };
                
                std::vector<LaunchAction> gaLaunchActionCache;
                
                typedef std::unordered_map<std::string, LaunchingActions::ActionDelegate> MapActionToDelegate;
                MapActionToDelegate gmapActionToDelegate;
                
                std::string ConvertURLToLaunchAction(const std::string& instrURL)
                {
                    std::vector<std::string> aSplits = Core::StringUtils::Split(instrURL, "//");
                    if(aSplits.size() == 2) //The URL must have a scheme and a data blob;
                    {
                        const std::string& strEncoded =  aSplits[1];
                        return BaseEncoding::Base64Decode(strEncoded);
                    }
                    
                    return "";
                }
                
                void CacheLaunchAction(const std::string& instrAction, const std::string& instrData)
                {
                    LaunchAction sAction;
                    sAction.strAction = instrAction;
                    sAction.strData = instrData;
                    
                    gaLaunchActionCache.push_back(sAction);
                }
            }
            //-----------------------------------------------------------------------
            /// Subscribe For Action Type
            ///
            /// Add a delegate as a listener to be notified when the system receives
            /// a launching action of a certain type. Only one delegate can
            /// be described per action. If the passed delegate is valid the system
            /// will iterate through any queued actions and pass the appropriate
            /// ones through.
            ///
            /// @param Action type
            /// @param Delegate
            //-----------------------------------------------------------------------
            void SubscribeForActionType(const std::string& instrActionType, const ActionDelegate& inDelegate)
            {
                if(!inDelegate)
                {
                    return;
                }
                
                gmapActionToDelegate[instrActionType] = inDelegate;
                
                for(std::vector<LaunchAction>::iterator it = gaLaunchActionCache.begin(); it != gaLaunchActionCache.end(); )
                {
                    if(it->strAction == instrActionType)
                    {
                        inDelegate(it->strData);
                        it = gaLaunchActionCache.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
            //-----------------------------------------------------------------------
            /// Unsubscribe For Action Type
            ///
            /// Remove the listener for the given action type
            ///
            /// @param Action type
            //-----------------------------------------------------------------------
            void UnsubscribeForActionType(const std::string& instrActionType)
            {
                gmapActionToDelegate.erase(instrActionType);
            }
            //-----------------------------------------------------------------------
            /// Application Did Receive Launching URL
            ///
            /// Called by the OS upon launching the application via a URL. Once the
            /// link has been decoded into JSON the action and data will be seperated.
            /// The data will then be passed to the appropriate delegate.
            ///
            /// @param Launching URL that must be decoded into an action
            //-----------------------------------------------------------------------
            void ApplicationDidReceiveLaunchingURL(const std::string& instrURL)
            {
                std::string strJSONAction = ConvertURLToLaunchAction(instrURL);
                
                if(strJSONAction.empty())
                {
                    return;
                }
                
                Json::Reader jReader;
                Json::Value jRoot(Json::objectValue);
                if(!jReader.parse(strJSONAction, jRoot))
                {
                    return;
                }
                
                if(!jRoot.isMember("Action") || !jRoot.isMember("Data"))
                {
                    return;
                }
                
                std::string strAction = jRoot["Action"].asString();
                std::string strData = jRoot["Data"].asString();
                
                MapActionToDelegate::const_iterator it = gmapActionToDelegate.find(strAction);
                
                if(it != gmapActionToDelegate.end())
                {
                    it->second(strData);
                }
                else
                {
                    CacheLaunchAction(strAction, strData);
                }
            }
        }
    }
}

