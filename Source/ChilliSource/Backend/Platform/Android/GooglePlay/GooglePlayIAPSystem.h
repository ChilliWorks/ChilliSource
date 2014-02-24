//
//  GooglePlayIAPSystem.h
//  MoFlow
//
//  Created by Scott Downie on 14/06/2013.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_PLATFORM_ANDROID_GOOGLEPLAY_GOOGLEPLAYIAPSYSTEM_H_
#define _MOFLOW_PLATFORM_ANDROID_GOOGLEPLAY_GOOGLEPLAYIAPSYSTEM_H_

#include <ChilliSource/Networking/IAP/IAPSystem.h>

#include <ChilliSource/Platform/Android/GooglePlay/GooglePlayIAPJavaInterface.h>

namespace ChilliSource
{
	namespace Android
	{
		class CGooglePlayIAPSystem : public Networking::IIAPSystem
		{
		public:
            //---------------------------------------------------------------
            /// Constructor
			///
			/// @param A dictionary of platform specific parameters. The
			/// parameters that relate to the Google Play IAP System are as
			/// follows:
            /// 	GooglePlayPublicKey  	The public key used for connecting
            ///								to the Google Play store.
            //---------------------------------------------------------------
			CGooglePlayIAPSystem(const Core::ParamDictionary& inParams);

            //---------------------------------------------------------------
            /// Register Products
            ///
            /// Inform the system of which products are available for
            /// purchase and whether they are managed or unmanaged
            ///
            /// @param List of products
            //---------------------------------------------------------------
            void RegisterProducts(const DYNAMIC_ARRAY<Networking::IAPProductRegInfo>& inaProducts);

            //---------------------------------------------------------------
			/// Get Provider ID
			///
			/// @return The ID off the IAP provider as a string.
            //---------------------------------------------------------------
			std::string GetProviderID() const;
            //---------------------------------------------------------------
			/// Get Provider Name
			///
			/// @return A displayable name for the IAP Provider.
            //---------------------------------------------------------------
			std::string GetProviderName() const;
            //---------------------------------------------------------------
			/// Is Purchasing Enabled
			///
			/// @return Whether the purchasing is allowed by the device/OS
            //---------------------------------------------------------------
            bool IsPurchasingEnabled();

            //---------------------------------------------------------------
			/// Start Listening For Transaction Updates
			///
			/// Calling this function will set the listener to which any
            /// transaction events are directed. This is not necessarily
            /// in response to a user action it may be previously outstanding
            /// transactions.
            ///
            /// @param Delegate
            //---------------------------------------------------------------
            void StartListeningForTransactionUpdates(const Networking::IAPTransactionDelegate& inRequestDelegate);
            //---------------------------------------------------------------
			/// Stop Listening For Transaction Updates
            ///
            /// Prevent any more transaction uppdates from being triggered.
            //---------------------------------------------------------------
            void StopListeningForTransactionUpdates();

            //---------------------------------------------------------------
			/// Request Product Descriptions
			///
            /// Starts a request to the store for details of the products.
            /// These details are name, description and price
            ///
			/// @param List of product IDs to request descriptions for
            /// @param Delegate to invoke when the request completes
            //---------------------------------------------------------------
            void RequestProductDescriptions(const DYNAMIC_ARRAY<std::string>& inaProductIDs, const Networking::IAPProductDescDelegate& inRequestDelegate);
            //---------------------------------------------------------------
            /// Request All Product Descriptions
            ///
            /// Starts a request to the store for details of all registered
            /// products. These details are name, description and price
            ///
            /// @param Delegate to invoke when the request completes
            //---------------------------------------------------------------
            void RequestAllProductDescriptions(const Networking::IAPProductDescDelegate& inRequestDelegate);
            //---------------------------------------------------------------
			/// Cancel Product Descriptions Request
			///
			/// Prevent the completion delegate being called for
            /// any pending product description requests and attempt to
            /// cancel the request to the store.
            //---------------------------------------------------------------
            void CancelProductDescriptionsRequest();

            //---------------------------------------------------------------
			/// Request Product Purchase
			///
			/// Make a request to the store to purchase the item.
            /// This will trigger a call to the transaction listener delegate
            ///
            /// @param Product ID
            //---------------------------------------------------------------
            void RequestProductPurchase(const std::string& instrProductID);

            //---------------------------------------------------------------
			/// Close Transaction
			///
			/// Tell the store to close the transaction as complete.
            /// NOTE: This should only be called after the goods have been
            /// awarded.
            ///
            /// @param Transaction to close
            /// @param Delegate called when closed successfully
            //---------------------------------------------------------------
            void CloseTransaction(const Networking::IAPTransactionPtr& inpTransaction, const Networking::IAPTransactionCloseDelegate& inDelegate);

            //---------------------------------------------------------------
			/// Restore Managed Purchases
			///
            /// Request that the store trigger new purchase requests for
            /// owned non-consumable items
            //---------------------------------------------------------------
            void RestoreManagedPurchases();

		private:

            DYNAMIC_ARRAY<Networking::IAPProductRegInfo> mProductRegInfos;

            GooglePlayIAPJavaInterfacePtr mpJavaInterface;
		};
	}
}

#endif
