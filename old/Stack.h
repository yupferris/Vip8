#ifndef __STACK_H__
#define __STACK_H__

template <class T> class Stack
{
public:
	Stack(int size)
	{
		data = new T[size];
		this->size = size;
		index = 0;
	}

	~Stack()
	{
		delete [] data;
	}

	void Push(T item)
	{
		if (index >= size) throw EXCEPTION("Stack overflow occurred.");
		data[index] = item;
		index++;
	}

	T Pop()
	{
		if (index <= 0) throw EXCEPTION("Stack underflow occurred.");
		index--;
		return data[index];
	}

private:
	T *data;
	int size;
	int index;
};

#endif
