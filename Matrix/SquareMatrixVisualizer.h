#pragma once

#include "MatrixElement.h"
#include "MatrixAlgorithm.h"
#include "SquareMatrix.h"
#include "Log.h"
#include <stdio.h>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Matrix
{
	template<typename ValueType>
	struct ElementToVisualize
	{
		Element<ValueType> element;
		char const * ansiColorOfElement;
	};
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

		// Требует предварительного использования print в пустой консоли
		template< template<class> class MatrixType, typename ElementType, typename AreaFunction >
		void Update(
			MatrixType<ElementType> &mtx,
			AreaFunction areaFunc, char const* ansiColorOfArea,
			Array<ElementToVisualize<ElementType>> elements
		);
		template<typename AreaFunction>
		void Update(SquareMatrix<int>& mtx, AreaFunction isInAreaToColor, char const * ansiColorOfArea, Array<ElementToVisualize<int>> &elements)
		{
			static const HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_SCREEN_BUFFER_INFO consolescreenBufferInfo;
			GetConsoleScreenBufferInfo(out, &consolescreenBufferInfo);

			COORD elementBegin = { 0,0 };
			COORD sourcePos = { consolescreenBufferInfo.dwCursorPosition.X, consolescreenBufferInfo.dwCursorPosition.Y };

			size_t order = mtx.GetOrder();
			for (int i = 0; i < order; ++i)
			{
				elementBegin.X = 2; // skip "| "
				SetConsoleCursorPosition(out, elementBegin);
				for (int j = 0; j < order; ++j)
				{
					Element<int> current( i, j, &mtx[i][j] );

					bool isElementAredyColored = false;
					auto elementsCount = elements.GetSize();
					for (int elementId = 0; elementId < elementsCount; ++elementId)
					{
						auto candidate = elements[elementId].element;
						if (candidate.column == current.column
							&& candidate.row == current.row)
						{
							printf("%s%*i ", elements[elementId].ansiColorOfElement, _numberOfDigitsInMaxElement, mtx[i][j]);
							isElementAredyColored = true;
							break;
						}
					}

					if (!isElementAredyColored)
					{
						if (isInAreaToColor(current))
						{
							printf("%s%*i ", ansiColorOfArea, _numberOfDigitsInMaxElement, mtx[i][j]);
						}
						else
						{
							printf("%*i ", _numberOfDigitsInMaxElement, mtx[i][j]);
						}
					}

					printf("%s", Log::ansiColorReset);

					elementBegin.X += _numberOfDigitsInMaxElement;
				}
				elementBegin.Y += 1;// go to next row
			}
			SetConsoleCursorPosition(out, sourcePos);
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
				if (max == nullptr || abs(mtx[i][j]) > abs(*max))
					max = &mtx[i][j];

		if (max == nullptr) // элементов нет
			_numberOfDigitsInMaxElement = 0;
		else
		{
			for (int tmpMax = *max; tmpMax; tmpMax /= 10)
				++_numberOfDigitsInMaxElement;

			if (*max < 0)
				++_numberOfDigitsInMaxElement;
		}

	}

	template<>
	void MatrixVisualizer::Print<SquareMatrix, int>(SquareMatrix<int> &mtx)
	{
		if (_isMatrixChanged)
		{
			_RecalculateNumberOfDigitsInMaxElement(mtx);
			_isMatrixChanged = false;
		}

		auto order = mtx.GetOrder();

		for (int i = 0; i < order; ++i)
		{
			printf("| ");
			for (int j = 0; j < order; ++j)
				printf("%*i ", _numberOfDigitsInMaxElement, mtx[i][j]);
			printf(" |\n");
		}
	}
}