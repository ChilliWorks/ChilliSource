/*
 *  ContactInformationProvider.cpp
 *  moFlo
 *
 *  Created by Stuart McGaw on 08/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/iOS/Social/Communications/ContactInformationProvider.h>

#include <ChilliSource/Core/String/StringUtils.h>

#include <AddressBook/AddressBook.h>
#include <Foundation/NSString.h>

namespace ChilliSource{

	namespace iOS {
		bool ContactInformationProvider::IsA(Core::InterfaceIDType inID) const{
			return inID == ContactInformationProvider::InterfaceID;
		}

		void ContactInformationProvider::LoadInformation(){
			masContacts.clear();
			

			ABAddressBookRef addressBook = ABAddressBookCreate( );
			CFArrayRef allPeople = ABAddressBookCopyArrayOfAllPeople( addressBook );
			CFIndex nPeople = ABAddressBookGetPersonCount( addressBook );
			
			for ( int i = 0; i < nPeople; i++ ){
				ABRecordRef personRef = CFArrayGetValueAtIndex( allPeople, i );
				
				NSString* pPersonFirstName = (NSString*)ABRecordCopyValue(personRef, kABPersonFirstNameProperty);
				NSString* pPersonLastName = (NSString*)ABRecordCopyValue(personRef, kABPersonLastNameProperty);
				NSString* pCombinedName = [NSString stringWithFormat:@"%@ %@", pPersonFirstName, pPersonLastName];
				
				[pPersonFirstName release];
				[pPersonLastName release];
				
				ABMultiValueRef pPersonPhone = (ABMultiValueRef)ABRecordCopyValue(personRef, kABPersonPhoneProperty);
				
				NSString * pMobileNumber = nil;
				
				for (u32 nPhoneNumber = 0; nPhoneNumber < (u32)ABMultiValueGetCount(pPersonPhone); nPhoneNumber++){
				
					NSString * pLabel = (NSString*)ABMultiValueCopyLabelAtIndex(pPersonPhone, nPhoneNumber); 
					NSString * pValue = (NSString*)ABMultiValueCopyValueAtIndex(pPersonPhone, nPhoneNumber);
										
					if ([pLabel isEqualToString:(NSString*)kABPersonPhoneMobileLabel] || [pLabel isEqualToString:(NSString*)kABPersonPhoneIPhoneLabel]){
						pMobileNumber = pValue;	
					} else {
						[pValue release];
					}

					[pLabel release];
				}
				
				NSString * pEmail = nil;
				
				ABMultiValueRef pPersonEmail = (ABMultiValueRef )ABRecordCopyValue(personRef, kABPersonEmailProperty);

				for (u32 nEmail = 0; nEmail < (u32)ABMultiValueGetCount(pPersonEmail); nEmail++){
					
					NSString * pLabel = (NSString*)ABMultiValueCopyLabelAtIndex(pPersonEmail, nEmail); 
					NSString * pValue = (NSString*)ABMultiValueCopyValueAtIndex(pPersonEmail, nEmail);
					
					if ([pLabel isEqualToString:(NSString*)kABHomeLabel] || [pLabel isEqualToString:(NSString*)kABOtherLabel]){
						pEmail = pValue;	
					} else {
						[pValue release];
					}
					
					[pLabel release];
				}				
				
				Social::ContactRecord sPersonRecord;
				
				sPersonRecord.strName = Core::StringUtils::NSStringToUTF8String(pCombinedName);
				sPersonRecord.bHasEmail = pEmail != nil;
				if (sPersonRecord.bHasEmail){
					sPersonRecord.strEmail = Core::StringUtils::NSStringToUTF8String(pEmail);
				}
				
				sPersonRecord.bHasMobileNumber = pMobileNumber != nil;
				if (sPersonRecord.bHasMobileNumber){
					sPersonRecord.strMobileNumber = Core::StringUtils::NSStringToUTF8String(pMobileNumber);
				}				
								
				masContacts.push_back(sPersonRecord);
				
				[pEmail release];
				[pMobileNumber release];
				CFRelease(pPersonPhone);
				CFRelease(pPersonEmail);
			}
			
		}
		
		u32 ContactInformationProvider::GetNumberContacts(){
			return masContacts.size();
		}
		const Social::ContactRecord & ContactInformationProvider::GetContactAtIndex(u32 inudwIndex){
			return masContacts[inudwIndex];
		}
		
	}
	
}

