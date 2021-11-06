#include "input.h"

using namespace king;

void Input::append_left_mouse_input(MousePosition& entry)
{
        current_left_mouse_entry = entry;
}

void Input::append_right_mouse_input(MousePosition& entry)
{
	current_right_mouse_entry = entry;
}
