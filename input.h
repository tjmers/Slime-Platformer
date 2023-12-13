#pragma once

#include <Windows.h>

enum class Key {
	W, A, S, D, SPACE, ESCAPE, ENTER, COUNT
};


class Input {
	struct KeyboardInput {
		bool is_down = false;
		int frame_number = 0;
	};

	static KeyboardInput keys[];

public:
	static KeyboardInput GetKey(Key key);
	static bool GetKeyDown(Key key);
	static void HandleKeyboardInput(UINT_PTR keyCode, bool isDown);
	static void Reset();
	static void Update();
};