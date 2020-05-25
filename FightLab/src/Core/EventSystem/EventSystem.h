#pragma once
#include "Event.h"
#include <typeindex>
#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H
class HandlerFunctionBase
{
public:
	void exec(Event* event)
	{
		call(event);
	}
private:
	// Implemented by MemberFunctionHandler
	virtual void call(Event* event) = 0;
};

template<class T, class EventType>
class MemberFunctionHandler : public HandlerFunctionBase
{
public:
	typedef void(T::* MemberFunction)(EventType*);

	MemberFunctionHandler(T* instance, MemberFunction memberFunction) : instance{ instance }, memberFunction{ memberFunction } {};

	void call(Event* evnt) {
		// Cast event to the correct type and call member function
		(instance->*memberFunction)(static_cast<EventType*>(evnt));
	}
private:
	// Pointer to class instance
	T* instance;

	// Pointer to member function
	MemberFunction memberFunction;
};

#endif

#ifndef EVENT_BUS_H
#define EVENT_BUS_H

typedef std::list<HandlerFunctionBase*> HandlerList;
class EventBus {
public:
    EventBus() {}
    ~EventBus() {}

    template<typename EventType>
    void publish(EventType* evnt) {
        HandlerList* handlers = subscribers[typeid(EventType)];

        if (handlers == nullptr) {
            return;
        }

        for (auto& handler : *handlers) {
            if (handler != nullptr) {
                handler->exec(evnt);
            }
        }
    }

    template<class T, class EventType>
    void subscribe(T* instance, void (T::* memberFunction)(EventType*)) {
        HandlerList* handlers = subscribers[typeid(EventType)];

        //First time initialization
        if (handlers == nullptr) {
            handlers = new HandlerList();
            subscribers[typeid(EventType)] = handlers;
        }

        handlers->push_back(new MemberFunctionHandler<T, EventType>(instance, memberFunction));
    }
private:
    std::map<std::type_index, HandlerList*> subscribers;
};


#endif