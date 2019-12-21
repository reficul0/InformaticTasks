// Task3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <type_traits>

/*
	Вариант 10.
	Вычислить 1/(1+
				1/(3+
					(... + 1/(101 
							+ 1/103
							)
					)
				)
			 )
*/

template<std::uint64_t lowerDivisor, std::uint64_t countOfStepsDown = 0, 
	typename = std::enable_if_t<lowerDivisor % 2>
>
struct Fraction
{
private:
	static std::uint64_t constexpr left = lowerDivisor - (lowerDivisor - (1 + countOfStepsDown * 2));
public:
	static std::uint64_t constexpr numerator = 1;
	static double constexpr denomirator = left + Fraction<lowerDivisor - 2, countOfStepsDown + 1>::calculate;

	static double constexpr calculate = numerator / denomirator;
};
template<std::uint64_t countOfStepsDown>
struct Fraction<1, countOfStepsDown>
{
	static std::uint64_t constexpr numerator = 1;
	static double constexpr denomirator = 1 + countOfStepsDown * 2;

	static double constexpr calculate = numerator / denomirator;
};

int main()
{
	constexpr auto d = Fraction<103>();
	std::cout << "Calculate 1/(1+ 1/(3+(... + 1/(101 + 1/103) )))";
	std::cout << "\nCalculation result: " << std::setprecision(std::numeric_limits<double>::digits10) << d.calculate << '\n';
	std::cout << d.numerator << " / " << std::setprecision(std::numeric_limits<double>::digits10) << d.denomirator << '\n';

    return 0;
}

