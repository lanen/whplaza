#ifndef CHAMPIONSHIP_MATCH_HEAD_FILE
#define CHAMPIONSHIP_MATCH_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameMatchSink.h"
#include "MatchServiceHead.h"

typedef CMap<DWORD,DWORD,IServerUserItem *, IServerUserItem *>CUserItemMap;
typedef CMap<IServerUserItem *, IServerUserItem *, DWORD, DWORD>CUserSeatMap;

#define IDI_TURN_MATCH_START		(IDI_MATCH_MODULE_START+1)
#define IDI_START_MATCH_GAME		(IDI_MATCH_MODULE_START+3)					//��ʼһ�̱���  
#define IDI_DELETE_OVER_MATCH		(IDI_MATCH_MODULE_START+5)					//ɾ�������ı�����
#define IDI_NORMAL_TIMER			(IDI_MATCH_MODULE_START+6)					//���涨ʱ��
#define IDI_START_MATCH_START		(IDI_MATCH_MODULE_START+7)					//��ʼһ������ 
#define IDI_CHECK_OVER_MATCH		(IDI_MATCH_MODULE_START+8)					//���һ������ 
//#define IDI_START_MATCH_END			(IDI_MATCH_MODULE_START+9)					//��ʼһ������ 
#define IDI_HALF_MINUTE				(IDI_MATCH_MODULE_START+9)					//30��
#define IDI_GROUP_START				(IDI_MATCH_MODULE_START+10)					//�������ڵĶ�ʱ��

//////////////////////////////////////////ÿ�������ʱ��������9��///////////////////////////////////////
#define IDI_CHECK_MATCH_GROUP		(IDI_GROUP_START+1)							//��Ѳ���п��������� 
#define IDI_WAITTABLE_FIRST_ROUND	(IDI_GROUP_START+2)							//�ȴ����� 
#define IDI_WAITTABLE_RAND_TABLE	(IDI_GROUP_START+3)							//�ȴ�������� 
#define IDI_SWITCH_WAIT_TIME		(IDI_GROUP_START+4)							//�����ȴ�ʱ�� 
#define IDI_AllDismissGame			(IDI_GROUP_START+5)							//ȫ����ɢ��Ϸ
#define IDI_LASTMATCHRESULT			(IDI_GROUP_START+6)							//��������
#define IDI_EnterNextRound			(IDI_GROUP_START+7)							//������һ��ѭ��
#define IDI_TabelRequestUser		(IDI_GROUP_START+8)							//�Զ���������

/////////////////////////////////////////ÿ�����ר�ö�ʱ��////////////////////////////////////////////
#define IDI_CONTINUE_GAME			1											//������Ϸ
#define IDI_EXIT_TABLE				2											//�뿪����
#define IDI_CheakTabelStart			3											//�����������Ƿ�ʼ(����п���׼���󲻿�ʼ���)

#define IDI_LAND_FIRST_END			10											//������Ԥ������
//��һ�ֹ���
enum enFirstRoundRule
{
	FirstRoundRule_Score,					//��������
	FirstRoundRule_Time,					//��ʱ����
	TimerRoundRule							//��ʱِ �̶�ʱ������뿪
};

struct tagMatchTimer
{
	DWORD					dwTimerID;						//��ʱ��ID
	int						iElapse;						//ִ�м����
	//DWORD					dwRepeat;						//ִ�д���
	WPARAM					wParam;							//���Ӳ���
	LPARAM					lParam;							//���Ӳ���
};

//////enum enMatchUserState
//////{
//////	MS_UserMatching,						//������
//////	MS_UserPromotion,						//����״̬
//////	MS_UserMaybeOut							//��ȷ����̭
//////};
//////struct tagMatchUser
//////{
//////	enMatchUserState		enUserState;			//�����û�״̬
//////	IServerUserItem *		pMahchUserItem;			//�������û�
//////};
//����ʱ��ģʽ
struct tagMatchTimeRule
{
	BYTE				cbStartHour;						//��ʼʱ�䣨Сʱ��
	BYTE				cbStartMinute;						//��ʼʱ�䣨�֣�
	BYTE				cbEndHour;							//����ʱ�䣨Сʱ��
	BYTE				cbEndMinute;						//����ʱ�䣨�֣�

