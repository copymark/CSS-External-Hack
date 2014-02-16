#include "NtFunctions.h"

CNtFunctions *g_pNtFunctions = new CNtFunctions();

CNtFunctions::CNtFunctions(void)
{
	m_pNtReadVirtualMemory = NULL;
	m_pNtWriteVirtualMemory = NULL;
	
	try 
	{
		loadNtFunctions();
	}
	catch (CError &e)
	{
		e.setMsgHeader("ERR - CExternalProcess");
		e.showError(ERRPOS);
	}
}


CNtFunctions::~CNtFunctions(void)
{
}

void CNtFunctions::loadNtFunctions(void)
{
	HMODULE ntdllModule = GetModuleHandle("ntdll.dll");
	if (!ntdllModule)
		throw(CError("CExternalProcess::loadNtFunctions : GetModuleHandle failed at finding 'ntdll.dll'", ERRPOS));

	m_pNtReadVirtualMemory = (PNTREADVIRTUALMEMORY)GetProcAddress(ntdllModule, "NtReadVirtualMemory");
	m_pNtWriteVirtualMemory = (PNTWRITEVIRTUALMEMORY)GetProcAddress(ntdllModule, "NtWriteVirtualMemory");
}