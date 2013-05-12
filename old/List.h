#ifndef __LIST_H__
#define __LIST_H__

#include "Common.h"

template <class T> class List
{
public:
	List()
	{
		size = count = 0;
		data = 0;
	}

	~List()
	{
		Clear();
	}

	bool Contains(const T& item)
	{
		for (int i = 0; i < count; i++)
		{
			if (data[i] == item) return true;
		}
		return false;
	}

	void Add(const T& item)
	{
		if (Contains(item)) throw EXCEPTION("List already contains the given item.");
		count++;
		ensureCapacity();
		data[count - 1] = item;
	}

	void Remove(const T& item)
	{
		for (int i = 0; i < count; i++)
		{
			if (data[i] == item)
			{
				Remove(i);
				return;
			}
		}
		throw EXCEPTION("List does not contain the given item.");
	}

	void Insert(int index, const T& item)
	{
		if (Contains(item)) throw EXCEPTION("List already contains the given item.");
		count++;
		ensureCapacity();
		for (int i = count - 1; i >= index; i--) data[i + 1] = data[i];
		data[index] = item;
	}

	void Remove(int index)
	{
		if (index < 0 || index >= count) throw EXCEPTION("List does not contain the given index.");
		count--;
		for (int i = index; i <= count; i++) data[i] = data[i + 1];
	}

	void RemoveRange(int start, int end)
	{
		for (int i = 0; i < end - start; i++) Remove(start);
	}

	int Count() const
	{
		return count;
	}

	void Clear()
	{
		size = count = 0;
		if (data) delete [] data;
	}

	T& At(int index) const
	{
		if (index < 0 || index >= count) throw EXCEPTION("List does not contain the given index.");
		return data[index];
	}

	T& operator [](int index) const
	{
		if (index < 0 || index >= count) throw EXCEPTION("List does not contain the given index.");
		return data[index];
	}

private:
	void ensureCapacity()
	{
		if (count > size)
		{
			T *newData = new T[count];
			for (int i = 0; i < size; i++) newData[i] = data[i];
			size = count;
			if (data) delete [] data;
			data = newData;
		}
	}

	int size, count;
	T *data;
};

#endif
