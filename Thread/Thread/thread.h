#include <atlcoll.h>
class CThread
{
public:
	CThread();
	virtual ~CThread();

	bool CreateThread(bool bEvent = false);
	void DestroyThread(unsigned int uMilliseconds = INFINITE);
	void TerminateDestroyThread();
	bool IsRunning();
	HANDLE GetThreadHandle();

protected:
	virtual void OnCreateThread() = 0;
	virtual void OnProcessThread() = 0;
	virtual void OnDestroyThread() = 0;

private:
	void ProcessThread();
	static unsigned _stdcall ThreadProc(LPVOID lParam);

private:
	HANDLE m_hThread;
	HANDLE m_hQuitEvent;
	bool   m_bEvent;
	unsigned int m_uThreadId;
};
