#pragma once

#ifdef WIN32
	#undef min
	#undef max
#endif

#include "Log.h"
#include "Matrix.h"
#include "MatrixTools.h"

#include "MatrixVisualizer.h"
#include "MatrixAlgorithmVisualization.h"
#include "MatrixAlgorithm.h"

namespace Demostration
{
	class SearchOfMaxElement
	{
		template< template<class> class MatrixType, typename ElementType>
		Matrix::Element<ElementType> _GetMaxElementWithVisualization(MatrixType<ElementType>* mtx)
		{
			size_t timeoutBetweenSteps = Matrix::Tools::GetSizeValueFromUser("Enter timeout between visuzlization steps(msec), recommended 100");

			auto elementsUpperSideDiagonalFunction = [mtx](Matrix::Element<ElementType>& elem)
			{
				return elem.column < (mtx->GetRows() - elem.row);
			};
			auto maxElementFunction = [](Matrix::Element<ElementType>& last, Matrix::Element<ElementType>& second)
			{
				return (*last.value) < (*second.value);
			};

			return Matrix::Algorithm::Visualization::FindElement(
				mtx,
				elementsUpperSideDiagonalFunction,
				Log::ansiColorYellow,
				maxElementFunction,
				Log::ansiColorGreen,
				Log::ansiColorRed,
				timeoutBetweenSteps
			);
		}

		template< template<class> class MatrixType, typename ElementType >
		Matrix::Element<ElementType> _GetMaxElement(MatrixType<ElementType>* mtx)
		{
			auto elementsUpperSideDiagonalFunction = [mtx](Matrix::Element<ElementType>& elem)
			{
				return elem.column < (mtx->GetRows() - elem.row);
			};
			auto maxElementFunction = [](Matrix::Element<ElementType>& last, Matrix::Element<ElementType>& second)
			{
				return (*last.value) < (*second.value);
			};

			return Matrix::Algorithm::FindElement(
				mtx,
				elementsUpperSideDiagonalFunction,
				maxElementFunction
			);
		}

	public:
		template< template<class> class MatrixType, typename ElementType >
		void Demonstrate(MatrixType<ElementType>* mtx)
		{
			size_t isVisualizationRequired = 0;
			printf("\nDo you want to see the visualization of the serach max element algorithm?(1/yes, 0/no) :");
			scanf("%i", &isVisualizationRequired);

			system("cls");

			Matrix::Element<ElementType> maxElem = isVisualizationRequired ? _GetMaxElementWithVisualization(mtx)
				: _GetMaxElement(mtx);

			printf("\nMax element upper side diagonal is: %smtx[%i][%i] == ", Log::ansiColorGreen, maxElem.row, maxElem.column);
			std::cout << *maxElem.value << Log::ansiColorReset << std::endl;

			system("pause");
		}
	};

	class Demonstrator
	{
	public:
		template< template<class> class MatrixType, typename ElementType > 
		void Demonstrate(MatrixType<ElementType> *mtx)
		{
			Matrix::MatrixVisualizer visualizer;

			size_t rows = mtx->GetRows();
			size_t columns = mtx->GetColumns();
			printf("Matrix %ux%u:\n", rows, columns);
			visualizer.Print(mtx);

			SearchOfMaxElement maxSearch;
			maxSearch.Demonstrate(mtx);
		}
	};
}