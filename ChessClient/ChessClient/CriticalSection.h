#include"stdafx.h"

#ifndef _CRITICALSECTION_H_
#define _CRITICALSECTION_H_

class CriticalSection
{
private:
	CRITICAL_SECTION criticalSection;

public:
	CriticalSection() { InitializeCriticalSection(&criticalSection); }
	~CriticalSection() { DeleteCriticalSection(&criticalSection); }
	inline void Enter() { EnterCriticalSection(&criticalSection); }
	inline void Leave() { LeaveCriticalSection(&criticalSection); }
};

class CriticalSectionSync
{
private:
	CriticalSection& lock;

public:
	CriticalSectionSync(CriticalSection& cs) :lock(cs)
	{
		lock.Enter();
	}
	~CriticalSectionSync()
	{
		lock.Leave();
	}
};


#endif // !_CRITICALSECTION_H_
