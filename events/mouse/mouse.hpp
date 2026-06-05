#pragma once
#include "../event.hpp"

struct MouseClick:Event
{
    MouseClick()=default;
    ~MouseClick()override=default;
    Event_Category_Def(MouseEvent);
    Event_Type_Def(MouseButtonPress);
};