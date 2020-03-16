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
#include <string_view>
#include <functional>

// Вариант 10.

template<typename IterT>
auto GetA8(IterT begin, IterT end)
{
	// А8(Х) - корень квадратный из суммы положительных элементов массива Х;
	return std::sqrt(
		std::accumulate(begin, end, uint64_t{0}, [](uint64_t &sum, decltype(*begin) const &element) {
			return sum += element > 0 ? element : 0; 
		})
	);
}
template<typename IterT>
auto GetA10(IterT begin, IterT end)
{
	// А10(Х) - сумма корней квадратных из положительных элементов массива Х;
	return std::accumulate(begin, end, double{0}, [](double &sum, decltype(*begin) const &current)
	{
		return sum += current > 0 ? std::sqrt(current) : 0;
	});
}

template<typename IterT>
auto GetM5(IterT begin, IterT end)
{
	// М5(Х)- количество элементов массива Х, значения которых больше А8.
	auto const a8 = GetA8(begin, end);
	return std::count_if(begin, end, [a8](decltype(*begin) const &element) { return element > a8; });
}

template<typename XIterT, typename YIterT>
auto GetZ(XIterT xbegin, XIterT xend, YIterT ybegin, YIterT yend)
{
	auto a10y = GetA10(ybegin, yend);
	return GetM5(xbegin, xend) > 2 ? 33.51 * std::pow( std::sin(a10y), 2 )
						: a10y + GetM5(ybegin, yend) * std::cos(GetA10(xbegin, xend));

}

template<typename CharT, typename ValueType>
auto GetValueFromUser(std::basic_string_view<CharT> message, std::function<bool(ValueType&)> const &isValueValid)
{
	using namespace std;

	ValueType value = 0;
	bool isValueInvalid = true;

	while (isValueInvalid)
	{
		cout << message;
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
template<typename CharT, typename ValueType>
auto GetValueFromUser(CharT const *message, std::function<bool(ValueType&)> const &isValueValid)
{
	return GetValueFromUser(std::basic_string_view<CharT>{message}, isValueValid);
}

int main()
{
	std::function<bool(std::int64_t&)> isItPositive = [](std::int64_t &value) { return value > 0; };
	size_t xSize = GetValueFromUser("Eneter x size:\n", isItPositive), 
		   ySize = GetValueFromUser("Eneter y size:\n", isItPositive);

	std::vector<int> x(xSize), y(ySize);

	std::srand(unsigned(std::time(0)));
	std::generate(x.begin(), x.end(), [](){ return std::rand() % 2000 - 1000; });
	std::generate(y.begin(), y.end(), [](){ return std::rand() % 2000 - 1000; });

	std::cout << "x = "; std::copy(x.cbegin(), x.cend(), std::ostream_iterator<decltype(*x.cbegin())>(std::cout, " ")); std::cout << '\n';
	std::cout << "y = "; std::copy(y.cbegin(), y.cend(), std::ostream_iterator<decltype(*y.cbegin())>(std::cout, " ")); std::cout << '\n';

	setlocale(LC_ALL, "ru");

	auto const a10y = GetA10(y.cbegin(), y.cend()), a10x = GetA10(x.cbegin(), x.cend());
	auto const m5y = GetM5(y.cbegin(), y.cend()), m5x = GetM5(x.cbegin(), x.cend());

	std::cout   << "\na8(arr) - корень квадратный из суммы положительных элементов массива arr\n"
				<< "a8(x)  = " << GetA8(x.cbegin(), x.cend()) << '\n'
				<< "a8(y)  = " << GetA8(y.cbegin(), y.cend()) << '\n'
				<< "\na10(arr) - сумма корней квадратных из положительных элементов массива arr\n"
				<< "a10(x) = " << a10x << '\n'
				<< "a10(y) = " << a10y << '\n'
				<< "\nm5(arr) - количество элементов массива arr, значения которых больше a8(arr)\n"
				<< "m5(x)  = " << m5x << '\n'
				<< "m5(y)  = " << m5y << '\n'
				<< "\nz = m5(x) > 2 ? 33.51*(sin(a10(y)^2) : a10(y) + m5(y)*cos(a10(x))\n"
				<< "z = "<< m5x << " > 2 ? 33.51*(sin("<< a10y << "^2) : " << a10y << " + " << m5y << "*cos(" << a10x << ")\n"
				<< "z = " << (m5x > 2) << " ? " << 33.51 * std::pow( std::sin(a10y), 2 ) << " : " << a10y + m5y*std::cos(a10x) << '\n'
				<< "z = " << GetZ(x.cbegin(), x.cend(), y.cbegin(), y.cend()) << "\n\n";

	system("pause");

    return 0;
}

