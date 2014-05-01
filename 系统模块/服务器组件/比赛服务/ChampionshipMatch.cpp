#include "StdAfx.h"
#include ".\championshipmatch.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INVALID_VALUE				0xFFFF								//��Чֵ
WORD CChampionshipMatch::m_wChairCount;

CChampionshipMatch::CChampionshipMatch(tagMatchRule &pMatchRule, DWORD dwMatchNO,IChampionshipMatchSink *pSink)
:m_pMatchRule(pMatchRule),m_dwMatchNO(dwMatchNO),m_pMatchSink(pSink)
{
	m_OnMatchUserMap.RemoveAll();
	m_FreeUserArray.RemoveAll();
	m_ReOutUserArray.RemoveAll();
	m_MatchTableArray.RemoveAll();
	m_dwTimeCount=0;
	m_enMatchStatus=MS_SIGNUPING;
	m_bIsContinue=false;
	m_dwCurBase=m_pMatchRule.dwInitalBase;
	m_wCurRountSec=0;

	m_wAndroidUserCount=0;
	m_LoopTimer=0;
	
}

CChampionshipMatch::~CChampionshipMatch(void)
{
	m_wAndroidUserCount=0;
	m_enMatchStatus=MS_NULL_STATUS;
	m_OnMatchUserMap.RemoveAll();
	m_FreeUserArray.RemoveAll();
	m_ReOutUserArray.RemoveAll();
	m_MatchTableArray.RemoveAll();
}

//�ӿڲ�ѯ
VOID* CChampionshipMatch::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameEventSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameEventSink,Guid,dwQueryVer);
	return NULL;
}


//�û�����
bool CChampionshipMatch::OnUserSignUp(IServerUserItem * pUserItem)
{
	//�û��ж�
	ASSERT(pUserItem!=NULL);
	if (pUserItem==NULL) return false;

	//�����û�
	IServerUserItem * pIServerUserExist=NULL;
	m_OnMatchUserMap.Lookup(pUserItem->GetUserID(),pIServerUserExist);

	//�û��ж�
	ASSERT(pIServerUserExist==NULL);
	if (pIServerUserExist!=NULL) return false;

	//����Ч��
	ASSERT(m_OnMatchUserMap.GetCount()<m_pMatchRule.wUserCount);
	if (m_OnMatchUserMap.GetCount()>=m_pMatchRule.wUserCount) return false;

	//�����ж�
	if (pUserItem->IsAndroidUser()==true)
	{
		if (m_wAndroidUserCount>=m_pMatchRule.wAndroidUserCount)
		{
			return false;
		}
		m_wAndroidUserCount++;
	}

	//���óɼ�
	tagUserInfo *pUserScore=pUserItem->GetUserInfo();
	pUserScore->dwWinCount=0L;
	pUserScore->dwLostCount=0L;
	pUserScore->dwFleeCount=0L;
	pUserScore->dwDrawCount=0L;
	pUserScore->lGrade=0L;
	if((SCORE)m_pMatchRule.dwInitalScore!=0)
	{
		pUserScore->lScore=(SCORE)m_pMatchRule.dwInitalScore;
	}


	//���ͳɼ�
	m_pMatchSink->SendGroupUserScore(pUserItem,NULL);

	//�����û�
	m_OnMatchUserMap[pUserItem->GetUserID()]=pUserItem;

	//��������
	if (m_OnMatchUserMap.GetCount()>=m_pMatchRule.wUserCount)
	{
		//�¼�֪ͨ
		ASSERT(m_pMatchSink!=NULL);
		if (m_pMatchSink!=NULL) m_pMatchSink->SendMatchInfo(0);
		EfficacyStartMatch();
	}
	else
	{
		BYTE cbMatchStatus=MS_SIGNUP;
		m_pMatchSink->SendData(pUserItem, MDM_GR_MATCH, SUB_GR_MATCH_STATUS, &cbMatchStatus, sizeof(cbMatchStatus));
	}

	return true;
}

//������ʼ
void CChampionshipMatch::EfficacyStartMatch()
{
	m_pMatchSink->InsertNullTable();
	//û�����ӵ����
	if( m_MatchTableArray.GetCount()<m_pMatchRule.wUserCount/m_wChairCount )
	{
		if(m_OnMatchUserMap.GetCount()<m_wChairCount)
		{
			//�������ݰ�
			CMD_CM_SystemMessage Message;
			Message.wType=SMT_EJECT|SMT_CHAT|SMT_CLOSE_GAME;
			lstrcpyn(Message.szString,TEXT("��Ǹ������ĳ�û�ǿ���˳��������ó�������ǰ����������Լ����ȴ�������"),CountArray(Message.szString));
			Message.wLength=CountStringBuffer(Message.szString);
			//��������
			WORD wSendSize=sizeof(Message)-sizeof(Message.szString)+Message.wLength*sizeof(TCHAR);
			m_pMatchSink->SendGroupData(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&Message,wSendSize, this);
			m_enMatchStatus=MS_SIGNUPING;
			return ;
		}

		m_enMatchStatus=MS_WAITTABLE;
		//3���������øú���ֱ��������
		SetMatchGameTimer(IDI_WAITTABLE_FIRST_ROUND,3000,1,NULL);

		//�������ݰ�
		CMD_CM_SystemMessage Message;
		Message.wType=SMT_CHAT;
		_sntprintf(Message.szString,CountArray(Message.szString),
			TEXT("��Ǹ�����ڹ����û������������������� %d �����ӿ���,��ȴ�������"),m_pMatchRule.wUserCount/m_wChairCount-m_MatchTableArray.GetCount());
		Message.wLength=CountStringBuffer(Message.szString);

		//��������
		WORD wSendSize=sizeof(Message)-sizeof(Message.szString)+Message.wLength*sizeof(TCHAR);
		m_pMatchSink->SendGroupData(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&Message,wSendSize, this);
		return ;
	}
	//Ԥ��������λ
	DWORD dwUserID=0L;
	IServerUserItem *pIServerUserItem=NULL;
	POSITION pos=m_OnMatchUserMap.GetStartPosition();

	WORD wChairID=0;
	while (pos!=NULL)
	{
		//��ȡ�û�
		m_OnMatchUserMap.GetNextAssoc(pos,dwUserID,pIServerUserItem);
		//Ч����
		ASSERT(pIServerUserItem!=NULL);
		m_FreeUserArray.Add(pIServerUserItem);
		//�����û�������
		pIServerUserItem->SetUserEnlist(MS_MATCHING);
	}

	//�����Զ�����
	TabelRequestUser(false);

	//����״̬
	if (m_pMatchRule.dwInitalScore==0)
	{
		m_enMatchStatus=MS_SECOND_ROUND;
		m_wCurRountSec++;
		m_bIsContinue=true;
	}
	else
	{
		m_enMatchStatus=MS_FIRST_ROUND;//MS_FIRST_ROUND;
		//m_enMatchStatus=MS_SECOND_ROUND;
		//m_wCurRountSec++;
	}



	//֪ͨ��ʼ
	m_dwTimeCount=(DWORD)time(NULL);
	m_tMatchStart=CTime::GetCurrentTime();
	
	if (m_pMatchSink!=NULL) m_pMatchSink->OnMatchStart(this);

	SetMatchGameTimer(IDI_CHECK_MATCH_GROUP,1000,TIMES_INFINITY,0);
}

//�û�����
BOOL CChampionshipMatch::OnUserQuitMatch(IServerUserItem *pUserItem,bool bByUser)
{
	//�û��ж�
	ASSERT(pUserItem!=NULL);
	if (pUserItem==NULL) return false;
	//ɾ���û�
	BOOL bSuccess=m_OnMatchUserMap.RemoveKey(pUserItem->GetUserID());
	if(bSuccess==false)return false;

	if (pUserItem->IsAndroidUser())
	{
		ASSERT(m_wAndroidUserCount>0);
		if (m_wAndroidUserCount>0) m_wAndroidUserCount--;
	}
	BYTE cbUserStatus=MS_NULL;
	m_pMatchSink->SendData(pUserItem, MDM_GR_MATCH, SUB_GR_MATCH_STATUS, &cbUserStatus, sizeof(cbUserStatus));
	if(m_enMatchStatus!=MS_SIGNUPING&&m_enMatchStatus!=MS_WAITTABLE)
	{
		m_pMatchSink->WriteUserAward(pUserItem,-1,m_dwMatchNO,TEXT("����������"),this);
	}
	return bSuccess;
}

//���ûص��ӿ�
bool CChampionshipMatch::SetMatchSink(IChampionshipMatchSink *pSink)
{
	return true;
}

//��������
void CChampionshipMatch::SetMatchTable(ITableFrame *pTable[], WORD wTableCount)
{
	
}

