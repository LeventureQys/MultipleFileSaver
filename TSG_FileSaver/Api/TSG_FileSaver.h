#pragma once
#include <QThreadPool>
#include "TSG_Framework.h"
#include "FileSaverRunnable.h"
#include "FileSaver.h"
#include "tsg_filesaver_global.h"

class TSG_FILESAVER_EXPORT TSG_FileSaver : public TSG_Framework
{
public:
	TSG_FileSaver();
	~TSG_FileSaver();
	void StartSaver();
	void StopSaver();
	void AppendFile(FileSaverRunnable* run);

private:
	const QString str_Class_name = "TSG_FileSaver";
	QThreadPool m_pThreadPool;
	bool SaverState = false;
};

