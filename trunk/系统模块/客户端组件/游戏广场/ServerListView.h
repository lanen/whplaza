#ifndef SERVER_LIST_VIEW_HEAD_FILE
#define SERVER_LIST_VIEW_HEAD_FILE

#include "Stdafx.h"
#include "ServerListData.h"
#include "WndUserInfoCtrl.h"
//////////////////////////////////////////////////////////////////////////////////
//�����б�
typedef CList<CGameServerItem *>	 CGameServerList;				//��Ϸ����

//�б�ڵ�
struct tagServerListItem
{
	INT             nPosX;
	INT             nPosY;
	WORD            wKindHeight;
	bool            bShowServer;
	CGameKindItem * pGameKindItem;
	CGameServerList ServerList;

	tagServerListItem()
	{
		nPosX=0;
		nPosY=0;
		wKindHeight=52;
		bShowServer=false;
		pGameKindItem=NULL;
	}

	~tagServerListItem()
	{
		ServerList.RemoveAll();
	}
};

//��Ϸ�б�
typedef CList<tagServerListItem *>	 CServerList;				//��Ϸ����
typedef CList<CGameTypeItem *>		CGameTypeItemList;			//��Ϸ����

//////////////////////////////////////////////////////////////////////////////////

//����˵��
typedef CMap<WORD,WORD,UINT,UINT>	CGameImageMap;						//��Ϸͼ��
typedef CList<WORD>	                CLastGameServerList;				//��Ϸ����

//////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// ���������Ϸ�б�ѡ����
//////////////////////////////////////////////////////////////////////////
class CPlazaGameListBar 
	: public CWnd
{
	typedef CWHArray< CWnd * > CPlazaGameListItemArray;

	//��������
public:
	//���캯��
	CPlazaGameListBar();
	//��������
	virtual ~CPlazaGameListBar();

	//���غ���
public:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//���ܺ���
public:
	//��������
	//bool SetItemCreateRect(CRect rcItemCreate);
	//��������
	bool InsertServiceItem( CWnd * pServiceItem );
	//��������
	//bool SetActiveServiceItem(CDlgServiceItem * pDlgServiceItem);
	//
	void SetParentWnd( CWnd * pParentWnd ){ m_pParentWnd = pParentWnd; }

	//�ڲ�����
private:
	//�����л�
	WORD SwitchToButtonIndex(CPoint MousePoint);

	//��Ϣ����
protected:
	//�ػ���Ϣ
	VOID OnPaint();
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()

	//���Ϣ
protected:
	bool										m_bHovering;						//������־
	WORD									m_wItemDown;						//��������
	WORD									m_wItemHover;						//��������
	WORD									m_wItemActive;					//��������
	WORD									m_wCurSelect;						//��ǰѡ��

	//������Ϣ
protected:
	CSize									m_SizeButton;						//��ť��С
	CRect									m_rcItemCreate;					//����λ��
	CPlazaGameListItemArray		m_arrGameListItem;				//��������

/*������*/
private:
	CWnd *									m_pParentWnd;

	//��Դ����
protected:
	CPngImage							m_imgBk;				//����ͼ

};



//////////////////////////////////////////////////////////////////////////
// �����������ھ���
//////////////////////////////////////////////////////////////////////////
class CPlazaMatchChampionBar 
	: public CWnd
{
	//��������
public:
	//���캯��
	CPlazaMatchChampionBar();
	//��������
	virtual ~CPlazaMatchChampionBar();

	//���غ���
public:
	//��Ϣ����
	//virtual BOOL PreTranslateMessage(MSG * pMsg);

	//���ܺ���
public:
	//��������
	//bool SetItemCreateRect(CRect rcItemCreate);
	//��������
	//bool InsertServiceItem( CWnd * pServiceItem );
	//��������
	//bool SetActiveServiceItem(CDlgServiceItem * pDlgServiceItem);

	//�ڲ�����
private:
	//�����л�
	WORD SwitchToButtonIndex(CPoint MousePoint);
	//�����ؼ�
	void RectifyControl(INT nWidth, INT nHeight);

	//��Ϣ����
protected:
	//�ػ���Ϣ
	VOID OnPaint();
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//�����С�ı�
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//������
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//������
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt); 

	DECLARE_MESSAGE_MAP()

/*�ؼ�����*/
protected:
	CSkinScrollBar						m_SkinScrollBar;					//��������
/*������Ϣ*/
protected:
	INT										m_nMaxPels;							//�������
	INT										m_nPagePels;						//ҳ������
	INT										m_nLineCount;						//������Ŀ
	INT										m_nWidthCount;					//������Ŀ
	INT										m_nCurrentPos;						//��ǰλ��
	INT										m_nXExcursion;						//����ƫ��
/*���Ϣ*/
protected:
	bool										m_bHovering;						//������־
	WORD									m_wItemDown;						//��������
	WORD									m_wItemHover;						//��������
	WORD									m_wItemActive;					//��������
/*������Ϣ*/
protected:
	CSize									m_SizeButton;						//��ť��С
	CRect									m_rcItemCreate;					//����λ��

};



//////////////////////////////////////////////////////////////////////////
// �����б�
//////////////////////////////////////////////////////////////////////////
class CServerListView : public CWnd, public IServerListDataSink
{
	//��������
public:
	//���캯��
	CServerListView();
	//��������
	virtual ~CServerListView();

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

	//���غ���
protected:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//���ں���
	virtual LRESULT DefWindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//����ͨ��
	VOID InitAssistantItem();
	//���ú���
	VOID InitServerTreeView();

