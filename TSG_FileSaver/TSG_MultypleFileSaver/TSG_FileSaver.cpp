#include "../Api/TSG_FileSaver.h"

TSG_FileSaver::TSG_FileSaver()
{
	//禁止线程执行超时，否则可能会导致图片在存放过程中丢失
	this->m_pThreadPool.setExpiryTimeout(-1);
	this->m_pThreadPool.setMaxThreadCount(10);	//最多十个线程
}

TSG_FileSaver::~TSG_FileSaver()
{
	this->m_pThreadPool.waitForDone();	//需要等待线程池全部执行完毕才会析构
}

void TSG_FileSaver::StartSaver()
{
	this->SaverState = true;
}

void TSG_FileSaver::StopSaver()
{
	this->SaverState = false;
}

void TSG_FileSaver::AppendFile(FileSaverRunnable* run)
{
	if (run == nullptr) {
		this->CallError(this->str_Class_name, QString("AppendFile"), QString("FileSaverRunnable is nullptr"));
		return;
	}

	if (!this->SaverState) {
		return;
	}
	this->m_pThreadPool.start(run);
	qDebug() << "线程池中线程数：" << this->m_pThreadPool.activeThreadCount();
}
