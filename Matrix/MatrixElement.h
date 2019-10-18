#pragma once

namespace Matrix
{
	template <typename ElementType>
	struct Element
	{
		size_t row;
		size_t column;
		ElementType *value = nullptr;


		Element()
		{
		}
		Element(size_t Row, size_t Column, ElementType *Value)
			: row(Row)
			, column(Column)
			, value(Value)
		{
		}

		Element operator=(Element other)
		{
			row = other.row;
			column = other.column;
			value = other.value;

			return *this;
		}
	};
}