#pragma once
#include <conio.h>

namespace enti
{
	enum class InputKey // Represents each key that the player can use
	{
		NONE = -1, w = 87, W = 119, a = 65, A = 97, s = 83, S = 115, d = 68, D = 100, z = 122, Z = 90, 
		NUM0 = 48, NUM1, NUM2, NUM3, NUM4, ENTER = 13, SPACEBAR = 32, ESC = 27
	};

	inline InputKey getInputKey() // Returns the key that has been pressed
	{
		return _kbhit() ? static_cast<InputKey>(_getch()) : InputKey::NONE;
	}

	inline void systemPause() // Pauses the execution until a key is pressed
	{
		while (getInputKey() == InputKey::NONE);
	}
};
