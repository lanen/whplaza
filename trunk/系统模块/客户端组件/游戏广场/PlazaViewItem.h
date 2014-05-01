#ifndef PLAZA_VIEW_ITEM_HEAD_FILE
#define PLAZA_VIEW_ITEM_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "PlatformPublicize.h"
#include "ServerViewItem.h"
#include "WndGridCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//������Ŀ
#define MAX_SERVER_COUNT			5									//������Ŀ
#define MAX_CONTROL_COUNT				12								//��ҳ������Ŀ

//////////////////////////////////////////////////////////////////////////////////

// ��Ϸ����
typedef CList<CGameKindItem *>	 CGameKindList;

struct tagGameTypeListItem
{
	CGameTypeItem * pGameTypeItem;
	CGameKindList GameKindList;

	tagGameTypeListItem()
	{
		pGameTypeItem = NULL;
	}

	~tagGameTypeListItem()
	{
		GameKindList.RemoveAll();
	}
};

typedef CList<tagGameTypeListItem *>  CGameTypeListItem;

typedef CList<CGameServerItem *> CGameServerList;

struct tagGameKindListItem
{
	CGameKindItem *pGameKindItem;
	CGameServerList GameServerItem;

	tagGameKindListItem()
	{
		pGameKindItem = NULL;
	}

	~tagGameKindListItem()
	{
		GameServerItem.RemoveAll();
	}
};

typedef CList<tagGameKindListItem *> CGameKindListItem;

//�������
class CPlazaBrowser : public CHtmlView
{
	//��������
public:
	//���캯��
	CPlazaBrowser();
	//��������
	virtual ~CPlazaBrowser();

	//���غ���
protected:
	//�󶨺���
	virtual VOID PreSubclassWindow();
	//���Ĵ���
	virtual VOID PostNcDestroy();

	//���ܺ���
public:
	//��������
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID, CCreateContext * pContext=NULL);

	//��Ϣ����
public:
	//�ر��¼�
	VOID OnWindowClosing(BOOL IsChildWindow, BOOL * bCancel);

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ�㳡
class CPlazaViewItem : public CDialog, public IUnknownEx, public IServerListDataSink
{
	friend class CPlatformFrame;
	//״̬����
protected:
	bool							m_bCreateFlag;						//������־
	INT								m_CurrPage;							//��ǰҳ

	//��ť�ؼ�
protected:
	CSkinButton						m_btnGridTypeCtrl[MAX_CONTROL_COUNT];				//ʮ�����б��
	//CSkinButton						m_btnGridKindCtrl[MAX_CONTROL_COUNT];			//ʮ���������
	CWndGridCtrl					m_wndGridCtrl[MAX_CONTROL_COUNT];					//ʮ���������
	CSkinButton						m_btnPrevious;										//��һ��
	CSkinButton						m_btnNext;											//��һ��
	CSkinButton						m_btnGetBack;										//���ذ�ť
	tagGameTypeListItem *			m_pGameTypeListItem;								//�б�����
	tagGameKindListItem *			m_pGameKindListItem;								//��Ϸ����

	//��Դ����
protected:
	CEncircleBMP					m_BrowserEncircle;					//�����Դ

	//ƽ̨�ӿ�
public:
	IPlatformServerSink *           m_pPlatformServerSink;              //ƽ̨�ӿ�

	//����
public:
	CGameTypeListItem					m_GameTypeList;					//��Ϸ����
	CGameKindListItem					m_GameKindList;					//�б�����

	//�ؼ�ָ��
protected:
	static CPlazaViewItem *			m_pPlazaViewItem;					//�㳡ָ��

	//��������
public:
	//���캯��
	CPlazaViewItem();
	//��������
	virtual ~CPlazaViewItem();


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
	//���ҳ��
	bool WebBrowse(LPCTSTR pszURL);
	//��ȡʵ��
	static CPlazaViewItem * GetInstance() { return m_pPlazaViewItem; }
	//���·��䰴ť
	void UpdateServerItemButton(CServerViewItemArray & ServerViewItemArray);

	//���溯��
private:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	//��ԴĿ¼
	VOID GetGameResDirectory(tagGameKind &GameKind,TCHAR szResDirectory[], WORD wBufferCount);
	tagGameTypeListItem *GetSubGameTypeList(CGameTypeItem * pGameTypeItem);
	tagGameKindListItem *GetSubGameKindList(CGameKindItem * pGameKindItem);
	UINT GetGameKindResID(LPCTSTR KindName);

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