#pragma once
#include "Globals.h"

#define ERRPOS __FILE__,__LINE__

class CError
{
public:
	CError(char *pcErrorText, char *pcFileName, int iLineNum);
	~CError(void);

private:
	char *m_pcText;
	int m_iLineNum;
	char *m_pcFileName;
	char *m_pcMsgHeader;
public:
	void showError(char *pcLine, int iLineNum);
	void setMsgHeader(char * pcHeader);
};

