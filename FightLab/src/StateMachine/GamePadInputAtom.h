#pragma once
#include "State.h"

#define BUTTON_COUNT 2
enum class DPadDirections: uint16_t
{
	Null                    = 1,
	DownBack                = 2,
	Down                    = 4,
	DownForward				= 8,
	Back                    = 16,
	Neutral                 = 32,
	Forward                 = 64,
	UpBack                  = 128,
	Up                      = 256,
	UpForward               = 512 
};

inline DPadDirections operator|(DPadDirections a, DPadDirections b)
{
	return static_cast<DPadDirections>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

enum class GamePadButtons : uint8_t
{
	Button0            = 0,
	Button1            = 1,
	/*,
	Down,
	Left,
	BumperLeft,
	TriggerLeft,
	BumperRight,
	TriggerRight,*/
	All
};
inline GamePadButtons operator|(GamePadButtons a, GamePadButtons b)
{
	return static_cast<GamePadButtons>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}
enum class ButtonState :uint8_t
{
	PRESSED,
	REPEAT,
	RELEASE,
	CHARGED,
	UP
};

struct GamePadDirInputAtom : InputAtom
{
	DPadDirections Direction;
	ButtonState State;
	GamePadDirInputAtom(std::string name = "", DPadDirections dir = DPadDirections::Neutral, ButtonState state = ButtonState::RELEASE)
	{
		Name = name;
		Direction = dir;
		State = state;
	}
};

struct ButtonGamePadInputAtom : InputAtom
{
	GamePadButtons Button;
	ButtonState State;
	ButtonGamePadInputAtom(std::string name = "",GamePadButtons button = GamePadButtons::Button0, ButtonState state = ButtonState::RELEASE)
	{
		Name = name;
		Button = button;
		State = state;
	}
};