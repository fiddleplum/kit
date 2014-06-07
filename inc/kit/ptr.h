#pragma once

#include <exception>

namespace kit
{
	// Forward declarations
	template <class T>
	class OwnPtr;

	template <class T>
	class UsePtr;

	template <class T>
	class Ptr;

	class _PtrCounter;

	// This is a function that can be passed to OwnPtr to delete arrays properly.
	template <class T>
	void deleteArray (T * p)
	{
		delete [] p;
	}

	// An exception for accessing objects that don't exist (null pointers, bad pointers, etc).
	class nullptr_exception : public std::exception
	{
	public:
		char const * what () const override
		{
			return "Null pointer exception. ";
		}
	};

	// OwnPtr is a smart pointer that controls ownership. It is copyable and maintains an OwnPtr reference count, that when the last OwnPtr reference of a particular object is deconstructed, the pointer is destroyed. When the last OwnPtr of a pointer is being destroyed, there must be no UsePtrs pointing to it. If there are, it will fail an assertion. This ensures that the OwnPtr and object always exist at least as long as the UsePtrs. OwnPtr can take a destroy function that must dispose of the pointer appropriately.
	/*
		In reference to cycles: The solution is to unlink all objects first, and then delete them. For doubly-linked trees, unlink the upward links first. For doubly linked lists, unlink all connections, etc.
	*/
	template <class T>
	class OwnPtr
	{
	public:
		// Initialize the pointer to null. It can take a function that destroys the object(default is the standard delete operator).
		OwnPtr ();

		// Initialize the pointer to newP, which can have a type that is subclass of T. Only pass in something that looks like 'new T()' to ensure that the raw pointer isn't used elsewhere. It can also take a function that destroys the object (default is the standard delete operator).
		template <class Y> explicit OwnPtr (Y * newP, void(* deleteFunction)(Y *) = nullptr);

		// Default copy constructor. Needed otherwise C++ will create its own.
		OwnPtr (OwnPtr<T> const & ptr);

		// Templated copy constructor. It can take a pointer that has a type that is a subclass of T.
		template <class Y> OwnPtr (OwnPtr<Y> const & ptr);

		// Destructor. If this is the last OwnPtr reference to the object, either delete is called or the destroy function is called if it is specified. There must be no UsePtrs pointing to the object. Note that it is not virtual, so don't subclass OwnPtr.
		~OwnPtr ();

		// Default assignment operator. Needed otherwise C++ will create its own.
		OwnPtr<T> & operator = (OwnPtr<T> const & ptr);

		// Templated assignment operator. It can take a pointer that has a type that is a subclass of T.
		template <class Y> OwnPtr<T> & operator = (OwnPtr<Y> const & ptr);

		// Returns true if this points to zero.
		bool isNull () const;

		// Returns true if this points to something non-zero.
		bool isValid () const;

		// Returns true if there is at least one UsePtr that points to the object this points to.
		bool isUsed () const;

		// Returns true if there is at least one UsePtr or Ptr that points to the object this points to.
		bool isReferenced () const;

		// Change the object that this points to newP, which can have a type that is subclass of T. Only pass in something that looks like 'new T()' to ensure that the raw pointer isn't used elsewhere. If there was a previous object pointed to, the same algorithm as the destructor is called.
		template <class Y> void set (Y * newP, void (*deleteFunction) (Y *) = nullptr);

		// Resets the object to point to nothing. If there was a previous object pointed to, the same algorithm as the destructor is called.
		void setNull ();

		// Provides access to the object's members.
		T * operator -> () const;

		// Provides access to the element located at index. Warning: this provides no index out-of-bounds checking.
		T & operator [] (int index) const;

		// Provides reference access to the object.
		T & operator * () const;

		// Only to be used for functions that require a pointer. Be careful how you use this.
		T * raw () const;

		// Get the unsigned integer value of the address of the object.
		operator unsigned int () const;

		// Returns a use pointer dynamically casted to Y.
		template <class Y> OwnPtr<Y> as () const;

		// Returns true if the address of this object is less than the address of ptr's object.
		bool operator < (OwnPtr<T> const & ptr) const;

		// Returns true if the address of this object is less than the address of ptr's object.
		bool operator < (UsePtr<T> const & ptr) const;

		// Returns true if the address of this object is less than the address of ptr's object.
		bool operator < (Ptr<T> const & ptr) const;

