
#pragma once

#include "Array.h"
#include "Log.h"

namespace Matrix
{
	template<typename ElementType>
	struct Matrix
	{
	private:
		Array<ElementType> **_ptr;
	protected:
		size_t _rows;
		size_t _columns;
	public:
		Matrix(size_t rows, size_t columns)
			: _ptr(nullptr)
			, _rows(rows)
			, _columns(columns)
		{
			if (_rows != 0 && _columns != 0 && !_Allocate())
			{
				_rows = 0;
				_columns = 0;
			}
		}
		~Matrix()
		{
			_Delete();
		}

		size_t GetRows()
		{
			return _rows;
		}
		size_t GetColumns()
		{
			return _columns;
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
			_ptr = new Array<ElementType>*[_rows];
			bool isAllocationSuccess = nullptr != _ptr;

			if (isAllocationSuccess)
				for (size_t i = 0; i < _rows; ++i)
				{
					_ptr[i] = new Array<ElementType>(_columns);
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
			_Delete(0, _rows);
		}
		void _Delete(size_t begin, size_t end)
		{
			if (_ptr)
			{
				size_t validEndId = end <= _rows ? end : _rows;
				for (size_t i = begin; i < validEndId; ++i)
					if (_ptr[i])
						delete _ptr[i];
				delete[] _ptr;
			}
		}
	};
}