	WORD				wMaxGameCount;						//�����Ϸ����
	WORD				wMinGameCount;						//��С��Ϸ����
	BYTE				cbOutRoomCount;						//�˳��������
};
struct tagMatchRule
{
	DWORD				dwMatchID;							//����ID
	TCHAR				szMatchName[LEN_SERVER];			//��������
	WORD				wUserCount;							//��������
	WORD				wAndroidUserCount;					//һ�������л����˱���������
	WORD				wWaitTime;							//�ﵽ��̭������ȴ����ȷ��������������λ����
	DWORD				dwInitalBase;						//��ʼ����
	BYTE				cbFixedBase;						//�Ƿ�̶���ע ����̶� ��ʼ��ע��������
	BYTE				dwGrowthTime;						//����ʱ��
	BYTE				cbGrowthRange;						//��������
	DWORD				dwFee;								//������
	DWORD				dwInitalScore;						//��ʼ����=0ʱ ����ʹ�õ�ǰ���Ϊ����
	WORD				wEnterBase;							//��ʼ����=0ʱ ������ٱ���Ҳ��б�������
	WORD				wLeaveBase;							//��̭ʱ����ڶ��ٱ�����̭
	WORD				wRemainUserCount;					//ʣ�������ֹͣ��̭
	DWORD				dwSecRoundUserCount;				//������һ��ʣ�µ�����
	BYTE				cbLeastScore;						//���ٻ���
	BYTE				cbFirScoreRule;						//���븴���Ĵ��ֹ���
	BYTE				cbFirGameCount;						//����N��
	BYTE				cbSecGameCount;						//����N��
	BYTE				cbSecSwitchTableCount;				//����N���Զ�����(0Ϊ�����������ڻ��ߵ���SecGameCountҲ�൱�ڲ�����)
	WORD				wSecSwitchTableTime;				//�Զ������ȴ���������ʱ�䵥λ�� ���5�� ���300�� �����������ʱ�佫���ȴ���������
	BYTE				cbSecScoreRule;						//�������ֹ���(0Ϊÿ��ÿ�����һ����ֵ�1������,1Ϊ�����������������)
	BYTE				cbFirRoundCount;					//�����׶���Ϸ����
	BYTE				cbSecRoundCount;					//�����׶���Ϸ�ִ�
	WORD				arrSecRoundUserCount[16];			//�����׶θ�����Ϸ����
	WORD				arrSecRoundExpand[16];				//�����׶θ���һ����չֵ
	enFirstRoundRule	enFirstRule;						//��һ�ֹ���
	DWORD				dwFirstRoundTime;					//��һ��ʱ������ʱ������Ч����λ����
	DWORD				dwAheadStatistics;					//��ǰͳ�Ƴɼ� ��λ����
	WORD				wPrecedeTimer;						//�������ֵ����������
	tagMatchTimeRule	sTimerRule;							//ʱ��ģʽ����
	tagMatchRule()
	{
		ZeroMemory(this, sizeof(tagMatchRule));
	}
};

#ifdef _UNICODE
#define VER_IChampionshipMatch INTERFACE_VERSION(1,1)
static const GUID IID_IChampionshipMatch={0x786c5501,0xa7a4,0x4531,0x0091,0xc5,0x3e,0x07,0x3e,0xa6,0x0a,0xf9};
#else
#define VER_IChampionshipMatch INTERFACE_VERSION(1,1)
static const GUID IID_IChampionshipMatch={0x1a4e0406,0x8ddc,0x4e34,0x0083,0x89,0xb5,0x5e,0x4e,0xfa,0x6b,0xb7};
#endif