		// Returns true if the address of this object is equal to the address of ptr's object.
		bool operator == (OwnPtr<T> const & ptr) const;

		// Returns true if the address of this object is equal to the address of ptr's object.
		bool operator == (UsePtr<T> const & ptr) const;

		// Returns true if the address of this object is equal to the address of ptr's object.
		bool operator == (Ptr<T> const & ptr) const;

	private:
		T * p;
		_PtrCounter * c;
		template <class Y> friend class OwnPtr;
		template <class Y> friend class UsePtr;
		template <class Y> friend class Ptr;
	};

	// UsePtr is a smart pointer that has no ownership and so never deletes the pointer that it references. The object it points to is guaranteed to exist while the UsePtr points to it.
	template <class T>
	class UsePtr
	{
	public:
		// Default constructor. Initializes the pointer to null.
		UsePtr ();

		// Default copy constructor. Needed otherwise C++ will create its own.
		UsePtr (UsePtr<T> const & ptr);

		// Templated copy constructor.
		template <class Y> UsePtr (UsePtr<Y> const & ptr);

		// Initializes this to point to the same object that ptr points to. While this exists and points to the object, it has a guarantee that the object exists.
		template <class Y> UsePtr (OwnPtr<Y> const & ptr);

		// Destructor.
		~UsePtr ();

		// Default assignment operator. Needed, otherwise C++ will create its own.
		UsePtr<T> & operator = (UsePtr<T> const & ptr);

		// Templated assignment operator.
		template <class Y> UsePtr<T> & operator = (UsePtr<Y> const & ptr);

		// Assigns this to point to the same object that ptr points to. If this pointed to a previous object, then that reference is removed.
		template <class Y> UsePtr<T> & operator = (OwnPtr<Y> const & ptr);

		// Returns true if this points to zero.
		bool isNull () const;

		// Returns true if this points to something non-zero.
		bool isValid () const;

		// Makes this point to nothing.
		void setNull ();

		// Provides access to the object's members.
		T * operator -> () const;

		// Provides access to the element located at index. Warning: this provides no index out-of-bounds checking.
		T & operator [] (int index) const;

		// Provides reference access to the object.
		T & operator * () const;

		// Get the unsigned integer value of the address of the object.
		operator unsigned int () const;

		// Only to be used for functions that require a pointer. Be careful how you use this.
		T * raw () const;

		// Returns a use pointer dynamically casted to Y.
		template <class Y> UsePtr<Y> as () const;

		// Returns true if the address of this object is less than the address of ptr's object.
		bool operator < (OwnPtr<T> const & ptr) const;

		// Returns true if the address of this object is less than the address of ptr's object.
		bool operator < (UsePtr<T> const & ptr) const;

		// Returns true if the address of this object is less than the address of ptr's object.
		bool operator < (Ptr<T> const & ptr) const;

		// Returns true if the address of this object is equal to the address of ptr's object.
		template <class Y> bool operator == (OwnPtr<Y> const & ptr) const;

		// Returns true if the address of this object is equal to the address of ptr's object.
		template <class Y> bool operator == (UsePtr<Y> const & ptr) const;

		// Returns true if the address of this object is equal to the address of ptr's object.
		template <class Y> bool operator == (Ptr<Y> const & ptr) const;

	private:
		T * p;
		_PtrCounter * c;
		template <class Y> friend class OwnPtr;
		template <class Y> friend class UsePtr;
		template <class Y> friend class Ptr;
	};

	// Ptr is a somewhat smart pointer that has no ownership and so never deletes the pointer that it references. If the object it points to is accessed but there are no OwnPtrs, it throws an std::exception.
	template <class T>
	class Ptr
	{
	public:
		// Default constructor. Initializes the pointer to null.
		Ptr ();

		// Default copy constructor. Needed otherwise C++ will create its own.
		Ptr (Ptr<T> const & ptr);

		// Templated copy constructor.
		template <class Y> Ptr (Ptr<Y> const & ptr);

		// Templated copy constructor.
		template <class Y> Ptr (UsePtr<Y> const & ptr);

		// Initializes this to point to the same object that ptr points to.
		template <class Y> Ptr (OwnPtr<Y> const & ptr);

		// Destructor.
		~Ptr ();

		// Default assignment operator. Needed, otherwise C++ will create its own.
		Ptr<T> & operator = (Ptr<T> const & ptr);

