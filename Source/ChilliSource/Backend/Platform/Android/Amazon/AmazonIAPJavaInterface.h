//
//  AmazonIAPJavaInterface.h
//  MoFlow
//
//  Created by Ian Copland on 10/12/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#ifndef MOFLOW_PLATFORM_ANDROID_AMAZON_AMAZONIAPJAVAINTERFACE_H
#define MOFLOW_PLATFORM_ANDROID_AMAZON_AMAZONIAPJAVAINTERFACE_H

#include <ChilliSource/Platform/Android/JavaInterface/JavaInterface.h>
#include <ChilliSource/Networking/IAP/IAPSystem.h>

namespace moFlo
{
	namespace AndroidPlatform
	{
		//========================================================================
		/// Amazon IAP Java Interface
		///
		/// A java interface for communicating with the java Amazon in-app purchasing
		/// system.
		//========================================================================
		class CAmazonIAPJavaInterface : public IJavaInterface
		{
		public:
			DECLARE_NAMED_INTERFACE(CAmazonIAPJavaInterface);
			//--------------------------------------------------------------
			/// Constructor
			///
			/// @param The private key.
			/// @param The UDID
			//--------------------------------------------------------------
			CAmazonIAPJavaInterface(const std::string& instrPrivateKey, const std::string& instrUDID);
			//--------------------------------------------------------------
			/// Is A
			///
			/// @return whether or not this object implements the given interface.
			//--------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
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
            /// Prevent any more transaction updates from being triggered.
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
            /// @param Product ID
            /// @param Transaction ID
            /// @param Delegate to call when closed successfully
            //---------------------------------------------------------------
            void CloseTransaction(const std::string& instrProductID, const std::string& instrTransactionID, const Networking::IAPTransactionCloseDelegate& inDelegate);
            //---------------------------------------------------------------
    		/// Restore Managed Purchases
    		///
            /// Request that the store trigger new purchase requests for
            /// owned non-consumable items
            //---------------------------------------------------------------
            void RestoreManagedPurchases();

            //---Only called by JNI - Otherwise would be private
            //---------------------------------------------------------------
    		/// On Product Descriptions Request Complete
            ///
            /// @param List of product descriptions
            //---------------------------------------------------------------
            void OnProductDescriptionsRequestComplete(const DYNAMIC_ARRAY<moNetworking::IAPProductDesc>& inaProducts);
            //---------------------------------------------------------------
    		/// On Transaction Status Updated
            ///
            /// @param Status ID
            /// @param Transaction description
            //---------------------------------------------------------------
            void OnTransactionStatusUpdated(u32 inudwStatus, const moNetworking::IAPTransaction& inTransaction);
            //---------------------------------------------------------------
    		/// On Transaction Closed
            ///
            /// @param Product ID
            /// @param Transaction ID
            //---------------------------------------------------------------
            void OnTransactionClosed(const std::string& instrProductID, const std::string& instrTransactionID);

		private:

            Networking::IAPProductDescDelegate mProductsRequestDelegate;
            Networking::IAPTransactionDelegate mTransactionStatusDelegate;
            Networking::IAPTransactionCloseDelegate mTransactionCloseDelegate;
		};

		typedef SHARED_PTR<CAmazonIAPJavaInterface> AmazonIAPJavaInterfacePtr;
	}
}

#endif
