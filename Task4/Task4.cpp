// Task4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <iostream>
#include <stdio.h>
#include <map>
#include <algorithm>
#include <locale>
#include <Windows.h>
#include <functional>

#include <experimental/filesystem>
/*
10. Каждая поликлиника города должна быть укомплектована врачами различной специализации (окулист, хирург, невропатолог и т.д.), по одному врачу каждой специальности. Составить программу, которая распечатывает по запросу:
| Номер поликлинники |								Штаты									   |
|					 | Хирург | Окулист | Невролог | Лор | Кардиолог | Эндокринолог | Терапевт |

вакантные места по заданному узкому специалисту.
*/

class Clinic
{
public:
	using EmployeesT = std::map<std::string/*vacancy name*/, size_t/*specialists count*/>;

	template<typename StringT, typename InEmployeesT>
	Clinic(StringT &&name, InEmployeesT &&employees)
		: Name(std::forward<StringT>(name))
		, Employees(std::forward<InEmployeesT>(employees))
	{
	}

	std::string Name;
	EmployeesT Employees;
};

class TransferClinic {
public:
	friend std::istream &operator >> (std::istream &in, TransferClinic &dest)
	{
		in >> dest.name;

		while (!in.eof() && (in.peek() != '\n'))
		{
			Clinic::EmployeesT::key_type employeeSpecialization;
			Clinic::EmployeesT::value_type::second_type employeesCount(0);
			in >> employeeSpecialization >> employeesCount;
			dest.employees.emplace(std::move(employeeSpecialization), employeesCount);
		}
		return in;
	}

	operator Clinic() const {
		return Clinic( std::move(name), std::move(employees) );
	}

private:
	std::string name;
	Clinic::EmployeesT mutable employees;
};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	std::ifstream file("input.txt", std::ifstream::in);
	if (!file.is_open())
	{ 
		std::cout << "An error occurred while opening a file \"" << std::experimental::filesystem::current_path() << "\\input.txt\"\n";
		system("pause");
		return -1;
	}

	auto clinics = std::vector<Clinic>(std::istream_iterator<TransferClinic>(file), std::istream_iterator<TransferClinic>());

	std::string specialistName;
	std::cout << "Enter any specialization name: ";
	std::cin >> specialistName;

	std::vector<std::string> thereAreIsOpenedVacancy;
	for (auto const& clinic : clinics)
	{
		auto const &employee = clinic.Employees.find(specialistName);
		if (employee == clinic.Employees.cend() || employee->second == 0)
			thereAreIsOpenedVacancy.push_back(clinic.Name);
	};

	if (!thereAreIsOpenedVacancy.empty())
	{
		std::cout << "\nThere is a vacancies in clinics(" << thereAreIsOpenedVacancy.size() << " of " << clinics.size() << "): \n";
		std::copy(thereAreIsOpenedVacancy.cbegin(), thereAreIsOpenedVacancy.cend(), std::ostream_iterator<decltype(*thereAreIsOpenedVacancy.cbegin())>(std::cout, "\n"));
		std::cout << "\n";
	}
	else
		std::cout << "\nNo vacancies were found";

    return 0;
}

