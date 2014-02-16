#include "Error.h"


CError::CError(char *pcErrorText, char *pcFileName, int iLineNum)
{
	this->m_iLineNum = iLineNum;
	this->m_pcFileName = pcFileName;
	this->m_pcText = pcErrorText;
	this->m_pcMsgHeader = NULL;
}


CError::~CError(void)
{
}


void CError::showError(char *pcFile, int iLineNum)
{
	DWORD dwErrorCode = GetLastError();

	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL );

	char buffer[256];
	sprintf_s(buffer, sizeof(buffer), "Error Position : File: %s | Line: %d\nCatch Position : File: %s | Line: %d\n\n%s\nErrorCode: %d :: %s", this->m_pcFileName, this->m_iLineNum, pcFile, iLineNum, this->m_pcText, dwErrorCode, lpMsgBuf);
	
	if (!this->m_pcMsgHeader)
		this->m_pcMsgHeader = "ERROR";

	MessageBox(0, buffer, this->m_pcMsgHeader, MB_ICONERROR | MB_TOPMOST | MB_OK);

	LocalFree(lpMsgBuf);
}


void CError::setMsgHeader(char *pcHeader)
{
	this->m_pcMsgHeader = pcHeader;
}
