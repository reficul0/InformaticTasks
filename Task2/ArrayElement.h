#pragma once

namespace Arr
{
	template <typename ElementType>
	struct Element
	{
		size_t id;
		ElementType *value = nullptr;


		Element()
		{
		}
		Element(size_t id, ElementType *Value)
			: id(id)
			, value(Value)
		{
		}

		Element operator=(Element other)
		{
			id = other.id;
			value = other.value;

			return *this;
		}
	};
}