		// Templated assignment operator.
		template <class Y> Ptr<T> & operator = (Ptr<Y> const & ptr);

		// Templated assignment operator.
		template <class Y> Ptr<T> & operator = (UsePtr<Y> const & ptr);

		// Assigns this to point to the same object that ptr points to. If this pointed to a previous object, then that reference is removed.
		template <class Y> Ptr<T> & operator = (OwnPtr<Y> const & ptr);

		// Returns true if this points to zero.
		bool isNull () const;

		// Returns true if this points to something non-zero.
		bool isValid () const;

		// Makes this point to nothing.
		void setNull ();

		// Provides access to the object's members.
		T * operator -> () const;

		// Provides access to the element located at index. Warning: this provides no index out-of-bounds checking.
		T & operator [] (int index) const;

		// Provides reference access to the object.
		T & operator * () const;

		// Get the unsigned integer value of the address of the object.
		operator unsigned int () const;

		// Only to be used for functions that require a pointer. Be careful how you use this.
		T * raw () const;

		// Returns a use pointer dynamically casted to Y.
		template <class Y> Ptr<Y> as () const;

		// Returns true if the address of this object is less than the address of ptr's object.
		bool operator < (OwnPtr<T> const & ptr) const;

		// Returns true if the address of this object is less than the address of ptr's object.
		bool operator < (UsePtr<T> const & ptr) const;

		// Returns true if the address of this object is less than the address of ptr's object.
		bool operator < (Ptr<T> const & ptr) const;

		// Returns true if the address of this object is equal to the address of ptr's object.
		template <class Y> bool operator == (OwnPtr<Y> const & ptr) const;

		// Returns true if the address of this object is equal to the address of ptr's object.
		template <class Y> bool operator == (UsePtr<Y> const & ptr) const;

		// Returns true if the address of this object is equal to the address of ptr's object.
		template <class Y> bool operator == (Ptr<Y> const & ptr) const;

	private:
		T * p;
		_PtrCounter * c;
		template <class Y> friend class OwnPtr;
		template <class Y> friend class UsePtr;
		template <class Y> friend class Ptr;
	};

	// Template Implementation.

	class _PtrCounter
	{
	public:
		virtual void destroy () = 0;

		int oc; // OwnPtr reference counter
		int uc; // UsePtr reference counter
		int pc; // Ptr reference counter
	};

	template <class T>
	class _PtrCounterTyped : public _PtrCounter
	{
	public:
		_PtrCounterTyped (T * p, void (* deleteFunction) (T *))
		{
			this->p = p;
			this->deleteFunction = deleteFunction;
		}

		void destroy () override
		{
			if (deleteFunction)
			{
				deleteFunction(p);
			}
			else
			{
				delete p;
			}
			deleteFunction = nullptr;
			p = nullptr;
		}

		T * p; // Derived type for correct destruction, even without base virtual destructor.
		void (* deleteFunction) (T *); // User-supplied destroy function.
	};

	// OwnPtr

	template <class T>
	OwnPtr<T>::OwnPtr () : p(nullptr), c(nullptr)
	{
	}

	template <class T> template <class Y>
	OwnPtr<T>::OwnPtr (Y * newP, void (*deleteFunction) (Y *)) : p(newP)
	{
		if(p != nullptr)
		{
			c = new _PtrCounterTyped<Y> (newP, deleteFunction);
			c->oc = 1;
			c->uc = 0;
			c->pc = 0;
		}
		else
		{
			c = nullptr;
		}
	}

	template <class T>
	OwnPtr<T>::OwnPtr (OwnPtr<T> const & ptr) : p(ptr.p), c(ptr.c)
	{
		if(p != 0)
		{
			c->oc++;
		}
	}

	template <class T> template <class Y>
	OwnPtr<T>::OwnPtr (OwnPtr<Y> const & ptr) : p(ptr.p), c(ptr.c)
	{
		if(p != nullptr)
		{
			c->oc++;
		}
	}

	template <class T>
	OwnPtr<T>::~OwnPtr ()
	{
		setNull();
	}

	template <class T>
	OwnPtr<T> & OwnPtr<T>::operator = (OwnPtr<T> const & ptr)
	{
		if(p != ptr.p)
		{
			setNull();
			// Set new pointer
			p = ptr.p;
			c = ptr.c;
			if(p != nullptr)
			{
				c->oc++;
			}
		}
		return *this;
	}

