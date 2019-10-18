#pragma once

#include <functional>

#include "SquareMatrix.h"
#include "MatrixElement.h"

namespace Matrix
{
	namespace Algorithm
	{
		template< template<class> class MatrixType, class ElementType, class Function>
		void ForEach(MatrixType<ElementType> *mtx, Function const &func)
		{
			size_t rows = mtx->GetRows();
			size_t columns = mtx->GetColumns();
			for (size_t i(0); i < rows; ++i)
				for (size_t j(0); j < columns; ++j)
				{
					Element<ElementType> element{ i, j, &(*mtx)[i][j] };
					func(element);
				}
		}

		template <template<class> class MatrixType, class ElementType, class AreaPredicate, class CompareFunction>
		Element<ElementType> FindElement(MatrixType<ElementType> *mtx, AreaPredicate const &isElementInArea, CompareFunction const &compare)
		{
			Element<ElementType> current;
			bool isFirst = true;
			ForEach(mtx, [&](Element<ElementType> &candidate)
			{
				if (isElementInArea(candidate))
					if (isFirst || compare(current, candidate))
					{
						isFirst = false;
						current = candidate;
					}
			});

			return current;
		}
	}
}