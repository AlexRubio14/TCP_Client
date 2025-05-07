#include "EventQueue.h"

void EventQueue::Push(const Event& event)
{
	std::lock_guard<std::mutex> lock(mutex);
	queue.push(event);
}

std::optional<EventQueue::Event> EventQueue::Pop()
{
	mutex.lock();
	if (!queue.empty())
	{
		Event event = queue.front();
		queue.pop();
		mutex.unlock();
		return event;
	}
	mutex.unlock();
	return std::nullopt;
}




bool EventQueue::GetEmpty() const
{
	std::lock_guard<std::mutex> lock(mutex);
	return queue.empty();
}
