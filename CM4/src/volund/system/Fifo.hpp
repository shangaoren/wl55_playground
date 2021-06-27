#pragma once
#include <cstdint>
#include "yggdrasil/framework/DualLinkedList.hpp"
#include <functional>

namespace volund
{
	namespace system
	{
		
		
		template <typename TFifoNodeType, typename TFifoNodeList>
			class FifoNode : public framework::DualLinkNode<TFifoNodeType, TFifoNodeList>
			{
			
			};
		template <typename TFifoType, typename TFifoList = TFifoType>
			class Fifo : public framework::DualLinkedList<TFifoType, TFifoList>
			{
	
			public:
	
	
				constexpr Fifo()
				{
		
				}
	
				// add a node to the fifo, will be at the end of the fifo
				void pushBack(TFifoType *node)
				{
					this->insertEnd((node));
					if (m_newDataInserted != nullptr)
						m_newDataInserted();
				}

				// get the first element of the fifo
				TFifoType *popFront()
				{
					auto result = this->getFirst();
					if (result == nullptr && m_bottomHit != nullptr)
						m_bottomHit();
					return result;
				}

				// add a hook on a new data is inserted in the fifo
				void onNewDataInserted(std::function<void()> callback)
				{
					m_newDataInserted = callback;
				}

				// add a hook on bottom of fifo hit
				void onBottomHit(std::function<void()> callback)
				{
					m_bottomHit = callback;
				}

			private:
				std::function<void()> m_newDataInserted;
				std::function<void()> m_bottomHit;
			}; // class Fifo
	} // namespace system
} // namespace volund