//��Ϸ��ʼ
bool CChampionshipMatch::OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount)
{
	pITableFrame->SetCellScore(m_dwCurBase);
	KillMatchTimer(IDI_CheakTabelStart,(WPARAM)pITableFrame);
	TCHAR szMsg[256]=TEXT("\0");
	if(m_enMatchStatus==MS_SECOND_ROUND)
	{
		if (m_pMatchRule.dwInitalScore==0)
		{
			for(WORD i=0; i<m_MatchTableArray.GetCount(); i++)
			{
				if(m_MatchTableArray[i]->pTableFrame==pITableFrame)
				{
					m_MatchTableArray[i]->cbStartTimes=0;
					m_MatchTableArray[i]->cbGameCount++;
					_sntprintf(szMsg, CountArray(szMsg),TEXT("��̭����%d�֣���%d�֣�,����һλ��ҷ����ܺ͵��ڻ����%d�����ó�����������ʼ%d��ĵȴ�ʱ�䣬�����ڼ��ҵ���%d������̭"),m_wCurRountSec,
						m_pMatchRule.cbSecRoundCount,m_pMatchRule.arrSecRoundExpand[m_wCurRountSec-1],m_pMatchRule.wSecSwitchTableTime,m_pMatchRule.dwFee*m_pMatchRule.wLeaveBase);
					//�������ݰ�
					CMD_CM_SystemMessage Message;
					Message.wType=SMT_CHAT|SMT_TABLE_ROLL;

					lstrcpyn(Message.szString,szMsg,CountArray(Message.szString));
					Message.wLength=CountStringBuffer(Message.szString);

					//��������
					WORD wSendSize=sizeof(Message)-sizeof(Message.szString)+Message.wLength*sizeof(TCHAR);
					pITableFrame->SendTableData(INVALID_CHAIR,SUB_GF_SYSTEM_MESSAGE, (void*)&Message,wSendSize,MDM_GF_FRAME);

					//��������
					CMD_GR_Match_ER_Sparrows Match_ER_Sparrows;
					ZeroMemory(&Match_ER_Sparrows,sizeof(Match_ER_Sparrows));
					Match_ER_Sparrows.wALLGameRound=m_pMatchRule.cbSecRoundCount;
					Match_ER_Sparrows.wALLGameRoundFan=m_pMatchRule.arrSecRoundExpand[m_pMatchRule.cbSecRoundCount-1];
					Match_ER_Sparrows.wCurGameRound=m_wCurRountSec;
					Match_ER_Sparrows.wCurGameRoundFan=m_pMatchRule.arrSecRoundExpand[m_wCurRountSec-1];
					if(m_wCurRountSec<m_wCurRountSec)
						Match_ER_Sparrows.wNextGameRoundFan=m_pMatchRule.arrSecRoundExpand[m_wCurRountSec];
					//_sntprintf(MatchInfo.szTitle[0], CountArray(MatchInfo.szTitle[0]),TEXT("��Ϸ������%d"), m_dwCurBase);
					//_sntprintf(MatchInfo.szTitle[1], CountArray(MatchInfo.szTitle[1]),TEXT("ֹͣ����������%d��"),m_pMatchRule.arrSecRoundExpand[m_wCurRountSec-1]-1);
					//

					for(WORD j=0; j<wChairCount; j++)
					{
						//��ȡ�û�
						IServerUserItem *pIServerUserItem=pITableFrame->GetTableUserItem(j);
						if(pITableFrame==NULL) continue;

						//��������
						tagUserInfo *pUserInfo=pIServerUserItem->GetUserInfo();
						Match_ER_Sparrows.wLocalGameFan=(WORD)pUserInfo->lGrade;
						Match_ER_Sparrows.wOtherGameFan=(WORD)pITableFrame->GetTableUserItem((j+1)%m_wChairCount)->GetUserInfo()->lGrade;
						////////MatchInfo.wGameCount=m_MatchTableArray[i]->cbGameCount; 
						//_sntprintf(MatchInfo.szTitle[2], CountArray(MatchInfo.szTitle[2]),TEXT("�Լҷ�����%d"),pITableFrame->GetTableUserItem((j+1)%m_wChairCount)->GetUserInfo()->lGrade);
						//_sntprintf(MatchInfo.szTitle[3], CountArray(MatchInfo.szTitle[3]),TEXT("��ķ�����%d"), pUserInfo->lGrade);
						//��������
						pITableFrame->SendTableData(j, SUB_GR_MATCH_INFO_ER_SPARROWS, &Match_ER_Sparrows, sizeof(Match_ER_Sparrows), MDM_GF_GAME);
						m_pMatchSink->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GR_MATCH_WAIT_TIP,NULL,0);
					}
				}
			}
			return true;
		}
		for(WORD i=0; i<m_MatchTableArray.GetCount(); i++)
		{		
			if(m_MatchTableArray[i]->pTableFrame==pITableFrame)
			{
				m_MatchTableArray[i]->cbStartTimes=0;
				m_MatchTableArray[i]->cbGameCount++;
				_sntprintf(szMsg, CountArray(szMsg),TEXT("��̭����%d�֣���%d�֣���%d�ֿ�ʼ����%d�֣���"),m_wCurRountSec,
					m_pMatchRule.cbSecRoundCount,m_MatchTableArray[i]->cbGameCount,m_pMatchRule.cbSecGameCount);

				//�������ݰ�
				CMD_CM_SystemMessage Message;
				Message.wType=SMT_CHAT;

				lstrcpyn(Message.szString,szMsg,CountArray(Message.szString));
				Message.wLength=CountStringBuffer(Message.szString);

				//��������
				WORD wSendSize=sizeof(Message)-sizeof(Message.szString)+Message.wLength*sizeof(TCHAR);
				pITableFrame->SendTableData(INVALID_CHAIR,SUB_GF_SYSTEM_MESSAGE, (void*)&Message,wSendSize,MDM_GF_FRAME);

				//��������
				CMD_GR_Match_Info MatchInfo;
				ZeroMemory(&MatchInfo,sizeof(MatchInfo));
				_sntprintf(MatchInfo.szTitle[0], CountArray(MatchInfo.szTitle[0]),TEXT("���ַ�ʽ������"));
				_sntprintf(MatchInfo.szTitle[1], CountArray(MatchInfo.szTitle[1]),TEXT("��Ϸ������%d"), m_dwCurBase);
				_sntprintf(MatchInfo.szTitle[2], CountArray(MatchInfo.szTitle[2]),TEXT("�������ƣ�%d�ֻ�����"),m_pMatchRule.cbSecGameCount);
				_sntprintf(MatchInfo.szTitle[3], CountArray(MatchInfo.szTitle[3]),TEXT("%d�ֻ�������߻�ʤ"), m_pMatchRule.cbSecGameCount);

				for(WORD j=0; j<wChairCount; j++)
				{
					//��ȡ�û�
					IServerUserItem *pIServerUserItem=pITableFrame->GetTableUserItem(j);
					if(pITableFrame==NULL) continue;

					//��������
					tagUserInfo *pUserInfo=pIServerUserItem->GetUserInfo();
					MatchInfo.wGameCount=m_MatchTableArray[i]->cbGameCount;

					//��������
					pITableFrame->SendTableData(j, SUB_GR_MATCH_INFO, &MatchInfo, sizeof(MatchInfo), MDM_GF_FRAME);
				}
				break;
			}
		}
	}
	else if(m_enMatchStatus==MS_FIRST_ROUND)
	{
		//��������
		CMD_GR_Match_Info MatchInfo;
		ZeroMemory(&MatchInfo,sizeof(MatchInfo));
		_sntprintf(MatchInfo.szTitle[0], CountArray(MatchInfo.szTitle[0]),TEXT("���ַ�ʽ���׷֡�����"));
		_sntprintf(MatchInfo.szTitle[1], CountArray(MatchInfo.szTitle[1]),TEXT("��Ϸ������%d"), m_dwCurBase);
		if(m_pMatchRule.enFirstRule==FirstRoundRule_Score)
		{
			_sntprintf(MatchInfo.szTitle[2], CountArray(MatchInfo.szTitle[2]),TEXT("�������ƣ���������"));
			_sntprintf(MatchInfo.szTitle[3], CountArray(MatchInfo.szTitle[3]),TEXT("���� %d �ֽ�����̭"), (int)(m_dwCurBase*m_pMatchRule.cbLeastScore/100));
		}
		else
		{
			_sntprintf(MatchInfo.szTitle[2], CountArray(MatchInfo.szTitle[2]),TEXT("�������ƣ���ʱ����"));
			CTimeSpan span=CTime::GetCurrentTime()-m_tMatchStart;
			_sntprintf(MatchInfo.szTitle[3], CountArray(MatchInfo.szTitle[3]),TEXT("��Լ %d ���Ӻ�����"), 
				(m_pMatchRule.dwFirstRoundTime-span.GetTotalSeconds()-m_pMatchRule.dwAheadStatistics)/60+1);
		}
		for(WORD i=0; i<wChairCount; i++)
		{
			//��ȡ�û�
			IServerUserItem *pIServerUserItem=pITableFrame->GetTableUserItem(i);
			if(pITableFrame==NULL) continue;

			//��������
			tagUserInfo *pUserInfo=pIServerUserItem->GetUserInfo();
			MatchInfo.wGameCount=(WORD)(pUserInfo->dwWinCount+pUserInfo->dwLostCount+pUserInfo->dwFleeCount+pUserInfo->dwDrawCount+1);

			//��������
			pITableFrame->SendTableData(i, SUB_GR_MATCH_INFO, &MatchInfo, sizeof(MatchInfo), MDM_GF_FRAME);
		}
	}
	return true;
}

//��Ϸ����
bool CChampionshipMatch::OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//��¼���ֺ���Щ�û����
	ITableFrameEx * pITableFrameEx=GetTableInterface(pITableFrame);
	ASSERT(pITableFrameEx);
	if (pITableFrameEx==NULL)return false;
	pITableFrameEx->m_FrontUserArray.RemoveAll();
	for (int i=0;i<m_wChairCount;i++)
	{
		IServerUserItem* UserItem=pITableFrame->GetTableUserItem(i);
		if(UserItem)
		{
			pITableFrameEx->m_FrontUserArray.Add(UserItem);
		}
	}

	if(m_pMatchRule.dwInitalScore==0)//2���齫��������
	{
		OnTableFinishRoundMJ(pITableFrame);
		CheckExitMatch();
		return true;
	}
	if(m_enMatchStatus==MS_STOP_OUT)
	{
		//if(m_OnMatchUserMap.GetCount()<(INT_PTR)m_pMatchRule.dwSecRoundUserCount)
		//{
		//	m_pMatchSink->SendGroupUserMessage(TEXT("������������������������������ٴα����μӱ���"),this);
		//	MatchOver();
		//	return TRUE;
		//}

		LPCTSTR szMsg=TEXT("�Ѿ��ﵽ��̭�������ȴ�������ҽ�����Ϸ��ȷ����������!");
		for(WORD j=0;j<m_wChairCount;j++)
		{
			IServerUserItem *pUser=pITableFrame->GetTableUserItem(j);
			if(pUser!=NULL)
			{
				m_pMatchSink->SendGameMessage(pUser, szMsg,SMT_CHAT|SMT_TABLE_ROLL);
				m_FreeUserArray.Add(pUser);
			}
		}
		SetRoundTableFinish(pITableFrame);
		SendWaitTip(pITableFrame);

		if (ChackRoundTableFinish())
		{
			m_enMatchStatus=MS_START_NEXT_ROUND;
			SetMatchGameTimer(IDI_EnterNextRound,5000,1,0);
			CheckExitMatch();
			return true;
		}
		
		//�뿪���� 5���
		//m_pMatchSink->SetGameTimer(IDI_EXIT_TABLE,5000,1,(WPARAM)pITableFrame,this);
	}
	else if(m_enMatchStatus==MS_FIRST_ROUND)
	{
		if(m_pMatchRule.enFirstRule==FirstRoundRule_Score)
			FirstRoundRuleScoreGameEnd(pITableFrame);
		else if(m_pMatchRule.enFirstRule==FirstRoundRule_Time)
			FirstRoundRuleTimeGameEnd(0);
	}
	else if(m_enMatchStatus==MS_SECOND_ROUND)
	{
		OnTableFinishRound(pITableFrame);
	}
	CheckExitMatch();
	//��������ֱ�ӷ���
	if(m_enMatchStatus==MS_NULL_STATUS)return true;

	if(m_OnMatchUserMap.GetCount()>0)
	{
		//����֪ͨ
		tagMatchScore *pScore = new tagMatchScore[m_OnMatchUserMap.GetCount()];
		ZeroMemory(pScore,sizeof(tagMatchScore)*m_OnMatchUserMap.GetCount());
		WORD wCount=SortMapUser(pScore);
		for (WORD i=0;i<wCount;i++)
		{
			m_pMatchSink->SendGroupUserScore(pScore[i].pUserItem,this);
		}
		SafeDeleteArray(pScore);
	}

	return true;
}

