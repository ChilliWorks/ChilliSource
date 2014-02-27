//
//  Locales.h
//  moFloTest
//
//  Created by Scott Downie on 18/03/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


#ifndef _MO_FLO_CORE_LOCALES_H_
#define _MO_FLO_CORE_LOCALES_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Localisation/MoFloLocale.h>

namespace ChilliSource
{
    namespace Core
    {
    	//---------------------------------------------
		/// This provides some default locales to
    	/// compare against.
		//---------------------------------------------
        const Locale kUnknownLocale("unknown");
        const Locale kEnglishLocale("en");
        const Locale kFrenchLocale("fr");
        const Locale kGermanLocale("de");
        const Locale kItalianLocale("it");
        const Locale kSpanishLocale("es");
        const Locale kPolishLocale("pl");
        const Locale kCzechLocale("cs");
        const Locale kRussianLocale("ru");
        const Locale kJapaneseLocale("jp");
    }
}

#endif
