#ifndef PLATFORM_FRAME_HEAD_FILE
#define PLATFORM_FRAME_HEAD_FILE

#pragma once

//ϵͳ�ļ�
#include "Stdafx.h"
#include "Resource.h"

//�ؼ��ļ�
#include "DlgWhisper.h"
#include "DlgCollocate.h"
#include "DlgSkinControl.h"
#include "ServerListView.h"

//���ڿؼ�
#include "WndViewItemCtrl.h"
#include "WndUserInfoCtrl.h"
#include "PlatformPublicize.h"

//�����ļ�
#include "MissionList.h"
#include "MissionLogon.h"

//���崰��
#include "PlazaViewItem.h"
#include "ServerViewItem.h"

//////////////////////////////////////////////////////////////////////////////////

//�ȼ�����
#define IDI_HOT_KEY_BOSS			0x0100								//�ϰ��ȼ�
#define IDI_HOT_KEY_WHISPER			0x0200								//˽���ȼ�

//��Ļ����
#define LESS_SCREEN_CY				738									//��С�߶�
#define LESS_SCREEN_CX				1024								//��С���

//////////////////////////////////////////////////////////////////////////////////

//�б�״̬
enum enServerListControl
{
	ServerListControl_Hide,			//�����б�
	ServerListControl_Show,			//��ʾ�б�
	ServerListControl_Turn,			//��ת�б�
};

//////////////////////////////////////////////////////////////////////////////////

//�б���Դ
class CListEncircle : public CEncircleBMP
{
	//��������
public:
	//���캯��
	CListEncircle();
	//��������
	virtual ~CListEncircle();

	//���غ���
protected:
	//�滭����
	virtual bool PreDrawEncircleImage(tagEncircleBMP & EncircleImage);
};

//////////////////////////////////////////////////////////////////////////////////

//�����Դ
class CFrameEncircle : public CEncircleBMP
{
	//��������
public:
	//���캯��
	CFrameEncircle();
	//��������
	virtual ~CFrameEncircle();

	//���غ���
protected:
	//�滭����
	virtual bool PreDrawEncircleImage(tagEncircleBMP & EncircleImage);
};

//////////////////////////////////////////////////////////////////////////////////

//ƽ̨���
class CPlatformFrame : public CFrameWnd, public ISplitterEvent, public IViewItemEvent, public ICustomFaceEvent, public IPlatformServerSink
{
	//�������
protected:
	bool							m_bMaxShow;							//����־
	bool							m_bRectify;							//������־
	CRect							m_rcNormalSize;						//����λ��
	DWORD                           m_dwAllOnLineCount;                 //����������

	//״̬����
protected:
	bool							m_bWhisperIcon;						//˽��ͼ��
	bool							m_bHideGameList;					//���ر�־
	bool							m_bLockGameList;					//������־

	//���ư�ť
public:
	CSkinButton						m_btMin;							//��С��ť
	CSkinButton						m_btSet;							//���ð�ť
	CSkinButton						m_btClose;							//�رհ�ť

	//���ư�ť
public:
	CSkinButton						m_btControl;						//���ư�ť
	//CSkinButton						m_btLockList;						//������ť

	//���ܰ�ť
public:
	CSkinButton						m_btPlatform[3];					//ƽ̨��ť
	CSkinButton						m_btNavigation[5];					//������ť
	CSkinButton						m_btGameList;					    //�б�ť

	//��ܿؼ�
protected:
	CPlazaViewItem					m_PlazaViewItem;					//��Ϸ�㳡
	CWndViewItemCtrl				m_WndViewItemCtrl;					//������
	CWndUserInfoCtrl				m_WndUserInfoCtrl;					//�û���Ϣ

	//��Ϸ�б�
protected:
	CServerListView					m_ServerListView;					//��Ϸ�б�
	CServerListData					m_ServerListData;					//��Ϸ����

	//�ؼ�����
protected:
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CSkinSplitter					m_SkinSplitter;						//��ֿؼ�
	CDlgSkinControl					m_DlgSkinControl;					//�������
	CSystemTrayIcon					m_SystemTrayIcon;					//����ͼ��
	CPlatformPublicize				m_PlatformPublicize;				//����ؼ�

	//�������
protected:
	CMissionList					m_MissionList;						//�б�����
	CMissionLogon					m_MissionLogon;						//��¼����
	CMissionManager					m_MissionManager;					//�������

	//�ؼ�����
protected:
	CServerViewItem *				m_pServerViewItem;					//��ǰ����
	CDlgWhisperItemArray			m_DlgWhisperItemArray;				//˽������
	CServerViewItemArray			m_ServerViewItemArray;				//��������

	//��Դ����
protected:
	CEncircleBMP					m_ItemEncircle;						//�����Դ
	CListEncircle					m_ListEncircle;						//�����Դ
	INT                             m_nServerListW;
	CFrameEncircle					m_FrameEncircle;					//�����Դ
	INT                             m_nTBorder;                         //�����߶�

	//��Ƶ����
protected:
	WORD							m_wAVServerPort;					//��Ƶ�˿�
	DWORD							m_dwAVServerAddr;					//��Ƶ��ַ

