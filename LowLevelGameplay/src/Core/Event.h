#pragma once
#include <vector>
#include <functional>
#include <iostream>
#include <utility>
#include <type_traits>

namespace LLGP
{
	template<typename... T>
	struct Binding
	{
	protected:
		void* contextObj;
		std::function<void(T...)> listener;

	public:
		Binding() = default;
		Binding(std::function<void(T...)> func, void* _contextObj)
		{
			contextObj = _contextObj;
			listener = std::move(func);
		}
		bool operator==(const Binding<T...>& rhs) { return contextObj == rhs.contextObj; }
		bool operator!=(const Binding<T...>& rhs) { return !(this == rhs); }
		constexpr size_t hash_code() const noexcept { return listener.target_type().hash_code(); }
		Binding<T...>& Invoke(T... args) { listener(static_cast<T&&>(args)...); return (*this); }
		void operator()(T... args) { listener(static_cast<T&&>(args)...); }
	};

	template<typename... T>
	struct Event
	{
	private:
		std::vector<Binding<T...>> listeners;

	public:

		Event<T...>& Invoke(T... args) { for (Binding<T...> l : listeners) l.Invoke(static_cast<T&&>(args)...); return (*this); }
		
		void AddListener(void* contextObj, const std::function<void(T...)> inFunc)
		{
			Binding<T...> listener = Binding<T...>(inFunc, contextObj);

			if (std::find_if(listeners.begin(), listeners.end(), [listener](Binding<T...> b) {
				return listener.hash_code() == b.hash_code() && listener == b; 
				}) == listeners.end())
			{
				listeners.push_back(listener);
			}
		}
		void RemoveListener(void* contextObj, const std::function<void(T...)> inFunc)
		{
			Binding<T...> listener = Binding<T...>(inFunc, contextObj);

			std::erase_if(listeners, [listener](Binding<T...> b) {return listener.hash_code() == b.hash_code() && listener == b; });
		}
		void Empty() { listeners.clear(); }

		Event<T...>& operator()(T... args) { Invoke(args...); return (*this); }
	};
}
