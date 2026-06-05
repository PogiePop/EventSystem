#pragma once
#include "../event.hpp"
#include <stdint.h>
namespace Wse {
    class Key : public Event
    {
    public:
        virtual ~Key() = default;
        Event_Category_Def(KeyEvent)
            virtual uint8_t GetKey() { return key; }
        Key(int key) :key(key) {}
    private:
        uint8_t key;
    };

    class KeyPress : public Key
    {
    public:
        ~KeyPress() = default;
        Event_Type_Def(KeyPress)
            explicit KeyPress(int key) :
            Key(key) {
        }
    };
}



