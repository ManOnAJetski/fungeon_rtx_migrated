#include <fngn_avtk_hello_triangle.h>
#include <imgui_manager.hpp>
#include <imgui.h>

fngn_avtk_hello_triangle::fngn_avtk_hello_triangle(avk::queue& aQueue)
	: mQueue{ &aQueue }
{
}

void fngn_avtk_hello_triangle::initialize()
{
	// Print some information about the available memory on the selected physical device:
	avk::context().print_available_memory_types();

	// Create a graphics pipeline:
	mPipeline = avk::context().create_graphics_pipeline_for(
		avk::vertex_shader("shaders/triangle.vert.spv"),
		avk::fragment_shader("shaders/triangle.frag.spv"),
		avk::cfg::front_face::define_front_faces_to_be_clockwise(),
		avk::cfg::viewport_depth_scissors_config::from_framebuffer(avk::context().main_window()->backbuffer_reference_at_index(0)),
		// Just use the main window's renderpass for this pipeline:
		avk::context().main_window()->get_renderpass()
	);

	// We want to use an updater => gotta create one:
	mUpdater.emplace();
	mUpdater->on(
		avk::swapchain_resized_event(avk::context().main_window()), // In the case of window resizes,
		avk::shader_files_changed_event(mPipeline.as_reference())   // or in the case of changes to the shader files (hot reloading), ...
	)
		.update(mPipeline); // ... it will recreate the pipeline.		

	auto imguiManager = avk::current_composition()->element_by_type<avk::imgui_manager>();
	if (nullptr != imguiManager) {
		imguiManager->add_callback([this]() {
			bool isEnabled = this->is_enabled();
			ImGui::Begin("Hello, world!");
			ImGui::SetWindowPos(ImVec2(1.0f, 1.0f), ImGuiCond_FirstUseEver);
			ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
			ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
			ImGui::Checkbox("Enable/Disable invokee", &isEnabled);
			if (isEnabled != this->is_enabled())
			{
				if (!isEnabled) this->disable();
				else this->enable();
			}
			static std::vector<float> values;
			values.push_back(1000.0f / ImGui::GetIO().Framerate);
			if (values.size() > 90) {
				values.erase(values.begin());
			}
			ImGui::PlotLines("ms/frame", values.data(), static_cast<int>(values.size()), 0, nullptr, 0.0f, FLT_MAX, ImVec2(0.0f, 100.0f));
			ImGui::End();
			});
	}
}

void fngn_avtk_hello_triangle::update()
{
	// On H pressed,
	if (avk::input().key_pressed(avk::key_code::h)) {
		// log a message:
		LOG_INFO_EM("Hello fngn_avtk!");
	}

	// On C pressed,
	if (avk::input().key_pressed(avk::key_code::c)) {
		// center the cursor:
		auto resolution = avk::context().main_window()->resolution();
		avk::context().main_window()->set_cursor_pos({ resolution[0] / 2.0, resolution[1] / 2.0 });
	}

	// On Esc pressed,
	if (avk::input().key_pressed(avk::key_code::escape) || avk::context().main_window()->should_be_closed()) {
		// stop the current composition:
		avk::current_composition()->stop();
	}
}

void fngn_avtk_hello_triangle::render()
{
	auto mainWnd = avk::context().main_window();

	// The main window's swap chain provides us with an "image available semaphore" for the current frame.
	// Only after the swapchain image has become available, we may start rendering into it.
	auto imageAvailableSemaphore = mainWnd->consume_current_image_available_semaphore();

	// Get a command pool to allocate command buffers from:
	auto& commandPool = avk::context().get_command_pool_for_single_use_command_buffers(*mQueue);

	// Create a command buffer and render into the *current* swap chain image:
	auto cmdBfr = commandPool->alloc_command_buffer(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

	avk::context().record({
		// Begin and end one renderpass:
		avk::command::render_pass(mPipeline->renderpass_reference(), avk::context().main_window()->current_backbuffer_reference(), {
			// And within, bind a pipeline and draw three vertices:
			avk::command::bind_pipeline(mPipeline.as_reference()),
			avk::command::draw(3u, 1u, 0u, 0u)
		})
		})
		.into_command_buffer(cmdBfr)
		.then_submit_to(*mQueue)
		.waiting_for(imageAvailableSemaphore >> avk::stage::color_attachment_output)
		.submit();

	// Use a convenience function of avk::window to take care of the command buffer's lifetime:
	// It will get deleted in the future after #concurrent-frames have passed by.
	avk::context().main_window()->handle_lifetime(std::move(cmdBfr));
}