//��ʱ��
bool CChampionshipMatch::OnTimeMessage(DWORD dwTimerID, WPARAM dwBindParameter)
{
	//״̬У��
	if(m_enMatchStatus==MS_NULL_STATUS) return true;

	switch(dwTimerID)
	{
	case IDI_CHECK_MATCH_GROUP:
		{
			//��������ģʽ
			static int timer=0;
			if(m_pMatchRule.enFirstRule==FirstRoundRule_Score&&timer++%5==0)//ÿ5�����һ��
			{
				//Ԥ��״̬
				if (m_enMatchStatus==MS_FIRST_ROUND)
				{
					bool bCanStart=true;
					DWORD dwTimeCount=(DWORD)time(NULL)-m_dwTimeCount;
					//Ԥ����ʱ�� ���ݺ�̨����ʱ���ۼӻ�ע
					if(m_pMatchRule.cbFixedBase==0&&m_enMatchStatus==MS_FIRST_ROUND && dwTimeCount>m_pMatchRule.dwGrowthTime)
					{
						m_dwCurBase=m_dwCurBase*(100+m_pMatchRule.cbGrowthRange)/100;
						m_dwTimeCount=(DWORD)time(NULL);
					}
					//�������û��Զ�����
					//TabelRequestUser();
				}

			}
			//����Ͷ�ݹ��ı�����ʱ��
			CaptureMatchTimer();
			return true;
		}
	case IDI_WAITTABLE_FIRST_ROUND:
		{
			if(m_enMatchStatus==MS_WAITTABLE)
			{
				EfficacyStartMatch();
			}
			return true;
		}
	case IDI_SWITCH_WAIT_TIME:
		{
			////�������û��Զ�����
			RearrangeUserSeat();
			TabelRequestUser();
			//ContinueGame((ITableFrame*)dwBindParameter);
			return true;
		}
	case IDI_TabelRequestUser:
		{
			TabelRequestUser();
			return true;
		}
	case IDI_EnterNextRound:
		{
			if(m_enMatchStatus==MS_START_NEXT_ROUND)
			{
				//������һ��
				ContinueRound();
			}
			return true;
		}
	case IDI_AllDismissGame:
		{
			RearrangeUserSeat();
			return true;
		}
	case IDI_LASTMATCHRESULT:
		{
			//���������
			LastMatchResult();
			return true;
		}
	}

	return true;
}

//ѡ������û�
void CChampionshipMatch::SelectPromotionUser()
{
	KillMatchGameTimer(IDI_AllDismissGame);
	INT_PTR nUserCount=m_OnMatchUserMap.GetCount();
	if( nUserCount==0 || nUserCount<(INT_PTR)m_pMatchRule.arrSecRoundUserCount[m_wCurRountSec])
	{
		SendGroupBox(TEXT("��Ǹ�����ڹ����û�ǿ���˳��������ó�������ǰ��������ӭ�����������μӱ�����"));
		LastMatchResult();
		return;
	}
	//ȫ��ǿ��ֹͣ��Ϸ �����������
	RearrangeUserSeat(false);
	
	//ȥ����������� �����ָ������� ֻ��ʣ�µ���������
	for (WORD i=WORD(m_pMatchRule.arrSecRoundUserCount[m_wCurRountSec]/m_wChairCount);i<m_MatchTableArray.GetCount();)
	{
		ITableFrameEx* pTempTable=m_MatchTableArray[i];
		m_MatchTableArray.RemoveAt(i);
		SafeDelete(pTempTable);
	}

	//�����������벻ͬ���ж�
	if (m_pMatchRule.dwInitalScore==0)
	{
		m_bIsContinue=true;
		SetEnterUserMJ();
	}
	else
	{
		POSITION pos=m_OnMatchUserMap.GetStartPosition();
		IServerUserItem *pLoopUserItem=NULL;
		DWORD dwUserID=0;
		while(pos!=NULL)
		{
			m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pLoopUserItem);
			//��������
			if(m_enMatchStatus==MS_STOP_OUT)
			{
				if(pLoopUserItem->GetUserInfo()->lScore>0)
					pLoopUserItem->GetUserInfo()->lScore=(SCORE)sqrt(FLOAT(pLoopUserItem->GetUserInfo()->lScore))*10L/3L;
				else
					pLoopUserItem->GetUserInfo()->lScore=(SCORE)(pLoopUserItem->GetUserInfo()->lScore*0.15);
			}
			else if(m_enMatchStatus==MS_START_NEXT_ROUND)
			{
				pLoopUserItem->GetUserInfo()->lScore=(SCORE)(pLoopUserItem->GetUserInfo()->lScore*0.3);
			}
			//���ͻ���
			m_pMatchSink->SendGroupUserScore(pLoopUserItem,this);
		}
		SetEnterUserLand();
	}
	m_enMatchStatus=MS_SECOND_ROUND;
	m_wCurRountSec++;
	if (m_pMatchRule.cbFixedBase==0)m_dwCurBase=m_pMatchRule.dwInitalBase*2;
	
	TabelRequestUser();
}

//���������
WORD CChampionshipMatch::SortMapUser(tagMatchScore score[])
{
	INT_PTR nCount=m_OnMatchUserMap.GetCount();
	POSITION pos=m_OnMatchUserMap.GetStartPosition();
	nCount=0;
	while(pos!=NULL)
	{
		m_OnMatchUserMap.GetNextAssoc(pos,score[nCount].dwUserID, score[nCount].pUserItem);
		score[nCount].dwUserID=score[nCount].pUserItem->GetUserID();
		score[nCount].lScore=score[nCount].pUserItem->GetUserScore();
		nCount++;
	}
	for(INT_PTR i = 1;i < nCount;i++)
	{
		INT_PTR left = 0,right = i - 1;
		tagMatchScore  Temp = score[i];
		while(left <= right)
		{
			INT_PTR mid = (left + right)/2;
			if(score[i].lScore > score[mid].lScore)right = mid - 1;
			else left = mid + 1;
		}
		for(INT_PTR j = i - 1;j >= left;j--)
			score[j+1] = score[j];
		score[left] = Temp;
	}

	return (WORD)nCount;
}

//��������� �ú����Ѿ���Ԫ����û���κ�����
WORD CChampionshipMatch::SortUserRanking(tagMatchRanking MatchRanking[],bool bIsLand)
{
	if(bIsLand)
	{
		INT_PTR nCount=m_OnMatchUserMap.GetCount();
		POSITION pos=m_OnMatchUserMap.GetStartPosition();
		nCount=0;
		while(pos!=NULL)
		{
			m_OnMatchUserMap.GetNextAssoc(pos,MatchRanking[nCount].dwUserID, MatchRanking[nCount].pUserItem);
			MatchRanking[nCount].dwUserID=MatchRanking[nCount].pUserItem->GetUserID();
			MatchRanking[nCount].lScore=MatchRanking[nCount].pUserItem->GetUserScore();
			nCount++;
		}
		for(INT_PTR i = 1;i < nCount;i++)
		{
			INT_PTR left = 0,right = i - 1;
			tagMatchRanking  Temp = MatchRanking[i];
			while(left <= right)
			{
				INT_PTR mid = (left + right)/2;
				if(MatchRanking[i].lScore > MatchRanking[mid].lScore)right = mid - 1;
				else left = mid + 1;
			}
			for(INT_PTR j = i - 1;j >= left;j--)
				MatchRanking[j+1] = MatchRanking[j];
			MatchRanking[left] = Temp;
		}

		return (WORD)nCount;
	}
	INT_PTR nCount=m_OnMatchUserMap.GetCount();
	POSITION pos=m_OnMatchUserMap.GetStartPosition();
	nCount=0;
	while(pos!=NULL)
	{
		m_OnMatchUserMap.GetNextAssoc(pos,MatchRanking[nCount].dwUserID, MatchRanking[nCount].pUserItem);
		MatchRanking[nCount].dwUserID=MatchRanking[nCount].pUserItem->GetUserID();
		MatchRanking[nCount].lScore=MatchRanking[nCount].pUserItem->GetUserScore();
		MatchRanking[nCount].lExpand=(LONG)MatchRanking[nCount].pUserItem->GetUserInfo()->lGrade;//2���齫�ķ���
		nCount++;
	}
	//���������齫������
	for(INT_PTR i = 1;i < nCount;i++)
	{
		INT_PTR left = 0,right = i - 1;
		tagMatchRanking  Temp = MatchRanking[i];
		while(left <= right)
		{
			INT_PTR mid = (left + right)/2;
			if(MatchRanking[i].lExpand > MatchRanking[mid].lExpand)right = mid - 1;
			else left = mid + 1;
		}
		for(INT_PTR j = i - 1;j >= left;j--)
			MatchRanking[j+1] = MatchRanking[j];
		MatchRanking[left] = Temp;
	}
	//�ڲ�ѯ�ж����ȵ�û �������������˭��
	for (INT_PTR i = 0;i < nCount-1;i++)
	{
		for (INT_PTR j = i+1;j < nCount;j++)
		{
			if (MatchRanking[i].lExpand==MatchRanking[j].lExpand&&MatchRanking[i].lScore<MatchRanking[j].lScore)
			{
				tagMatchRanking  Temp = MatchRanking[i];
				MatchRanking[i]=MatchRanking[j];
				MatchRanking[j]=Temp;
			}else break;
		}
	}
	return (WORD)nCount;
}

