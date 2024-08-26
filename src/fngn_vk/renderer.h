#pragma once
#include <fngn_vk/base.h>
#include <memory>
#include <vector>

namespace fngn_vk
{
	class swap_chain;
	class logical_device;
	class pipeline_layout;
	class render_pass;
	class graphics_pipeline;
	class shader;
	class frame_buffer;
	class command_pool;
	class command_buffer;
	class binary_semaphore;
	class fence;

	class renderer
	{
	public:
		PREVENT_COPY(renderer);

		renderer(const logical_device& device);

		void draw();

	private:
		std::unique_ptr<fngn_vk::swap_chain> m_swap_chain;
		std::unique_ptr<fngn_vk::pipeline_layout> m_pipeline_layout;
		std::unique_ptr<fngn_vk::render_pass> m_render_pass;
		std::unique_ptr<fngn_vk::graphics_pipeline> m_graphics_pipline;
		std::vector<std::unique_ptr<fngn_vk::shader>> m_shaders;
		std::vector< std::unique_ptr<fngn_vk::frame_buffer>> m_swap_chain_frame_buffers;
		std::unique_ptr<fngn_vk::command_pool> m_command_pool;
		std::unique_ptr<fngn_vk::command_buffer> m_command_buffer;
		std::unique_ptr<fngn_vk::binary_semaphore> m_image_available_semaphore;
		std::unique_ptr<fngn_vk::binary_semaphore> m_render_finished_semaphore;
		std::unique_ptr<fngn_vk::fence> m_in_flight_fence;
	};
}