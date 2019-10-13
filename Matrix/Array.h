#pragma once

#include "Log.h"

template <typename ElementType>
struct Array
{
private:
	ElementType *_ptr = nullptr;
	size_t _size = 0;
public:
	Array(size_t size)
		: _size(size)
	{
		if (_size != 0 && !_Allocate())
			_size = 0;
	}
	~Array()
	{
		_Delete();
	}

	ElementType& operator[](size_t elementId)
	{
		return _ptr[elementId];
	}
private:
	bool _Allocate()
	{
		_ptr = new ElementType[_size];
		bool isAllocationSuccess = nullptr != _ptr;
		if (!isAllocationSuccess)
			Log::Error("failed attempt to allocate memory for array");

		return isAllocationSuccess;
	}
	void _Delete()
	{
		if (_ptr != nullptr)
			delete[] _ptr;
	}
};