//�������������
void CChampionshipMatch::RearrangeUserSeat(bool bDismissGame)
{
	KillMatchGameTimer(IDI_SWITCH_WAIT_TIME);
	for(WORD i=0; i<m_MatchTableArray.GetCount();i++)
	{
		ITableFrame* pTableFrame=m_MatchTableArray[i]->pTableFrame;
		if (pTableFrame==NULL)continue;
		if (bDismissGame==false)
		{
			m_MatchTableArray[i]->cbGameCount=0;
			m_MatchTableArray[i]->bRoundTableFinish=false;
			m_MatchTableArray[i]->bSwtichTableFinish=false;
		}
		//ǿ�ƽ�ɢ��Ϸ
		if(bDismissGame==false&&pTableFrame->IsGameStarted())
		{
			CTraceService::TraceString(TEXT("�µ�һ�ֿ�ʼ�ˣ������������ڱ�����"),TraceLevel_Exception);
		}
		if(bDismissGame&&pTableFrame->IsGameStarted())
		{
			pTableFrame->DismissGame();
		}
		//�����û�ȫ���뿪
		AllUserExitTable(pTableFrame);
	}
	return;
}

//�Ƿ�����뿪��λ 
bool CChampionshipMatch::IsCanStanUp(IServerUserItem *pUserItem)
{
	//AfxMessageBox(L"�Ƿ�����뿪��λ");
	return false;
}

//��Ҷ��߻����� 
void CChampionshipMatch::SetUserOffline(WORD wTableID,IServerUserItem *pUserItem, bool bOffline)
{
	//AfxMessageBox(L"��Ҷ��߻�����");
}

//��ȡ����
WORD CChampionshipMatch::GetUserRank(IServerUserItem *pUserItem, ITableFrame *pITableFrame)
{
	//����Ч��
	ASSERT(pUserItem!=NULL);
	if(pUserItem==NULL) return INVALID_WORD;
	//�������
	SCORE lUserScore=pUserItem->GetUserScore();
	WORD wRank=1;
	DWORD dwUserID=0;
	IServerUserItem *pLoopUserItem=NULL;

	if(pITableFrame==NULL)
	{
		//��������
		POSITION pos=m_OnMatchUserMap.GetStartPosition();
		while(pos!=NULL)
		{
			m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pLoopUserItem);
			if(pLoopUserItem!=pUserItem && pLoopUserItem->GetUserScore()>lUserScore)
				wRank++;
		}
	}
	else
	{
		ASSERT(pUserItem->GetTableID()==pITableFrame->GetTableID());
		if(pUserItem->GetTableID()!=pITableFrame->GetTableID()) return INVALID_WORD;

		for(WORD i=0; i<m_wChairCount; i++)
		{
			pLoopUserItem=pITableFrame->GetTableUserItem(i);
			if(pLoopUserItem==NULL)continue;

			if(pLoopUserItem!=pUserItem && pLoopUserItem->GetUserScore()>lUserScore)
				wRank++;
		}
	}

	return wRank;
}

//��ȡ����
WORD CChampionshipMatch::GetUserRankMJ(IServerUserItem *pUserItem, ITableFrame *pITableFrame)
{
	//����Ч��
	ASSERT(pUserItem!=NULL);
	if(pUserItem==NULL) return INVALID_WORD;
	//�������
	SCORE lGrade=pUserItem->GetUserInfo()->lGrade;
	WORD wRank=1;
	DWORD dwUserID=0;
	IServerUserItem *pLoopUserItem=NULL;

	if(pITableFrame==NULL)
	{
		INT_PTR nUserCount=m_OnMatchUserMap.GetCount();
		tagMatchRanking *pScore=new tagMatchRanking[nUserCount];
		ZeroMemory(pScore,sizeof(tagMatchRanking)*nUserCount);

		SortUserRanking(pScore,false);

		for (int i=0;i<nUserCount;i++)
		{
			if(pUserItem->GetUserID()==pScore[i].dwUserID)wRank=i+1;
		}

		SafeDeleteArray(pScore);
	}
	else
	{
		ASSERT(pUserItem->GetTableID()==pITableFrame->GetTableID());
		if(pUserItem->GetTableID()!=pITableFrame->GetTableID()) return INVALID_WORD;

		for(WORD i=0; i<m_wChairCount; i++)
		{
			pLoopUserItem=pITableFrame->GetTableUserItem(i);
			if(pLoopUserItem==NULL)continue;

			//���жϷ��� ������жϽ��
			if(pLoopUserItem!=pUserItem && pLoopUserItem->GetUserInfo()->lGrade>lGrade)
				wRank++;
			else if(pLoopUserItem!=pUserItem && pLoopUserItem->GetUserInfo()->lGrade==lGrade&&
				pLoopUserItem->GetUserScore()>pUserItem->GetUserScore())
				wRank++;
		}
	}

	return wRank;
}

//��ȡ����ר�ýӿ�
ITableFrameEx* CChampionshipMatch::GetTableInterface(ITableFrame* ITable)
{
	for (int i=0;i<m_MatchTableArray.GetCount();i++)
	{
		if (m_MatchTableArray[i]->pTableFrame==ITable)return m_MatchTableArray[i];
	}
	return NULL;
}

//���һ��
bool CChampionshipMatch::OnTableFinishRound(ITableFrame *pITableFrame)
{
	ITableFrameEx * pITableFrameEx=GetTableInterface(pITableFrame);
	ASSERT(pITableFrameEx);
	if (pITableFrameEx==NULL)return false;

	if (pITableFrameEx->cbGameCount>=m_pMatchRule.cbSecGameCount)
	{
		if(m_wCurRountSec>=m_pMatchRule.cbSecRoundCount)
		{
			//��������
			TCHAR szMsg[256]=TEXT("");
			_sntprintf(szMsg,CountArray(szMsg),TEXT("������������ȴ�ϵͳΪ��������"));

			POSITION pos=m_OnMatchUserMap.GetStartPosition();
			IServerUserItem *pUser=NULL;
			DWORD dwUserID;
			while(pos!=NULL)
			{
				m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUser);
				m_pMatchSink->SendGameMessage(pUser,szMsg,SMT_CHAT|SMT_TABLE_ROLL);
			}

			SetMatchGameTimer(IDI_LASTMATCHRESULT,5000,1,(WPARAM)pITableFrame);
			return true;
		}
		//�趨�������
		SetRoundTableFinish(pITableFrame);
		//ÿ����һ���������� ����Ϊ�������
		if(m_pMatchRule.cbSecScoreRule==0)
			SetEnterUserOROutUser(pITableFrame);
		SendWaitTip(pITableFrame);
		//���������Ƿ�ȫ����� ���ȫ����ɽ���һ���׶εı���
		//�����¸��׶�
		if (ChackRoundTableFinish())
		{
			//�������������
			if(m_pMatchRule.cbSecScoreRule==1)
				SetEnterUserOROutUser();
			m_enMatchStatus=MS_START_NEXT_ROUND;
			SetMatchGameTimer(IDI_EnterNextRound,5000,1,0);
			
			return true;
		}
	}
	else
	{
		//��������б�
		for (int i=0;i<m_wChairCount;i++)
		{
			IServerUserItem * ServerUserItem=pITableFrame->GetTableUserItem(i);
			if(ServerUserItem==NULL)continue;
			m_FreeUserArray.Add(ServerUserItem);
		}

		//�����Զ������Ŀ���
		if(m_pMatchRule.cbSecSwitchTableCount>0&&pITableFrameEx->cbGameCount>=m_pMatchRule.cbSecSwitchTableCount)
		{
			SetSwitchTableFinish(pITableFrame);
			SendWaitTip(pITableFrame);
			//���������Ƿ���Ի���
			if (CheckSwitchTableFinish())
			{
				SetMatchGameTimer(IDI_SWITCH_WAIT_TIME,5000,1,0);
				return true;
			}
			SetMatchGameTimer(IDI_SWITCH_WAIT_TIME,m_pMatchRule.wSecSwitchTableTime*1000,1,0);
		}else//������
		{
			PostMatchTimer(IDI_CONTINUE_GAME,5,(WPARAM)pITableFrame);
			return true;
		}
	}
	//�뿪���� 5���
	//PostMatchTimer(IDI_EXIT_TABLE,5,(WPARAM)pITableFrame);
	return true;
}

