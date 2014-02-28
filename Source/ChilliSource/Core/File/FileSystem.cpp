//
//  FileSystem.cpp
//  iOSTemplate
//
//  Created by Scott Downie on 07/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Cryptographic/HashMD5.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>

namespace ChilliSource
{
    namespace Core
    {
        std::string FileSystem::mastrResourceDirectory[3];
        f32 FileSystem::mfAssetsDensity = 1.0f;
        
        ChilliSource::Core::StorageLocation GetStorageLocationFromString(const std::string & instrStorage)
        {
            if(instrStorage == "Package")
                return StorageLocation::k_package;
            else if(instrStorage == "Cache")
                return StorageLocation::k_cache;
            else if(instrStorage == "DLC")
                return StorageLocation::k_DLC;
            else if(instrStorage == "SaveData")
            	return StorageLocation::k_saveData;
            else if(instrStorage == "Root")
            	return StorageLocation::k_root;
            else
                return StorageLocation::k_none;
        }
        
    	CS_DEFINE_NAMEDTYPE(FileSystem);

        const std::string kstrDefaultPackageDLCDirectory = "DLC/";
        
        FileSystem::FileSystem() : mstrPackageDLCPath(kstrDefaultPackageDLCDirectory)
        {
            
        }
		//-------------------------------------------------------------------------
		/// Is A
		//-------------------------------------------------------------------------
		bool FileSystem::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == FileSystem::InterfaceID;
		}
        //--------------------------------------------------------------
        /// Is Storage Location Writeable
        //--------------------------------------------------------------
        bool FileSystem::IsStorageLocationWritable(StorageLocation ineSourceStorageLocation) const
        {
            switch (ineSourceStorageLocation)
            {
                case StorageLocation::k_saveData:
                case StorageLocation::k_cache:
                case StorageLocation::k_DLC:
                case StorageLocation::k_root:
                    return true;
                default:
                    return false;
            }
        }
        //--------------------------------------------------------------
        /// Set Package DLC Directory
        //--------------------------------------------------------------
        void FileSystem::SetPackageDLCDirectory(const std::string& instrDirectory)
        {
            mstrPackageDLCPath = StringUtils::StandardisePath(instrDirectory);
        }
        //--------------------------------------------
        /// Get Device Resource Directory
        ///
        /// @return Directory to load device dependent assets from
        //--------------------------------------------
        const std::string& FileSystem::GetDeviceResourceDirectory()
        {
            return mastrResourceDirectory[0];
        }
        //--------------------------------------------
        /// Get Default Device Resource Directory
        ///
        /// @return Directory to load device dependent assets from
        //--------------------------------------------
        const std::string& FileSystem::GetDefaultDeviceResourceDirectory()
        {
            return mastrResourceDirectory[1];
        }
        //--------------------------------------------
        /// Get Default Resource Directory
        ///
        /// @return Directory to load shared assets from
        //--------------------------------------------
        const std::string& FileSystem::GetDefaultResourceDirectory()
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
        void FileSystem::SetResourceDirectories(const std::string& instrDeviceDirectory, const std::string& instrDefaultDeviceDirectory, const std::string& instrDefaultDirectory, f32 infAssetsDensity)
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
        f32 FileSystem::GetDeviceResourcesDensity()
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
        std::string FileSystem::GetFileMD5Checksum(StorageLocation ineLocation, const std::string& instrFilePath) const
        {
            const u32 kudwChunkSize = 256;
            s8 byData[kudwChunkSize];
            
            HashMD5::MD5 Hash;
            s32 dwSize = kudwChunkSize;
            
            FileStreamSPtr pFile = CreateFileStream(ineLocation, instrFilePath, FileMode::k_readBinary);
            
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
        std::string FileSystem::GetDirectoryMD5Checksum(StorageLocation ineStorageLocation, const std::string& instrDirectory) const
		{
        	std::vector<std::string> astrHashes;
			std::vector<std::string> astrFilenames;
			GetFileNamesInDirectory(ineStorageLocation, instrDirectory, true, astrFilenames);
            
			//get a hash for each of the files.
			for (std::vector<std::string>::iterator it = astrFilenames.begin(); it != astrFilenames.end(); ++it)
			{
                std::string strFileHash = GetFileMD5Checksum(ineStorageLocation, instrDirectory + *it);
                std::string strPathHash = HashMD5::GenerateBinaryHashCode(it->c_str(), it->length());
                astrHashes.push_back(strFileHash);
                astrHashes.push_back(strPathHash);
			}
            
			//sort the list so that ordering of
            std::sort(astrHashes.begin(), astrHashes.end());
            
			//build this into a hashable string
			std::string strHashableDirectoryContents;
			for (std::vector<std::string>::iterator it = astrHashes.begin(); it != astrHashes.end(); ++it)
			{
				strHashableDirectoryContents += (*it);
			}
            
			//return the hash of this as the output
            std::string strOutput = 0;
			if (strHashableDirectoryContents.length() > 0)
            {
				strOutput = HashMD5::GenerateBinaryHashCode(strHashableDirectoryContents.c_str(), strHashableDirectoryContents.length());
            }
			return strOutput;
		}
        //--------------------------------------------------------------
		/// Get Package DLC Directory
		//--------------------------------------------------------------
		const std::string& FileSystem::GetPackageDLCDirectory() const
		{
			return mstrPackageDLCPath;
		}
        //--------------------------------------------------------------
		/// Get File CRC32 Checksum
		//--------------------------------------------------------------
		u32 FileSystem::GetFileCRC32Checksum(StorageLocation ineStorageLocation, const std::string&  instrFilepath) const
		{
			u32 udwOutput = 0;

			//open the file
			FileStreamSPtr pFile = CreateFileStream(ineStorageLocation, instrFilepath, FileMode::k_readBinary);
			if (pFile->IsOpen() == true && pFile->IsBad() == false)
			{
				//get the length of the file
				pFile->SeekG(0, SeekDir::k_end);
				s32 dwLength = pFile->TellG();
				pFile->SeekG(0, SeekDir::k_beginning);

				//read contents of file
				s8* abyContents = new s8[dwLength];
				pFile->Read(abyContents, dwLength);

				//get the hash
				udwOutput = HashCRC32::GenerateHashCode(abyContents, dwLength);
				CS_SAFEDELETE_ARRAY(abyContents);
			}
			return udwOutput;
		}
        //--------------------------------------------------------------
		/// Get Directory CRC32 Checksum
		//--------------------------------------------------------------
		u32 FileSystem::GetDirectoryCRC32Checksum(StorageLocation ineStorageLocation, const std::string&  instrDirectory) const
		{
			std::vector<u32> audwHashes;
			std::vector<std::string> astrFilenames;
			GetFileNamesInDirectory(ineStorageLocation, instrDirectory, true, astrFilenames);

			//get a hash for each of the files.
			for (std::vector<std::string>::iterator it = astrFilenames.begin(); it != astrFilenames.end(); ++it)
			{
				u32 udwFileHash = GetFileCRC32Checksum(ineStorageLocation, instrDirectory + *it);
				u32 udwPathHash = HashCRC32::GenerateHashCode(it->c_str(), it->length());
				audwHashes.push_back(udwFileHash);
				audwHashes.push_back(udwPathHash);
			}

			//sort the list so that ordering of
			std::sort(audwHashes.begin(), audwHashes.end());

			//build this into a hashable string
			std::string strHashableDirectoryContents;
			for (std::vector<u32>::iterator it = audwHashes.begin(); it != audwHashes.end(); ++it)
			{
				strHashableDirectoryContents += ToString(*it);
			}

			//return the hash of this as the output
			u32 udwOutput = 0;
			if (strHashableDirectoryContents.length() > 0)
				udwOutput = HashCRC32::GenerateHashCode(strHashableDirectoryContents.c_str(), strHashableDirectoryContents.length());
			return udwOutput;
		}

        //--------------------------------------------------------------
		/// Get File length
		//--------------------------------------------------------------
		u32 FileSystem::GetFileSize(StorageLocation ineStorageLocation, const std::string&  instrFilepath) const
		{
			//open the file
			FileStreamSPtr pFile = CreateFileStream(ineStorageLocation, instrFilepath, FileMode::k_readBinary);
			if (pFile->IsOpen() == true && pFile->IsBad() == false)
			{
				//get the length of the file
				pFile->SeekG(0, SeekDir::k_end);
				s32 dwLength = pFile->TellG();
				pFile->Close();
				return dwLength;
			}

			return 0;
		}

        //--------------------------------------------------------------
		/// Get Total File size of all Files in Directory
		//--------------------------------------------------------------
		u32 FileSystem::GetDirectorySize(StorageLocation ineStorageLocation, const std::string&  instrDirectory) const
		{
			std::vector<std::string> astrFilenames;
			GetFileNamesInDirectory(ineStorageLocation, instrDirectory, true, astrFilenames);

			u32 udwTotalSize = 0;

			//get a hash for each of the files.
			for (std::vector<std::string>::iterator it = astrFilenames.begin(); it != astrFilenames.end(); ++it)
			{
				u32 udwFileSize = GetFileSize(ineStorageLocation, instrDirectory +"/"+ *it);
				udwTotalSize += udwFileSize;
			}


			u32 udwTotalFiles = astrFilenames.size();
			CS_LOG_DEBUG("Total Files = " + ToString(udwTotalFiles) + ", Total Size = " + ToString(udwTotalSize));
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
        void FileSystem::GetBestPathToFile(Core::StorageLocation ineStorageLocation, const std::string& instrFileName, std::string& outFilePath) const
        {
            if(ineStorageLocation == Core::StorageLocation::k_package)
            {
                outFilePath = GetDirectoryForPackageFile(instrFileName);
            }
            else if(ineStorageLocation == Core::StorageLocation::k_DLC)
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
