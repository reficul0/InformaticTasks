#pragma once

#include "MatrixElement.h"
#include "SquareMatrix.h"
#include "SquareMatrixVisualizer.h"
#include "windows.h"

namespace Matrix
{
	namespace Algorithm
	{
		namespace Visualization
		{
			template <template<class> class MatrixType, class ElementType, class AreaPredicate, class CompareFunction>
			Element<ElementType> FindElement(MatrixType<ElementType> &mtx, AreaPredicate const &isElementInArea, char const * ansiColorOfArea, CompareFunction const &compare, char const * currentElementColor, char const * candidateColor);

			template <class ElementType, class AreaPredicate, class CompareFunction>
			Element<ElementType> FindElement(
				SquareMatrix<ElementType> &mtx,
				AreaPredicate const &isElementInArea,
				char const * ansiColorOfArea,
				CompareFunction const &compare,
				char const * currentElementColor,
				char const * candidateColor
			)
			{
				size_t order = mtx.GetOrder();
				Element<ElementType> current;
				bool isFirst = true;

				MatrixVisualizer visualizer;

				ForEach(mtx, [&](Element<ElementType> &candidate)
				{
					if (isElementInArea(candidate))
						if (isFirst || compare(current, candidate))
						{
							isFirst = false;
							current = candidate;
						}

					system("cls");
					visualizer.PrintElementOfArea(
						mtx,
						isElementInArea,
						ansiColorOfArea,
						current,
						currentElementColor,
						candidate,
						candidateColor
					);
					Sleep(100);
				});

				return current;
			}
		}
	}
}