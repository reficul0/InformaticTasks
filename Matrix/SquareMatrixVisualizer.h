#pragma once

#include "MatrixElement.h"
#include "MatrixAlgorithm.h"
#include "SquareMatrix.h"
#include "Log.h"
#include <stdio.h>

namespace Matrix
{

	class MatrixVisualizer
	{
		size_t _numberOfDigitsInMaxElement;
		bool _isMatrixChanged;
	public:
		MatrixVisualizer()
			: _isMatrixChanged(true)
			, _numberOfDigitsInMaxElement(0)
		{

		}

		void OnMatrixChanged()
		{
			_isMatrixChanged = true;
		}

		template< template<class> class MatrixType, typename ElementType > 
		void Print(MatrixType<ElementType> &mtx);

		template< template<class> class MatrixType, typename ElementType, typename AreaFunction >
		void PrintElementOfArea(
			MatrixType<ElementType> &mtx,
			AreaFunction areaFunc, 
			char const* ansiColorOfArea, 
			Element<ElementType> element,
			char const* ansiColorOfElement 
		);

		template<typename AreaFunction>
		void PrintElementOfArea(SquareMatrix<int>& mtx, AreaFunction isInAreaToColor, char const * ansiColorOfArea, Element<int> elementToColor, char const * ansiColorOfElement)
		{
			if (_isMatrixChanged)
			{
				_RecalculateNumberOfDigitsInMaxElement(mtx);
				_isMatrixChanged = false;
			}

			size_t order = mtx.GetOrder();
			printf("Matrix %ux%u:\n", order, order);
			for (int i = 0; i < order; ++i)
			{
				printf("| ");
				for (int j = 0; j < order; ++j)
				{
					Element<int> current = { i, j, &mtx[i][j] };

					if (elementToColor.column == current.column
						&& elementToColor.row == current.row)
					{
						printf("%s", ansiColorOfElement);
						printf("%*i ", _numberOfDigitsInMaxElement, mtx[i][j]); 
					}
					else if (isInAreaToColor( current ))
					{
						printf("%s", ansiColorOfArea);
						printf("%*i ", _numberOfDigitsInMaxElement, mtx[i][j]);
					}
					else
					{
						printf("%s", Log::ansiColorReset);
						printf("%*i ", _numberOfDigitsInMaxElement, mtx[i][j]);
					}

					printf("%s", Log::ansiColorReset);
				}
				printf(" |\n");
			}
		}
	private:
		template< template<class> class MatrixType, typename ElementType >
		void _RecalculateNumberOfDigitsInMaxElement(MatrixType<ElementType> &mtx);
	};

	template<>
	void MatrixVisualizer::_RecalculateNumberOfDigitsInMaxElement<SquareMatrix, int>(SquareMatrix<int> &mtx)
	{
		size_t order = mtx.GetOrder();
		int *max = nullptr;

		for (size_t i(0); i < order; ++i)
			for (size_t j(0); j < order; ++j)
				if (max == nullptr || mtx[i][j] > (*max))
					max = &mtx[i][j];

		if (max == nullptr) // элементов нет
			_numberOfDigitsInMaxElement = 0;
		else
			for (int tmpMax = *max; tmpMax; tmpMax /= 10)
				++_numberOfDigitsInMaxElement;

	}

	template<>
	void MatrixVisualizer::Print<SquareMatrix, int>(SquareMatrix<int> &mtx)
	{
		if (_isMatrixChanged)
		{
			_RecalculateNumberOfDigitsInMaxElement(mtx);
			_isMatrixChanged = false;
		}

		size_t order = mtx.GetOrder();
		printf("Matrix %ux%u:\n", order, order);
		for (int i = 0; i < order; ++i)
		{
			printf("| ");
			for (int j = 0; j < order; ++j)
				printf("%*i ", _numberOfDigitsInMaxElement, mtx[i][j]);
			printf(" |\n");
		}
	}
}