interface IChampionshipMatch :public IUnknownEx
{
	//�û�����
	virtual bool OnUserSignUp(IServerUserItem *pUserItem)=NULL;
	//�û�����
	virtual bool OnUserQuitMatch(IServerUserItem *pUserItem,bool bByUser=false)=NULL;
	//��������
	virtual void SetMatchTable(ITableFrame *pTable[], WORD wTableCount)=NULL;
};

#ifdef _UNICODE
#define VER_IChampionshipMatchSink INTERFACE_VERSION(1,1)
static const GUID IID_IChampionshipMatchSink={0x30e4794a,0x610e,0x4873,0x00bd,0x23,0x9c,0xc7,0x2c,0xbe,0xd4,0xc5};
#else
#define VER_IChampionshipMatchSink INTERFACE_VERSION(1,1)
static const GUID IID_IChampionshipMatchSink={0xfc8dc7b5,0x2318,0x411f,0x00b9,0x09,0x6c,0xdd,0x39,0x38,0x0d,0x18};
#endif

class CChampionshipMatch;

interface IChampionshipMatchSink:public IUnknownEx
{
	//������������ʼ����
	virtual bool  OnMatchStart(CChampionshipMatch *pMatch)=NULL;
	//�ȴ���ʱ
	virtual bool  OnWaitTimeOut(DWORD dwMatchNO)=NULL;
	//�û���̭
	virtual bool  OnUserOut(IServerUserItem *pUserItem,LPCTSTR szReason,WORD wLen,WORD wRank,CChampionshipMatch *pMatch)=NULL;
	//��������
	virtual bool  SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//��ʱ��
	virtual bool  SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter,CChampionshipMatch *pMatch)=NULL;
	//��ʱ��
	virtual bool  KillGameTimer(DWORD dwTimerID,CChampionshipMatch *pMatch)=NULL;
	//������Ϣ
	virtual bool  SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType)=NULL;
	//д�뽱��
	virtual bool  WriteUserAward(IServerUserItem *pIServerUserItem, WORD wRank,DWORD dwMatchNO,LPCTSTR szDescribe,CChampionshipMatch *pMatch)=NULL;
	//��������
	virtual bool  OnEventMatchOver(CChampionshipMatch *pMatch)=NULL;
	//�����û�����
	virtual bool  SendGroupUserScore(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch)=NULL;
	//����״̬
	virtual bool  SendGroupUserStatus(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch)=NULL;
	//������Ϣ
	virtual bool  SendGroupUserMessage(LPCTSTR pStrMessage,CChampionshipMatch *pMatch)=NULL;
	//�������ݵ������� 
	virtual bool  SendDataToGameServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//�������ݵ�һ���û�
	virtual bool  SendGroupData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize,CChampionshipMatch *pMatch)=NULL;
	//��ȡһ�����еĻ�����
	virtual IAndroidUserItem * GetFreeAndroidUserItem()=NULL;
	//�����油������
	virtual bool SetReplaceAndroidItem(IServerUserItem *pIServerUserItem)=NULL;
	//��������
	virtual bool SendMatchInfo(IServerUserItem * pIServerUserItem)=NULL;
	//����յ�����
	virtual void InsertNullTable()=NULL;

};
typedef CWHArray<IServerUserItem *> CMatchUserItemArray;

struct ITableFrameEx 
{
	ITableFrame		*pTableFrame;
	DWORD			dwBase;
	WORD			wTableID;
	BYTE			cbStartTimes;
	BYTE			cbGameCount;
	BOOL			bRoundTableFinish;
	BOOL			bSwtichTableFinish;
	CMatchUserItemArray		m_FrontUserArray;				//��һ�������������û�
};
typedef CWHArray<ITableFrameEx *>CTableFrameMananerArray;
typedef CWHArray<IServerUserItem *> CMatchUserItemArray;
typedef CWHArray<tagMatchTimer *> CUserMatchTimerArray;

