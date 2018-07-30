#include "Thread.h"
#define	SAFE_CLOSE_HANDLE(hHandle)		    if ( hHandle != NULL && hHandle != INVALID_HANDLE_VALUE ) { CloseHandle(hHandle); hHandle = NULL; }

CThread::CThread() : m_uThreadId(0), m_bEvent(false)
{
	m_hThread = NULL;
	m_hQuitEvent = NULL;
}

CThread::~CThread()
{
	SAFE_CLOSE_HANDLE(m_hThread);
	SAFE_CLOSE_HANDLE(m_hQuitEvent);
}

//------------------------------------------------------------------------
/**
@brief 쓰레드 생성
*///----------------------------------------------------------------------
bool CThread::CreateThread(bool bEvent/* = false*/)
{
	m_bEvent = bEvent;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, CThread::ThreadProc, this, 0, &m_uThreadId);
	if (m_hThread == NULL)
		return false;

	// 종료 이벤트 사용
	if (m_bEvent)
	{
		m_hQuitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (m_hQuitEvent == NULL)
		{
			SAFE_CLOSE_HANDLE(m_hThread);
			return false;
		}
	}
	return true;
}

//------------------------------------------------------------------------
/**
@brief 쓰레드 종료
*///----------------------------------------------------------------------
void CThread::DestroyThread(unsigned int uMilliseconds/* = INFINITE*/)
{
	if (!IsRunning())
		return;

	if (m_bEvent && m_hQuitEvent)
	{
		WaitForSingleObject(m_hQuitEvent, uMilliseconds);
		SAFE_CLOSE_HANDLE(m_hQuitEvent);
		m_hQuitEvent = NULL;
	}

	SAFE_CLOSE_HANDLE(m_hThread);
	m_hThread = NULL;
	m_uThreadId = 0;
}

void CThread::TerminateDestroyThread()
{
	if (m_bEvent && m_hQuitEvent)
	{
		SAFE_CLOSE_HANDLE(m_hQuitEvent);
	}

	TerminateThread(m_hThread, -1);

	SAFE_CLOSE_HANDLE(m_hThread);
	m_hThread = NULL;
	m_uThreadId = 0;
}

bool CThread::IsRunning()
{
	return (m_hThread && m_uThreadId) ? true : false;
}

HANDLE CThread::GetThreadHandle()
{
	return m_hThread;
}

void CThread::ProcessThread()
{
	OnCreateThread();
	OnProcessThread();
	OnDestroyThread();
}

unsigned _stdcall CThread::ThreadProc(LPVOID lParam)
{
	CThread *pthis = (CThread *)lParam;
	if (pthis == NULL)
		return 1;

	pthis->ProcessThread();

	if (pthis->m_bEvent && pthis->m_hQuitEvent)
		SetEvent(pthis->m_hQuitEvent);

	return 1;
}


