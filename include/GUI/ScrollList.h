#pragma once

#include <vector>

#include "Interactive.h"

namespace gui
{
	template <typename type> class scrollList final : public Interactive
	{
		static_assert(std::is_base_of<Interactive, type>::value,
			"Template Argument must inherit from class gui::Interactive.");
	public:

	private:
		std::vector<type> elements;
	};
};