	template <class T> template <class Y>
	OwnPtr<T> & OwnPtr<T>::operator = (OwnPtr<Y> const & ptr)
	{
		if(p != ptr.p)
		{
			setNull();
			// Set new pointer
			p = ptr.p;
			c = ptr.c;
			if(p != nullptr)
			{
				c->oc++;
			}
		}
		return *this;
	}

	template <class T>
	bool OwnPtr<T>::isNull () const
	{
		return p == nullptr;
	}

	template <class T>
	bool OwnPtr<T>::isValid () const
	{
		return p != nullptr;
	}

	template <class T>
	bool OwnPtr<T>::isUsed () const
	{
		return p != nullptr && c->uc != 0;
	}

	template <class T>
	bool OwnPtr<T>::isReferenced () const
	{
		return p != nullptr && c->uc != 0 && c->pc != 0;
	}

	template <class T> template <class Y>
	void OwnPtr<T>::set (Y * newP, void (*deleteFunction) (Y *))
	{
		setNull();
		p = newP;
		if(p != nullptr)
		{
			c = new _PtrCounterTyped<Y> (newP, deleteFunction);
			c->oc = 1;
			c->uc = 0;
			c->pc = 0;
		}
		else
		{
			c = nullptr;
		}
	}

	template <class T>
	void OwnPtr<T>::setNull ()
	{
		if(p != nullptr)
		{
			c->oc--;
			if(c->oc == 0)
			{
				if(c->uc != 0)
				{
					c->oc++; // Rewind function.
					throw std::exception(); // This OwnPtr still has UsePtrs out there, so it can't be deleted.
				}
				c->destroy();
				if(c->pc == 0) // If there are Ptrs still out there, keep the counter around. They'll take care of deleting it.
				{
					delete c;
				}
			}
			p = nullptr;
			c = nullptr;
		}
	}

	template <class T>
	T * OwnPtr<T>::operator -> () const
	{
		if(p == nullptr)
		{
			throw nullptr_exception ();
		}
		return p;
	}

	template <class T>
	T & OwnPtr<T>::operator [] (int index) const
	{
		if(p == nullptr)
		{
			throw nullptr_exception ();
		}
		return p[index];
	}

	template <class T>
	T & OwnPtr<T>::operator * () const
	{
		if(p == nullptr)
		{
			throw nullptr_exception ();
		}
		return *p;
	}

	template <class T>
	T * OwnPtr<T>::raw () const
	{
		return p;
	}

	template <class T>
	OwnPtr<T>::operator unsigned int () const
	{
		return (unsigned int)p;
	}

	template <class T> template <class Y> OwnPtr<Y> OwnPtr<T>::as () const
	{
		OwnPtr<Y> up;
		up.p = dynamic_cast<Y *>(p);
		up.c = c;
		if(p != nullptr)
		{
			c->oc++;
		}
		return up;
	}

	template <class T>
	bool OwnPtr<T>::operator < (OwnPtr<T> const & ptr) const
	{
		return p < ptr.p;
	}

	template <class T>
	bool OwnPtr<T>::operator < (UsePtr<T> const & ptr) const
	{
		return p < ptr.p;
	}

	template <class T>
	bool OwnPtr<T>::operator < (Ptr<T> const & ptr) const
	{
		return p < ptr.p;
	}

	template <class T>
	bool OwnPtr<T>::operator == (OwnPtr<T> const & ptr) const
	{
		return p == ptr.p;
	}

	template <class T>
	bool OwnPtr<T>::operator == (UsePtr<T> const & ptr) const
	{
		return p == ptr.p;
	}

	template <class T>
	bool OwnPtr<T>::operator == (Ptr<T> const & ptr) const
	{
		return p == ptr.p;
	}

	// UsePtr

	template <class T>
	UsePtr<T>::UsePtr () : p(nullptr), c(nullptr)
	{
	}

	template <class T>
	UsePtr<T>::UsePtr (UsePtr<T> const & ptr) : p(ptr.p), c(ptr.c)
	{
		if(p != nullptr)
		{
			c->uc++;
		}
	}

	template <class T> template <class Y>
	UsePtr<T>::UsePtr (UsePtr<Y> const & ptr) : p(ptr.p), c(ptr.c)
	{
		if(p != nullptr)
		{
			c->uc++;
		}
	}

