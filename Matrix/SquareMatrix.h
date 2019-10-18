#pragma once

#include "Matrix.h"
#include "Log.h"

namespace Matrix
{
	template<typename ElementType>
	struct SquareMatrix
		: Matrix<ElementType>
	{
	public:
		SquareMatrix(size_t mtxOrder)
			: Matrix(mtxOrder, mtxOrder)
		{
		}

		size_t GetOrder()
		{
			return Matrix::_rows;
		}
	};
}