//���һ��
bool CChampionshipMatch::OnTableFinishRoundMJ(ITableFrame *pITableFrame)
{
	ITableFrameEx * pITableFrameEx=GetTableInterface(pITableFrame);
	ASSERT(pITableFrameEx);
	if (pITableFrameEx==NULL)return false;

	//�����û�����
	OnEventGameStart(pITableFrame,m_wChairCount);

	for (int i=0;i<m_wChairCount;i++)
	{
		IServerUserItem* UserItem=pITableFrame->GetTableUserItem(i);
		if(UserItem)
		{
			//��ҵ���һ����������������̭
			if(UserItem->GetUserInfo()->lScore<m_pMatchRule.dwInitalBase*m_pMatchRule.wLeaveBase)
			{
				m_pMatchSink->WriteUserAward(UserItem, (WORD)m_OnMatchUserMap.GetCount(),m_dwMatchNO,NULL,this);
			}else
			{
				m_FreeUserArray.Add(UserItem);
			}
			//�û��ķ������̨���� ֹͣ����
			if(m_bIsContinue&&UserItem->GetUserInfo()->lGrade>=m_pMatchRule.arrSecRoundExpand[m_wCurRountSec-1])
			{
				m_bIsContinue=false;
				//�ȴ�һ��ʱ�� �������ʱ�仹�����ڱ��� ǿ�Ƹ��ֵı���
				SetMatchGameTimer(IDI_AllDismissGame,m_pMatchRule.wSecSwitchTableTime*1000,1,(WPARAM)pITableFrame);

				TCHAR szMsg[256]=TEXT("");
				_sntprintf(szMsg,CountArray(szMsg),TEXT("��ϲ %s ���ȵ������������%d��󼴽���������������ȷ��������ң�"),
					UserItem->GetUserInfo()->szNickName,m_pMatchRule.wSecSwitchTableTime);

				POSITION pos=m_OnMatchUserMap.GetStartPosition();
				IServerUserItem *pUser=NULL;
				DWORD dwUserID;
				while(pos!=NULL)
				{
					m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUser);
					m_pMatchSink->SendGameMessage(pUser,szMsg,SMT_CHAT|SMT_TABLE_ROLL);
				}
			}
		}
	}
	//��������
	if (m_bIsContinue)
	{
		PostMatchTimer(IDI_CONTINUE_GAME,5,(WPARAM)pITableFrame);
		return true;
	}

	//�������ӽ������ֱ��
	SetRoundTableFinish(pITableFrame);
	SendWaitTipMJ(pITableFrame);
	
	if (ChackRoundTableFinish())
	{
		if(m_wCurRountSec>=m_pMatchRule.cbSecRoundCount)
		{
			//��������
			TCHAR szMsg[256]=TEXT("");
			_sntprintf(szMsg,CountArray(szMsg),TEXT("������������ȴ�ϵͳΪ��������"));

			POSITION pos=m_OnMatchUserMap.GetStartPosition();
			IServerUserItem *pUser=NULL;
			DWORD dwUserID;
			while(pos!=NULL)
			{
				m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUser);
				m_pMatchSink->SendGameMessage(pUser,szMsg,SMT_CHAT|SMT_TABLE_ROLL);
			}

			SetMatchGameTimer(IDI_LASTMATCHRESULT,5000,1,(WPARAM)pITableFrame);
			return true;
		}
		m_enMatchStatus=MS_START_NEXT_ROUND;
		SetMatchGameTimer(IDI_EnterNextRound,5000,1,0);
		return true;
	}
	return true;
}

//��ȡ�û�
IServerUserItem * CChampionshipMatch::GetSeatUserItem(DWORD dwSeatID)
{

	
	return NULL;
}

//�û�����
bool CChampionshipMatch::OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if( bLookonUser == false )
	{
		
	}
	return true;
}

//�û�����
bool CChampionshipMatch::OnActionUserStandUp(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if( bLookonUser == false )
	{
		
	}
	return true;
}

//�û�ͬ��
bool CChampionshipMatch::OnActionUserOnReady(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	
	return false;
}

//��������
void CChampionshipMatch::MatchOver()
{
	AllKillMatchTimer();
	KillMatchGameTimer(IDI_CHECK_MATCH_GROUP);
	KillMatchGameTimer(IDI_WAITTABLE_FIRST_ROUND);
	KillMatchGameTimer(IDI_WAITTABLE_RAND_TABLE);
	KillMatchGameTimer(IDI_SWITCH_WAIT_TIME);
	KillMatchGameTimer(IDI_AllDismissGame);
	KillMatchGameTimer(IDI_LASTMATCHRESULT);
	KillMatchGameTimer(IDI_TabelRequestUser);
	RearrangeUserSeat();

	if(m_enMatchStatus!=MS_NULL_STATUS)
	{
		m_enMatchStatus=MS_NULL_STATUS;
		m_pMatchSink->OnEventMatchOver(this);
	}
}

//��ʱ������Ϸ����
void CChampionshipMatch::FirstRoundRuleTimeGameEnd(ITableFrameEx *pITableFrameEx)
{
	
}

//�������ֶ�������Ϸ����
void CChampionshipMatch::FirstRoundRuleScoreGameEnd(ITableFrame *pITableFrame)
{
	for (WORD j=0;j<m_wChairCount;j++)
	{
		IServerUserItem *pUserItem=pITableFrame->GetTableUserItem(j);

		if(pUserItem!=NULL)
		{
			BYTE cbOutUserCount=0;
			const tagUserInfo *pScore=pUserItem->GetUserInfo();

			if (pScore->lScore<m_dwCurBase*m_pMatchRule.cbLeastScore/100)
			{
				TCHAR szReason[256]=TEXT("");
				_sntprintf(szReason, CountArray(szReason), TEXT("���ź����������ĵ�ǰ���ֲ��������%d%%��������̭�����ˡ��ڱ��α����л�õ�%d����"),
					m_pMatchRule.cbLeastScore,m_OnMatchUserMap.GetCount());
				if(m_OnMatchUserMap.GetCount()<=m_pMatchRule.wRemainUserCount)
				{
					//�����ж�һ�� ��ֹ����������ֹͣ��̭����һ������̭������
					CheckMatchStopOut(pITableFrame);
					m_FreeUserArray.Add(pUserItem);
					continue;
				}
				m_pMatchSink->WriteUserAward(pUserItem, (WORD)m_OnMatchUserMap.GetCount(), m_dwMatchNO, szReason,this);
				m_OnMatchUserMap.RemoveKey(pUserItem->GetUserID());
				//Ԥ������̭�ľ��뿪��λ
				pITableFrame->PerformStandUpAction(pUserItem);
				CheckMatchStopOut(pITableFrame);
			}
			else
			{
				LPCTSTR lpStr=TEXT("�����ѽ��������Ժ�ϵͳ������...");
				m_pMatchSink->SendGameMessage(pUserItem,lpStr, SMT_CHAT|SMT_TABLE_ROLL);
				m_FreeUserArray.Add(pUserItem);
			}
		}
	}
	if (ChackRoundTableFinish())
	{
		m_enMatchStatus=MS_START_NEXT_ROUND;
		SetMatchGameTimer(IDI_EnterNextRound,5000,1,0);
		return ;
	}
	//
	if(m_enMatchStatus!=MS_STOP_OUT)
	{
		SetMatchGameTimer(IDI_TabelRequestUser,5000,1,0);
		//PostMatchTimer(IDI_CONTINUE_GAME,5,(WPARAM)pITableFrame);
		//PostMatchTimer(IDI_EXIT_TABLE,5,(WPARAM)pITableFrame);
	}
}

//������ʾ
VOID CChampionshipMatch::SendWaitTip(ITableFrame *pTableFrame)
{
	//״̬У��
	if(m_enMatchStatus==MS_NULL_STATUS || m_enMatchStatus==MS_SIGNUPING )
		return;

	//��������
	WORD wPlaying=0;
	for(WORD i=0;i<m_MatchTableArray.GetCount();i++)
	{
		if(m_MatchTableArray[i]->pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY)
			wPlaying++;
	}

	//��������
	CMD_GR_Match_Wait_Tip WaitTip;
	ZeroMemory(&WaitTip,sizeof(WaitTip));
	WaitTip.wPlayingTable=wPlaying;
	WaitTip.wUserCount=(WORD)m_OnMatchUserMap.GetCount();
	lstrcpyn(WaitTip.szMatchName, m_pMatchRule.szMatchName,LEN_SERVER);

	if(m_enMatchStatus==MS_STOP_OUT || m_enMatchStatus==MS_SECOND_ROUND || m_enMatchStatus==MS_START_NEXT_ROUND)
	{
		for(WORD i=0;i<m_wChairCount;i++)
		{
			IServerUserItem *pTableUserItem=pTableFrame->GetTableUserItem(i);
			if(pTableUserItem==NULL) continue;
			WaitTip.wCurTableRank=GetUserRank(pTableUserItem,pTableFrame);
			if(WaitTip.wCurTableRank==INVALID_WORD) continue;

			
			WaitTip.wRank=GetUserRank(pTableUserItem);
			//if(m_enMatchStatus==MS_SECOND_ROUND && WaitTip.wCurTableRank==m_wChairCount) continue;
			WaitTip.lScore=pTableUserItem->GetUserScore();

			m_pMatchSink->SendData(pTableUserItem,MDM_GF_FRAME,SUB_GR_MATCH_WAIT_TIP,&WaitTip,sizeof(WaitTip));
		}
	}

	//���±����������ı�����Ϣ
	for(WORD i=0;i<m_MatchTableArray.GetCount();i++)
	{
		if(pTableFrame->GetTableID()==m_MatchTableArray[i]->pTableFrame->GetTableID())continue;
		if(m_MatchTableArray[i]->pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY) continue; 
		for(WORD j=0;j<m_wChairCount;j++)
		{
			IServerUserItem *pTableUserItem=m_MatchTableArray[i]->pTableFrame->GetTableUserItem(j);
			if(pTableUserItem==NULL) continue;
			WaitTip.wCurTableRank=GetUserRank(pTableUserItem,m_MatchTableArray[i]->pTableFrame);
			if(WaitTip.wCurTableRank==INVALID_WORD) continue;

			WaitTip.wRank=GetUserRank(pTableUserItem);
			//if(m_enMatchStatus==MS_SECOND_ROUND && WaitTip.wCurTableRank==m_wChairCount) continue;
			WaitTip.lScore=pTableUserItem->GetUserScore();

			m_pMatchSink->SendData(pTableUserItem,MDM_GF_FRAME,SUB_GR_MATCH_WAIT_TIP,&WaitTip,sizeof(WaitTip));
		}
	}
	//for(WORD i=0;i<m_wTableCount;i++)
	//{
	//	if(pTableFrame->GetTableID()==m_pAssignTable[i].pTableFrame->GetTableID())continue;
	//	if(m_pAssignTable[i].pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY) continue; 
	//	if(m_pAssignTable[i].cbOnSeatUserCount==0) continue;

	//	for(WORD j=0;j<m_wChairCount;j++)
	//	{
	//		IServerUserItem *pTableUserItem=m_pAssignTable[i].pTableFrame->GetTableUserItem(j);
	//		if(pTableUserItem==NULL) continue;
	//		WaitTip.wCurTableRank=GetUserRank(pTableUserItem,m_pAssignTable[i].pTableFrame);
	//		if(WaitTip.wCurTableRank==INVALID_WORD) continue;

	//		WaitTip.wRank=GetUserRank(pTableUserItem)+1;
	//		if(m_enMatchStatus==MS_SECOND_ROUND && WaitTip.wCurTableRank==m_wChairCount) continue;
	//		WaitTip.lScore=pTableUserItem->GetUserScore();

	//		m_pMatchSink->SendData(pTableUserItem,MDM_GF_FRAME,SUB_GR_MATCH_WAIT_TIP,&WaitTip,sizeof(WaitTip));
	//	}
	//}
}

