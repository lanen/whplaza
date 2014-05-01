#ifndef KIND_VIEW_ITEM_HEAD_FILE
#define KIND_VIEW_ITEM_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "PlatformPublicize.h"
#include "ServerViewItem.h"


//////////////////////////////////////////////////////////////////////////////////

//������Ŀ
#define MAX_SERVER_COUNT			5								//������Ŀ
#define MAX_CONTROL_COUNT			5								//��ҳ������Ŀ

/////////////////////////////////////////////////////////////////////////

//��������
typedef CList<CGameServerItem *> CServerRoomList;

struct tagKindListItem
{
	CGameKindItem *pGameKindItem;
	CServerRoomList GameServerItem;

	tagKindListItem()
	{
		pGameKindItem = NULL;
	}

	~tagKindListItem()
	{
		GameServerItem.RemoveAll();
	}
};

typedef CList<tagKindListItem *> CKindListItem;

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ�㳡
class CKindViewItem : public CDialog, public IUnknownEx, public IServerListDataSink
{
	friend class CPlatformFrame;
	//״̬����
protected:
	bool							m_bCreateFlag;						//������־
	INT								m_CurrPage;							//��ǰҳ

	//��ť�ؼ�
protected:
	CSkinButton						m_btnPrevious;										//��һ��
	CSkinButton						m_btnNext;											//��һ��
	CSkinButton						m_btnGetBack;										//���ذ�ť
	tagKindListItem *				m_pGameKindListItem;								//��Ϸ����

	//��Դ����
protected:
	CEncircleBMP					m_BrowserEncircle;					//�����Դ

	//ƽ̨�ӿ�
public:
	IPlatformServerSink *           m_pPlatformServerSink;              //ƽ̨�ӿ�

	//����
public:
	CKindListItem					m_GameKindList;						//�б�����

	//�ؼ�ָ��
protected:
	static CKindViewItem *			m_pKindViewItem;					//�㳡ָ��

	//��������
public:
	//���캯��
	CKindViewItem();
	//��������
	virtual ~CKindViewItem();


	//״̬֪ͨ
public:
	//��ȡ֪ͨ
	virtual VOID OnGameItemFinish();
	//��ȡ֪ͨ
	virtual VOID OnGameKindFinish(WORD wKindID);
	//����֪ͨ
	virtual VOID OnGameItemUpdateFinish();

	//����֪ͨ
public:
	//����֪ͨ
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem);
	//����֪ͨ
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem);
	//ɾ��֪ͨ
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��������
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//ȷ������
	virtual VOID OnOK() { return; };
	//ȡ������
	virtual VOID OnCancel() { return; };

	//���ܺ���
public:
	//��ȡʵ��
	static CKindViewItem * GetInstance() { return m_pKindViewItem; }

	//���溯��
private:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	tagKindListItem *GetSubGameKindList(CGameKindItem * pGameKindItem);

	//��Ϣ����
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif