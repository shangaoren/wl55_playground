#pragma once
#include <cstdint>
#include <functional>
#include "yggdrasil/framework/DualLinkedList.hpp"

namespace volund
{
	namespace system
	{
		namespace listenable
		{
			class Listener : public framework::DualLinkNode<Listener, Listener>
			{
			public:
				Listener(std::function<void(void)> t_function)
					: m_function(t_function)
				{
				}

				void operator()()
				{
					if (m_function != nullptr)
						m_function();
				}

			private:
				const std::function<void(void)> m_function;

			}; // class Listener

			class ListenableList : public framework::DualLinkedList<Listener, Listener>
			{
			public:
				void notify()
				{
					foreach([](Listener *t_listener) { (*t_listener)(); });
				}

				void subscribe(Listener &t_listener)
				{
					insertEnd(&t_listener);
				}

				void unsubscribe(Listener &t_listener)
				{
					remove(&t_listener);
				}

			}
			; // class ListenableList
		} // namespace listenable
	}// namespace system
} // namespace volund
