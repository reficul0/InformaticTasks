#pragma once

#include "MatrixElement.h"
#include "SquareMatrix.h"
#include "MatrixVisualizer.h"
#include "windows.h"

namespace Matrix
{
	namespace Algorithm
	{
		namespace Visualization
		{
			template <template<class> class MatrixType, class ElementType, class AreaPredicate, class CompareFunction>
			Element<ElementType> FindElement(MatrixType<ElementType> *mtx, AreaPredicate const &isElementInArea, char const * ansiColorOfArea, CompareFunction const &compare, char const * currentElementColor, char const * candidateColor, size_t timeoutBetweenStepsMsec)
			{
				Element<ElementType> current;
				bool isFirst = true;

				MatrixVisualizer visualizer;
				Array<ElementToVisualize<int>> elements(2);
				elements[0].ansiColorOfElement = currentElementColor;
				elements[1].ansiColorOfElement = candidateColor;

				system("cls");
				visualizer.Print(mtx);

				printf("\nColors: %ssearch area, %scurrent element, %scandidate\n%s", ansiColorOfArea, currentElementColor, candidateColor, Log::ansiColorReset);

				ForEach(mtx, [&](Element<ElementType> &candidate)
				{
					if (isElementInArea(candidate))
						if (isFirst || compare(current, candidate))
						{
							isFirst = false;
							current = candidate;
							elements[0].element = current;
						}

					elements[1].element = candidate;

					visualizer.Update(
						mtx,
						isElementInArea,
						ansiColorOfArea,
						elements
					);
					Sleep(timeoutBetweenStepsMsec);
				});

				return current;
			}	
		}
	}
}