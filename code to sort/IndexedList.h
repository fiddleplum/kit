#pragma once

#include <vector>
#include <cassert>

// O(1) addition, remove, and indexing (except for resizing of array).
// Index of added item never changes.

template <typename T>
class IndexedList
{
public:
	IndexedList ();
	explicit IndexedList (IndexedList<T> const & indexedList);
	void operator = (IndexedList<T> const & indexedList);

	T const & operator [] (int index) const;
	T & operator [] ( int index);
	int getSize () const;
	bool isUsed (int index) const;
	int add (T const& item);
	void remove (int index);

private:
	std::vector<int> freeIndices;
	std::vector<bool> used;
	std::vector<T> items;
};

template <typename T>
IndexedList<T>::IndexedList ()
{
}

template <typename T>
IndexedList<T>::IndexedList (IndexedList<T> const & indexedList)
: freeIndices(indexedList.freeIndices), items(indexedList.items), used(indexedList.used)
{
}

template <typename T>
void IndexedList<T>::operator = (IndexedList<T> const & indexedList)
{
    if (this == &indexedArray)
    {
        return;
    }
    freeIndices = indexedList.freeIndices;
    items = indexedList.items;
    used = indexedList.used;
}

template <typename T>
T const & IndexedList<T>::operator [] (int index) const
{
	assert(0 <= index && index < used.size() && used[index]);
	return items[index];
}

template <typename T>
T & IndexedList<T>::operator [] (int index)
{
	assert(0 <= index && index < used.size() && used[index]);
	return items[index];
}

template <typename T>
int IndexedList<T>::size () const
{
	return items.size();
}

template <typename T>
bool IndexedList<T>::isUsed (int index) const
{
    if (index < 0 || used.size() <= index)
    {
        return false;
    }
    return used[index];
}

template <typename T>
int IndexedList<T>::add (T const & item)
{
	int index;
	if (freeIndices.isEmpty()) // index = size
	{
        index = items.size();
		items.push_back(item);
		used.push_back(true);
	}
	else
	{
		index = freeIndices.back();
        freeIndices.pop_back();
		items[index] = item;
		used[index] = true;
	}
	return index;
}

template <typename T>
void IndexedList<T>::remove (int index)
{
	assert(0 <= index && index < used.size() && used[index]); // invalid index!
	freeIndices.push_back(index);
	used[index] = false;
}

