//
//  FileSystem.cpp
//  iOSTemplate
//
//  Created by Scott Downie on 07/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <moFlo/Core/FileIO/FileSystem.h>
#include <moFlo/Core/StringUtils.h>
#include <moFlo/Core/HashMD5.h>
#include <moFlo/Core/HashCRC32.h>

namespace moFlo
{
    namespace Core
    {
        std::string IFileSystem::mastrResourceDirectory[3];
        f32 IFileSystem::mfAssetsDensity = 1.0f;
        
        moFlo::Core::STORAGE_LOCATION GetStorageLocationFromString(const std::string & instrStorage)
        {
            if(instrStorage == "Package")
                return SL_PACKAGE;
            else if(instrStorage == "Cache")
                return SL_CACHE;
            else if(instrStorage == "DLC")
                return SL_DLC;
            else if(instrStorage == "SaveData")
            	return SL_SAVEDATA;
            else if(instrStorage == "Root")
            	return SL_ROOT;
            else
                return SL_NONE;
        }
        
    	DEFINE_NAMED_INTERFACE(IFileSystem);

        const std::string kstrDefaultPackageDLCDirectory = "DLC/";
        
        IFileSystem::IFileSystem() : mstrPackageDLCPath(kstrDefaultPackageDLCDirectory)
        {
            
        }
		//-------------------------------------------------------------------------
		/// Is A
		//-------------------------------------------------------------------------
		bool IFileSystem::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IFileSystem::InterfaceID;
		}
        //--------------------------------------------------------------
        /// Is Storage Location Writeable
        //--------------------------------------------------------------
        bool IFileSystem::IsStorageLocationWritable(STORAGE_LOCATION ineSourceStorageLocation) const
        {
            switch (ineSourceStorageLocation)
            {
                case SL_SAVEDATA:
                case SL_CACHE:
                case SL_DLC:
                case SL_ROOT:
                    return true;
                default:
                    return false;
            }
        }
        //--------------------------------------------------------------
        /// Set Package DLC Directory
        //--------------------------------------------------------------
        void IFileSystem::SetPackageDLCDirectory(const std::string& instrDirectory)
        {
            mstrPackageDLCPath = CStringUtils::StandardisePath(instrDirectory);
        }
        //--------------------------------------------
        /// Get Device Resource Directory
        ///
        /// @return Directory to load device dependent assets from
        //--------------------------------------------
        const std::string& IFileSystem::GetDeviceResourceDirectory()
        {
            return mastrResourceDirectory[0];
        }
        //--------------------------------------------
        /// Get Default Device Resource Directory
        ///
        /// @return Directory to load device dependent assets from
        //--------------------------------------------
        const std::string& IFileSystem::GetDefaultDeviceResourceDirectory()
        {
            return mastrResourceDirectory[1];
        }
        //--------------------------------------------
        /// Get Default Resource Directory
        ///
        /// @return Directory to load shared assets from
        //--------------------------------------------
        const std::string& IFileSystem::GetDefaultResourceDirectory()
        {
            return mastrResourceDirectory[2];
        }
        //--------------------------------------------
        /// Set Resource Directories
        ///
        /// @param Directory to load device dependent assets from
        /// @param Fallback to load device depenedent assets from
        /// @param Directory to load shared assets from
        /// @param Density of assets in the device dependent folder
        //--------------------------------------------
        void IFileSystem::SetResourceDirectories(const std::string& instrDeviceDirectory, const std::string& instrDefaultDeviceDirectory, const std::string& instrDefaultDirectory, f32 infAssetsDensity)
        {
            mastrResourceDirectory[0] = instrDeviceDirectory;
            if (!instrDeviceDirectory.empty())
                mastrResourceDirectory[0] += "/";
            mastrResourceDirectory[1] = instrDefaultDeviceDirectory;
            if (!instrDefaultDeviceDirectory.empty())
                mastrResourceDirectory[1] += "/";
            mastrResourceDirectory[2] = instrDefaultDirectory;
            if (!instrDefaultDirectory.empty())
                mastrResourceDirectory[2] += "/";
            
            mfAssetsDensity = infAssetsDensity;
        }
        //--------------------------------------------
        /// Get Resources Density
        ///
        /// @return Density of assets in device
        /// dependent folder
        //--------------------------------------------
        f32 IFileSystem::GetDeviceResourcesDensity()
        {
            return mfAssetsDensity;
        }
        //--------------------------------------------------------------
        /// Get File MD5 Checksum 
        ///
        /// Calculate the MD5 checksum of the file at the
        /// given directory
        ///
        /// @param Storage location
        /// @param File path
        /// @return MD5 checksum
        //--------------------------------------------------------------
        std::string IFileSystem::GetFileMD5Checksum(STORAGE_LOCATION ineLocation, const std::string& instrFilePath) const
        {
            const u32 kudwChunkSize = 256;
            s8 byData[kudwChunkSize];
            
            CHashMD5::MD5 Hash;
            s32 dwSize = kudwChunkSize;
            
            FileStreamPtr pFile = CreateFileStream(ineLocation, instrFilePath, FM_READ_BINARY);
            
            if(!pFile->IsOpen() || pFile->IsBad())
            {
                return "";
            }
            
            while(dwSize != 0)
            {
                dwSize = pFile->ReadSome(byData, kudwChunkSize);
                Hash.update(byData, dwSize);
            }
            
            Hash.finalize();
            return Hash.binarydigest();
        }
        //--------------------------------------------------------------
        /// Get Directory MD5 Checksum 
        ///
        /// Calculate the MD5 checksum of the given directory
        ///
        /// @param Storage location
        /// @param File path
        /// @return MD5 checksum
        //--------------------------------------------------------------
        std::string IFileSystem::GetDirectoryMD5Checksum(STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory) const
		{
        	DYNAMIC_ARRAY<std::string> astrHashes;
			DYNAMIC_ARRAY<std::string> astrFilenames;
			GetFileNamesInDirectory(ineStorageLocation, instrDirectory, true, astrFilenames);
            
			//get a hash for each of the files.
			for (DYNAMIC_ARRAY<std::string>::iterator it = astrFilenames.begin(); it != astrFilenames.end(); ++it)
			{
                std::string strFileHash = GetFileMD5Checksum(ineStorageLocation, instrDirectory + *it);
                std::string strPathHash = CHashMD5::GenerateBinaryHashCode(it->c_str(), it->length());
                astrHashes.push_back(strFileHash);
                astrHashes.push_back(strPathHash);
			}
            
			//sort the list so that ordering of
            std::sort(astrHashes.begin(), astrHashes.end());
            
			//build this into a hashable string
			std::string strHashableDirectoryContents;
			for (DYNAMIC_ARRAY<std::string>::iterator it = astrHashes.begin(); it != astrHashes.end(); ++it)
			{
				strHashableDirectoryContents += (*it);
			}
            
			//return the hash of this as the output
            std::string strOutput = 0;
			if (strHashableDirectoryContents.length() > 0)
            {
				strOutput = CHashMD5::GenerateBinaryHashCode(strHashableDirectoryContents.c_str(), strHashableDirectoryContents.length());
            }
			return strOutput;
		}
        //--------------------------------------------------------------
		/// Get Package DLC Directory
		//--------------------------------------------------------------
		const std::string& IFileSystem::GetPackageDLCDirectory() const
		{
			return mstrPackageDLCPath;
		}
        //--------------------------------------------------------------
		/// Get File CRC32 Checksum
		//--------------------------------------------------------------
		u32 IFileSystem::GetFileCRC32Checksum(STORAGE_LOCATION ineStorageLocation, const std::string&  instrFilepath) const
		{
			u32 udwOutput = 0;

			//open the file
			FileStreamPtr pFile = CreateFileStream(ineStorageLocation, instrFilepath, FM_READ_BINARY);
			if (pFile->IsOpen() == true && pFile->IsBad() == false)
			{
				//get the length of the file
				pFile->SeekG(0, SD_END);
				s32 dwLength = pFile->TellG();
				pFile->SeekG(0, SD_BEGINNING);

				//read contents of file
				s8* abyContents = new s8[dwLength];
				pFile->Read(abyContents, dwLength);

				//get the hash
				udwOutput = CHashCRC32::GenerateHashCode(abyContents, dwLength);
				SAFE_DELETE_ARRAY(abyContents);
			}
			return udwOutput;
		}
        //--------------------------------------------------------------
		/// Get Directory CRC32 Checksum
		//--------------------------------------------------------------
		u32 IFileSystem::GetDirectoryCRC32Checksum(STORAGE_LOCATION ineStorageLocation, const std::string&  instrDirectory) const
		{
			DYNAMIC_ARRAY<u32> audwHashes;
			DYNAMIC_ARRAY<std::string> astrFilenames;
			GetFileNamesInDirectory(ineStorageLocation, instrDirectory, true, astrFilenames);

			//get a hash for each of the files.
			for (DYNAMIC_ARRAY<std::string>::iterator it = astrFilenames.begin(); it != astrFilenames.end(); ++it)
			{
				u32 udwFileHash = GetFileCRC32Checksum(ineStorageLocation, instrDirectory + *it);
				u32 udwPathHash = CHashCRC32::GenerateHashCode(it->c_str(), it->length());
				audwHashes.push_back(udwFileHash);
				audwHashes.push_back(udwPathHash);
			}

			//sort the list so that ordering of
			std::sort(audwHashes.begin(), audwHashes.end());

			//build this into a hashable string
			std::string strHashableDirectoryContents;
			for (DYNAMIC_ARRAY<u32>::iterator it = audwHashes.begin(); it != audwHashes.end(); ++it)
			{
				strHashableDirectoryContents += STRING_CAST(*it);
			}

			//return the hash of this as the output
			u32 udwOutput = 0;
			if (strHashableDirectoryContents.length() > 0)
				udwOutput = CHashCRC32::GenerateHashCode(strHashableDirectoryContents.c_str(), strHashableDirectoryContents.length());
			return udwOutput;
		}

