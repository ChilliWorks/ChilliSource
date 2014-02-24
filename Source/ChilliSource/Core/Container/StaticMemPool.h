#ifndef _MO_FLO_CORE_STATIC_MEM_POOL_H_
#define _MO_FLO_CORE_STATIC_MEM_POOL_H_

namespace ChilliSource
{
    namespace Core
    {
        //==================================================================================
        /// Static Memory Pool
        ///
        /// Allocates a block from the heap allowing for allocation from the pool. The
        /// pool speeds up allocation, deallocation and reduces memory fragmentation.
        /// Each block of memory allocated is efficiently aligned
        //==================================================================================
        const u32 kudwBlockAlignmentSize = 16;	//16 byte aligned

        class CStaticMemPool
        {
        public:
            //----------------------------------------------------------------------
            /// Constructor
            ///
            /// Allocate a fixed size pool from the heap
            ///
            /// @param Size of memory pool in bytes
            /// @param Alignment in bytes
            //----------------------------------------------------------------------
            CStaticMemPool(u32 inudwHeapSize, u32 inudwAlignmentSize = kudwBlockAlignmentSize);
            //----------------------------------------------------------------------
            /// Destructor
            //----------------------------------------------------------------------
            ~CStaticMemPool();

            //----------------------------------------------------------------------
            /// Allocate
            ///
            /// Allocate a block of memory from the pool of the given size
            ///
            /// @param Size in bytes
            /// @return Pointer to block of memory
            //----------------------------------------------------------------------
            template <typename T> T* Allocate(u32 inudwSize)
            {
                if(mudwFreeMemory < inudwSize)
                {
                    ERROR_LOG("Static mem pool out of memory");
                    return NULL;
                }

                //Work out how many chunks are required based on the size requested
                u32 udwNumChunks = (inudwSize/mudwAlignmentSize + 1);

                //Find a free chunk
                for(DYNAMIC_ARRAY<MemChunk>::iterator it = mChunks.begin(); it != mChunks.end(); ++it)
                {
                    if(!it->bIsAllocated)
                    {
                        bool bFoundBlock = true;
                        //Found a chunk that is not in use
                        for(u32 i=1; i<udwNumChunks; ++i)
                        {
                            if((it + i)->bIsAllocated)
                            {
                                bFoundBlock = false;
                                break;
                            }
                        }

                        if(bFoundBlock)
                        {
                            //The data will be split between blocks and the udwDataSize
                            //value will be incorrect for each block. However it will let us know
                            //how many blocks need to be freed
                            it->udwDataSize = inudwSize;
                            mudwFreeMemory -= inudwSize;
                            mudwUsedMemory += inudwSize;

                            for(u32 i=0; i<udwNumChunks; ++i)
                            {
                                (it + i)->bIsAllocated = true;
                            }

                            return (T*)it->pbyData;
                        }
                    }
                }

                return NULL;
            }
            //-----------------------------------------------------------------------
            /// Release
            ///
            /// Give the memory held by the object back to the memory pool
            ///
            /// @param Pointer to memory held by pool
            //-----------------------------------------------------------------------
            void Release(void* inpData);

        private:

            struct MemChunk
            {
                u32 udwDataSize;

                s8* pbyData;

                bool bIsAllocated;
            };

            u32 mudwAlignmentSize;
            u32 mudwFreeMemory;
            u32 mudwUsedMemory;

            s8* mpMemPool;

            DYNAMIC_ARRAY<MemChunk> mChunks;
        };
    }
}

#endif