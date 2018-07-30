#include "thread.h"

#define	SAFE_DELETE(pMem)					if(pMem) { delete pMem; pMem = NULL; }

class TestThread : public CThread
{
public:
	TestThread();
	~TestThread();
	
	bool Begin();
	void End(unsigned int uMilliseconds);
protected:
	virtual void OnCreateThread() {}
	virtual void OnProcessThread();
	virtual void OnDestroyThread();

private:
	bool m_bLoop;
};
TestThread::TestThread()
{
}
TestThread::~TestThread()
{
}
void TestThread::OnProcessThread()
{
	int count = 0;
	while (m_bLoop)
	{
		printf("OnProcessThread() - Do it %d \n", count++);
		// Return Control.
		Sleep(1);
	}
}				 
void TestThread::OnDestroyThread()
{
	CThread::DestroyThread();
}
bool TestThread::Begin()
{
	printf("TestThread::Begin()\n");
	m_bLoop = true;
	return CThread::CreateThread(true);
}
void TestThread::End(unsigned int uMilliseconds)
{
	m_bLoop = false;
	printf("TestThread::End()\n");
	CThread::DestroyThread(uMilliseconds);
}
int main()
{
	printf("hi thread\n");
	
	TestThread *tt;
	tt = new TestThread();
	tt->Begin();
	getchar();
	tt->End(10);
	
	SAFE_DELETE(tt);
	printf("bye thread");
	return 1;
}


