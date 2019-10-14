#pragma once

#include <functional>

#include "SquareMatrix.h"
#include "MatrixElement.h"

namespace Matrix
{
	namespace Algorithm
	{
		template< template<class> class MatrixType, class ElementType, class Function>
		void ForEach(MatrixType<ElementType> &mtx, Function const &func);

		template< class ElementType, class Function >
		void ForEach(SquareMatrix<ElementType> &mtx, Function const &func)
		{
			size_t order = mtx.GetOrder();
			for (size_t i(0); i < order; ++i)
				for (size_t j(0); j < order; ++j)
				{
					Element<ElementType> element = { i, j, &mtx[i][j] };
					func(element);
				}
		}

		template <template<class> class MatrixType, class ElementType, class AreaPredicate, class CompareFunction>
		Element<ElementType> FindElement(MatrixType<ElementType> &mtx, AreaPredicate const &isElementInArea, CompareFunction const &compare);

		template <class ElementType, class AreaPredicate, class CompareFunction>
		Element<ElementType> FindElement(SquareMatrix<ElementType> &mtx, AreaPredicate const &isElementInArea, CompareFunction const &compare)
		{
			size_t order = mtx.GetOrder();
			Element<ElementType> target;
			bool isFirst = true;
			ForEach(mtx, [&](Element<ElementType> &elem)
			{
				if (isElementInArea(elem))
					if (isFirst || compare(target, elem))
					{
						isFirst = false;
						target = elem;
					}
			});

			return target;
		}
	}
}