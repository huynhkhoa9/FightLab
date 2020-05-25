#pragma once
#include "pch.h"
enum class EventType
{
	ENUM_TYPE_MESSAGE,
	ENUM_TYPE_MOUSE,
	ENUM_TYPE_BUTTON_DOWN,
	ENUM_TYPE_BUTTON_UP,
	ENUM_TYPE_COLLISION
};

enum class ApplicationState
{
	RUNNING, PAUSED, QUIT, MINIZED
};

enum class InputState
{
	Up, Down, Held
};

struct EventData
{

};

struct Event
{
	EventType m_type;
};

struct MouseMoveEvent : Event
{
	MouseMoveEvent(double _x, double _y, double _lastX, double _lastY)
	{ 
		x = _x; y = _y;
		lastX = _lastX;
		lastY = _lastY;
	}
	double x;
	double y;
	double lastX;
	double lastY;
};

struct ButtonEvent : Event
{
	ButtonEvent(int _code, InputState _state)
	{
		keyCode = _code;
		state = _state;
	}

	int keyCode;
	InputState state;
};

struct SpawnEntityEvent : Event
{
	SpawnEntityEvent()
	{

	}
};

struct ChangeAppStateEvent : Event
{
	ChangeAppStateEvent(ApplicationState state)
	{
		m_state = state;
	}
	ApplicationState m_state;
};