#pragma once
#include <queue>
#include <mutex>
#include <optional>
#include <functional>
#include <condition_variable>

class EventQueue
{
public:

	using Event = std::function<void()>;

private:

	mutable std::mutex mutex;
	std::queue<Event> queue;
	std::condition_variable condition;
public:


	void Push(const Event& event);
	std::optional<Event> Pop();
	bool GetEmpty() const;
};

