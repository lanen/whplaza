#include "Stdafx.h"
#include "Resource.h"
#include "DlgServerMatch.h"
#include ".\dlgservermatch.h"

//���캯��
CDlgServerMatch::CDlgServerMatch() : CDialog(IDD_SERVER_MATCH)
{
	//ģ�����
	ZeroMemory(&m_ModuleInitParameter,sizeof(m_ModuleInitParameter));

	return;
}

//��������
CDlgServerMatch::~CDlgServerMatch()
{
}
void CDlgServerMatch::DoDataExchange(CDataExchange* pDX)
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::DoDataExchange(pDX);
}
//�򿪱����Ի���
bool CDlgServerMatch::OpenGameMatch()
{
	//������Դ
	AfxSetResourceHandle(GetModuleHandle(MODULE_MANAGER_DLL_NAME));

	//���÷���
	if (DoModal()==IDC_LOAD_SERVER)
	{
		return true;
	}

	return false;
}
