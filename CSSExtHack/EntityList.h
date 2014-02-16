#pragma once

#include "Globals.h"
#include "CSSHack.h"

#define NUM_ENT_ENTRY_BITS		(11/*MAX_EDICT_BITS*/ + 1)
#define NUM_ENT_ENTRIES			(1 << NUM_ENT_ENTRY_BITS)
#define ENT_ENTRY_MASK			(NUM_ENT_ENTRIES - 1)
#define INVALID_EHANDLE_INDEX	0xFFFFFFFF

class CBaseHandle
{
public:
	unsigned long m_Index;

	inline CBaseHandle()
	{
		m_Index = INVALID_EHANDLE_INDEX;
	}

	inline CBaseHandle(unsigned long value)
	{
		m_Index = value;
	}

	inline bool IsValid() const 
	{
		return m_Index != INVALID_EHANDLE_INDEX;
	}

	inline int GetEntryIndex() const
	{
		return m_Index & ENT_ENTRY_MASK;
	}

	inline int GetSerialNumber() const
	{
		return m_Index >> NUM_ENT_ENTRY_BITS;
	}
};

class CEntityList
{
public:
	CEntityList();
	~CEntityList();

	static DWORD LookupEntity(const CBaseHandle handle);
};

