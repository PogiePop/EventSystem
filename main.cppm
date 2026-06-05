
#include "events/event_bus.hpp"
#include "events/key/key.hpp"
#include <print>
#include "events/event_field.hpp"
#include <string>
using namespace Wse;
class A
{
public:
    A():id(1),OnPress(this){}
   int id;
   
   EventField<KeyPress> OnPress;

  
};

class B
{
public:
    std::string name = "JackB";
    A a;
    bool press_inner(object sender, Event& e)
    {
        std::println("a.id = {}, this={}", ((A*)sender)->id, this->name);
        return false;
    }
};

bool press_A(object sender, Event& e)
{
    std::println("id={}, class={}", ((A*)sender)->id, typeid(A).name());
    return true;
}

bool press_A2(object sender, Event& e)
{
    std::println("我是A2");
    return true;
}

int main()
{
    A a;
    a.OnPress += press_A;
    a.OnPress += press_A2;
    a.OnPress -= press_A;
    B b;
    b.a.OnPress += std::make_pair(&b, &B::press_inner);
    b.a.OnPress += press_A;
    auto bus = EventBus::instance();
    bus.Post<KeyPress>((object)nullptr, 1);
    bus.DispatchTo();
    return 0;
}