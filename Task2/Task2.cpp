// Task2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <ctime>
#include <iterator>

template<typename ContainerT>
double GetA8(ContainerT const &container)
{
	// А8(Х) - корень квадратный из суммы положительных элементов массива Х;
	auto const posotoveElementsSum = std::accumulate(container.cbegin(), container.cend(), 0, [](auto &res, auto const &element) { return res += element > 0 ? element : 0; });
	return std::sqrt(posotoveElementsSum);
}
template<typename ContainerT>
double GetA10(ContainerT const &container)
{
	// А10(Х) - сумма корней квадратных из положительных элементов массива Х;
	double a10{ 0 };
	for (auto const &i : container)
		if (i > 0)
			a10 += std::sqrt(i);
	return a10;
}

template<typename ContainerT>
size_t GetM5(ContainerT const &container)
{
	// М5(Х)- количество элементов массива Х, значения которых больше А8.
	auto const a8 = GetA8(container);
	return std::count_if(container.cbegin(), container.cend(), [a8](auto const &element) { return element > a8; });
}

template<typename XContainerT, typename YContainerT>
double GetZ(XContainerT const &x, YContainerT const &y)
{
	auto a10y = GetA10(y);
	return GetM5(x) > 2 ? 33.51 * std::pow( std::sin(a10y), 2 )
						: a10y + GetM5(y) * std::cos(GetA10(x));

}

template<typename ValueType, typename IsValueValidFunction, typename MessageT>
ValueType GetValueFromUser(MessageT&& message, IsValueValidFunction isValueValid)
{
	using namespace std;

	ValueType value = 0;
	bool isValueInvalid = true;

	while (isValueInvalid)
	{
		cout << std::forward<MessageT>(message);
		cin >> value;

		bool isStreamInFailureState = cin.fail();
		isValueInvalid = isStreamInFailureState || !isValueValid(value);

		if (isValueInvalid)
		{
			if (isStreamInFailureState)
				cin.clear();// возвращаем объект потока в обычный режим
			cout << "Value is invalid. Try enter again. " << endl;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	system("cls");

	return value;
}

int main()
{
	auto isItPositive = [](auto &value) { return value > 0; };
	size_t xSize = GetValueFromUser<std::int64_t>("Eneter x size:\n", isItPositive), 
		   ySize = GetValueFromUser<std::int64_t>("Eneter y size:\n", isItPositive);

	std::vector<int> x(xSize), y(ySize);

	std::srand(unsigned(std::time(0)));
	std::generate(x.begin(), x.end(), [](){ return std::rand() % 2000 - 1000; });
	std::generate(y.begin(), y.end(), [](){ return std::rand() % 2000 - 1000; });

	std::cout << "x = "; std::copy(x.cbegin(), x.cend(), std::ostream_iterator<decltype(*x.cbegin())>(std::cout, " ")); std::cout << '\n';
	std::cout << "y = "; std::copy(y.cbegin(), y.cend(), std::ostream_iterator<decltype(*y.cbegin())>(std::cout, " ")); std::cout << '\n';

	setlocale(LC_ALL, "ru");

	auto const a10y = GetA10(y), a10x = GetA10(x);
	auto const m5y = GetM5(y), m5x = GetM5(x);

	std::cout   << "\na8(arr) - корень квадратный из суммы положительных элементов массива arr\n"
				<< "a8(x)  = " << GetA8(x) << '\n'
				<< "a8(y)  = " << GetA8(y) << '\n'
				<< "\na10(arr) - сумма корней квадратных из положительных элементов массива arr\n"
				<< "a10(x) = " << a10x << '\n'
				<< "a10(y) = " << a10y << '\n'
				<< "\nm5(arr) - количество элементов массива arr, значения которых больше a8(arr)\n"
				<< "m5(x)  = " << m5x << '\n'
				<< "m5(y)  = " << m5y << '\n'
				<< "\nz = m5(x) > 2 ? 33.51*(sin(a10(y)^2) : a10(y) + m5(y)*cos(a10(x))\n"
				<< "z = "<< m5x << " > 2 ? 33.51*(sin("<< a10y << "^2) : " << a10y << " + " << m5y << "*cos(" << a10x << ")\n"
				<< "z = " << (m5x > 2) << " ? " << 33.51 * std::pow( std::sin(a10y), 2 ) << " : " << a10y + m5y*std::cos(a10x) << '\n'
				<< "z = " << GetZ(x,y) << "\n\n";

	system("pause");

    return 0;
}

