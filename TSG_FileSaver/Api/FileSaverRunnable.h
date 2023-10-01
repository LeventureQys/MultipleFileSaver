#pragma once

#include <QObject>
#include <QRunnable>
#include "FileSaver.h"
#include "tsg_filesaver_global.h"
//将此类注册进入QThreadPool即可
class TSG_FILESAVER_EXPORT FileSaverRunnable : public QRunnable
{

public:
	/// <summary>
	/// 输入参数确定保存对象
	/// </summary>
	/// <param name="filecontain">文件内容</param>
	/// <param name="fileSize">文件大小</param>
	/// <param name="fileName">文件名称</param>
	/// <param name="savePath">保存文件路径</param>
	FileSaverRunnable(uchar* filecontain, qint64 fileSize, const QString& fileName, const QString& savePath);
	~FileSaverRunnable();

	//保存前的行为

	//保存中的行为
	virtual void  SavingFunction() = 0;
	uchar* m_data = nullptr;
	qint64 size = 0;
	QString m_path;
	QString m_name;
	FileSaver* saver = nullptr;
	bool blnSaver = true;	//默认下是继续保存的
protected:

	void run();


};
