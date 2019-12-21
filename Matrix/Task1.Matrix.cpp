// HomeWork1_10.cpp : Defines the entry point for the console application.
//


#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <random>
#include <time.h>

#include "Log.h"
#include "Demonstrarion.h"
#include "MatrixTools.h"

#ifdef WIN32
	#undef min
	#undef max
#endif

/*
Вариант 10.
Написать программу, которая:
1. Создает динамическую матрицу размерности, элементы матрицы А – целые числа, заданные случайным образом. 
2. Найти минимальный элемент матрицы, лежащий выше побочной диагонали.
2. Изменить порядок столбцов матрицы – упорядочить столбцы матрицы по возрастанию первого элемента столбца.
*/

template<
	template<class> class MatrixType, 
	typename ElementType,
	typename = std::enable_if_t<std::is_arithmetic<ElementType>::value>
>
void RandomElements(MatrixType<ElementType> *mtx)
{
	srand(time(NULL));

	ElementType min = 0,
				max = 0;

	std::cout << "Enter the lower bound for random: ";
	std::cin >> min;

	std::cout << "Enter the upper bound for random: ";
	std::cin >> max;
	std::cout << std::endl;

	bool isMinEqualsMax = false;

	if (min > max)// корректируем значения, если нужно
	{
		Log::Warning("Upper bound is less than lower. They will be swapped.");
		ElementType tmp = max;
		max = min;
		min = tmp;
	}
	else if (min == max)
	{
		Log::Warning("Upper bound is equals lower.");
		isMinEqualsMax = true;
	}

	Matrix::Algorithm::ForEach(mtx, [&](Matrix::Element<ElementType> &elem)
	{
		*(elem.value) = isMinEqualsMax
						? min
						: (min + rand() % (max - min));
	});
}


int main(int argc, char *argv[])
{
	using ElementType = int;
	Matrix::Matrix<ElementType>::Ptr mtx;
	size_t const mtxOrder = Matrix::Tools::GetValueFromUser<int64_t>(
		"Enter the order of the matrix > 0",
		[](int64_t const &value) noexcept { return value > 0; }
	);
	try
	{
		mtx = std::make_shared< Matrix::SquareMatrix<ElementType>>(mtxOrder);
	}
	catch (std::bad_alloc &)
	{
		Log::Error("Memory allocation error.");
		return -1;
	}

	RandomElements(mtx.get());

	Demostration::Demonstrator demonstrator;
	demonstrator.Demonstrate(mtx.get());
		

	return 0;
}

