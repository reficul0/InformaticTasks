#pragma once

#include "Array.h"
#include "Log.h"

namespace Matrix
{
	template<typename ElementType>
	struct SquareMatrix
	{
	private:
		Array<ElementType> **_ptr;
		size_t _order;
	public:

		SquareMatrix(size_t mtxOrder)
			: _ptr(nullptr)
			, _order(mtxOrder)
		{
			if (_order != 0 && !_Allocate())
				_order = 0;
		}
		~SquareMatrix()
		{
			_Delete();
		}

		size_t GetOrder()
		{
			return _order;
		}

		// ссылка для того, чтобы значение можно было использовать как изменяемое lvalue
		// Это дает возможность использовать оператор для записи значений.
		// mtx[i] = someRowValue
		Array<ElementType> &const operator[](size_t rowId)
		{
			return *(_ptr[rowId]);
		}
	private:
		bool _Allocate()
		{
			_ptr = new Array<ElementType>*[_order];
			bool isAllocationSuccess = nullptr != _ptr;

			if (isAllocationSuccess)
				for (size_t i = 0; i < _order; ++i)
				{
					_ptr[i] = new Array<ElementType>(_order);
					isAllocationSuccess = nullptr != _ptr[i];
					if (!isAllocationSuccess)
					{
						_Delete(0, i);
						break;
					}
				}

			if (!isAllocationSuccess)
				Log::Error("failed attempt to allocate memory for matrix");

			return isAllocationSuccess;
		}

		void _Delete()
		{
			_Delete(0, _order);
		}
		void _Delete(size_t begin, size_t end)
		{
			if (_ptr)
			{
				size_t validEndId = end <= _order ? end : _order;
				for (size_t i = begin; i < validEndId; ++i)
					if (_ptr[i])
						delete _ptr[i];
				delete[] _ptr;
			}
		}

	};
}