	template <class T> template <class Y>
	UsePtr<T>::UsePtr (OwnPtr<Y> const & ptr) : p(ptr.p), c(ptr.c)
	{
		if(p != nullptr)
		{
			c->uc++;
		}
	}

	template <class T>
	UsePtr<T>::~UsePtr ()
	{
		if(p != nullptr)
		{
			c->uc--;
		}
	}

	template <class T>
	UsePtr<T> & UsePtr<T>::operator = (UsePtr<T> const & ptr)
	{
		if(p != ptr.p)
		{
			if(p != nullptr)
			{
				c->uc--;
			}
			p = ptr.p;
			c = ptr.c;
			if(p != nullptr)
			{
				c->uc++;
			}
		}
		return *this;
	}

	template <class T> template <class Y>
	UsePtr<T> & UsePtr<T>::operator = (UsePtr<Y> const & ptr)
	{
		if(p != ptr.p)
		{
			if(p != nullptr)
			{
				c->uc--;
			}
			p = ptr.p;
			c = ptr.c;
			if(p != nullptr)
			{
				c->uc++;
			}
		}
		return *this;
	}

	template <class T> template <class Y>
	UsePtr<T> & UsePtr<T>::operator = (OwnPtr<Y> const & ptr)
	{
		if(p != ptr.p)
		{
			if(p != nullptr)
			{
				c->uc--;
			}
			p = ptr.p;
			c = ptr.c;
			if(p != nullptr)
			{
				c->uc++;
			}
		}
		return *this;
	}

	template <class T>
	bool UsePtr<T>::isNull () const
	{
		return p == nullptr;
	}

	template <class T>
	bool UsePtr<T>::isValid () const
	{
		return p != nullptr;
	}

	template <class T>
	void UsePtr<T>::setNull ()
	{
		if(p != nullptr)
		{
			c->uc--;
		}
		p = nullptr;
		c = nullptr;
	}

	template <class T>
	T * UsePtr<T>::operator -> () const
	{
		if(p == nullptr)
		{
			throw nullptr_exception ();
		}
		return p;
	}

	template <class T>
	T & UsePtr<T>::operator [] (int index) const
	{
		if(p == nullptr)
		{
			throw nullptr_exception ();
		}
		return p[index];
	}

	template <class T>
	T & UsePtr<T>::operator * () const
	{
		if(p == nullptr)
		{
			throw nullptr_exception ();
		}
		return *p;
	}

	template <class T>
	T * UsePtr<T>::raw () const
	{
		return p;
	}

	template <class T>
	UsePtr<T>::operator unsigned int () const
	{
		return (unsigned int)p;
	}

	template <class T> template <class Y> UsePtr<Y> UsePtr<T>::as () const
	{
		UsePtr<Y> up;
		up.p = dynamic_cast<Y *>(p);
		up.c = c;
		if(p != nullptr)
		{
			c->uc++;
		}
		return up;
	}

	template <class T>
	bool UsePtr<T>::operator < (OwnPtr<T> const & ptr) const
	{
		return p < ptr.p;
	}

	template <class T>
	bool UsePtr<T>::operator < (UsePtr<T> const & ptr) const
	{
		return p < ptr.p;
	}

	template <class T>
	bool UsePtr<T>::operator < (Ptr<T> const & ptr) const
	{
		return p < ptr.p;
	}

	template <class T> template <class Y>
	bool UsePtr<T>::operator == (OwnPtr<Y> const & ptr) const
	{
		return p == ptr.p;
	}

	template <class T> template <class Y>
	bool UsePtr<T>::operator == (UsePtr<Y> const & ptr) const
	{
		return p == ptr.p;
	}

	template <class T> template <class Y>
	bool UsePtr<T>::operator == (Ptr<Y> const & ptr) const
	{
		return p == ptr.p;
	}

	// Ptr

	template <class T>
	Ptr<T>::Ptr () : p(nullptr), c(nullptr)
	{
	}

	template <class T>
	Ptr<T>::Ptr (Ptr<T> const & ptr) : p(ptr.p), c(ptr.c)
	{
		if(p != nullptr)
		{
			c->pc++;
		}
	}

	template <class T> template <class Y>
	Ptr<T>::Ptr (Ptr<Y> const & ptr) : p(ptr.p), c(ptr.c)
	{
		if(p != nullptr)
		{
			c->pc++;
		}
	}

