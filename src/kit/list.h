#pragma once

#include <list>

namespace kit
{
	template <typename T>
	class List
	{
	public:
		List();
		List(std::initializer_list<T> il);

		T & front();
		T const & front() const;
		T & back();
		T const & back() const;

		typename std::list<T>::iterator begin();
		typename std::list<T>::iterator end();
		typename std::list<T>::const_iterator cbegin() const;
		typename std::list<T>::const_iterator cend() const;
		typename std::list<T>::reverse_iterator rbegin();
		typename std::list<T>::reverse_iterator rend();
		typename std::list<T>::const_reverse_iterator crbegin() const;
		typename std::list<T>::const_reverse_iterator crend() const;

		void erase(typename std::list<T>::const_iterator it);
		void clear();
		void insert(typename std::list<T>::const_iterator it, T const & t);
		void push_front(T const & t);
		void push_back(T const & t);

		void processErases();

	private:
		std::list<T> l;
		std::list<typename std::list<T>::const_iterator> erases;
	};

	// Template Implementation

	template <typename T>
	List<T>::List()
	{
	}

	template <typename T>
	List<T>::List(std::initializer_list<T> il)
	{
		for(auto item : il)
		{
			l.push_back(item);
		}
	}

	template <typename T>
	T & List<T>::front()
	{
		return l.front();
	}

	template <typename T>
	T const & List<T>::front() const
	{
		return l.front();
	}

	template <typename T>
	T & List<T>::back()
	{
		return l.back();
	}

	template <typename T>
	T const & List<T>::back() const
	{
		return l.back();
	}

	template <typename T>
	typename std::list<T>::iterator List<T>::begin()
	{
		return l.begin();
	}

	template <typename T>
	typename std::list<T>::iterator List<T>::end()
	{
		return l.end();
	}

	template <typename T>
	typename std::list<T>::const_iterator List<T>::cbegin() const
	{
		return l.cbegin();
	}

	template <typename T>
	typename std::list<T>::const_iterator List<T>::cend() const
	{
		return l.cend();
	}

	template <typename T>
	typename std::list<T>::reverse_iterator List<T>::rbegin()
	{
		return l.rbegin();
	}

	template <typename T>
	typename std::list<T>::reverse_iterator List<T>::rend()
	{
		return l.rend();
	}

	template <typename T>
	typename std::list<T>::const_reverse_iterator List<T>::crbegin() const
	{
		return l.crbegin();
	}

	template <typename T>
	typename std::list<T>::const_reverse_iterator List<T>::crend() const
	{
		return l.crend();
	}

	template <typename T>
	void List<T>::erase(typename std::list<T>::const_iterator it)
	{
		erases.insert(it);
	}

	template <typename T>
	void List<T>::clear()
	{
		for(auto it = l.begin(); it != l.end(); it++)
		{
			erases.push_back(it);
		}
	}

	template <typename T>
	void List<T>::insert(typename std::list<T>::const_iterator it, T const & t)
	{
		l.insert(it, t);
	}

	template <typename T>
	void List<T>::push_front(T const & t)
	{
		l.push_front(t);
	}

	template <typename T>
	void List<T>::push_back(T const & t)
	{
		l.push_back(t);
	}

	template <typename T>
	void List<T>::processErases()
	{
		for(auto it : erases)
		{
			l.erase(it);
		}
		erases.clear();
	}
}

