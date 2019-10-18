// HomeWork1_10.cpp : Defines the entry point for the console application.
//


#include "pch.h"
#include <windows.h>
#include <stdio.h>

#include <random>
#include <time.h>

#include "SquareMatrix.h"
#include "MatrixVisualizer.h"
#include "Log.h"

#include "MatrixAlgorithmVisualization.h"


size_t GetMatrixOrderFromUser()
{
	size_t order = 0;
	printf("Enter the order of the matrix: ");
	scanf("%u", &order);

	system("cls");

	return order;
}

void RandomElements(Matrix::SquareMatrix<int> &mtx)
{
	srand(time(NULL));

	int min = 0,
		max = 0;

	printf("Enter the lower bound for random: ");
	scanf("%i", &min);

	printf("Enter the upper bound for random: ");
	scanf("%i", &max);
	printf("\n");

	bool isMinEqualsMax = false;

	if (min > max)// корректируем значения, если нужно
	{
		Log::Warning("Upper bound is less than lower. They will be swapped.");
		int tmp = max;
		max = min;
		min = tmp;
	}
	else if (min == max)
	{
		Log::Warning("Upper bound is equals lower.");
		isMinEqualsMax = true;
	}

	size_t const order = mtx.GetOrder();

	Matrix::Algorithm::ForEach(&mtx, [&](Matrix::Element<int> &elem)
	{
		*(elem.value) = isMinEqualsMax
					? min
					: (min + rand() % (max - min));
	});
}

int main(int argc, char *argv[])
{
	Matrix::SquareMatrix<int> mtx(GetMatrixOrderFromUser());

	RandomElements(mtx);

	Matrix::MatrixVisualizer visualizer;

	size_t order = mtx.GetOrder();
	printf("Matrix %ux%u:\n", order, order);
	visualizer.Print(&mtx);

	size_t timeoutBetweenSteps;
	printf("\nEnter timeout between steps(msec): ");
	scanf("%u", &timeoutBetweenSteps);
	printf("\n");

	system("pause");
	system("cls");

	auto elementsUpperSideDiagonalFunction = [&mtx](Matrix::Element<int> &elem)
	{
		return elem.column < (mtx.GetOrder() - elem.row);
	};

	auto maxElem = Matrix::Algorithm::Visualization::FindElement(
		&mtx, 
		elementsUpperSideDiagonalFunction,
		Log::ansiColorYellow,
		[](Matrix::Element<int> &last, Matrix::Element<int> &second)
		{
			return (*last.value) < (*second.value);
		},
		Log::ansiColorGreen,
		Log::ansiColorRed,
		timeoutBetweenSteps
	);

	printf("%sMax element upper side diagonal is:\nmtx[%i][%i] == %i\n", Log::ansiColorGreen, maxElem.row, maxElem.column, *maxElem.value);
	printf("%s", Log::ansiColorReset);

	system("pause");

	return 0;
}