	//��̬����
protected:
	static CPlatformFrame *			m_pPlatformFrame;					//���ָ��

	//״̬����
protected:
	bool							m_bClickFace;						//�����־
	bool							m_bMouseEvent;						//ע���־

	//������־
protected:
	bool							m_bHoverFace;						//������־
	bool							m_bHoverUnderWrite;					//������־

	//����λ��
protected:
	CRect							m_rcFaceArea;						//ͷ������
	CRect							m_rcUnderWrite;						//ǩ������

	//�ؼ�����
public:
	CSkinButton						m_btExchange;						//�û���ť
	CSkinButton						m_btSafe;						    //�û���ť
	CEditUnderWrite					m_EditUnderWrite;					//�༭�ؼ�

	//��������
public:
	//���캯��
	CPlatformFrame();
	//��������
	virtual ~CPlatformFrame();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//�¼�����
protected:
	//�����¼�
	virtual VOID OnViewItemEvent(WORD wItemIndex, CWnd * pItemView);
	//����¼�
	virtual VOID OnSplitterEvent(UINT uControlID, INT nXPos, INT nYPos);

	//ͷ��ӿ�
protected:
	//����ʧ��
	virtual VOID OnEventSystemFace(DWORD dwUserID, WORD wFaceID);
	//ͷ������
	virtual VOID OnEventCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo);

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//�ӿں���
public:
	//��ȡ����
	virtual CServerViewItemArray& GetServerViewArrary(){return m_ServerViewItemArray;}
	//���뷿��
	virtual void EnterServerViewItem(CServerViewItem *pServerViewItem);

	//���溯��
private:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);

	//���ܺ���
public:
	//���ҳ��
	bool WebBrowse(LPCTSTR pszURL, bool bAutoFullView);
	//�б����
	bool ControlServerList(enServerListControl ServerListControl, bool bFocusControl);

	//˽�ĺ���
public:
	//��ȡ˽��
	bool ShowWhisperItem();
	//����˽��
	bool InsertWhisperItem(CDlgWhisper * pDlgWhisper);
	//ɾ��˽��
	bool RemoveWhisperItem(CDlgWhisper * pDlgWhisper);

	//�������
public:
	//�����
	bool ActiveServerViewItem(CServerViewItem * pServerViewItem);
	//ɾ������
	bool DeleteServerViewItem(CServerViewItem * pServerViewItem);

	//���亯��
public:
	//ö�ٷ���
	CServerViewItem * EmunServerViewItem(WORD wIndex);
	//���ҷ���
	CServerViewItem * SearchServerViewItem(WORD wServerID);
	//���뷿��
	CServerViewItem * EntranceServerItem(CGameKindItem * pGameKindItem);
	//���뷿��
	CServerViewItem * EntranceServerItem(CGameServerItem * pGameServerItem);

	//��Ƶ����
public:
	void SetVideoOption(WORD wAVServerPort, DWORD dwAVServerAddr);

	//���溯��
protected:
	//��ԭ����
	bool RestoreWindow();
	//��󴰿�
	bool MaxSizeWindow();

	//��ť��Ϣ
protected:
	//ƽ̨��ť
	VOID OnBnClickedPlatform1();
	//ƽ̨��ť
	VOID OnBnClickedPlatform2();
	//ƽ̨��ť
	VOID OnBnClickedPlatform3();
	//ƽ̨��ť
	VOID OnBnClickedExchange();
	//ƽ̨��ť
	VOID OnBnClickedSafe();
	//ƽ̨��ť
	VOID OnBnClickedSysSet();
	//ƽ̨��ť
	VOID OnBnClickedGameList();

	//��Ϣ����
protected:
	//�ر���Ϣ
	VOID OnClose();
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//��������
	LRESULT OnNcHitTest(CPoint Point);
	//ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//λ����Ϣ
	VOID OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);
	//�ı���Ϣ
	VOID OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	//�Զ���Ϣ
protected:
	//�ȼ���Ϣ
	LRESULT OnMessageHotKey(WPARAM wParam, LPARAM lParam);
	//ͼ����Ϣ
	LRESULT OnMessageTrayIcon(WPARAM wParam, LPARAM lParam);
	//�¼���Ϣ
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);
	//�¼���Ϣ
	LRESULT OnMessageInstallCancelEvent(WPARAM wParam, LPARAM lParam);
	//�¼���Ϣ
	LRESULT OnMessageInsurePlazaEvent(WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//��ȡʵ��
	static CPlatformFrame * GetInstance() { return m_pPlatformFrame; }

	DECLARE_MESSAGE_MAP()

	//���ܺ���
public:
	//��������
	void UpDataAllOnLineCount();
	//��������
	DWORD GetAllOnLineCount();
	//�����Ϸ
	void AddLastPlayGame(WORD wServerID);
	//����ʧЧ
	bool ListGameServerInvalidation(WORD wKindID, WORD wServerID);
	//�û���Ϣ
	void DrawUserInfo(CDC * pDC);

	//��Ϣ����
protected:
	//��Ϣ����
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

//////////////////////////////////////////////////////////////////////////////////

#endif