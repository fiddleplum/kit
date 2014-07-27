#pragma once

#include <set>
#include <map>
#include <unordered_set>

namespace kit
{
	template <typename T>
	class Set
	{
	public:
		Set();
		Set(std::initializer_list<T> il);

		template <typename Y> typename std::set<T>::iterator find(Y const & y);
		template <typename Y> typename std::set<T>::const_iterator find(Y const & y) const;

		size_t size() const;

		typename std::set<T>::iterator begin();
		typename std::set<T>::iterator end();
		typename std::set<T>::const_iterator cbegin() const;
		typename std::set<T>::const_iterator cend() const;
		typename std::set<T>::reverse_iterator rbegin();
		typename std::set<T>::reverse_iterator rend();
		typename std::set<T>::const_reverse_iterator crbegin() const;
		typename std::set<T>::const_reverse_iterator crend() const;

		void erase(typename std::set<T>::const_iterator it);
		template <typename Y> void erase(Y const & y);

		void clear();
		void insert(T const & t);

		void processErases();

	private:
		class IteratorHash;

		std::set<T> s; // the items
		std::map<unsigned int, typename std::set<T>::const_iterator> m; // a map from a hash to the corresponding item
		std::unordered_set<typename std::set<T>::iterator, IteratorHash> erases; // accumulated erases since the last processErases
	};

	// Internal Implementation

	template <typename T>
	Set<T>::Set()
	{
	}

	template <typename T>
	Set<T>::Set(std::initializer_list<T> il)
	{
		for(auto item : il)
		{
			s.insert(item);
			m[(unsigned int)item] = item;
		}
	}

	template <typename T> template <typename Y>
	typename std::set<T>::iterator Set<T>::find(Y const & y)
	{
		return m.find((unsigned int)y).second;
	}

	template <typename T> template <typename Y>
	typename std::set<T>::const_iterator Set<T>::find(Y const & y) const
	{
		return m.find((unsigned int)y).second;
	}

	template <typename T>
	size_t Set<T>::size() const
	{
		return s.size();
	}

	template <typename T>
	typename std::set<T>::iterator Set<T>::begin()
	{
		return s.begin();
	}

	template <typename T>
	typename std::set<T>::iterator Set<T>::end()
	{
		return s.end();
	}

	template <typename T>
	typename std::set<T>::const_iterator Set<T>::cbegin() const
	{
		return s.cbegin();
	}

	template <typename T>
	typename std::set<T>::const_iterator Set<T>::cend() const
	{
		return s.cend();
	}

	template <typename T>
	typename std::set<T>::reverse_iterator Set<T>::rbegin()
	{
		return s.rbegin();
	}

	template <typename T>
	typename std::set<T>::reverse_iterator Set<T>::rend()
	{
		return s.rend();
	}

	template <typename T>
	typename std::set<T>::const_reverse_iterator Set<T>::crbegin() const
	{
		return s.crbegin();
	}

	template <typename T>
	typename std::set<T>::const_reverse_iterator Set<T>::crend() const
	{
		return s.crend();
	}

	template <typename T>
	void Set<T>::erase(typename std::set<T>::const_iterator it)
	{
		erases.insert(it);
	}

	template <typename T> template <typename Y>
	void Set<T>::erase(Y const & y)
	{
		auto it = m.find(y);
		if(it != m.end())
		{
			erases.insert(it->second);
		}
	}

	template <typename T>
	void Set<T>::clear()
	{
		for(auto it = s.begin(); it != s.end(); it++)
		{
			erases.insert(it);
		}
	}

	template <typename T>
	void Set<T>::insert(T const & t)
	{
		auto tIt = s.insert(t);
		m[(unsigned int)t] = tIt.first;
	}

	template <typename T>
	void Set<T>::processErases()
	{
		for(auto it : erases)
		{
			unsigned int hash = *it;
			s.erase(it);
			m.erase(hash);
		}
		erases.clear();
	}

	template <typename T>
	class Set<T>::IteratorHash
	{
	public:
		size_t operator () (typename std::set<T>::iterator const & it)
		{
			return (unsigned int)(*it);
		}
	};
}

