#pragma once
#include <fngn_vk/base.h>
#include <fngn_vk/swap_chain.h>

namespace fngn_vk
{
	class graphics_pipeline
	{
		PREVENT_COPY(graphics_pipeline);

		graphics_pipeline(const swap_chain& swap_chain);
	};
}