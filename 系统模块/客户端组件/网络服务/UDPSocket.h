#pragma once
#include "stdafx.h"
#include "winsock2.h"

class CUDPSocket
{
public:
	CUDPSocket(void);
	~CUDPSocket(void);

	bool CreateSocket();
	void Disconnect();
	void Shutdown();
	bool Bind(UINT nPort,const char* sServerIP);
	int SendTo(void* pBuffer,int iLen,UINT nHostPort, const char* lpszHostAddress);
	int RecvFrom(void* pBuffer,int iLen,CString& rSocketAddress,UINT& rSocketPort);
	int SetTimeOut(int iTimeOut);


	SOCKET m_Socket;
	CRITICAL_SECTION m_CriticalSec;
};
