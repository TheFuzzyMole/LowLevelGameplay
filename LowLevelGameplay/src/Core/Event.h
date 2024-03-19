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
		size_t hash;
		std::function<void(T...)> listener;
	public:
		Binding(std::function<void(T...)> func) : hash(func.target_type().hash_code()), listener(std::move(func)) {}
		bool operator==(const Binding<T...>& rhs) { return hash == rhs.hash; }
		bool operator!=(const Binding<T...>& rhs) { return hash != rhs.hash; }
		constexpr size_t hash_code() const throw() { return hash; }
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
		void AddListener(const Binding<T...> listener)
		{
			if (std::find_if(listeners.begin(), listeners.end(), [listener](Binding<T...> b) {return listener.hash_code() == b.hash_code(); }) == listeners.end())
			{
				listeners.push_back(listener);
			}
		}
		void RemoveListener(const Binding<T...> listener) { std::erase_if(listeners, [listener](Binding<T...> b) {return listener.hash_code() == b.hash_code(); }); }
		void Empty() { listeners.clear(); }

		Event<T...>& operator()(T... args) { Invoke(args...); return (*this); }
		Event<T...>& operator+=(const Binding<T...> listener) { AddListener(listener); return (*this); }
		Event<T...>& operator+=(const std::function<void(T...)> func) { Binding<T...> b(func); *this += b; return (*this); }
		Event<T...>& operator-=(const Binding<T...> listener) { RemoveListener(listener); return (*this); }
		Event<T...>& operator-=(const std::function<void(T...)> func) { Binding<T...> b(func); *this -= b; return (*this); }
	};
}