	//�滭����
private:
	//�滭����
	VOID DrawTreeItem(CDC * pDC, CRect & rcClient, CRect & rcClipBox);
	//�滭����
	VOID DrawTreeBack(CDC * pDC, CRect & rcClient, CRect & rcClipBox);

	//�滭����
private:
	//�����ı�
	VOID DrawItemString(CDC * pDC, CRect rcRect, HTREEITEM hTreeItem, bool bSelected);

	//ͼ�꺯��
private:
	//��ȡͼ��
	UINT GetGameImageIndex(CGameKindItem * pGameKindItem);
	//��ȡͼ��
	UINT GetGameImageIndex(CGameServerItem * pGameServerItem);

	//���⺯��
private:
	//��ȡ����
	LPCTSTR GetGameItemTitle(CGameKindItem * pGameKindItem, LPTSTR pszTitle, UINT uMaxCount);
	//��ȡ����
	LPCTSTR GetGameItemTitle(CGameServerItem * pGameServerItem, LPTSTR pszTitle, UINT uMaxCount);

	//��������
private:
	//ɾ������
	VOID DeleteUpdateItem(CGameListItem * pGameListItem);
	//�޸�����
	VOID ModifyGameListItem(HTREEITEM hTreeItem, LPCTSTR pszTitle, UINT uImage);
	//��������
	HTREEITEM InsertInsideItem(LPCTSTR pszTitle, UINT uImage, DWORD dwInsideID, HTREEITEM hParentItem);
	//��������
	HTREEITEM InsertGameListItem(LPCTSTR pszTitle, UINT uImage, CGameListItem * pGameListItem, HTREEITEM hParentItem);
	//��ԴĿ¼
	VOID GetGameResDirectory(tagGameKind &GameKind,TCHAR szResDirectory[], WORD wBufferCount);
	//��������
	tagServerListItem *HitTest(CPoint &MousePoint);
	//��������
	CGameServerItem *HitSubTest(tagServerListItem *pServerListItem,CPoint &MousePoint);
	//����б�
	void ClearServerList();
	//����λ��
	void ModifyListPos();
	//����λ��
	void SetScrollMaxPos();

	//ϵͳ��Ϣ
protected:
	//�ػ���Ϣ
	VOID OnPaint();
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//������Ϣ
	VOID OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint MousePoint);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	//�б���Ϣ
protected:
	//�Ҽ��б�
	VOID OnNMRClick(NMHDR * pNMHDR, LRESULT * pResult);
	//����б�
	VOID OnNMLClick(NMHDR * pNMHDR, LRESULT * pResult);
	//�б�ı�
	VOID OnTvnSelchanged(NMHDR * pNMHDR, LRESULT * pResult);
	//�б�չ��
	VOID OnTvnItemexpanding(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()

protected:
	//��÷��为����Ϣ
	LPCTSTR GetLoadInfoOfServer(DWORD dwOnLineCount, DWORD dwMaxCount, LPTSTR pszBuffer, WORD wBufferSize);
	//��÷��为����Ϣ
	LPCTSTR GetLoadInfoOfServer(tagGameServer * pGameServer, LPTSTR pszBuffer, WORD wBufferSize);
	//��÷��为����Ϣ
	LPCTSTR GetLoadInfoOfServer(tagGameKind * pGameKind, LPTSTR pszBuffer, WORD wBufferSize);
	//���ؼ�¼
	void LoadLastPlayGame();
	//�����¼
	void SaveLastPlayGame();

public:
	//���ýӿ�
	VOID SetPlazaViewSink(IServerListDataSink * pIPlazaViewSink){m_pIPlazaViewSink = pIPlazaViewSink;}

	//
	const CGameTypeItemList& GetGameTypeItemList() const { return m_GameTypeItemList; }

	//λ�ñ���
protected:
	INT										m_nXScroll;							//����ƫ��
	INT										m_nYScroll;							//����ƫ��
	INT										m_nScrollPos;						//����λ��
	INT										m_nScrollPosMax;					//���λ��

	//״̬��Ϣ
protected:
	bool										m_bHovering;						//������־
	bool										m_bHoveKind;						//������־
	bool										m_bLMouseDown;					//�����־

	//�б���
protected:
	CServerList								m_ServerList;						//
	CGameTypeItemList					m_GameTypeItemList;					//

	//��Դ����
protected:
	CFont									m_FontBold;							//��������
	CSkinScrollBar						m_SkinScrollBar;					//��������

public:
	IServerListDataSink *			m_pIPlazaViewSink;				//�ص��ӿ�

	//��Դ����
protected:
	CImageList						m_ImageList;					//ͼƬ��Դ
	CGameImageMap					m_GameImageMap;					//ͼ������
	CLastGameServerList				m_LastPlayGameList;				//��������Ϸ

	//��������
protected:
	tagServerListItem *				m_pItemMouseHover;				//��������
	tagServerListItem *				m_pItemMouseDown;				//�������
	CGameServerItem *				m_pServerItemHover;				//��������
	CGameServerItem *				m_pServerItemDown;				//�������

	//
protected:
	CWndUserInfoCtrl					m_WndUserInfoCtrl;			//�û���Ϣ��
	CPlazaGameListBar					m_PlazaGameListBar;			//���������Ϸ�б���
	CPlazaMatchChampionBar		m_PlazaMatchChampionBar;			//�����������ھ���

};



#endif	//SERVER_LIST_VIEW_HEAD_FILE