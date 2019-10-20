#pragma once

#include <cctype>
#include <time.h>
#include <type_traits>
#include <cstdint>
#include <iostream>

namespace Matrix
{
	namespace Tools
	{
		size_t GetSizeValueFromUser(const char* message)
		{
			using namespace std;

			int64_t value = 0;
			bool isValueInvalid = true;

			while (isValueInvalid)
			{
				cout << message << ": ";
				cin >> value;

				bool isStreamInFailureState = cin.fail(),
					isValuePositive = value > 0;
				isValueInvalid = isStreamInFailureState || !isValuePositive;

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
		size_t GetMatrixOrderFromUser()
		{
			return GetSizeValueFromUser("Enter the order of the matrix");
		}
	}
}