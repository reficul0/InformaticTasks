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

#include <experimental/filesystem>
/*
10. ������ ����������� ������ ������ ���� �������������� ������� ��������� ������������� (�������, ������, ������������ � �.�.), �� ������ ����� ������ �������������. ��������� ���������, ������� ������������� �� �������:
| ����� ������������ |								�����									   |
|					 | ������ | ������� | �������� | ��� | ��������� | ������������ | �������� |

��������� ����� �� ��������� ������ �����������.
*/

class �linic
{
public:
	using EmployeesT = std::map<std::string/*vacancy name*/, size_t/*specialists count*/>;

	template<typename InEmployeesT>
	�linic(std::string const &number, InEmployeesT &&employees)
		: Number(std::move(number))
		, Employees(std::forward<InEmployeesT>(employees))
	{
	}

	std::string Number;
	EmployeesT Employees;
};

class Transfer�linic {
public:
	friend std::istream &operator >> (std::istream &in, Transfer�linic &dest)
	{
		in >> dest.number;

		while (!in.eof() && (in.peek() != '\n'))
		{
			�linic::EmployeesT::key_type employeeSpecialization;
			in >> employeeSpecialization;
			dest.employees.emplace(std::move(employeeSpecialization), 1);
		}
		return in;
	}

	operator �linic() const {
		return �linic( std::move(number), std::move(employees) );
	}

private:
	std::string number;
	�linic::EmployeesT mutable employees;
};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	std::vector<�linic> clinics;
	std::ifstream file;
	file.open("input.txt", std::ifstream::in);

	if (!file.is_open())
	{ 
		std::cout << "An error occurred while opening a file \"" << std::experimental::filesystem::current_path() << "\\input.txt\"\n";
		system("pause");
		return -1;
	}

	std::copy(std::istream_iterator<Transfer�linic>(file), std::istream_iterator<Transfer�linic>(), std::back_inserter(clinics));

	std::string specialistName;
	std::cout << "Enter any specialization name: ";
	std::cin >> specialistName;

	std::vector<std::string> thereIsVacancyOpen;
	for (auto const& clinic : clinics)
		if (clinic.Employees.find(specialistName) == clinic.Employees.end())
			thereIsVacancyOpen.push_back(clinic.Number);

	if (!thereIsVacancyOpen.empty())
	{
		std::cout << "\nThere is a vacancies in clinics(" << thereIsVacancyOpen.size() << "): \n";
		std::copy(thereIsVacancyOpen.cbegin(), thereIsVacancyOpen.cend(), std::ostream_iterator<decltype(*thereIsVacancyOpen.cbegin())>(std::cout, "\n"));
		std::cout << "\n";
	}
	else
		std::cout << "\nNo vacancies were found";

    return 0;
}

