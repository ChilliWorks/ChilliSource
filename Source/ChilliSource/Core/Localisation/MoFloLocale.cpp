/*
 *  Locale.h
 *  moFlo
 *
 *  Created by Ian Copland on 11/07/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Localisation/MoFloLocale.h>

namespace ChilliSource
{
	namespace Core
	{
		Locale::Locale(std::string instrLanguageCode, std::string instrCountryCode)
		{
			mstrLanguageCode = instrLanguageCode;
			mstrCountryCode = instrCountryCode;
			mstrLocaleCode = mstrLanguageCode + "-" + mstrCountryCode;
		}
		Locale::~Locale()
		{

		}
		//---------------------------------------------------------------------
		/// Get Locale Code
		///
		/// @return The full locale code.
		//---------------------------------------------------------------------
		const std::string& Locale::GetLocaleCode() const
		{
			return mstrLocaleCode;
		}
		//---------------------------------------------------------------------
		/// Get Country Code
		///
		/// @return The country code.
		//---------------------------------------------------------------------
		const std::string& Locale::GetCountryCode() const
		{
			return mstrCountryCode;
		}
		//---------------------------------------------------------------------
		/// Get Language Code
		///
		/// @return The language code.
		//---------------------------------------------------------------------
		const std::string& Locale::GetLanguageCode() const
		{
			return mstrLanguageCode;
		}
		//---------------------------------------------------------------------
		/// Set Locale Code
		///
		/// @param The full locale code.
		//---------------------------------------------------------------------
		void Locale::SetLocaleCode(const std::string& instrCode)
		{
			mstrLocaleCode = instrCode;
		}
		//---------------------------------------------------------------------
		/// Set Country Code
		///
		/// @param The country code.
		//---------------------------------------------------------------------
		void Locale::SetCountryCode(const std::string& instrCode)
		{
			mstrCountryCode = instrCode;
		}
		//---------------------------------------------------------------------
		/// Set Language Code
		///
		/// @param The language code.
		//---------------------------------------------------------------------
		void Locale::SetLanguageCode(const std::string& instrCode)
		{
			mstrLanguageCode = instrCode;
		}
	}
}
