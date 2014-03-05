//
//  GooglePlayIAPSystem.cpp
//  MoFlow
//
//  Created by Scott Downie on 14/06/2013.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Extensions/GooglePlay/GooglePlayIAPSystem.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>

namespace ChilliSource
{
	namespace Android
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
			bool TryGetRegisteredProductType(const std::vector<Networking::IAPProductRegInfo>& inProductRegInfos, const std::string& instrProductID, Networking::IAPProductRegInfo::Type& outeType)
			{
				for(std::vector<Networking::IAPProductRegInfo>::const_iterator it = inProductRegInfos.begin(); it != inProductRegInfos.end(); ++it)
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
		GooglePlayIAPSystem::GooglePlayIAPSystem(const Core::ParamDictionary& inParams)
		{
			std::string strPublicKey = "";
			if (inParams.HasValue(kstrGooglePlayPublicKeyKey) == true)
			{
				strPublicKey = inParams.ValueForKey(kstrGooglePlayPublicKeyKey);
			}

    		mpJavaInterface = GooglePlayIAPJavaInterfacePtr(new GooglePlayIAPJavaInterface(strPublicKey));
	        ChilliSource::Android::CJavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpJavaInterface);
		}
        //---------------------------------------------------------------
        /// Register Products
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::RegisterProducts(const std::vector<Networking::IAPProductRegInfo>& inaProducts)
        {
            mProductRegInfos = inaProducts;
        }
        //---------------------------------------------------------------
		/// Get Provider ID
        //---------------------------------------------------------------
		std::string GooglePlayIAPSystem::GetProviderID() const
		{
			return "GooglePlayV3";
		}
        //---------------------------------------------------------------
		/// Get Provider Name
        //---------------------------------------------------------------
		std::string GooglePlayIAPSystem::GetProviderName() const
		{
			return "PlayStoreV3";
		}
        //---------------------------------------------------------------
		/// Is Purchasing Enabled
        //---------------------------------------------------------------
        bool GooglePlayIAPSystem::IsPurchasingEnabled()
        {
        	return mpJavaInterface->IsPurchasingEnabled();
        }
        //---------------------------------------------------------------
		/// Start Listening For Transaction Updates
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::StartListeningForTransactionUpdates(const Networking::IAPTransactionDelegate& inRequestDelegate)
        {
        	mpJavaInterface->StartListeningForTransactionUpdates(inRequestDelegate);
        }
        //---------------------------------------------------------------
		/// Stop Listening For Transaction Updates
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::StopListeningForTransactionUpdates()
        {
        	mpJavaInterface->StopListeningForTransactionUpdates();
        }
        //---------------------------------------------------------------
		/// Request Product Descriptions
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::RequestProductDescriptions(const std::vector<std::string>& inaProductIDs, const Networking::IAPProductDescDelegate& inRequestDelegate)
        {
        	mpJavaInterface->RequestProductDescriptions(inaProductIDs, inRequestDelegate);
        }
        //---------------------------------------------------------------
        /// Request All Product Descriptions
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::RequestAllProductDescriptions(const Networking::IAPProductDescDelegate& inRequestDelegate)
        {
            std::vector<std::string> aIDs;
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
        void GooglePlayIAPSystem::CancelProductDescriptionsRequest()
        {
        	mpJavaInterface->CancelProductDescriptionsRequest();
        }
        //---------------------------------------------------------------
		/// Request Product Purchase
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::RequestProductPurchase(const std::string& instrProductID)
        {
        	Networking::IAPProductRegInfo::Type eType;
        	if(!TryGetRegisteredProductType(mProductRegInfos, instrProductID, eType))
        	{
        		CS_LOG_FATAL("Cannot find IAP product with ID " + instrProductID + ". Please register it");
        		return;
        	}

        	mpJavaInterface->RequestProductPurchase(instrProductID, eType);
        }
        //---------------------------------------------------------------
		/// Close Transactione
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::CloseTransaction(const Networking::IAPTransactionPtr& inpTransaction, const Networking::IAPTransactionCloseDelegate& inDelegate)
        {
        	 mpJavaInterface->CloseTransaction(inpTransaction->strProductID, inpTransaction->strTransactionID, inDelegate);
        }
        //---------------------------------------------------------------
		/// Restore Managed Purchases
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::RestoreManagedPurchases()
        {
        	mpJavaInterface->RestoreManagedPurchases();
        }
	}
}
