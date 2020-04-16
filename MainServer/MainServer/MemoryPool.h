#ifndef _MEMORYPOOL_H_
#define _MEMORYPOOL_H_
#include"stdafx.h"

template<class T, std::size_t ALLOC_BLOCK_SIZE = 1024>
class MemoryPool
{
public:
	static void* operator new(std::size_t allocSize)
	{
		assert(sizeof(T) == allocSize);
		assert(sizeof(T) >= sizeof(unsigned char*));

		if (!freePointer)
			allocBlock();

		unsigned char* returnPointer = freePointer;
		freePointer = *reinterpret_cast<unsigned char**>(returnPointer);

		return freePointer;
	}

	static void operator delete(void* deletePointer)
	{
		*reinterpret_cast<unsigned char**>(deletePointer) = freePointer;
		freePointer = static_cast<unsigned char*>(deletePointer);
	}

private:
	static void allocBlock()
	{
		freePointer						= new unsigned char[sizeof(T) * ALLOC_BLOCK_SIZE];

		unsigned char**		current		= reinterpret_cast<unsigned char**>(freePointer);
		unsigned char*		next		= freePointer;

		for (std::size_t i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i)
		{
			next += sizeof(T);
			*current = next;
			current = reinterpret_cast<unsigned char**>(next);
		}

		*current = 0;
	}

private:
	static unsigned char*	freePointer;

	//protected:
public:
	~MemoryPool() {}

};

template<class T, std::size_t ALLOC_BLOCK_SIZE>
unsigned char* MemoryPool<T, ALLOC_BLOCK_SIZE>::freePointer;

#endif // !_MEMORYPOOL_H_