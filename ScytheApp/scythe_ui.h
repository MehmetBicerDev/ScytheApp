#pragma once
#include <netincludes.h>





namespace scythe
{
	namespace ui
	{
		extern int header_height;
		void render();
		void insert_newlines_in_text(std::string& text, float max_width_threshold);
	}
}