#include "event_bus.hpp"
#include <xutility>



namespace Wse {

	//成员键值
	struct MemKey
	{
		void* inst;
		void* memptr;
		bool operator==(const MemKey& o)const
		{
			return inst == o.inst && memptr == o.memptr;
		}
	};
}

//特化哈希值
namespace std
{
	template<>
	struct hash<Wse::MemKey>
	{
		size_t operator()(const Wse::MemKey& k)const
		{
			size_t h1 = hash<void*>()(k.inst);
			size_t h2 = hash<void*>()(k.memptr);
			return h1 ^ (h2 << 1);
		}
	};
}


namespace Wse{
	template<typename T>
	class EventField
	{
	public:
		EventField(object owner) : owner(owner) {}

		void AddEvent(RawUserFunc user)
		{
			if (maps.count(user))return;
			auto inner = [this, user](Event& e)
				{
					EventDispatch dispatcher(e);
					return dispatcher.Dispatch<T>([this, user](Event& e) {
						return user(owner, e);
						});
				};
			maps[user] = std::make_unique<InnerEventFunc>(std::move(inner));
			auto& bus = EventBus::instance();
			bus.Subscribe<T>(this, maps[user].get());
		}

		void RemoveEvent(RawUserFunc user)
		{
			if (!maps.count(user))return;
			auto &&inner = maps[user];
			auto& bus = EventBus::instance();
			bus.UnSubscribe(this, inner.get());
			maps.erase(user);
		}


		EventField& operator+=(RawUserFunc user)
		{
			AddEvent(user);
			return *this;
		}

		EventField& operator-=(RawUserFunc user)
		{
			RemoveEvent(user);
			return *this;
		}
		//成员函数
		template<typename E>
		void AddEvent(E* pObj, bool(E::* pFn)(Wse::object, Wse::Event&))
		{
			MemKey key{ pObj, *(void**)&pFn };
			if (memMaps.count(key))return;
			auto inner = [this, pObj, pFn](Event& e)
				{
					EventDispatch dispatcher(e);
					return dispatcher.Dispatch<T>([=](Event& e)
						{
							return (pObj->*pFn)(owner, e);
						});
				};

			memMaps[key] = std::make_unique<InnerEventFunc>(std::move(inner));
			auto& bus = EventBus::instance();
			bus.Subscribe<T>(this, memMaps[key].get());
		}

		template<typename E>
		void RemoveEvent(E* pObj, bool(E::* pFn)(object, Event&))
		{
			MemKey key{ pObj, *(void**)&pFn };
			if (!memMaps.count(key)) return;

			auto&& inner = memMaps[key];
			auto& bus = EventBus::instance();
			bus.UnSubscribe(this, inner.get());
			memMaps.erase(key);
		}

		template<typename E>
		EventField& operator+=(std::pair < E*, bool(E::*)(object, Event&)>bind)
		{
			AddEvent(bind.first, bind.second);
			return *this;
		}

		template<typename E>
		EventField& operator-=(std::pair<T*, bool(T::*)(object, Event&)> bind)
		{
			RemoveEvent(bind.first, bind.second);
			return *this;
		}

		
	private:
		//持有者
		object owner;
		//已订阅事件
		//外部事件-内部事件映射
		std::unordered_map<RawUserFunc, std::unique_ptr<InnerEventFunc>>maps;

		//成员函数版本
		std::unordered_map<MemKey, std::unique_ptr<InnerEventFunc>>memMaps;
	};
}