struct tagMatchScore
{
	DWORD			dwUserID;
	IServerUserItem *pUserItem;
	SCORE			lScore;
};
struct tagMatchRanking
{
	DWORD			dwUserID;
	IServerUserItem *pUserItem;
	SCORE			lScore;
	LONG			lExpand;					//��չֵ
};
typedef CWHArray<tagMatchScore *> CUserMatchScoreArray;
//////typedef CWHArray<tagMatchUser *> CMatchUserItemArrayEX;
//����״̬
enum enMatchStatus
{
	MS_NULL_STATUS=0,		//��״̬
	MS_SIGNUPING,			//������
	MS_WAITTABLE,			//û������Ԥ�� �ȴ�����
	MS_STOP_OUT,
	MS_FIRST_ROUND,
	MS_START_NEXT_ROUND,
	MS_SECOND_ROUND
};

class CChampionshipMatch:public IGameEventSink
{
	friend class CGameMatchServiceManager;
public:
	CWHArray<DWORD>						m_RunWayArray;					//����ID ���ܵ�ʱ���¼ ��ֹ�ظ�д��
protected:
	static WORD						m_wChairCount;					//������Ŀ
	DWORD							m_dwMatchNO;
	CUserItemMap					m_OnMatchUserMap;				//�����û�
	//////CMatchUserItemArrayEX			m_MatchUserArray;				//�����û�

	CMatchUserItemArray				m_FreeUserArray;				//����δ��̭���û�
	CMatchUserItemArray				m_ReOutUserArray;				//���м�����̭���û�

	DWORD							m_dwCurBase;					// ��ǰ�Ļ���
	tagMatchRule					m_pMatchRule;
	IChampionshipMatchSink *		m_pMatchSink;

	enMatchStatus					m_enMatchStatus;				//�����׶�
	
	CTableFrameMananerArray			m_MatchTableArray;				//Ԥ����������
	CUserMatchTimerArray			m_MatchTimerArray;				//����ר�ö�ʱ��

	CTime							m_tMatchStart;					//��ʼʱ��
	DWORD							m_dwTimeCount;					//ʱ����
	WORD							m_wCurRountSec;					//��ǰ�����ڸ������Ľ׶�

	bool							m_bIsContinue;					//�Ƿ��������

	BYTE							m_LoopTimer;					//ѭ����ʱ��

	WORD							m_wAndroidUserCount;			//�����˸���
	
public:
	CChampionshipMatch(tagMatchRule &pMatchRule, DWORD dwMatchNO,IChampionshipMatchSink *pSink);
	virtual ~CChampionshipMatch(void);

public:
 	//�ͷŶ���
 	virtual VOID  Release(){}
 	//�ӿڲ�ѯ
	virtual VOID *  QueryInterface(REFGUID Guid, DWORD dwQueryVer);
	//���ûص��ӿ�
	 bool SetMatchSink(IChampionshipMatchSink *pSink);
	//�û�����
	 bool OnUserSignUp(IServerUserItem *pUserItem);
	//�û�����
	 BOOL OnUserQuitMatch(IServerUserItem *pUserItem,bool bByUser=false);
	//��������
	 void SetMatchTable(ITableFrame *pTable[], WORD wTableCount);
	 //��ʱ��
	 bool OnTimeMessage(DWORD dwTimerID, WPARAM dwBindParameter);

