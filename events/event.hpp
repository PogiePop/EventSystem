#pragma once
#include <iostream>

namespace Wse {
    enum class EventCategory : int
    {
        KeyEvent = 1,
        MouseEvent = 2
    };

    inline int operator&(const EventCategory& e1, const EventCategory& e2)
    {
        return static_cast<int>(e1) & static_cast<int>(e2);
    }

    enum class EventType
    {
        KeyPress,
        KeyRepeat,
        KeyRelease,
        KeyInput,
        MouseMove,
        MouseScroll,
        MouseButtonPress,
        MouseButtonRelease
    };

#define Event_Category_Def(_type)                                            \
    static EventCategory GetStaticCategory() { return EventCategory::_type; } \
    virtual EventCategory GetCategory()const { return GetStaticCategory(); }      \
    virtual const char *GetCategoryName()const { return #_type; }

#define Event_Type_Def(_type)                                    \
    static EventType GetStaticType() { return EventType::_type; } \
    virtual EventType GetType()const { return GetStaticType(); }      \
    virtual const char *GetTypeName()const { return #_type; }

    class Event
    {
    public:
        virtual ~Event() = default;
        virtual EventCategory GetCategory()const = 0;
        virtual const char* GetCategoryName()const = 0;
        virtual EventType GetType()const = 0;
        virtual const char* GetTypeName()const = 0;
        bool handle = false;
        bool IsInCategory(EventCategory cat)const
        {
            return GetCategory() & cat;
        }
        friend std::ostream& operator<<(std::ostream& os, const Event& e);
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        os << "typename: " << e.GetTypeName();
        return os;
    }


    class EventDispatch
    {
    public:
        EventDispatch(Event& e) :e(e) {}
        template<typename E, typename Fn>
        bool Dispatch(const Fn& func)
        {
            bool res = false;
            if (E::GetStaticType() == e.GetType())
            {
                res = func(static_cast<E&>(e));
                e.handle |= res;
            }
            return res;
        }
    private:
        Event& e;
    };
}