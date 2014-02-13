package com.taggames.moflow.nativeinterface;

import java.util.ArrayList;
import android.app.Activity;
import android.content.ContentResolver;
import android.database.Cursor;
import android.provider.ContactsContract;
import android.provider.ContactsContract.CommonDataKinds.Email;
import android.provider.ContactsContract.CommonDataKinds.Phone;
import android.provider.ContactsContract.Data;
import android.provider.ContactsContract.RawContacts;

public class CContactInformationProviderNativeInterface
{
	private static Activity msActivity;
	private static ContentResolver msContentResolver;
	private static ArrayList<String> mastrNames;
	private static ArrayList<String> mastrNumbers;
	private static ArrayList<String> mastrEmails;
	
	native public static void SetData(int inudwSize,String[] inastrNames,String[] inastrNumbers,String[] inastrEmails);
	
	//---------------------------------------------------------------------
	/// Setup
	///
	/// Initialises the contact provider native interface
	///
	/// @param this applications main activity.
	//---------------------------------------------------------------------
	public static void Setup(Activity inActivity)
	{
		msActivity = inActivity;
		msContentResolver = msActivity.getContentResolver();
		
		mastrNames = new ArrayList<String>();
		mastrNumbers = new ArrayList<String>();
		mastrEmails = new ArrayList<String>();
		
	}
	
	//---------------------------------------------------------------------
	/// Load Information
	///
	/// Creates arrays for contact data and returns this to native
	/// through native callbacks
	///
	//---------------------------------------------------------------------
	public static void LoadInformation()
	{		
		String[] astrProjection = new String[]{RawContacts.CONTACT_ID,RawContacts.DELETED};
		
		Cursor sRawContacts = msContentResolver.query(ContactsContract.RawContacts.CONTENT_URI, astrProjection, null, null, null);
		
		int udwIDIndex = sRawContacts.getColumnIndex(RawContacts.CONTACT_ID);
		int udwDeletedIndex = sRawContacts.getColumnIndex(RawContacts.DELETED);
		
		if(sRawContacts.moveToFirst())
		{
			while(!sRawContacts.isAfterLast())
			{		// still a valid entry left?
				final int udwID = sRawContacts.getInt(udwIDIndex);
				final boolean bDeleted = (sRawContacts.getInt(udwDeletedIndex) == 1);
				if(!bDeleted) 
				{
					StoreContact(udwID);
				}
				sRawContacts.moveToNext();			
			}
		}
		sRawContacts.close();
		if(mastrNames.size() == mastrNumbers.size() && mastrNames.size() == mastrEmails.size())
		{
			String[] astrNames = new String[mastrNames.size()];
			mastrNames.toArray(astrNames);
			String[] astrNumbers = new String[mastrNumbers.size()];
			mastrNumbers.toArray(astrNumbers);
			String[] astrEmails = new String[mastrEmails.size()];
			mastrEmails.toArray(astrEmails);			
			SetData(mastrNames.size(),astrNames,astrNumbers,astrEmails);
		}
	}
	//---------------------------------------------------------------------
	/// Store Contact
	///
	/// Stores the name,number and email address
	/// of the contact
	///
	/// @param contact ID
	//---------------------------------------------------------------------
	public static void StoreContact(int inudwContactID)
	{			
		mastrNames.add(GetName(inudwContactID));
		mastrNumbers.add(GetNumber(inudwContactID));
		mastrEmails.add(GetEmailAddress(inudwContactID));	
	}
	//---------------------------------------------------------------------
	/// Get Email address
	///
	/// Retrieves the email address of the specified
	/// contact
	///
	/// @param contact ID
	//---------------------------------------------------------------------
	public static String GetEmailAddress(int inudwContactID)
	{
		String[] astrProjection = new String[]{Email.DATA};
		Cursor sEmailInfo = msContentResolver.query(Email.CONTENT_URI,astrProjection,Data.CONTACT_ID + "=?",new String[]{String.valueOf(inudwContactID)},null);
		if(sEmailInfo.moveToFirst())
		{
			final int udwEmailColumnIndex = sEmailInfo.getColumnIndex(Email.DATA);
			if(udwEmailColumnIndex != -1)
			{
				final String strAddress = sEmailInfo.getString(udwEmailColumnIndex);
				sEmailInfo.close();
				return strAddress;
			}
		}
		sEmailInfo.close();
		return "";
	}
	//---------------------------------------------------------------------
	/// Get Name
	///
	/// Retrieves the name of the specified
	/// contact
	///
	/// @param contact ID
	//---------------------------------------------------------------------		
	public static String GetName(int inudwContactID)
	{
		String[] astrProjection = new String[]{"display_name"};
		Cursor sContactsInfo = msContentResolver.query(Phone.CONTENT_URI,astrProjection,Data.CONTACT_ID + "=?",new String[]{String.valueOf(inudwContactID)},null);
		if(sContactsInfo.moveToFirst())
		{
			final int udwNameColumnIndex = sContactsInfo.getColumnIndex("display_name");
			if(udwNameColumnIndex != -1)
			{
				final String strName = sContactsInfo.getString(udwNameColumnIndex);
				sContactsInfo.close();
				return strName;
			}
		}
		sContactsInfo.close();
		return "";
	}
	
	//---------------------------------------------------------------------
	/// Get Number
	///
	/// Retrieves the number of the specified
	/// contact
	///
	/// @param contact ID
	//---------------------------------------------------------------------
	public static String GetNumber(int inudwContactID)
	{
		String[] astrProjection = new String[]{Phone.DATA,Phone.TYPE};	
		Cursor sNumberInfo = msContentResolver.query(Phone.CONTENT_URI,astrProjection,Data.CONTACT_ID + "=?",new String[]{String.valueOf(inudwContactID)},null);
		if(sNumberInfo.moveToFirst())
		{
			final int dwNumberColumnIndex = sNumberInfo.getColumnIndex(Phone.DATA);
			final int dwNumberType = sNumberInfo.getColumnIndex(Phone.TYPE);
			if(dwNumberColumnIndex != -1 && dwNumberColumnIndex != -1)
			{
				final int dwType = sNumberInfo.getInt(dwNumberType);
				if(dwType == Phone.TYPE_MOBILE)
				{
					final String strNumber = sNumberInfo.getString(dwNumberColumnIndex);
					sNumberInfo.close();
					return strNumber;
				}
			}
			sNumberInfo.moveToNext();
		}
		sNumberInfo.close();
		return "";
	}
}