        //--------------------------------------------------------------
		/// Get File length
		//--------------------------------------------------------------
		u32 IFileSystem::GetFileSize(STORAGE_LOCATION ineStorageLocation, const std::string&  instrFilepath) const
		{
			//open the file
			FileStreamPtr pFile = CreateFileStream(ineStorageLocation, instrFilepath, FM_READ_BINARY);
			if (pFile->IsOpen() == true && pFile->IsBad() == false)
			{
				//get the length of the file
				pFile->SeekG(0, SD_END);
				s32 dwLength = pFile->TellG();
				pFile->Close();
				return dwLength;
			}

			return 0;
		}

        //--------------------------------------------------------------
		/// Get Total File size of all Files in Directory
		//--------------------------------------------------------------
		u32 IFileSystem::GetDirectorySize(STORAGE_LOCATION ineStorageLocation, const std::string&  instrDirectory) const
		{
			DYNAMIC_ARRAY<std::string> astrFilenames;
			GetFileNamesInDirectory(ineStorageLocation, instrDirectory, true, astrFilenames);

			u32 udwTotalSize = 0;

			//get a hash for each of the files.
			for (DYNAMIC_ARRAY<std::string>::iterator it = astrFilenames.begin(); it != astrFilenames.end(); ++it)
			{
				u32 udwFileSize = GetFileSize(ineStorageLocation, instrDirectory +"/"+ *it);
				udwTotalSize += udwFileSize;
			}


			u32 udwTotalFiles = astrFilenames.size();
			DEBUG_LOG("Total Files = " + STRING_CAST(udwTotalFiles) + ", Total Size = " + STRING_CAST(udwTotalSize));
			return udwTotalSize;
		}
        
        //--------------------------------------------------------------
        /// Get Best Path To File
        ///
        /// @param Storage location
        /// @param File name
        /// @param Out: The path to the most up to date file with the
        /// given name. This argument is unchanged if file is not found
        //--------------------------------------------------------------
        void IFileSystem::GetBestPathToFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFileName, std::string& outFilePath) const
        {
            if(ineStorageLocation == Core::SL_PACKAGE)
            {
                outFilePath = GetDirectoryForPackageFile(instrFileName);
            }
            else if(ineStorageLocation == Core::SL_DLC)
            {
                outFilePath = GetDirectoryForDLCFile(instrFileName);
            }
            
            if(outFilePath.empty())
            {
                outFilePath = GetStorageLocationDirectory(ineStorageLocation) + instrFileName;
            }
        }
    }
}
