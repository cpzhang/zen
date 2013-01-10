#pragma once
#include "misc/Singleton.h"
#include "misc/stdHead.h"
//events
enum eEvent
{
	eEvent_SelectModel,
};
// arguments
class EventArgs
{
public:
	EventArgs(void) : handled(0) {}
	virtual ~EventArgs(void) {}

public:
	int handled;
};
class SelectModelEventArgs : public EventArgs
{
public:
	tstring FilePath_;
};
//functor
class SlotFunctorBase
{
public:
	virtual ~SlotFunctorBase() {};
	virtual bool operator()(const EventArgs& args) = 0;
};

template<typename T>
class MemberFunctionSlot : public SlotFunctorBase
{
public:
	//! Member function slot type.
	typedef bool(T::*MemberFunctionType)(const EventArgs&);

	MemberFunctionSlot(MemberFunctionType func, T* obj) :
	d_function(func),
		d_object(obj)
	{}

	virtual bool operator()(const EventArgs& args)
	{
		return (d_object->*d_function)(args);
	}

private:
	MemberFunctionType d_function;
	T* d_object;
};
class SubscriberSlot
{
public:
    SubscriberSlot()
	{
		d_functor_impl = NULL;
	}
    ~SubscriberSlot()
	{

	}
    bool operator()(const EventArgs& args) const
    {
		if (d_functor_impl)
		{
			return (*d_functor_impl)(args);
		}
        return false;
    }


    template<typename T>
    SubscriberSlot(bool (T::*function)(const EventArgs&), T* obj) :
        d_functor_impl(new MemberFunctionSlot<T>(function, obj))
    {}

private:
    SlotFunctorBase* d_functor_impl;
};
class EventManager
{
public:
	static EventManager& GetInstance()
	{
		static EventManager se;
		return se;
	}
public:
	void subscribeEvent(eEvent e, SubscriberSlot slot)
	{
		mEventSubscriberSlots.insert(std::pair<eEvent, SubscriberSlot>(e, slot));
	}
	void fireEvent(eEvent e, const EventArgs& args)
	{
		EventSubScriberSlotMap::iterator it;
		std::pair<EventSubScriberSlotMap::iterator, EventSubScriberSlotMap::iterator> ret;
		ret = mEventSubscriberSlots.equal_range(e);
		for (it=ret.first; it!=ret.second; ++it)
		{
			(*it).second(args);
		}
	}
private:
	typedef std::multimap<eEvent, SubscriberSlot> EventSubScriberSlotMap;
	EventSubScriberSlotMap mEventSubscriberSlots;
};