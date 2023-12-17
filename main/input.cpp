#include "input.h"


Input::KeyboardInput Input::keys[static_cast<int>(Key::COUNT)];
Input::LeftButton Input::left_mouse;


void Input::HandleKeyboardInput(UINT_PTR key_code, bool is_down) {
	switch (key_code) {
	case VK_SPACE:
		if (keys[static_cast<int>(Key::SPACE)].is_down != is_down)
			keys[static_cast<int>(Key::SPACE)].frame_number = 0;
		keys[static_cast<int>(Key::SPACE)].is_down = is_down;
		break;
	case VK_ESCAPE:
		if (keys[static_cast<int>(Key::ESCAPE)].is_down != is_down)
			keys[static_cast<int>(Key::ESCAPE)].frame_number = 0;
		keys[static_cast<int>(Key::ESCAPE)].is_down = is_down;
		break;
	case VK_RETURN:
		if (keys[static_cast<int>(Key::ENTER)].is_down != is_down)
			keys[static_cast<int>(Key::ENTER)].frame_number = 0;
		keys[static_cast<int>(Key::ENTER)].is_down = is_down;
		break;
	case 'W':
		if (keys[static_cast<int>(Key::W)].is_down != is_down)
			keys[static_cast<int>(Key::W)].frame_number = 0;
		keys[static_cast<int>(Key::W)].is_down = is_down;
		break;
	case 'A':
		if (keys[static_cast<int>(Key::A)].is_down != is_down)
			keys[static_cast<int>(Key::A)].frame_number = 0;
		keys[static_cast<int>(Key::A)].is_down = is_down;
		break;
	case 'S':
		if (keys[static_cast<int>(Key::S)].is_down != is_down)
			keys[static_cast<int>(Key::S)].frame_number = 0;
		keys[static_cast<int>(Key::S)].is_down = is_down;
		break;
	case 'D':
		if (keys[static_cast<int>(Key::D)].is_down != is_down)
			keys[static_cast<int>(Key::D)].frame_number = 0;
		keys[static_cast<int>(Key::D)].is_down = is_down;
		break;
	case VK_LEFT:
		if (keys[static_cast<int>(Key::LEFT)].is_down != is_down)
			keys[static_cast<int>(Key::LEFT)].frame_number = 0;
		keys[static_cast<int>(Key::LEFT)].is_down = is_down;
		break;
	case VK_RIGHT:
		if (keys[static_cast<int>(Key::RIGHT)].is_down != is_down)
			keys[static_cast<int>(Key::RIGHT)].frame_number = 0;
		keys[static_cast<int>(Key::RIGHT)].is_down = is_down;
		break;
	case VK_UP:
		if (keys[static_cast<int>(Key::UP)].is_down != is_down)
			keys[static_cast<int>(Key::UP)].frame_number = 0;
		keys[static_cast<int>(Key::UP)].is_down = is_down;
		break;
	case VK_DOWN:
		if (keys[static_cast<int>(Key::DOWN)].is_down != is_down)
			keys[static_cast<int>(Key::DOWN)].frame_number = 0;
		keys[static_cast<int>(Key::DOWN)].is_down = is_down;
		break;
	}
}

void Input::Reset() {
	for (int i = 0; i < static_cast<int>(Key::COUNT); i++) {
		keys[i].is_down = false;
	}
}

void Input::Update()
{
	for (int i = 0; i < static_cast<int>(Key::COUNT); ++i)
	{
		++keys[i].frame_number;
	}
	++left_mouse.frame_number;
}