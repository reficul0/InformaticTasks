#pragma once

#include "MatrixElement.h"
#include "MatrixAlgorithm.h"
#include "SquareMatrix.h"
#include "Log.h"
#include <stdio.h>
#include <iostream>
#include <iomanip> 

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

		template< template<typename Type> class MatrixType, typename ElementType >
		void Print(MatrixType<ElementType> *mtxPtr) 
		{
			if (_isMatrixChanged)
			{
				_RecalculateNumberOfDigitsInMaxElement(mtxPtr);
				_isMatrixChanged = false;
			}

			size_t rows = mtxPtr->GetRows();
			size_t columns = mtxPtr->GetColumns();

			for (int i = 0; i < rows; ++i)
			{
				std::cout << "| ";
				for (int j = 0; j < columns; ++j)
					std::cout << std::setfill(' ') << std::setw(_numberOfDigitsInMaxElement) << (*mtxPtr)[i][j] << std::ends << " ";
				std::cout << " |" << std::endl;
			}
		}

		// Требует предварительного использования print в пустой консоли
		template< template<class> class MatrixType, typename ElementType, typename AreaFunction >
		void Update(
			MatrixType<ElementType> *mtxPtr,
			AreaFunction isInAreaToColor, char const* ansiColorOfArea,
			Array<ElementToVisualize<ElementType>> &elements
		) 
		{
			static const HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_SCREEN_BUFFER_INFO consolescreenBufferInfo;
			GetConsoleScreenBufferInfo(out, &consolescreenBufferInfo);

			COORD elementBegin = { 0,0 };
			COORD sourcePos = { consolescreenBufferInfo.dwCursorPosition.X, consolescreenBufferInfo.dwCursorPosition.Y };

			size_t rows = mtxPtr->GetRows();
			size_t columns = mtxPtr->GetColumns();
			for (int i = 0; i < rows; ++i)
			{
				elementBegin.X = 2; // skip "| "
				SetConsoleCursorPosition(out, elementBegin);
				for (int j = 0; j < columns; ++j)
				{
					Element<ElementType> current(i, j, &(*mtxPtr)[i][j]);

					bool isElementAredyColored = false;
					auto elementsCount = elements.GetSize();
					for (int elementId = 0; elementId < elementsCount; ++elementId)
					{
						auto candidate = elements[elementId].element;
						if (candidate.column == current.column
							&& candidate.row == current.row)
						{
							std::cout << elements[elementId].ansiColorOfElement << std::setfill(' ') << std::setw(_numberOfDigitsInMaxElement) <<  (*mtxPtr)[i][j] << std::ends << " ";
							isElementAredyColored = true;
							break;
						}
					}

					if (!isElementAredyColored)
					{
						if (isInAreaToColor(current))
						{
							std::cout << ansiColorOfArea << std::setfill(' ') << std::setw(_numberOfDigitsInMaxElement) << (*mtxPtr)[i][j] << std::ends << " ";
						}
						else
						{
							std::cout << std::setfill(' ') << std::setw(_numberOfDigitsInMaxElement) << (*mtxPtr)[i][j] << std::ends << " ";
						}
					}

					std::cout << Log::ansiColorReset;

					elementBegin.X += _numberOfDigitsInMaxElement;
				}
				elementBegin.Y += 1;// go to next row
			}
			SetConsoleCursorPosition(out, sourcePos);
		}
	private:
		template< template<class> class MatrixType, typename ElementType >
		void _RecalculateNumberOfDigitsInMaxElement(MatrixType<ElementType> *mtx)
		{
			size_t rows = mtx->GetRows();
			size_t columns = mtx->GetColumns();

			ElementType *max = nullptr;

			for (size_t i(0); i < rows; ++i)
				for (size_t j(0); j < columns; ++j)
					if (max == nullptr || abs((*mtx)[i][j]) > abs(*max))
						max = &(*mtx)[i][j];

			if (max == nullptr) // элементов нет
				_numberOfDigitsInMaxElement = 0;
			else
			{
				for (ElementType tmpMax = *max; tmpMax; tmpMax /= 10)
					++_numberOfDigitsInMaxElement;

				if (*max < 0)
					++_numberOfDigitsInMaxElement;
			}
		}
	};
}