//������ʾ
VOID CChampionshipMatch::SendWaitTipMJ(ITableFrame *pTableFrame)
{
	//״̬У��
	if(m_enMatchStatus==MS_NULL_STATUS || m_enMatchStatus==MS_SIGNUPING || m_enMatchStatus==MS_FIRST_ROUND || m_enMatchStatus==MS_START_NEXT_ROUND)
		return;

	//��������
	WORD wPlaying=0;
	for(WORD i=0;i<m_MatchTableArray.GetCount();i++)
	{
		if(m_MatchTableArray[i]->pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY)
			wPlaying++;
	}

	//��������
	CMD_GR_Match_Wait_Tip WaitTip;
	ZeroMemory(&WaitTip,sizeof(WaitTip));
	WaitTip.wPlayingTable=wPlaying;
	WaitTip.wUserCount=(WORD)m_OnMatchUserMap.GetCount();
	lstrcpyn(WaitTip.szMatchName, m_pMatchRule.szMatchName,LEN_SERVER);

	if(m_enMatchStatus==MS_STOP_OUT || m_enMatchStatus==MS_SECOND_ROUND)
	{
		for(WORD i=0;i<m_wChairCount;i++)
		{
			IServerUserItem *pTableUserItem=pTableFrame->GetTableUserItem(i);
			if(pTableUserItem==NULL) continue;
			WaitTip.wCurTableRank=GetUserRankMJ(pTableUserItem,pTableFrame);
			if(WaitTip.wCurTableRank==INVALID_WORD) continue;
			WaitTip.wRank=GetUserRankMJ(pTableUserItem);
			WaitTip.lScore=pTableUserItem->GetUserScore();
			m_pMatchSink->SendData(pTableUserItem,MDM_GF_FRAME,SUB_GR_MATCH_WAIT_TIP,&WaitTip,sizeof(WaitTip));
		}
	}
}

//���������������û�
void CChampionshipMatch::TabelRequestUser(bool bPrecedeSit)
{
	//�����û����� ����������Ҫ��ֹ����
	if (CheckMatchUser())
	{
		return;
	}
	CTableFrameMananerArray			FreeTableArray;				//�ձ�������
	//���Ȼ�ȡ������
	for (INT_PTR i=0;i<m_MatchTableArray.GetCount();i++)
	{
		if(m_MatchTableArray[i]->pTableFrame->IsGameStarted()==false&&m_MatchTableArray[i]->bRoundTableFinish==false)
		{
			FreeTableArray.Add(m_MatchTableArray[i]);
		}
	}
	DWORD dwCurTime=(DWORD)time(NULL);

	bool bIsContinueCheck=true;//�Ż���������
	for (INT_PTR i=0;i<FreeTableArray.GetCount();i++)
	{
		DWORD nCurUserCount=(DWORD)m_FreeUserArray.GetCount();
		//�ҵ�һ������ �����û�������ڵ�������
		if(nCurUserCount/m_wChairCount>0)
		{
			AllUserExitTable(FreeTableArray[i]->pTableFrame);
			for (int j=0;j<m_wChairCount;j++)
			{
				//�û��������
				WORD randmer=INVALID_VALUE;
				if(bPrecedeSit&&bIsContinueCheck)
					randmer=GetPrecedeSitUser(dwCurTime);
				if(randmer==INVALID_VALUE)
				{
					bIsContinueCheck=false;//�Ѿ�û�������û���
					randmer=rand()%(WORD)m_FreeUserArray.GetCount();
				}
				
				//������������ �ø�������ȫ������
				if(m_FreeUserArray[randmer]->GetTableID()!=INVALID_TABLE)
				{
					for(int m=0;m<m_MatchTableArray.GetCount();m++)
					{
						if(m_FreeUserArray[randmer]->GetTableID()==m_MatchTableArray[m]->wTableID)
						{
							m_MatchTableArray[m]->pTableFrame->PerformStandUpAction(m_FreeUserArray[randmer]);
							//AllUserExitTable(m_MatchTableArray[m]->pTableFrame);
							break;
						}
					}
				}
				if (FreeTableArray[i]->pTableFrame->PerformSitDownAction(j,m_FreeUserArray[randmer]))
				{
					m_FreeUserArray.RemoveAt(randmer);
				}else
				{
					CTraceService::TraceString(TEXT("����ʧ�ܣ�"),TraceLevel_Exception);
				}
			}
			//30�����һ����� ���ָ�����û�п�ʼ��Ϸ ˵���Ѿ�������
			PostMatchTimer(IDI_CheakTabelStart,30,(WPARAM)FreeTableArray[i]->pTableFrame);
		}
	}

	return ;
}

//����һ����Ϣ
void CChampionshipMatch::SendGroupBox(LPCTSTR pStrMessage)
{
	//�������ݰ�
	CMD_CM_SystemMessage Message;
	Message.wType=SMT_EJECT|SMT_CHAT;
	lstrcpyn(Message.szString,pStrMessage,CountArray(Message.szString));
	Message.wLength=CountStringBuffer(Message.szString);
	//��������
	WORD wSendSize=sizeof(Message)-sizeof(Message.szString)+Message.wLength*sizeof(TCHAR);
	m_pMatchSink->SendGroupData(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&Message,wSendSize, this);
}

//���ý����û�����̭�û� ÿ����1������
void CChampionshipMatch::SetEnterUserOROutUser(ITableFrame *pITableFrame)
{
	//�����������̭�û�
	CMatchUserItemArray UserItemArray;
	UserItemArray.RemoveAll();
	for (int i=0;i<m_wChairCount;i++)
	{
		IServerUserItem* UserItem=pITableFrame->GetTableUserItem(i);
		if(UserItem)
		{
			UserItemArray.Add(UserItem);
		}
	}
	if (UserItemArray.GetCount()>0)
	{
		SCORE dwMaxScore=UserItemArray.GetAt(0)->GetUserScore();
		//�������һ���û�������ֵ
		for (int i=1;i<UserItemArray.GetCount();i++)
		{
			if(UserItemArray.GetAt(i)->GetUserScore()>dwMaxScore)dwMaxScore=UserItemArray.GetAt(i)->GetUserScore();
		}
		for (int i=0;i<UserItemArray.GetCount();i++)
		{
			//С������ֵ ����ת��Ԥ��̭���� �Ƿ�ֱ��ת�ƿ�������ֱ�ӽ���
			if(UserItemArray.GetAt(i)->GetUserScore()<dwMaxScore)
			{
				if(m_ReOutUserArray.GetCount()==0)m_ReOutUserArray.Add(UserItemArray.GetAt(i));
				else
				{
					//׼������̭�� �����ʱ�� ���ź�
					BYTE ReOutUserCount=(BYTE)m_ReOutUserArray.GetCount();
					for (int j=0;j<ReOutUserCount;j++)
					{
						if(m_ReOutUserArray[j]->GetUserScore()<UserItemArray.GetAt(i)->GetUserScore())
						{
							m_ReOutUserArray.InsertAt(j,UserItemArray.GetAt(i));
							break;
						}
						if(j==ReOutUserCount-1)m_ReOutUserArray.Add(UserItemArray.GetAt(i));
					}
				}
			}
			else 
				m_FreeUserArray.Add(UserItemArray.GetAt(i));
		}

	}
}

//���ý����û�����̭�û� �������������
void CChampionshipMatch::SetEnterUserOROutUser()
{
	DWORD dwUserID=0L;
	IServerUserItem *pIServerUserItem=NULL;
	POSITION pos=m_OnMatchUserMap.GetStartPosition();

	WORD wChairID=0;
	while (pos!=NULL)
	{
		//��ȡ�û�
		m_OnMatchUserMap.GetNextAssoc(pos,dwUserID,pIServerUserItem);
		//Ч����
		ASSERT(pIServerUserItem!=NULL);
		m_FreeUserArray.Add(pIServerUserItem);
	}
	//DWORD dwUserID=0;
	//IServerUserItem *pUserItem=NULL;
	//tagMatchScore *pScore = new tagMatchScore[m_OnMatchUserMap.GetCount()];
	//ZeroMemory(pScore,sizeof(tagMatchScore)*m_OnMatchUserMap.GetCount());
	//WORD wCount=SortMapUser(pScore);

	//
	//for (WORD i=0;i<wCount;i++)
	//{
	//	
	//}
	//SafeDeleteArray(pScore);
}

//���ý����û� 2���齫����
void CChampionshipMatch::SetEnterUserMJ()
{
	INT_PTR nUserCount=m_OnMatchUserMap.GetCount();
	tagMatchRanking *pScore=new tagMatchRanking[nUserCount];
	ZeroMemory(pScore,sizeof(tagMatchRanking)*nUserCount);

	SortUserRanking(pScore,false);
	CString str;
	for (INT_PTR i=0;i<nUserCount;i++)
	{
		if(i>=(INT_PTR)m_pMatchRule.arrSecRoundUserCount[m_wCurRountSec])
		{
			str.Format(TEXT("��Ǹ����δ�ܳɹ��������ڱ��α���������õ�%d��"), i+1);
			//�����û� ɾ�����������ѱ���̭���û� ����ʵ�ֻ������
			IServerUserItem * pIServerUserExist=NULL;
			m_OnMatchUserMap.Lookup(pScore[i].dwUserID,pIServerUserExist);
			for (int j=0;j<m_FreeUserArray.GetCount();j++)
			{
				if(m_FreeUserArray[j]==pIServerUserExist)
				{
					m_FreeUserArray.RemoveAt(j);
					break;
				}
			}
			m_OnMatchUserMap.RemoveKey(pScore[i].dwUserID);

			m_pMatchSink->WriteUserAward(pScore[i].pUserItem, (WORD)i+1, m_dwMatchNO, str,this);
			m_pMatchSink->SendData(pScore[i].pUserItem,MDM_GF_FRAME,SUB_GR_MATCH_WAIT_TIP,NULL,0);
		}
		else
		{
			str=TEXT("��ϲ�������ɹ���������ʼ��");
			m_pMatchSink->SendGameMessage(pScore[i].pUserItem, str, SMT_CHAT|SMT_TABLE_ROLL);
		}
	}
	SafeDeleteArray(pScore);
}

