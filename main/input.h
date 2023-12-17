#pragma once

#include <Windows.h>

#include "vec2.h"

enum class Key {
	W, A, S, D, UP, DOWN, LEFT, RIGHT, SPACE, ESCAPE, ENTER, COUNT
};



class Input {
	struct KeyboardInput {
		bool is_down = false;
		int frame_number = 0;
	};
	struct LeftButton {
		bool is_down = false;
		Vec2I position = {0, 0};
		int frame_number = 0;
	};

	static KeyboardInput keys[];
	static LeftButton left_mouse;
public:
	inline static int GetKeyFrame(Key key) { return keys[static_cast<int>(key)].frame_number; }
	inline static bool GetKeyDown(Key key) { return keys[static_cast<int>(key)].is_down; }
	inline static bool GetLeftMouseDown() { return left_mouse.is_down; }
	inline static int GetLeftMouseFrame() { return left_mouse.frame_number; }
	inline static Vec2I GetLeftMousePosition() { return left_mouse.position; }
	static void HandleKeyboardInput(UINT_PTR key_code, bool is_down);
	inline static void HandleMouseDown(Vec2I position, bool is_down)
	{
		left_mouse.position = position;
		left_mouse.frame_number = 0;
		left_mouse.is_down = is_down;
	}
	static void Update();
	static void Reset();
};