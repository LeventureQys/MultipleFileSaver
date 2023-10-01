#include "../Api/FileSaverRunnable.h"



FileSaverRunnable::FileSaverRunnable(uchar* filecontain, qint64 fileSize, const QString& fileName, const QString& savePath)
{
	//将数据深拷贝一下，避免数据在外部被释放
	if (!filecontain) {
		// 处理空指针的情况
		// 可以抛出异常或者返回错误状态
		return;
	}
	uchar* copyData;
	copyData = new uchar[fileSize];
	if (copyData) {
		memcpy(copyData, filecontain, fileSize);
		this->m_data = copyData;
		this->size = fileSize;
		this->m_path = savePath;
		this->m_name = fileName;
	}
	else {
		// 处理内存分配失败的情况，可以抛出异常或者返回错误状态
		// 例如：throw std::runtime_error("Memory allocation failed.");
		qDebug() << "failed!";
		return;
	}
	//memcpy(copyData, filecontain, fileSize);

	this->m_data = copyData;
	this->size = fileSize;
	this->m_path = savePath;
	this->m_name = fileName;

}

FileSaverRunnable::~FileSaverRunnable()
{
	if (m_data != nullptr) {
		delete[] m_data;
		m_data = nullptr;
	}
	if (this->saver != nullptr)
	{
		delete this->saver;
		this->saver = nullptr;
	}

}

void FileSaverRunnable::run()
{
	this->SavingFunction();
	if (this->blnSaver) {
		/*this->saver = new FileSaver(m_data, size, m_name, m_path);
		this->saver->Save();*/
		QString filePath = this->m_path + "/" + m_name;
		QFile f(filePath);
		bool isFileOpen = f.open(QIODevice::WriteOnly | QIODevice::Truncate);
		if (!isFileOpen) {
			qWarning() << "[image saver]" << "file not open because: ";
			qWarning() << "[image saver]" << f.errorString();
			return;
		}

		qint64 bytesWriten = f.write((char*)m_data, size);

		f.close();
		qInfo() << "[image saver]" << m_path << "[" << size << "]" << " writen " << bytesWriten << " bytes";
	}

}
