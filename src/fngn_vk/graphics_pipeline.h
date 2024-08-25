#pragma once
#include <fngn_vk/base.h>

namespace fngn_vk
{
	class swap_chain;

	class graphics_pipeline
	{
		PREVENT_COPY(graphics_pipeline);

		graphics_pipeline(const swap_chain& swap_chain);
	};
}