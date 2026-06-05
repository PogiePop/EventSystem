#pragma once
#include <unordered_map>
#include <unordered_set>
#include <print>
#include <functional>
#include "event.hpp"
#include <queue>


namespace Wse {
    using object = void*;
    using func_ptr = void*;
    //using UserEventFunc = std::function<bool(object, Event&)>;
    using InnerEventFunc = std::function<bool(Event&)>;
    using TypeMap = std::unordered_map<EventType, std::unordered_set<InnerEventFunc*>>;
    using PostObj = std::pair<object, Event*>;
    using RawInnerFunc = bool(*)(Event&);
    using RawUserFunc = bool(*)(object, Event&);
    class EventBus
    {
        std::unordered_map<object, TypeMap> _bus_map;
        std::queue<PostObj> _bus_queue;
    public:
        static EventBus& instance()
        {
            static EventBus _bus = EventBus();
            return _bus;
        }

        //订阅事件
        template<typename T>
        void Subscribe(object obj, InnerEventFunc* func)
        {
            if (!obj || !func)return;
            auto _type = T::GetStaticType();
            _bus_map[obj][_type].insert(func);
        }

        //投递事件, 如果是system的广播则obj=nullptr,否则填指定投递人
        template<typename T, typename Sender, typename...Args>
        void Post(Sender* obj, Args&&...args)
        {
            _bus_queue.push({ (object)obj, new T(std::forward<Args>(args)...) });
        }


        //分派处理事件
        void DispatchTo()
        {
            while (!_bus_queue.empty())
            {
                auto post = _bus_queue.front();
                auto _type = post.second->GetType();
                _bus_queue.pop();
                //单播
                if (post.first)
                {
                    auto iter = _bus_map.find(post.first);
                    if (iter == _bus_map.end())continue;
                    auto iter_type = iter->second.find(_type);
                    if (iter_type == iter->second.end())continue;
                    auto funcs = iter_type->second;
                    for (auto& func : funcs)
                        if (func)
                            (*func)(*post.second);
                }
                //广播
                else
                {
                    for (auto&& [obj, typemap] : _bus_map)
                    {
                        auto iter = typemap.find(_type);
                        if (iter == typemap.end())continue;
                        auto funcs = iter->second;
                        for (auto& func : funcs)
                            if (func)
                                (*func)(*post.second);
                    }
                }
                delete post.second;
                post.second = nullptr;
            }
        }


        //订阅移除
        void UnSubscribe(object obj, InnerEventFunc* func)
        {
            if (!obj || !func)return;
            auto iter = _bus_map.find(obj);
            if (iter == _bus_map.end())return;
            auto& typemap = iter->second;
            for (auto&& [_type, sets] : typemap)
            {
                sets.erase(func);
            }
        }

    };
}