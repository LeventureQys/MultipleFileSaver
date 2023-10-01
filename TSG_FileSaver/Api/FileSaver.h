#pragma once
#include <qrunnable.h>
#include "qobject.h"
#include "qfile.h"
#include "qdir.h"
#include "qfileinfo.h"
#include "qmutex.h"
#include "qdebug.h"
#include "qthread.h"
//FileSaver是塞到FileSaverRunnable中使用的，不需要删除指针
//此类作为父类，每一个种类的文件都需要使用这个FileSaver去将文件保存

class FileSaver
{
public:

	FileSaver(uchar* filecontain, qint64 fileSize, const QString& fileName, const QString& savePath);
	~FileSaver();
	/// <summary>
	/// 开始任务阶段，在开始任务之前提供一个
	/// </summary>
	void Save();
private:
	uchar* data;
	qint64 size;
	QString path;
	QString fileName;

	QMutex mutex;
};



