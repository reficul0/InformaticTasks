#pragma once
#pragma once

#include <functional>

#include "../Matrix/Array.h"
#include "ArrayElement.h"

namespace Arr
{
	namespace Algorithm
	{
		template< class ElementType, class Function>
		void ForEach(Array<ElementType> *arr, Function const &func)
		{
			size_t size = arr->GetSize();
			for (size_t i(0); i < size; ++i)
				func( Element<ElementType>{ i, &(*mtx)[i][j] } );
		}
	}
}