#include "Input.h"

Input::KeyboardInput Input::keys[Key::COUNT];

Input::KeyboardInput Input::GetKey(Key key) {
	return keys[key];
}

bool Input::GetKeyDown(Key key) {
	return keys[key].is_down;
}

void Input::HandleKeyboardInput(UINT_PTR key_code, bool is_down) {
	switch (key_code) {
	case VK_SPACE:
		if (keys[SPACE].is_down != is_down)
			keys[SPACE].frame_number = 0;
		keys[SPACE].is_down = is_down;
		break;
	case VK_ESCAPE:
		if (keys[ESCAPE].is_down != is_down)
			keys[ESCAPE].frame_number = 0;
		keys[ESCAPE].is_down = is_down;
		break;
	case VK_RETURN:
		if (keys[ENTER].is_down != is_down)
			keys[ENTER].frame_number = 0;
		keys[ENTER].is_down = is_down;
		break;
	case 'W':
		if (keys[W].is_down != is_down)
			keys[W].frame_number = 0;
		keys[W].is_down = is_down;
		break;
	case 'A':
		if (keys[A].is_down != is_down)
			keys[A].frame_number = 0;
		keys[A].is_down = is_down;
		break;
	case 'S':
		if (keys[S].is_down != is_down)
			keys[S].frame_number = 0;
		keys[S].is_down = is_down;
		break;
	case 'D':
		if (keys[D].is_down != is_down)
			keys[D].frame_number = 0;
		keys[D].is_down = is_down;
		break;
	}
}

void Input::Reset() {
	for (int i = 0; i < Key::COUNT; i++) {
		keys[i].is_down = false;
	}
}

void Input::Update()
{
	for (int i = 0; i < Key::COUNT; ++i)
	{
		++keys[i].frame_number;
	}
}