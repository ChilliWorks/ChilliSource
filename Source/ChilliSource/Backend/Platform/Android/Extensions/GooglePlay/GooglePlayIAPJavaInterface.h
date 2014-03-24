//
//  GooglePlayIAPJavaInterface.h
//  MoFlow
//
//  Created by Scott Downie on 14/06/2013.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_PLATFORM_ANDROID_GOOGLEPLAY_GOOGLEPLAYIAPJAVAINTERFACE_H_
#define _MOFLOW_PLATFORM_ANDROID_GOOGLEPLAY_GOOGLEPLAYIAPJAVAINTERFACE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterface.h>
#include <ChilliSource/Networking/IAP/IAPSystem.h>

namespace ChilliSource
{
	namespace Android
	{
		class GooglePlayIAPJavaInterface : public IJavaInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE(GooglePlayIAPJavaInterface);

			GooglePlayIAPJavaInterface(const std::string& instrPublicKey);

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
            void RequestProductDescriptions(const std::vector<std::string>& inaProductIDs, const Networking::IAPProductDescDelegate& inRequestDelegate);
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
            /// @param Product type (managed, unmanaged)
            //---------------------------------------------------------------
            void RequestProductPurchase(const std::string& instrProductID, Networking::IAPProductRegInfo::Type ineType);

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
            void OnProductDescriptionsRequestComplete(const std::vector<Networking::IAPProductDesc>& inaProducts);
            //---------------------------------------------------------------
    		/// On Transaction Status Updated
            ///
            /// @param Status ID
            /// @param Transaction description
            //---------------------------------------------------------------
            void OnTransactionStatusUpdated(u32 inudwStatus, const Networking::IAPTransaction& inTransaction);
            //---------------------------------------------------------------
    		/// Delegate called when the transaction is closed either
            /// with success or failure
            ///
            /// @author S Downie
            ///
            /// @param Product ID
            /// @param Transaction ID
            /// @param Whether the closing of the transaction was successful
            //---------------------------------------------------------------
            void OnTransactionClosed(std::string instrProductID, std::string instrTransactionID, bool in_success);

		private:

            Networking::IAPProductDescDelegate mProductsRequestDelegate;
            Networking::IAPTransactionDelegate mTransactionStatusDelegate;
            Networking::IAPTransactionCloseDelegate mTransactionCloseDelegate;
		};
	}
}

#endif
