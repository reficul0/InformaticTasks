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
		template<class ElementType>
		static bool IsElementAboveSideDiagonal(Matrix::Element<ElementType>& elem) noexcept
		{
			return elem.column >= elem.row;
		}
		template<class ElementType>
		static bool MaxElement(Matrix::Element<ElementType>& last, Matrix::Element<ElementType>& second) noexcept
		{
			return (*last.value) < (*second.value);
		};

		template< template<class> class MatrixType, typename ElementType>
		Matrix::Element<ElementType> _GetMaxElementWithVisualization(MatrixType<ElementType>* mtx)
		{
			size_t timeoutBetweenSteps = Matrix::Tools::GetValueFromUser<int64_t>(
				"Enter timeout between visuzlization steps(msec), recommended 100",
				[] (int64_t const &value) noexcept { return value > 0; }
			);

			return Matrix::Algorithm::Visualization::FindElement(
				mtx,
				std::bind(&SearchOfMaxElement::IsElementAboveSideDiagonal<ElementType>, std::placeholders::_1),
				Log::ansiColorYellow,
				std::bind(&SearchOfMaxElement::MaxElement<ElementType>, std::placeholders::_1, std::placeholders::_2),
				Log::ansiColorGreen,
				Log::ansiColorRed,
				timeoutBetweenSteps
			);
		}

		template< template<class> class MatrixType, typename ElementType >
		Matrix::Element<ElementType> _GetMaxElement(MatrixType<ElementType>* mtx)
		{
			Matrix::Element<ElementType> element =  Matrix::Algorithm::FindElement(
				mtx,
				std::bind(&SearchOfMaxElement::IsElementAboveSideDiagonal<ElementType>, std::placeholders::_1),
				std::bind(&SearchOfMaxElement::MaxElement<ElementType>, std::placeholders::_1, std::placeholders::_2)
			);

			Array<Matrix::ElementToVisualize<ElementType>> arr(1);
			arr[0] = Matrix::ElementToVisualize<ElementType>{ element, Log::ansiColorGreen };
			Matrix::MatrixVisualizer visualizer;
			visualizer.Print(mtx);
			visualizer.Update(
				mtx,
				std::bind(&SearchOfMaxElement::IsElementAboveSideDiagonal<ElementType>, std::placeholders::_1),
				Log::ansiColorYellow,
				arr
			);

			return element;
		}

	public:
		template< template<class> class MatrixType, typename ElementType >
		void Demonstrate(MatrixType<ElementType>* mtx)
		{
			char const userChoice = Matrix::Tools::GetValueFromUser<char>(
				"Do you want to see the visualization of the serach max element algorithm?(y/yes, n/no)",
				[](char const &value) noexcept { 
					auto const lowerCaseChar = std::tolower(value);
					return lowerCaseChar == 'y' || lowerCaseChar == 'n';
				}
			);
			size_t isVisualizationRequired = userChoice == 'y';

			system("cls");

			Matrix::Element<ElementType> maxElem = isVisualizationRequired 
														? _GetMaxElementWithVisualization(mtx)														   
														: _GetMaxElement(mtx);

			printf("\nMax element upper side diagonal is: %smtx[%i][%i] == ", Log::ansiColorGreen, maxElem.row, maxElem.column);
			std::cout << *maxElem.value << Log::ansiColorReset << std::endl;

			system("pause");
		}
	};

	class SortViaFirstColumnElement
	{
		template<typename ElementType, typename CompareFunction>
		void Quicksort(Array<size_t> &sourceIndexes, Array<ElementType> &arr, int64_t low, int64_t high, CompareFunction isFirst)
		{
			int64_t i = low, j = high, pivotId = (i + j) / 2, tempId;
			ElementType pivot = arr[pivotId];
			ElementType tempValue;

			while (i <= j)
			{
				while ( isFirst(arr[i], pivot) ) i++;
				while ( isFirst(pivot, arr[j]) ) j--;

				if (i <= j)
				{
					tempValue = arr[i];
					arr[i] = arr[j];
					arr[j] = tempValue;

					tempId = sourceIndexes[i];
					sourceIndexes[i] = sourceIndexes[j];
					sourceIndexes[j] = tempId;

					i++;
					j--;
				}
			}

			if (j > low)
				Quicksort(sourceIndexes, arr, low, j, isFirst);
			if (i < high)
				Quicksort(sourceIndexes, arr, i, high, isFirst);
		}

		template< typename ElementType >
		size_t GetNumberOfSymbolsInMaxElement(Array<ElementType> &arr)
		{
			size_t size = arr.GetSize(),
				numberOfSymbolsInMaxElement = 0;

			ElementType* max = nullptr;

			for (size_t i(0); i < size; ++i)
				if (max == nullptr || abs(arr[i]) > abs(*max))
					max = &arr[i];

			if (max == nullptr) // элементов нет
				numberOfSymbolsInMaxElement = 0;
			else
			{
				for (ElementType tmpMax = *max; tmpMax; tmpMax /= 10)
					++numberOfSymbolsInMaxElement;

				if (*max < 0)
					++numberOfSymbolsInMaxElement;
			}

			return numberOfSymbolsInMaxElement;
		}

		template<template<class> class MatrixType, typename ElementType>
		typename MatrixType<ElementType>::Ptr TransformMatrixColumns(Array<size_t>& transformColumns, MatrixType<ElementType>* sourceMtx)
		{
			ElementType tmp;
			typename MatrixType<ElementType>::Ptr result = std::make_shared<MatrixType<ElementType>>(sourceMtx->GetRows(), sourceMtx->GetColumns());

			for (size_t resultColumntId = 0; resultColumntId < transformColumns.GetSize(); ++resultColumntId)
			{
				size_t sourceColumnId = transformColumns[resultColumntId];
				
				for (size_t rowId = 0; rowId < sourceMtx->GetRows(); ++rowId)
				{
					(*result)[rowId][resultColumntId] = (*sourceMtx)[rowId][sourceColumnId];
				}
			}
			return result;
		}
	public:
		template< template<class> class MatrixType, typename ElementType >
		void Demonstrate(MatrixType<ElementType> *mtx)
		{
			// Отсортируем первую строку, запоминая при этом исходные индексы столбцов, 
			// после чего создадим новую матрицу на основе исходной матрицы и трансформации столбцов.

			size_t columns = mtx->GetColumns();

			// инициализация значений первой строки и индексов их столбцов
			Array<ElementType> firstRow(columns);
			Array<size_t> transformColumnsIndexes(columns);
			for (size_t column = 0; column < columns; ++column)
			{
				firstRow[column] = (*mtx)[0][column];
				transformColumnsIndexes[column] = column;
			}

			Quicksort(transformColumnsIndexes, firstRow, 0, columns-1, [](ElementType &first, ElementType &second) noexcept {
				return first < second;
			});

			system("cls");
			// Получаем трансформированную матрицу, у которой элементы первой строки идут в возрастающем порядке.
			typename MatrixType<ElementType>::Ptr transformedMtx = TransformMatrixColumns(transformColumnsIndexes, mtx);

			Matrix::MatrixVisualizer visualzer;
			std::cout << "Matrix before sort:" << std::endl;
			visualzer.Print(mtx); 

			std::cout << std::endl;

			std::cout << "Matrix after sort:" << std::endl;
			visualzer.Print(transformedMtx.get());

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

			SortViaFirstColumnElement sort;
			sort.Demonstrate(mtx);
		}
	};
}