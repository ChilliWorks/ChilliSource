//
//  GooglePlayIAPSystem.cpp
//  MoFlow
//
//  Created by Scott Downie on 14/06/2013.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <moFlo/Platform/Android/GooglePlay/GooglePlayIAPSystem.h>

#include <moFlo/Platform/Android/JavaInterface/JavaInterfaceManager.h>

namespace moFlo
{
	namespace AndroidPlatform
	{
		namespace
		{
			const std::string kstrGooglePlayPublicKeyKey = "GooglePlayPublicKey";

			//---------------------------------------------------------------
			/// Try Get Registered Product Type
			///
			/// Locate the product in the registered list and return its
			/// type - managed or unmanaged
			///
			/// @param List of registered products
			/// @param Product ID to find
			/// @param Out: Type if found
			/// @return Whether product was found
			//---------------------------------------------------------------
			bool TryGetRegisteredProductType(const DYNAMIC_ARRAY<Networking::IAPProductRegInfo>& inProductRegInfos, const std::string& instrProductID, Networking::IAPProductRegInfo::Type& outeType)
			{
				for(DYNAMIC_ARRAY<Networking::IAPProductRegInfo>::const_iterator it = inProductRegInfos.begin(); it != inProductRegInfos.end(); ++it)
				{
					if(it->strID == instrProductID)
					{
						outeType = it->eType;
						return true;
					}
				}

				return false;
			}
		}
        //---------------------------------------------------------------
        /// Constructor
        //---------------------------------------------------------------
		CGooglePlayIAPSystem::CGooglePlayIAPSystem(const Core::ParamDictionary& inParams)
		{
			std::string strPublicKey = "";
			if (inParams.HasValue(kstrGooglePlayPublicKeyKey) == true)
			{
				strPublicKey = inParams.ValueForKey(kstrGooglePlayPublicKeyKey);
			}

    		mpJavaInterface = GooglePlayIAPJavaInterfacePtr(new CGooglePlayIAPJavaInterface(strPublicKey));
	        moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpJavaInterface);
		}
        //---------------------------------------------------------------
        /// Register Products
        //---------------------------------------------------------------
        void CGooglePlayIAPSystem::RegisterProducts(const DYNAMIC_ARRAY<Networking::IAPProductRegInfo>& inaProducts)
        {
            mProductRegInfos = inaProducts;
        }
        //---------------------------------------------------------------
		/// Get Provider ID
        //---------------------------------------------------------------
		std::string CGooglePlayIAPSystem::GetProviderID() const
		{
			return "GooglePlayV3";
		}
        //---------------------------------------------------------------
		/// Get Provider Name
        //---------------------------------------------------------------
		std::string CGooglePlayIAPSystem::GetProviderName() const
		{
			return "PlayStoreV3";
		}
        //---------------------------------------------------------------
		/// Is Purchasing Enabled
        //---------------------------------------------------------------
        bool CGooglePlayIAPSystem::IsPurchasingEnabled()
        {
        	return mpJavaInterface->IsPurchasingEnabled();
        }
        //---------------------------------------------------------------
		/// Start Listening For Transaction Updates
        //---------------------------------------------------------------
        void CGooglePlayIAPSystem::StartListeningForTransactionUpdates(const Networking::IAPTransactionDelegate& inRequestDelegate)
        {
        	mpJavaInterface->StartListeningForTransactionUpdates(inRequestDelegate);
        }
        //---------------------------------------------------------------
		/// Stop Listening For Transaction Updates
        //---------------------------------------------------------------
        void CGooglePlayIAPSystem::StopListeningForTransactionUpdates()
        {
        	mpJavaInterface->StopListeningForTransactionUpdates();
        }
        //---------------------------------------------------------------
		/// Request Product Descriptions
        //---------------------------------------------------------------
        void CGooglePlayIAPSystem::RequestProductDescriptions(const DYNAMIC_ARRAY<std::string>& inaProductIDs, const Networking::IAPProductDescDelegate& inRequestDelegate)
        {
        	mpJavaInterface->RequestProductDescriptions(inaProductIDs, inRequestDelegate);
        }
        //---------------------------------------------------------------
        /// Request All Product Descriptions
        //---------------------------------------------------------------
        void CGooglePlayIAPSystem::RequestAllProductDescriptions(const Networking::IAPProductDescDelegate& inRequestDelegate)
        {
            DYNAMIC_ARRAY<std::string> aIDs;
            aIDs.reserve(mProductRegInfos.size());

            for(u32 i=0; i<mProductRegInfos.size(); ++i)
            {
                aIDs.push_back(mProductRegInfos[i].strID);
            }

            RequestProductDescriptions(aIDs, inRequestDelegate);
        }
        //---------------------------------------------------------------
		/// Cancel Product Descriptions Request
        //---------------------------------------------------------------
        void CGooglePlayIAPSystem::CancelProductDescriptionsRequest()
        {
        	mpJavaInterface->CancelProductDescriptionsRequest();
        }
        //---------------------------------------------------------------
		/// Request Product Purchase
        //---------------------------------------------------------------
        void CGooglePlayIAPSystem::RequestProductPurchase(const std::string& instrProductID)
        {
        	Networking::IAPProductRegInfo::Type eType;
        	if(!TryGetRegisteredProductType(mProductRegInfos, instrProductID, eType))
        	{
        		FATAL_LOG("Cannot find IAP product with ID " + instrProductID + ". Please register it");
        		return;
        	}

        	mpJavaInterface->RequestProductPurchase(instrProductID, eType);
        }
        //---------------------------------------------------------------
		/// Close Transactione
        //---------------------------------------------------------------
        void CGooglePlayIAPSystem::CloseTransaction(const Networking::IAPTransactionPtr& inpTransaction, const Networking::IAPTransactionCloseDelegate& inDelegate)
        {
        	 mpJavaInterface->CloseTransaction(inpTransaction->strProductID, inpTransaction->strTransactionID, inDelegate);
        }
        //---------------------------------------------------------------
		/// Restore Managed Purchases
        //---------------------------------------------------------------
        void CGooglePlayIAPSystem::RestoreManagedPurchases()
        {
        	mpJavaInterface->RestoreManagedPurchases();
        }
	}
}
