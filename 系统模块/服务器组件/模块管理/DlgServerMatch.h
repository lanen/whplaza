//#ifndef DLG_SERVER_MATCH_HEAD_FILE
//#define DLG_SERVER_MATCH_HEAD_FILE

#pragma once

#include "ModuleManagerHead.h"
#include "ServerInfoManager.h"
#include "ServerListControl.h"

//���Ի���
class MODULE_MANAGER_CLASS CDlgServerMatch : public CDialog
{
	//���ñ���
public:
	tagModuleInitParameter			m_ModuleInitParameter;				//���ò���

	//��������
public:
	//���캯��
	CDlgServerMatch();
	//��������
	virtual ~CDlgServerMatch();
	//���ú���
public:
	//�򿪱����Ի���
	bool OpenGameMatch();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
};