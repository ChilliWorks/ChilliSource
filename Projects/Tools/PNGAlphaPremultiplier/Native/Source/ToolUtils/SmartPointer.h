/**
 * SmartPointer.h
 * ToolUtils
 * 
 * Created by Ian Copland on 15/03/2012
 * Copyright ©2010 Tag Games Limited. All rights reserved
 */

#ifndef _TOOLUTILS_SMARTPOINTER_H_
#define _TOOLUTILS_SMARTPOINTER_H_

namespace ToolUtils
{
	//============================================
	/// Smart Pointer
	///
	/// A simple smart pointer.
	//============================================
	template <class T> class CSmartPointer
	{
	protected:
		T * mpPointer;
		int * mpRefCount;
		
	public:
		//-----------------------------------------------
		/// Constructor
		//-----------------------------------------------
		CSmartPointer()
		{
			mpPointer = NULL;
			mpRefCount = NULL;
		}
		//-----------------------------------------------
		/// Constructor
		///
		/// Takes a reference to the passed in class and 
		/// sets the reference count to 1.
		///
		/// @param a pointer to an instance of T.
		//-----------------------------------------------
		CSmartPointer(T * inpObject)
		{
			mpPointer = inpObject;
			mpRefCount = new int;
			(*mpRefCount) = 1;
		}
		//-----------------------------------------------
		/// Constructor
		///
		/// This takes a COPY of the class parameter and 
		/// sets the reference count to 1.
		///
		/// @param a reference to an instance of T.
		//-----------------------------------------------
		CSmartPointer(const T inpObject)
		{
			mpPointer = new T;
			*mpPointer = inpObject;
			mpRefCount = new int;
			(*mpRefCount) = 1;
		}
		//-----------------------------------------------
		/// Copy Constructor
		///
		/// Copys the contents of the passed in smart 
		/// pointer and incriments the reference count.
		///
		/// @param another smart pointer of the same type
		///		   T.
		//-----------------------------------------------
		CSmartPointer(const CSmartPointer<T> & inpSmartPointer)
		{
			mpPointer = inpSmartPointer.mpPointer;
			mpRefCount = inpSmartPointer.mpRefCount;
			(*mpRefCount)++;
		}
		//-----------------------------------------------
		/// Cast
		///
		/// This is used to cast the pointer. For example 
		/// this would be used it you had an instance of 
		/// DerrivedClass stored as a BaseClass. The 
		/// contents of DerrivedClass "A" could then be 
		/// accessed by calling:
		///	A.Cast<DerrivedClass>()->DerrivedClassFunction()
		//-----------------------------------------------
		template <class U> U * Cast()
		{
			return (U*)mpPointer;
		}
		//-----------------------------------------------
		/// Is Null
		///
		/// Returns whether the pointer is currently NULL. 
		/// Through proper use of the class this should 
		/// rarely be needed.
		///
		/// @return whether or not this has a null pointer.
		//-----------------------------------------------
		bool IsNull()
		{
			if (mpPointer == NULL)
				return true;
			return false;
		}
		//-----------------------------------------------
		/// Delete
		///
		/// This lowers the reference count to the pointer. 
		/// If the reference count hits 0, then the memory 
		/// will be deallocated.
		//-----------------------------------------------
		void Delete()
		{
			//if ref count exists
			if (mpRefCount != NULL)
			{
				//decriment the ref count
				(*mpRefCount)--;

				//if ref count is 0 then release the object
				if (*mpRefCount < 1)
				{
					//if the object exists, delete.
					if (mpPointer != NULL)
					{
						delete mpPointer;
						mpPointer = NULL;
					}

					//delete the reference counter
					delete mpRefCount;
					mpRefCount = NULL;
				}
			}
			//if object somehow exists even though the reference count does not, delete it.
			else if (mpPointer != NULL)
			{
				delete mpPointer;
			}

			//set to null
			mpPointer = NULL;
			mpRefCount = NULL;
		}
		//-----------------------------------------------
		/// Destructor
		//-----------------------------------------------
		~CSmartPointer()
		{
			Delete();
		}
		//-----------------------------------------------
		/// Equals Operator
		//-----------------------------------------------
		void operator=(const CSmartPointer<T> & inpSmartPointer)
		{
			if (mpPointer != NULL || mpRefCount != NULL)
				Delete();
			mpPointer = inpSmartPointer.mpPointer;
			mpRefCount = inpSmartPointer.mpRefCount;
			(*mpRefCount)++;
		}
		//-----------------------------------------------
		/// Indirection operator
		//-----------------------------------------------
		T& operator*()
		{
			return *mpPointer;
		}
		//-----------------------------------------------
		/// Structure dereference operator
		//-----------------------------------------------
		T * operator->()
		{
			return mpPointer;
		}
	};
}

#endif