	template <class T> template <class Y>
	Ptr<T>::Ptr (UsePtr<Y> const & ptr) : p(ptr.p), c(ptr.c)
	{
		if(p != nullptr)
		{
			c->pc++;
		}
	}

	template <class T> template <class Y>
	Ptr<T>::Ptr (OwnPtr<Y> const & ptr) : p(ptr.p), c(ptr.c)
	{
		if(p != nullptr)
		{
			c->pc++;
		}
	}

	template <class T>
	Ptr<T>::~Ptr ()
	{
		setNull();
	}

	template <class T>
	Ptr<T> & Ptr<T>::operator = (Ptr<T> const & ptr)
	{
		if(p != ptr.p)
		{
			setNull();
			p = ptr.p;
			c = ptr.c;
			if(p != nullptr)
			{
				c->pc++;
			}
		}
		return *this;
	}

	template <class T> template <class Y>
	Ptr<T> & Ptr<T>::operator = (Ptr<Y> const & ptr)
	{
		if(p != ptr.p)
		{
			setNull();
			p = ptr.p;
			c = ptr.c;
			if(p != nullptr)
			{
				c->pc++;
			}
		}
		return *this;
	}

	template <class T> template <class Y>
	Ptr<T> & Ptr<T>::operator = (UsePtr<Y> const & ptr)
	{
		if(p != ptr.p)
		{
			setNull();
			p = ptr.p;
			c = ptr.c;
			if(p != nullptr)
			{
				c->pc++;
			}
		}
		return *this;
	}

	template <class T> template <class Y>
	Ptr<T> & Ptr<T>::operator = (OwnPtr<Y> const & ptr)
	{
		if(p != ptr.p)
		{
			setNull();
			p = ptr.p;
			c = ptr.c;
			if(p != nullptr)
			{
				c->pc++;
			}
		}
		return *this;
	}

	template <class T>
	bool Ptr<T>::isNull () const
	{
		return p == nullptr;
	}

	template <class T>
	bool Ptr<T>::isValid () const
	{
		return p != nullptr && c->oc != 0;
	}

	template <class T>
	void Ptr<T>::setNull ()
	{
		if(p != nullptr)
		{
			c->pc--;
			if(c->pc == 0 && c->oc == 0) // If there are no OwnPtrs around, that means this the last to reference the Counter.
			{
				delete c;
			}
			p = nullptr;
			c = nullptr;
		}
	}

	template <class T>
	T * Ptr<T>::operator -> () const
	{
		if(p == nullptr || c->oc == 0)
		{
			throw nullptr_exception ();
		}
		return p;
	}

	template <class T>
	T & Ptr<T>::operator [] (int index) const
	{
		if(p == nullptr || c->oc == 0)
		{
			throw nullptr_exception ();
		}
		return p[index];
	}

	template <class T>
	T & Ptr<T>::operator * () const
	{
		if(p == nullptr || c->oc == 0)
		{
			throw nullptr_exception ();
		}
		return *p;
	}

	template <class T>
	T * Ptr<T>::raw () const
	{
		return p;
	}

	template <class T>
	Ptr<T>::operator unsigned int () const
	{
		return (unsigned int)p;
	}

	template <class T> template <class Y> Ptr<Y> Ptr<T>::as () const
	{
		Ptr<Y> pp;
		pp.p = dynamic_cast<Y *>(p);
		pp.c = c;
		if(p != nullptr)
		{
			c->pc++;
		}
		return pp;
	}

	template <class T>
	bool Ptr<T>::operator < (OwnPtr<T> const & ptr) const
	{
		return p < ptr.p;
	}

	template <class T>
	bool Ptr<T>::operator < (UsePtr<T> const & ptr) const
	{
		return p < ptr.p;
	}

	template <class T>
	bool Ptr<T>::operator < (Ptr<T> const & ptr) const
	{
		return p < ptr.p;
	}

	template <class T> template <class Y>
	bool Ptr<T>::operator == (OwnPtr<Y> const & ptr) const
	{
		return p == ptr.p;
	}

	template <class T> template <class Y>
	bool Ptr<T>::operator == (UsePtr<Y> const & ptr) const
	{
		return p == ptr.p;
	}

	template <class T> template <class Y>
	bool Ptr<T>::operator == (Ptr<Y> const & ptr) const
	{
		return p == ptr.p;
	}
}

