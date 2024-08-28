#pragma once
#include <invokee.hpp>

class fngn_avtk_hello_triangle : public avk::invokee
{
public:
	fngn_avtk_hello_triangle(avk::queue& queue);

	void initialize() override;
	void update() override;
	void render() override;

private:

	avk::queue* mQueue;
	avk::graphics_pipeline mPipeline;
};