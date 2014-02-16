#pragma once
#include "Globals.h"

class CNtFunctions
{
public:
	CNtFunctions(void);
	~CNtFunctions(void);

	/*
	NtReadVirtualMemory Funktionspointer und Prototyp
	*/
	typedef NTSTATUS (NTAPI *PNTREADVIRTUALMEMORY)(IN HANDLE ProcessHandle, IN PVOID BaseAddress, OUT PVOID Buffer, IN ULONG BufferLength, OUT PULONG ReturnLength OPTIONAL);
	PNTREADVIRTUALMEMORY m_pNtReadVirtualMemory;

	/*
	NtWriteVirtualMemory Funktionspointer und Prototyp
	*/
	typedef NTSTATUS (NTAPI *PNTWRITEVIRTUALMEMORY)(IN HANDLE ProcessHandle, IN PVOID BaseAddress, IN PVOID Buffer, IN ULONG NumberOfBytesToWrite, OUT PULONG NumberOfBytesWritten OPTIONAL); 
	PNTWRITEVIRTUALMEMORY m_pNtWriteVirtualMemory;

	// Initialisiert die Funktionen aus der ntdll.dll
	void loadNtFunctions(void);
};

extern CNtFunctions *g_pNtFunctions;