//���ý����û� ����������
void CChampionshipMatch::SetEnterUserLand()
{
	INT_PTR nUserCount=m_OnMatchUserMap.GetCount();
	tagMatchRanking *pScore=new tagMatchRanking[nUserCount];
	ZeroMemory(pScore,sizeof(tagMatchRanking)*nUserCount);


	if(m_pMatchRule.cbSecScoreRule==0)		//����ÿ����1������
	{
		//�����û��������� ��Ȩ
		for (int i=0;i<m_FreeUserArray.GetCount();i++)
		{
			m_FreeUserArray[i]->GetUserInfo()->lGrade=1;
		}
		SortUserRanking(pScore,false);
		//ȡ����Ȩ
		for (int i=0;i<m_FreeUserArray.GetCount();i++)
		{
			m_FreeUserArray[i]->GetUserInfo()->lGrade=0;
		}
	}else if(m_pMatchRule.cbSecScoreRule==1)//����������
	{
		SortUserRanking(pScore,true);
	}


	CString str;
	for (INT_PTR i=0;i<nUserCount;i++)
	{
		if(i>=(INT_PTR)m_pMatchRule.arrSecRoundUserCount[m_wCurRountSec])
		{
			str.Format(TEXT("��Ǹ����δ�ܳɹ��������ڱ��α���������õ�%d��"), i+1);
			//�����û� ɾ�����������ѱ���̭���û� ����ʵ�ֻ������
			IServerUserItem * pIServerUserExist=NULL;
			m_OnMatchUserMap.Lookup(pScore[i].dwUserID,pIServerUserExist);
			for (int j=0;j<m_FreeUserArray.GetCount();j++)
			{
				if(m_FreeUserArray[j]->GetUserID()==pIServerUserExist->GetUserID())
				{
					m_FreeUserArray.RemoveAt(j);
					break;
				}
			}
			m_OnMatchUserMap.RemoveKey(pScore[i].dwUserID);

			m_pMatchSink->WriteUserAward(pScore[i].pUserItem, (WORD)i+1, m_dwMatchNO, str,this);
			m_pMatchSink->SendData(pScore[i].pUserItem,MDM_GF_FRAME,SUB_GR_MATCH_WAIT_TIP,NULL,0);
		}
		else
		{
			str=TEXT("��ϲ�������ɹ���������ʼ��");
			m_pMatchSink->SendGameMessage(pScore[i].pUserItem, str, SMT_CHAT|SMT_TABLE_ROLL);
		}
	}
	SafeDeleteArray(pScore);
}

//����������ɱ��
void CChampionshipMatch::SetRoundTableFinish(ITableFrame *pITableFrame)
{
	ITableFrameEx * pITableFrameEx=GetTableInterface(pITableFrame);
	ASSERT(pITableFrameEx);
	if (pITableFrameEx)pITableFrameEx->bRoundTableFinish=true;
}

//����������ɱ��
bool CChampionshipMatch::ChackRoundTableFinish()
{
	for (int i=0;i<m_MatchTableArray.GetCount();i++)
	{
		if(m_MatchTableArray[i]->bRoundTableFinish==false)return false;
	}
	return true;
}

//����������ɱ��
void CChampionshipMatch::SetSwitchTableFinish(ITableFrame *pITableFrame)
{
	ITableFrameEx * pITableFrameEx=GetTableInterface(pITableFrame);
	ASSERT(pITableFrameEx);
	if (pITableFrameEx)pITableFrameEx->bSwtichTableFinish=true;
}

//����������ɱ��
bool CChampionshipMatch::CheckSwitchTableFinish()
{
	for (int i=0;i<m_MatchTableArray.GetCount();i++)
	{
		if(m_MatchTableArray[i]->bSwtichTableFinish==false)return false;
	}
	return true;
}

//���������û��뿪
void CChampionshipMatch::AllUserExitTable(ITableFrame *pITableFrame)
{
	if(pITableFrame)
	{
		//�����û�ȫ���뿪
		for (int i=0;i<m_wChairCount;i++)
		{
			IServerUserItem* pUserItem=pITableFrame->GetTableUserItem(i);
			if(pUserItem&&pUserItem->GetTableID()!=INVALID_TABLE)pITableFrame->PerformStandUpAction(pUserItem);
		}
	}
}

//����������Ϸ
void CChampionshipMatch::ContinueGame(ITableFrame *pITableFrame)
{
	if (CheckMatchUser())
	{
		return;
	}
	if(m_FreeUserArray.GetCount()<m_wChairCount)
	{
		SetSwitchTableFinish(pITableFrame);
		SetRoundTableFinish(pITableFrame);
		for (int i=0;i<m_FreeUserArray.GetCount();i++)
		{
			LPCTSTR lpStr=TEXT("������Ϸǰ����鵽�������㣬�����ѽ��������Ժ�ϵͳ������...");
			m_pMatchSink->SendGameMessage(m_FreeUserArray[i],lpStr, SMT_CHAT|SMT_TABLE_ROLL);
		}
		//AllUserExitTable(pITableFrame);
		if (ChackRoundTableFinish())
		{
			m_enMatchStatus=MS_START_NEXT_ROUND;
			SetMatchGameTimer(IDI_EnterNextRound,5000,1,0);
			return ;
		}
		return;
	}
	ITableFrameEx * pITableFrameEx=GetTableInterface(pITableFrame);
	ASSERT(pITableFrameEx);
	if (pITableFrameEx)
	{
		AllUserExitTable(pITableFrame);
		for(int i=0;i<m_wChairCount;i++)
		{
			IServerUserItem* pUserItem=m_FreeUserArray[0];//pITableFrameEx->m_FrontUserArray[i];
			//����ȱ�� �����һ�� �ڿ���ʱ���뿪������
			if(pUserItem==NULL)
			{
				CTraceService::TraceString(TEXT("�յ��û������ڵȴ��б��С�"),TraceLevel_Exception);
				SetSwitchTableFinish(pITableFrame);
				SetRoundTableFinish(pITableFrame);
				//AllUserExitTable(pITableFrame);
				m_FreeUserArray.RemoveAt(0);
				if (ChackRoundTableFinish())
				{
					m_enMatchStatus=MS_START_NEXT_ROUND;
					SetMatchGameTimer(IDI_EnterNextRound,5000,1,0);
					return ;
				}
				return;
			}
			//for (int j=0;j<m_FreeUserArray.GetCount();j++)
			//{
			//	if(m_FreeUserArray[j]==pUserItem)
			//	{
			//		m_FreeUserArray.RemoveAt(j);
			//		break;
			//	}
			//}
			//���û��Ѿ��������� �ϴ�û���뿪
			//if(pUserItem->GetTableID()!=INVALID_TABLE)
			//{
			//	pUserItem->SetUserStatus(US_READY,pITableFrame->GetTableID(),i);
			//}else
			//{
			//	pITableFrame->PerformSitDownAction(i,pUserItem);
			//}
			//������������ �ø�������ȫ������
			BOOL bIsReady=false;
			if(pUserItem->GetTableID()!=INVALID_TABLE)
			{
				for(int m=0;m<m_MatchTableArray.GetCount();m++)
				{
					if(pUserItem->GetTableID()==m_MatchTableArray[m]->wTableID)
					{
						m_MatchTableArray[m]->pTableFrame->PerformStandUpAction(pUserItem);
						//if(pITableFrame->GetTableID()==pUserItem->GetTableID())
						//{
						//	if(pUserItem->SetUserStatus(US_READY,pITableFrame->GetTableID(),i))
						//	{
						//		bIsReady=true;
						//	}else
						//	{
						//		CTraceService::TraceString(TEXT("׼������"),TraceLevel_Exception);
						//	}
						//}
						//else
						//{
						//	AllUserExitTable(m_MatchTableArray[m]->pTableFrame);
						//}
						break;
					}
				}
			}

			if(bIsReady==false)
			{
				if (pITableFrame->PerformSitDownAction(i,pUserItem))
				{
					m_FreeUserArray.RemoveAt(0);
				}else
				{
					CTraceService::TraceString(TEXT("����ʧ�ܣ�1"),TraceLevel_Exception);
				}
			}
		}
		////�Ƴ�ǰ����
		//for (int j=0;j<m_wChairCount;j++)
		//{
		//	m_FreeUserArray.RemoveAt(0);
		//}
		//30�����һ����� ���ָ�����û�п�ʼ��Ϸ ˵���Ѿ�������
		PostMatchTimer(IDI_CheakTabelStart,30,(WPARAM)pITableFrame);
	}
}

//��������Ƿ�ʼ��Ϸ
void CChampionshipMatch::CheakTableStart(ITableFrame *pITableFrame)
{
	if(pITableFrame&&pITableFrame->IsGameStarted()==false)
	{
		//pITableFrame->DismissGame();

		//�����û�ȫ���뿪
		for (int i=0;i<m_wChairCount;i++)
		{
			IServerUserItem* pUserItem=pITableFrame->GetTableUserItem(i);
			if(pUserItem&&pUserItem->GetTableID()!=INVALID_TABLE)
			{
				if(pUserItem->IsClientReady())	//�Ѿ������ͻ��˵�
				{
					m_pMatchSink->SendGameMessage(pUserItem, TEXT("���ڱ�����ِ����ȴ��ٴ�Ϊ��������"),SMT_CHAT|SMT_TABLE_ROLL);
					pITableFrame->PerformStandUpAction(pUserItem);
					m_FreeUserArray.Add(pUserItem);
				}else							//û�������ͻ��˵�
				{
					//ɾ���û�
					m_OnMatchUserMap.RemoveKey(pUserItem->GetUserID());

					if (pUserItem->IsAndroidUser())
					{
						ASSERT(m_wAndroidUserCount>0);
						if (m_wAndroidUserCount>0) m_wAndroidUserCount--;
					}
					//����������һЩ�û�
					for (INT_PTR i=0;i<m_FreeUserArray.GetCount();i++)
					{
						if(m_FreeUserArray[i]->GetUserID()==pUserItem->GetUserID())
						{
							m_FreeUserArray.RemoveAt(i);
							break;
						}
					}
					for (INT_PTR i=0;i<m_ReOutUserArray.GetCount();i++)
					{
						if(m_ReOutUserArray[i]->GetUserID()==pUserItem->GetUserID())
						{
							m_ReOutUserArray.RemoveAt(i);
							break;
						}
					}

					m_pMatchSink->WriteUserAward(pUserItem,-1,m_dwMatchNO,TEXT("�������쳣���ߣ�Ϊ�˱�֤�����������У�ϵͳ����������"),this);
				}
			}
		}
		if(CheckMatchUser())return ;

		PostMatchTimer(IDI_CONTINUE_GAME,5,(WPARAM)pITableFrame);
	}
}

