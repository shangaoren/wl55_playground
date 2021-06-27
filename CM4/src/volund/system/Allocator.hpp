#pragma once

#include <cstdint>
#include <array>
#include "volund/interfaces/system/IAllocator.hpp"


class AllocatorNode
{
	
	
	/*Allocator Schema				
								
		|----------------|						
		|  Node N next   |						
		|----------------|						
		|  Node N used ? |
		|----------------|
		| Node N previous|
		|----------------|
		|                |
		~~~~~~~~~~~~~~~~~~
		|				 |
		|    Data 1      |
		|				 |						
		|----------------|						
		|  Node 1 next   |						
		|----------------|						
		|  Node 1 used ? |
		|----------------|
		| Node 1 previous|
		|----------------|
		|				 |
		|	  Data 0     |
		|				 |
		|----------------|				
		|  Node 0 next   |						
		|----------------|						
		|  Node 0 used ? |
		|----------------|
		| Node 0 previous|
		|----------------|
								*/
	
public:
	AllocatorNode* volatile previousNode;
	volatile uint32_t isUsed;
	AllocatorNode* volatile nextNode;
	
	
	AllocatorNode()
	{
		
	}
	
	~AllocatorNode()
	{
		
	}

	/*return a bool to know if the space is used or not by giving the pointer of the beginning of space*/
	bool spaceIsUsed()
	{
		if (isUsed > 0)
			return true;
		else
			return false;
	}

	//Return true if there is a section after this one, false otherwise
	bool isThereASectionAfter()
	{
		return nextNode != nullptr ? true : false;
	}

	bool isThereASectionBefore()
	{
		return previousNode != nullptr ? true : false;
	}

	// return the size of the section in bytes (only data)
	std::size_t sectionSize()
	{
		if (nextNode != nullptr)
		{
			return reinterpret_cast<uint32_t>(nextNode) - reinterpret_cast<uint32_t>(this) - sizeof(AllocatorNode);
		}		
		else
			return 0;
	}

	void *data()
	{
		auto offset = (sizeof(AllocatorNode *) * 2 + sizeof(uint32_t))/ sizeof(void*);
		return reinterpret_cast<uint32_t *>(this) + offset;
	}

	static AllocatorNode *getNodeFromData(void *data)
	{
		auto offset = (sizeof(AllocatorNode *) * 2 + sizeof(uint32_t)) / sizeof(void *);
		return reinterpret_cast<AllocatorNode *>(reinterpret_cast<uint32_t *>(data) - offset);
	}

  private:
};

template<std::size_t containerSize>
	class ContainedAllocator : public interfaces::system::IAllocator
{
public:
	ContainedAllocator() : m_heapStartPointer(reinterpret_cast<uint32_t*>(&m_heap[0]))
	{	
		AllocatorNode* startNode = new(&m_heapStartPointer[0]) AllocatorNode();	//place a virtual node on start and end of heap
		AllocatorNode* endNode = new(&m_heapStartPointer[containerSize - allocatorNodeSize()] ) AllocatorNode();

		m_startNode = startNode;
		m_endNode = endNode;

		startNode->previousNode = nullptr;	//no container before
		startNode->nextNode = endNode;		// link to the next node
		startNode->isUsed = 0;				//not used for the moment
		
		endNode->nextNode = nullptr;		// no container after this limit
		endNode->previousNode = startNode;	//link to the beginning of heap
		endNode->isUsed = 1;				//tell used but don't care since there is no place after this

		m_sizeLeft = containerSize;
	}
	
	virtual void* allocate(std::size_t size) override
	{

#ifdef DEBUG
		if (isInUse == true)
			asm volatile("bkpt #0");
		if(sanityCheck() != true)
			asm volatile("bkpt #0");
#endif // DEBUG
		isInUse = true;
		if (size == 0)
		{
			isInUse = false;
			return nullptr;
		}

		AllocatorNode* volatile iterator = m_startNode;
		bool found = false;
		while (found == false)
		{
			if ((iterator->isUsed == true) | (iterator->sectionSize() < size))
				iterator = iterator->nextNode;
			else
				found = true;
			if (iterator == m_endNode) // end node reach but no space 
			{
				isInUse = false;
				return nullptr; 
			}

		}
		// size is superior than a node, needed size and at least a data with size of uint32_t
		auto sectionSize8 = iterator->sectionSize();
		auto neededSize8 = size + sizeof(AllocatorNode);
		
		auto sectionSize32 = sizeIn32Bits(sectionSize8);
		auto neededSize32 = sizeIn32Bits(neededSize8);
		if(sectionSize32 > neededSize32)
		{
			AllocatorNode * volatile newNode = new(reinterpret_cast<uint32_t *>(iterator->data()) + sizeIn32Bits(size)) AllocatorNode();
			AllocatorNode* volatile next = iterator->nextNode;
			newNode->isUsed = 0;
			newNode->nextNode = next;
			newNode->previousNode = iterator;
			
			iterator->nextNode = newNode;
			next->previousNode = newNode;

			//return the node
#ifdef DEBUG
			if (sanityCheck() != true)
				asm volatile("bkpt #0");
#endif // DEBUG
			iterator->isUsed = 1;
			isInUse = false;
			m_sizeLeft -= iterator->sectionSize() + sizeof(AllocatorNode);
			return iterator->data();
		}
		else
		{ // node is sufficient for needed size 
			iterator->isUsed = true;
#ifdef DEBUG
			if (sanityCheck() != true)
				asm volatile("bkpt #0");
#endif // DEBUG
			isInUse = false;
			m_sizeLeft -= iterator->sectionSize() + sizeof(AllocatorNode);
			return iterator->data();
		}
	}
	
