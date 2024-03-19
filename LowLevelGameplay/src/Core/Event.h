#pragma once
#include <Core/isFunctionPointer_concept.h>
#include <unordered_set>
#include <functional>
#include <iostream>
#include <memory>
#include <algorithm>

namespace LLGP
{

	template<typename... T>
	struct Event
	{
	public:
		Event() = default;
		Event(Event&& rhs) : boundEvents(std::move(rhs.boundEvents)) {}
		~Event() = default;

		std::unordered_set<std::shared_ptr<std::function<void(T...)>>> boundEvents;

		void PrintListenerCount() { std::cout << boundEvents.size() << std::endl; }

		void Invoke(T... args)
		{
			std::for_each(boundEvents.begin(),
				boundEvents.end(),
				[&args...](const std::shared_ptr<std::function<void(T...)>>& listener) { (*listener)(args...); });
		}

		void AddListener(std::function<void(T...)> listener)
		{
			boundEvents.insert(std::make_shared<std::function<void(T...)>>(listener));
		}

		void RemoveListener(const std::function<void(T...)> listener)
		{
			auto search = boundEvents.find(std::make_shared<std::function<void(T...)>>(listener));
			if (search != boundEvents.end())
				boundEvents.erase(search);
		}
	};
}