//������һ��
void CChampionshipMatch::ContinueRound()
{
	BYTE FreeUserCount=(BYTE)m_FreeUserArray.GetCount();
	//�������ֵ�3�����
	if(FreeUserCount>=m_pMatchRule.arrSecRoundUserCount[m_wCurRountSec])
	{
		SelectPromotionUser();
	}else if (FreeUserCount<m_pMatchRule.arrSecRoundUserCount[m_wCurRountSec])
	{
		//���������ͽ���
		if((m_ReOutUserArray.GetCount()+FreeUserCount)<m_pMatchRule.arrSecRoundUserCount[m_wCurRountSec])
		{
			SendGroupBox(TEXT("��Ǹ�����ڲ����û�ǿ���˳��������ó�������ǰ��������ӭ�����������μӱ�����"));
			LastMatchResult();
			return ;
		}
		for (int i=0;i<m_pMatchRule.arrSecRoundUserCount[m_wCurRountSec]-FreeUserCount;i++)
		{
			m_FreeUserArray.Add(m_ReOutUserArray[0]);
			//����һ���Ƴ�
			m_ReOutUserArray.RemoveAt(0);
		}

		//�����һ�ֿ��� 
		SelectPromotionUser();
	}
	m_FreeUserArray.RemoveAll();
	m_ReOutUserArray.RemoveAll();
}

//�趨����ר�ö�ʱ��
void CChampionshipMatch::SetMatchGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter)
{
	if(m_pMatchSink)
		m_pMatchSink->SetGameTimer(dwTimerID+10*m_LoopTimer,dwElapse,dwRepeat,dwBindParameter,this);
}

//ɱ������ר�ö�ʱ��
void CChampionshipMatch::KillMatchGameTimer(DWORD dwTimerID)
{
	if(m_pMatchSink)
		m_pMatchSink->KillGameTimer(dwTimerID+10*m_LoopTimer,this);
}

//������
void CChampionshipMatch::LastMatchResult()
{
	WORD wCount=0;
	DWORD dwUserID=0;
	IServerUserItem *pUserItem=NULL;
	tagMatchRanking *pScore = new tagMatchRanking[m_OnMatchUserMap.GetCount()];
	ZeroMemory(pScore,sizeof(tagMatchRanking)*m_OnMatchUserMap.GetCount());
	if(m_pMatchRule.dwInitalScore==0)wCount=SortUserRanking(pScore,false);
	else wCount=SortUserRanking(pScore,true);
	for (WORD i=0;i<wCount;i++)
	{
		m_pMatchSink->WriteUserAward(pScore[i].pUserItem, i+1,m_dwMatchNO,NULL,this);
	}
	SafeDeleteArray(pScore);

	MatchOver();
}

//����������
bool CChampionshipMatch::CheckMatchUser()
{
	//�����û����� ����������Ҫ��ֹ����
	DWORD nCurUserCount=(DWORD)m_OnMatchUserMap.GetCount();
	if (nCurUserCount<m_wChairCount)
	{
		//�������ݰ�
		CString szString;
		szString.Format(TEXT("��Ǹ�����ڱ����û�ǿ���˳��������������� %d �ˣ���ǰ���㡣��ӭ�����������μӱ�����"),m_wChairCount);
		SendGroupBox(szString);
		LastMatchResult();
		return true;
	}
	return false;
}

//���ֹͣ��̭
void CChampionshipMatch::CheckMatchStopOut(ITableFrame *pITableFrame)
{
	INT_PTR nCount=m_OnMatchUserMap.GetCount();
	if(nCount<=m_pMatchRule.wRemainUserCount && m_enMatchStatus!=MS_STOP_OUT)
	{
		//������û��ʼ�����Ӵ�����
		KillMatchGameTimer(IDI_TabelRequestUser);
		m_enMatchStatus=MS_STOP_OUT;
		if(nCount<(INT_PTR)m_pMatchRule.dwSecRoundUserCount)
		{
			m_pMatchSink->SendGroupUserMessage(TEXT("������������������������������ٴα����μӱ���"),this);
			LastMatchResult();
			return ;
		}
		TCHAR szMsg[256]=TEXT("");
		_sntprintf(szMsg,CountArray(szMsg),TEXT("�Ѿ��ﵽ��̭������%d��󼴽���������������ȷ��������ң�"),m_pMatchRule.wWaitTime);
		SetMatchGameTimer(IDI_AllDismissGame,m_pMatchRule.wWaitTime*1000,1,NULL);


		for (int m=0;m<m_MatchTableArray.GetCount();m++)
		{
			if (m_MatchTableArray[m]->pTableFrame->IsGameStarted()==false)
				m_MatchTableArray[m]->bRoundTableFinish=true;
		}
		SetRoundTableFinish(pITableFrame);

		SendWaitTip(pITableFrame);

		POSITION pos=m_OnMatchUserMap.GetStartPosition();
		IServerUserItem *pUser=NULL;
		DWORD dwUserID;
		while(pos!=NULL)
		{
			m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUser);
			m_pMatchSink->SendGameMessage(pUser,szMsg,SMT_CHAT|SMT_TABLE_ROLL);
		}
	}
}

//�����ِ
void CChampionshipMatch::CheckExitMatch()
{
	//�����;���� �ȵ��������� �ڵ�������Ϸ�������� �ֻᱻ���뵽 ������
	for (int i=0;i<m_FreeUserArray.GetCount();i++)
	{
		//�����û�
		IServerUserItem * pIServerUserExist=NULL;
		m_OnMatchUserMap.Lookup(m_FreeUserArray.GetAt(i)->GetUserID(),pIServerUserExist);

		//�û��ж�
		if(pIServerUserExist==NULL)
		{
			m_FreeUserArray.RemoveAt(i);
		}
	}
	for (int i=0;i<m_ReOutUserArray.GetCount();i++)
	{
		//�����û�
		IServerUserItem * pIServerUserExist=NULL;
		m_OnMatchUserMap.Lookup(m_ReOutUserArray.GetAt(i)->GetUserID(),pIServerUserExist);

		//�û��ж�
		if(pIServerUserExist==NULL)
		{
			m_ReOutUserArray.RemoveAt(i);
		}
	}
}

//��ȡ���������û�
WORD CChampionshipMatch::GetPrecedeSitUser(DWORD dwCurTime)
{
	for(int i=0;i<m_FreeUserArray.GetCount();i++)
	{
		tagTimeInfo* pTimerInfo=m_FreeUserArray[i]->GetTimeInfo();
		if(dwCurTime-pTimerInfo->dwEndGameTimer>m_pMatchRule.wPrecedeTimer)return i;
	}
	return INVALID_VALUE;
}

//�������ר�ö�ʱ��
void CChampionshipMatch::CaptureMatchTimer()
{
	for(int i=0;i<m_MatchTimerArray.GetCount();i++)
	{
		if(--m_MatchTimerArray[i]->iElapse<=0)
		{
			if(m_MatchTimerArray[i]->dwTimerID==IDI_CONTINUE_GAME)
			{
				//�����û�������Ϸ
				ContinueGame((ITableFrame*)m_MatchTimerArray[i]->wParam);
			}
			else if(m_MatchTimerArray[i]->dwTimerID==IDI_EXIT_TABLE)
			{
				//�뿪����
				AllUserExitTable((ITableFrame*)m_MatchTimerArray[i]->wParam);
			}else if(m_MatchTimerArray[i]->dwTimerID==IDI_CheakTabelStart)
			{
				//��������Ƿ�ʼ��Ϸ
				CheakTableStart((ITableFrame*)m_MatchTimerArray[i]->wParam);
			}
			//ɾ����ʱ��
			if(KillMatchTimer(i))i--;
			else ASSERT(false);
		}
	}
}

//Ͷ�ݱ���ר�ö�ʱ��
void CChampionshipMatch::PostMatchTimer(DWORD dwTimerID, int iElapse, WPARAM wParam, LPARAM lParam)
{
	tagMatchTimer* pMatchTimer=new tagMatchTimer;
	pMatchTimer->dwTimerID=dwTimerID;
	pMatchTimer->iElapse=iElapse;
	//pMatchTimer->dwRepeat=dwRepeat;
	pMatchTimer->wParam=wParam;
	pMatchTimer->lParam=lParam;
	m_MatchTimerArray.Add(pMatchTimer);
}

//ɱ������ר�ö�ʱ��
bool CChampionshipMatch::KillMatchTimer(DWORD dwTimerID, WPARAM wParam)
{
	for(int i=0;i<m_MatchTimerArray.GetCount();i++)
	{
		if(m_MatchTimerArray[i]->dwTimerID==dwTimerID&&m_MatchTimerArray[i]->wParam==wParam)
		{
			tagMatchTimer* pMatchTimer=m_MatchTimerArray[i];
			m_MatchTimerArray.RemoveAt(i);
			SafeDelete(pMatchTimer);
			return true;
		}
	}
	return false;
}

//ɱ������ר�ö�ʱ��
bool CChampionshipMatch::KillMatchTimer(INT_PTR dwIndexID)
{
	ASSERT(dwIndexID >= 0 && dwIndexID < m_MatchTimerArray.GetCount());
	if( dwIndexID >= 0 && dwIndexID < m_MatchTimerArray.GetCount() )
	{
		tagMatchTimer* pMatchTimer=m_MatchTimerArray[dwIndexID];
		m_MatchTimerArray.RemoveAt(dwIndexID);
		SafeDelete(pMatchTimer);
		return true;
	}
	return false;
}

//ȫ��ɱ������ר�ö�ʱ��
void CChampionshipMatch::AllKillMatchTimer()
{
	for(int i=0;i<m_MatchTimerArray.GetCount();/*i++*/)
	{
		tagMatchTimer* pMatchTimer=m_MatchTimerArray[i];
		m_MatchTimerArray.RemoveAt(i);
		SafeDelete(pMatchTimer);
	}
}