	virtual bool deAllocate(void* ptr) override
	{
#ifdef DEBUG
		if (isInUse == true)
		{
			asm volatile("bkpt #0");
		}
		if (sanityCheck() != true)
			asm volatile("bkpt #0");
#endif // DEBUG
		isInUse = true;
		AllocatorNode *volatile iterator = m_startNode;
		AllocatorNode *volatile node = AllocatorNode::getNodeFromData(ptr);

		while (iterator != node)
		{
			iterator = iterator->nextNode;
			if ((iterator == m_endNode) | (iterator > node))
			{
				isInUse = false;
				return false;
			}
		}

		iterator->isUsed = 0;
		m_sizeLeft += iterator->sectionSize() + sizeof(AllocatorNode);
		
		// reassemble with node before
		if((iterator != m_startNode) && (iterator->previousNode->isUsed == false))
		{
			AllocatorNode* volatile toDel = iterator;
			AllocatorNode* volatile next = toDel->nextNode;
			iterator = toDel->previousNode;
			
			iterator->nextNode = next;
			next->previousNode = iterator;
			
			toDel->previousNode = nullptr;
			toDel->nextNode = nullptr;
			m_sizeLeft += sizeof(AllocatorNode);
			
		}
#ifdef DEBUG
		if (sanityCheck() != true)
			asm volatile("bkpt #0");
#endif // DEBUG
		
		// reassemble with node after
		if((iterator != m_endNode) && (iterator->nextNode !=m_endNode) && (iterator->nextNode->isUsed == false))
		{
			AllocatorNode* volatile next = iterator->nextNode;

			iterator->nextNode = next->nextNode;
			iterator->nextNode->previousNode = iterator;
			
			next->previousNode = nullptr;
			next->nextNode = nullptr;
			m_sizeLeft += sizeof(AllocatorNode);
		
		}

#ifdef DEBUG
		if (sanityCheck() != true)
			asm volatile("bkpt #0");
#endif // DEBUG
		isInUse = false;
		return true;
	}
private:
  AllocatorNode* m_startNode;
  AllocatorNode* m_endNode = new (&m_heapStartPointer[containerSize - allocatorNodeSize()]) AllocatorNode();
  uint32_t m_heap[containerSize] __attribute__((aligned(4)));
  uint32_t *m_heapStartPointer;
  uint32_t m_sizeLeft;
  volatile bool isInUse = false;
	

  /* Return the number of int32 used to contain a number of bytes*/
  static constexpr std::size_t sizeIn32Bits(std::size_t sizeIn8Bits)
  {
	  return sizeIn8Bits % sizeof(uint32_t) != 0 ? (sizeIn8Bits / sizeof(uint32_t) + 1) : (sizeIn8Bits / sizeof(uint32_t));
	}
	
	/*Return the number of int32 used to contain the size of a node*/
	static constexpr std::size_t allocatorNodeSize()
	{
		return sizeof(AllocatorNode) % sizeof(uint32_t) != 0 ? (sizeof(AllocatorNode) / sizeof(uint32_t)) + 1 : sizeof(AllocatorNode) / sizeof(uint32_t);
	}

	bool sanityCheck()
	{
		AllocatorNode* secondNode = new(&m_heapStartPointer[0]) AllocatorNode();
		AllocatorNode* firstNode = secondNode->nextNode;
		AllocatorNode *endNode = new (&m_heapStartPointer[containerSize - allocatorNodeSize()]) AllocatorNode();
		AllocatorNode *startNode = new (&m_heapStartPointer[0]) AllocatorNode(); //place a virtual node on start and end of heap
		while (firstNode < endNode)
		{
			if (firstNode != secondNode->nextNode)
				return false;
			if (firstNode->previousNode != secondNode)
				return false;
			if (firstNode == endNode)
				return true;
			secondNode = firstNode;
			firstNode = firstNode->nextNode;
			
		}
		return true;
	}
	
};