	 //��Ϸ��ʼ
	 virtual bool  OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount);
	 //��Ϸ����
	 virtual bool  OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	 //�Ƿ�����뿪��λ 
	 virtual bool  IsCanStanUp(IServerUserItem *pUserItem);
	 //��Ҷ��߻����� 
	 virtual void  SetUserOffline(WORD wTableID,IServerUserItem *pUserItem, bool bOffline);
	 //�û�����
	 virtual bool OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	 //�û�����
	 virtual bool OnActionUserStandUp(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	 //�û�ͬ��
	 virtual bool OnActionUserOnReady(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

protected:
	//ѡ��������
	void SelectPromotionUser();
	//�������
	WORD SortMapUser(tagMatchScore score[]);
	//���������
	WORD SortUserRanking(tagMatchRanking MatchRanking[],bool bIsLand);
	//�����������
	void RearrangeUserSeat(bool bDismissGame=true);
	//��ȡ����
	WORD GetUserRank(IServerUserItem *pUserItem, ITableFrame *pITableFrame=NULL);
	//��ȡ����
	WORD GetUserRankMJ(IServerUserItem *pUserItem, ITableFrame *pITableFrame=NULL);
	//��ȡ�û�
	IServerUserItem * GetSeatUserItem(DWORD dwSeatID);
	//��ʱ������Ϸ����
	void FirstRoundRuleTimeGameEnd(ITableFrameEx *pITableFrameEx);
	//����������Ϸ����
	void FirstRoundRuleScoreGameEnd(ITableFrame *pITableFrame);
    //���һ��
	bool OnTableFinishRound(ITableFrame *pITableFrame);
	//���һ��
	bool OnTableFinishRoundMJ(ITableFrame *pITableFrame);
	//Ч�������ʼ
	void EfficacyStartMatch();
	//��������
	void MatchOver();

	//������ʾ
	VOID SendWaitTip(ITableFrame *pTableFrame);
	//������ʾ
	VOID SendWaitTipMJ(ITableFrame *pTableFrame);
	//�����������
	void AddMatchTable(ITableFrameEx* ITable){m_MatchTableArray.Add(ITable);}

	//��ȡ����ר�ýӿ�
	ITableFrameEx* GetTableInterface(ITableFrame* ITable);

	//���������������û�
	void TabelRequestUser(bool bPrecedeSit=true);
	//����һ����Ϣ
	void SendGroupBox(LPCTSTR pStrMessage);
	//���ý����û�����̭�û� ÿ����һ����������
	void SetEnterUserOROutUser(ITableFrame *pITableFrame);
	//���ý����û�����̭�û� �������������
	void SetEnterUserOROutUser();
	//���ý����û� 2���齫����
	void SetEnterUserMJ();
	//���ý����û� ����������
	void SetEnterUserLand();
	//����������ɱ��
	void SetRoundTableFinish(ITableFrame *pITableFrame);
	//���������ɱ��
	bool ChackRoundTableFinish();
	//����������ɱ��
	void SetSwitchTableFinish(ITableFrame *pITableFrame);
	//����������ɱ��
	bool CheckSwitchTableFinish();
	//���������û��뿪
	void AllUserExitTable(ITableFrame *pITableFrame);
	//����������Ϸ
	void ContinueGame(ITableFrame *pITableFrame);
	//��������Ƿ�ʼ��Ϸ
	void CheakTableStart(ITableFrame *pITableFrame);
	//������һ��
	void ContinueRound();
	//������
	void LastMatchResult();
	//����������
	bool CheckMatchUser();
	//���ֹͣ��̭
	void CheckMatchStopOut(ITableFrame *pITableFrame);
	//�����ِ
	void CheckExitMatch();
	//��ȡ���������û�
	WORD GetPrecedeSitUser(DWORD dwCurTime);

	//������ʱ�� ��������ʱ�� ������ר�ñ�����ʱ�� ��Ȼ����ͬʱ�����������һ����ʱ�� ����żȻ��BUG
protected:
	//�������ר�ö�ʱ��
	void CaptureMatchTimer();
	//Ͷ�ݱ���ר�ö�ʱ��
	void PostMatchTimer(DWORD dwTimerID, int iElapse, WPARAM wParam=0, LPARAM lParam=0);
	//ɱ������ר�ö�ʱ��
	bool KillMatchTimer(DWORD dwTimerID, WPARAM wParam);
	//ɱ������ר�ö�ʱ��
	bool KillMatchTimer(INT_PTR dwIndexID);
	//ȫ��ɱ������ר�ö�ʱ��
	void AllKillMatchTimer();

	//�趨������ʱ��
	void SetMatchGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter);
	//ɱ��������ʱ��
	void KillMatchGameTimer(DWORD dwTimerID);
};


#endif