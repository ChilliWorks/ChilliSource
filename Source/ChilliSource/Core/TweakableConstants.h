#ifndef _MOFLO_CORE_TWEAKABLECONSTANTS_H
#define _MOFLO_CORE_TWEAKABLECONSTANTS_H

#include <string>
#include <moFlo/Core/XML/tinyxml.h>

#define USETWEAKABLECONSTANTS

#ifdef USETWEAKABLECONSTANTS

//Constants in root namespace
#define TC_FLOAT(name) ::moFlo::Core::CTweakableConstants::GetSingleton().GetFloat(#name)
#define TC_INT(name) ::moFlo::Core::CTweakableConstants::GetSingleton().GetInt(#name)
#define TC_STR(name) ::moFlo::Core::CTweakableConstants::GetSingleton().GetString(#name)
#define TC_BOOL(name) ::moFlo::Core::CTweakableConstants::GetSingleton().GetBool(#name)
#define TC_SAMPLECURVE(name,value) ::moFlo::Core::CTweakableConstants::GetSingleton().SampleCurve(#name,value)

//Constants in namespace
#define TC_FLOATNS(namespace,name) ::moFlo::Core::CTweakableConstants::GetSingleton().GetFloat(#namespace,#name)
#define TC_INTNS(namespace,name) ::moFlo::Core::CTweakableConstants::GetSingleton().GetInt(#namespace,#name)
#define TC_STRNS(namespace,name) ::moFlo::Core::CTweakableConstants::GetSingleton().GetString(#namespace,#name)
#define TC_BOOLNS(namespace,name) ::moFlo::Core::CTweakableConstants::GetSingleton().GetBool(#namespace,#name)
#define TC_SAMPLECURVENS(namespace,name,value) ::moFlo::Core::CTweakableConstants::GetSingleton().SampleCurve(#namespace,#name,value)

namespace moFlo
{
	namespace Core
	{
		
		class CTweakableConstants
		{
		public: 
			CTweakableConstants();
			~CTweakableConstants();
			static CTweakableConstants & GetSingleton();
			static CTweakableConstants * GetSingletonPtr();
			
			void AddSourceFile(STORAGE_LOCATION ineStorageLocation, const std::string & inFileName, bool inbLoadNow = true);
			
			void Reload();
			
			f32 GetFloat(const std::string & inNameSpace,const std::string & inConstName) const;
			f32 GetFloat(const std::string & inConstName) const;
			
			s32 GetInt(const std::string & inNameSpace, const std::string & inConstName) const;
			s32 GetInt(const std::string & inConstName) const;
			
			const std::string & GetString(const std::string & inNameSpace, const std::string & inConstName) const;
			const std::string & GetString(const std::string & inConstName) const;
			
			bool GetBool(const std::string & inNameSpace, const std::string & inConstName) const;
			bool GetBool(const std::string & inConstName) const;
			
			f32 SampleCurve(const std::string & inNameSpace, const std::string & inCurveName, f32 infXValue) const;
			f32 SampleCurve(const std::string & inCurveName, f32 infXValue) const;
			
			struct CurvePoint {
				f32 X;
				f32 Y;
			};
		protected:
			static CTweakableConstants * msSingleton;
			void LoadValues();
			void LoadFile(STORAGE_LOCATION ineStorageLocation, const std::string & inFileName);
			
			void ParseFileElement(TiXmlElement * inpElement, const std::string & outCurrentNamespace);
			

			
			typedef DYNAMIC_ARRAY<CurvePoint> CurvePointVector;
			
			typedef HASH_MAP<std::string,float> MapNameToFloat;
			typedef MapNameToFloat::const_iterator MapNameToFloatItr;
			typedef HASH_MAP<std::string,int> MapNameToInt;
			typedef MapNameToInt::const_iterator MapNameToIntItr;
			typedef HASH_MAP<std::string,std::string> MapNameToString;
			typedef MapNameToString::const_iterator MapNameToStringItr;
			typedef HASH_MAP<std::string,bool> MapNameToBool;
			typedef MapNameToBool::const_iterator MapNameToBoolItr;
			
			typedef HASH_MAP<std::string,CurvePointVector> MapNameToCurvePoints;
			typedef MapNameToCurvePoints::const_iterator MapNameToCurvePointsItr;

			struct ConstantsGroup{
				MapNameToFloat Floats;
				MapNameToInt Ints;
				MapNameToString Strings;
				MapNameToBool Bools;
				MapNameToCurvePoints Curves;
			};
			
			typedef HASH_MAP<std::string,ConstantsGroup> MapNamespaceToConstantsGroup;
			typedef MapNamespaceToConstantsGroup::iterator MapNamespaceToConstantsGroupItr;
			typedef MapNamespaceToConstantsGroup::const_iterator MapNamespaceToConstantsGroupCItr;

			MapNamespaceToConstantsGroup mNamespaces;
			
			DYNAMIC_ARRAY<Core::StorageLocationAndFilenamePtr> mFileNames;
			std::string mBlank;//blank string to return for failed string constant lookups
		};
		
		
	}
}

#else
#include "BakedConstants.h"
#define TC_FLOAT(x) x
#define TC_INT(x) x
#define TC_STR(x) x
#define TC_BOOL(x) x

#endif //USETWEAKABLECONSTANTS

#endif //TWEAKABLECONSTANTS_H
