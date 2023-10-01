#include "../Api/FileSaver.h"

FileSaver::FileSaver(uchar* filecontain, qint64 fileSize, const QString& fileName, const QString& savePath)
{
	this->data = filecontain;
	this->size = fileSize;
	this->path = savePath;
	this->fileName = fileName;
}

FileSaver::~FileSaver()
{

}

void FileSaver::Save()
{
	//// 检查指针和文件大小是否有效
	//if (!this->data || this->size <= 0) {
	//	qDebug() << "Invalid file data or size.";
	//	return;
	//}

	//// 构建完整的文件路径
	//QString filePath = this->path + "/" + fileName;

	//// 创建文件对象并打开文件
	//QFile file(filePath);
	//if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
	//	qDebug() << "Failed to open file for writing: " << filePath;
	//	return;
	//}

	//// 写入文件数据
	//qint64 bytesWritten = file.write(reinterpret_cast<const char*>(this->data), this->size);
	//file.close();

	//if (bytesWritten != this->size) {
	//	qDebug() << "Failed to write the entire file.";
	//	return;
	//}

	//qDebug() << "File saved successfully: " << filePath;
	//return;
	// 构建完整的文件路径

	QString filePath = this->path + "/" + fileName;
	QFile f(filePath);
	bool isFileOpen = f.open(QIODevice::WriteOnly | QIODevice::Truncate);
	if (!isFileOpen) {
		qWarning() << "[image saver]" << "file not open because: ";
		qWarning() << "[image saver]" << f.errorString();
		return;
	}

	qint64 bytesWriten = f.write((char*)data, size);

	f.close();
	qInfo() << "[image saver]" << path << "[" << size << "]" << " writen " << bytesWriten << " bytes";
}

