#pragma once
#ifndef TSG_FRAMEWORK
#define TSG_FRAMEWORK
//filename:TSG_Framework.h
//author:Leventure
//time:20323.9.1
//info:此文件用于声明基础框架，其中包含很多抽象类声明和枚举类型声明，用于规范接口设计
#include "qstring.h"
#include "qvariant.h"
#include "qjsonobject.h"
#include "qjsondocument.h"
#include "qjsonarray.h"
#include "qmetaobject.h"
#include "qdebug.h"
#pragma region Lev_Json
/// <summary>
/// name:Lev_Json
/// 说明：此类用作辅助参数类与json字符串之间的转换，使用此类请使用Q_PROPERTY声明所有的类成员变量
/// </summary>
class Lev_Json : QObject {

public:
	template<class T1>
	static bool ValidateJsonKeys(const QString& jsonString, const T1* T_Class) {
		QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
		if (!jsonDoc.isObject()) {
			return false;
		}

		QJsonObject jsonObject = jsonDoc.object();
		const QMetaObject* metaObject = T_Class->metaObject();

		for (int i = 0; i < metaObject->propertyCount(); ++i) {
			QMetaProperty property = metaObject->property(i);
			QString propName = property.name();
			if (propName.contains("objectName"))
				continue;
			if (!jsonObject.contains(propName)) {
				return false;
			}
		}

		return true;
	}
	/// <summary>
	/// 判断这个Json字符串对于这个Object而言是否合法
	/// </summary>
	/// <typeparam name="T1"></typeparam>
	/// <param name="jsonString"></param>
	/// <returns></returns>
	template<class T1>
	static bool ValidateJsonKeys(const QString& jsonString, QSharedPointer<T1> T_Class_1) {
		QObject* T_Class = dynamic_cast<QObject*>(T_Class_1.data());
		QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
		if (!jsonDoc.isObject()) {
			return false; // Return false if JSON is not an object
		}

		QJsonObject jsonObject = jsonDoc.object();
		const QMetaObject* metaObject = T_Class->metaObject();

		for (int i = 0; i < metaObject->propertyCount(); ++i) {
			QMetaProperty property = metaObject->property(i);
			QString propName = property.name();

			if (!jsonObject.contains(propName)) {
				return false;
			}
		}

		return true;
	}
	/// <summary>
	/// 推荐，序列化Qt对象，请用Q_PROPERTY包裹成员变量，使用内存安全的QSharedPointer
	/// </summary>
	/// <typeparam name="T1">模板对象，可以不声明，会自动识别</typeparam>
	/// <param name="T_Class_1">输入的对象</param>
	/// <returns></returns>
	template<class T1>
	static QString JsonSerialization(QSharedPointer<T1> T_Class_1) {
		QJsonObject ret;
		QObject* T_Class = dynamic_cast<QObject*>(T_Class_1.data());
		const QMetaObject* metaObject = T_Class->metaObject();

		for (int i = 0; i < metaObject->propertyCount(); ++i) {
			QMetaProperty property_ = metaObject->property(i);
			QVariant propValue = property_.read(T_Class);

			if (!QString(property_.name()).contains("objectName")) {
				ret.insert(property_.name(), variantToJsonValue(propValue));
			}
		}

		QJsonDocument jsonDoc(ret);
		return jsonDoc.toJson(QJsonDocument::Compact);
	}
	/// <summary>
	/// 推荐，反序列化Qt对象，请用Q_PROPERTY包裹成员变量，会返回一个内存安全的QSharedPointer
	/// </summary>
	/// <typeparam name="T1"></typeparam>
	/// <param name="jsonString"></param>
	/// <returns></returns>
	template<class T1>
	static QSharedPointer<T1> JsonDeserialization(const QString& jsonString) {
		QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
		if (!jsonDoc.isObject()) {
			return QSharedPointer<T1>();
		}

		QJsonObject jsonObject = jsonDoc.object();
		QSharedPointer<T1> result = QSharedPointer<T1>::create();

		const QMetaObject* metaObject = result->metaObject();
		for (int i = 0; i < metaObject->propertyCount(); ++i) {
			QMetaProperty property = metaObject->property(i);
			QString propName = property.name();

			if (jsonObject.contains(propName)) {
				QJsonValue propJsonValue = jsonObject[propName];
				QVariant propValue = jsonValueToVariant(propJsonValue, property.userType());

				if (propValue.isValid()) {
					property.write(result.data(), propValue);
				}
			}
		}

		return result;
	}

	/// <summary>
	/// 可以用，序列化Qt对象，请用Q_PROPERTY包裹成员变量
	/// </summary>
	/// <typeparam name="T1">模板对象，可以不声明，会自动识别</typeparam>
	/// <param name="T_Class_1">输入的对象</param>
	/// <returns></returns>
	template<class T1>
	static QString JsonSerialization(const T1* T_Class) {
		QJsonObject ret;

		const QMetaObject* metaObject = T_Class->metaObject();

		for (int i = 0; i < metaObject->propertyCount(); ++i) {
			QMetaProperty property_ = metaObject->property(i);
			QVariant propValue = property_.read(T_Class);

			if (!QString(property_.name()).contains("objectName")) {
				ret.insert(property_.name(), variantToJsonValue(propValue));
			}
		}

		QJsonDocument jsonDoc(ret);
		return jsonDoc.toJson(QJsonDocument::Compact);
	}
	/// <summary>
	/// 不推荐使用，不安全的内存方案
	/// </summary>
	/// <typeparam name="T1"></typeparam>
	/// <param name="result"></param>
	/// <param name="jsonString"></param>
	/// <returns></returns>
	template<class T1>
	static QSharedPointer<T1> JsonDeserialization(T1* result, const QString& jsonString) {
		QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
		if (!jsonDoc.isObject()) {
			return QSharedPointer<T1>();
		}

		QJsonObject jsonObject = jsonDoc.object();
		const QMetaObject* metaObject = result->metaObject();
		for (int i = 0; i < metaObject->propertyCount(); ++i) {
			QMetaProperty property = metaObject->property(i);
			QString propName = property.name();

			if (jsonObject.contains(propName)) {
				QJsonValue propJsonValue = jsonObject[propName];
				QVariant propValue = jsonValueToVariant(propJsonValue, property.userType());

				if (propValue.isValid()) {
					property.write(result.data(), propValue);
				}
			}
		}

		return result;
	}

private:
	static QJsonValue variantToJsonValue(const QVariant& variant) {
		if (variant.canConvert<QString>()) {
			return QJsonValue::fromVariant(variant.toString());
		}
		else if (variant.canConvert<int>()) {
			return QJsonValue::fromVariant(variant.toInt());
		}
		else if (variant.canConvert<double>()) {
			return QJsonValue::fromVariant(variant.toDouble());
		}
		else if (variant.canConvert<bool>()) {
			return QJsonValue::fromVariant(variant.toBool());
		}
		else if (variant.userType() == qMetaTypeId<QList<int>>()) {
			return listToJsonArray<int>(variant.value<QList<int>>());
		}
		else if (variant.userType() == qMetaTypeId<QList<QString>>()) {
			return listToJsonArray<QString>(variant.value<QList<QString>>());
		}
		else if (variant.userType() == qMetaTypeId<QList<bool>>()) {
			return listToJsonArray<bool>(variant.value<QList<bool>>());
		}
		return QJsonValue::Null;
	}

	template<typename T>
	static QJsonArray listToJsonArray(const QList<T>& list) {
		QJsonArray jsonArray;
		for (const T& value : list) {
			jsonArray.append(QJsonValue::fromVariant(value));
		}
		return jsonArray;
	}
	static QVariant jsonValueToVariant(const QJsonValue& jsonValue, int userType) {
		QVariant result;
		if (jsonValue.isString()) {
			result = jsonValue.toString();
		}
		else if (jsonValue.isDouble()) {
			if (userType == QMetaType::Int) {
				result = jsonValue.toInt();
			}
			else if (userType == QMetaType::Double) {
				result = jsonValue.toDouble();
			}
		}
		else if (jsonValue.isBool()) {
			if (userType == QMetaType::Bool) {
				result = jsonValue.toBool();
			}
		}
		else if (jsonValue.isArray()) {
			QJsonArray jsonArray = jsonValue.toArray();
			if (userType == qMetaTypeId<QList<int>>()) {
				QList<int> intList;
				for (const QJsonValue& element : jsonArray) {
					intList.append(element.toInt());
				}
				result = QVariant::fromValue(intList);
			}
			else if (userType == qMetaTypeId<QList<QString>>()) {
				QList<QString> stringList;
				for (const QJsonValue& element : jsonArray) {
					stringList.append(element.toString());
				}
				result = QVariant::fromValue(stringList);
			}
			// Add more cases for other QList types if needed
		}
		return result;
	}
};
#pragma endregion
/// <summary>
/// 枚举对象命名空间
/// </summary>
namespace TSG_Enum_Space {
	Q_NAMESPACE
		/// <summary>
		/// 设备当前运行状态，仅用于展示和检查当前设备运行状态
		/// </summary>
		enum class DeviceState {
		DS_None,
		DS_Unknown,
		DS_Disconnected,
		DS_Connected,
		DS_Working
	}; Q_ENUM_NS(DeviceState)

		/// <summary>
		/// 设备流程，指当前设备运行状态，用于执行流程控制
		/// </summary>
		enum class DeviceProcedure {
		DP_None,
		DP_Init,
		DP_Connect,
		DP_SettingParam,
		DP_SettingMission,
		DP_StartGrapping,
		DP_PauseGrapping,
		DP_StopGrapping
	};
	/// <summary>
	/// 设备启动状态
	/// </summary>
	enum class DeviceOpen {
		DO_Open,
		DO_Close
	}; Q_ENUM_NS(DeviceOpen)

		/// <summary>
		/// 运行状态
		/// </summary>
		enum class WorkState {
		WS_None,
		WS_Init,
		WS_PreStart,
		WS_Start,
		WS_Stop,
		WS_Pause,
		WS_Close
	}; Q_ENUM_NS(WorkState)
}

using namespace TSG_Enum_Space;
/// <summary>
/// 基础通信类，TSG中所有的类成员都应该继承这个类，用于通过回调函数前后传递信息
/// 抽象类，不允许实例化
/// 此为通信框架，后续的类不允许覆写CallMethod和SendCMD方法
/// </summary>
class TSG_Framework
{
public:
	using CallMethod = std::function<void(const QString& sModule, const QString& sDescribe, const QString& sVariable, const QVariant& extra)>;
	using SendCMD = std::function<void(const QString& sModule, const QString& sDescribe, const QString& sVariable, const QVariant& extra)>;

	void RegisterCallMethod(CallMethod callback) {
		callbacks_.append(callback);
	}
	void RegisterSendCMD(SendCMD callback) {
		sendcmds_.append(callback);
	}

	void Signal_CallMethod(const QString& sModule, const QString& sDescribe, const QString& sVariable, const QVariant& extra) {
		for (CallMethod callback : callbacks_) {
			if (callback) {
				callback(sModule, sDescribe, sVariable, extra);
			}
		}
	}
	void Signal_SendCMD(const QString& sModule, const QString& sDescribe, const QString& sVariable, const QVariant& extra) {
		for (SendCMD callback : callbacks_) {
			if (callback) {
				callback(sModule, sDescribe, sVariable, extra);
			}
		}
	}

	virtual void CallDialog(const QString& str_class_name, const QString& FunctionName, const QString& Message) {
		QJsonObject obj;
		obj.insert("ModuleName", str_class_name);
		obj.insert("FunctionName", FunctionName);
		obj.insert("NormalMessage", Message);

		Signal_CallMethod("TSG_Dialog", "WriteNormalMessage", QJsonDocument(obj).toJson(), QVariant());
		Signal_CallMethod("Message", "CallDialog", QJsonDocument(obj).toJson(), QVariant());
	}
	virtual void CallError(const QString& str_class_name, const QString& FunctionName, const QString& Message) {
		QJsonObject obj;
		obj.insert("ModuleName", str_class_name);
		obj.insert("FunctionName", FunctionName);
		obj.insert("ErrorMessage", Message);
		Signal_CallMethod("TSG_Dialog", "WriteErrorMessage", QJsonDocument(obj).toJson(), QVariant());
		Signal_CallMethod("Error", "CallError", QJsonDocument(obj).toJson(), QVariant());
	}

	//template<typename T1>
	//inline void CallDialog(const QString& FuntionName, const T1& Message)
	//{
	//	QMetaEnum statusMetaEnum = QMetaEnum::fromType<T1>();
	//	TSG_Framework::CallDialog(this->str_class_name, FuntionName, enumToString(statusMetaEnum, static_cast<int>(Message)));
	//}

	//template<typename T2>
	//inline void CallError(const QString& FuntionName, const T2& Message)
	//{
	//	QMetaEnum statusMetaEnum = QMetaEnum::fromType<T2>();
	//	TSG_Framework::CallError(this->str_class_name, FuntionName, enumToString(statusMetaEnum, static_cast<int>(Message)));
	//}
	virtual QString enumToString(const QMetaEnum& metaEnum, int value) {
		return QString::fromLatin1(metaEnum.valueToKey(value));
	}

	virtual qint32 slot_ReturnValue(const QString& sModule, const QString& sDescribe, const QString& sVariable, const QVariant& extra) { return -1; };
	virtual qint32 slot_GetCMD(const QString& sModule, const QString& sDescribe, const QString& sVariable, const QVariant& extra) { return -1; };
private:

	QList<CallMethod> callbacks_;
	QList<SendCMD> sendcmds_;
};
//指代设备类，是具体的设备控制层
template<class InitParam, class SettingParam, class MissionParam>
class TSG_Device : public TSG_Framework {
	/// 流程
	virtual bool Init(QSharedPointer< InitParam> initParams) = 0;
	virtual bool SettingDevice(QSharedPointer< SettingParam> params) = 0;
	virtual bool SettingMission(QSharedPointer<MissionParam> params) = 0;
	virtual bool OpenDevice() = 0;		//设备启动
	virtual bool CloseDevice() = 0;		//设备关闭
	virtual bool StartGrapping() = 0;   //开启采集任务
	virtual bool PauseGrapping() = 0;	//暂停采集任务
	virtual bool EndGrapping() = 0;		//终止采集
	virtual bool AnalyseResult() = 0;	//结果处理阶段

	//设备属性
	virtual QString Get_DeviceName() = 0;

	virtual DeviceState Get_DeviceState() = 0;		//获得当前设备运行状态，仅标识
	virtual DeviceProcedure Get_Procedure() = 0;	//获得当前设备运行阶段，流程控制下的
	virtual DeviceOpen Get_DeviceOpen() = 0;		//获得设备是否启动

	virtual QSharedPointer<InitParam> Get_InitParam() = 0;
	virtual QSharedPointer<SettingParam> Get_SettingParam() = 0;	//返回得到当前的设置信息
	virtual QSharedPointer<MissionParam> Get_MissionParam() = 0;
	//通信框架
	qint32 slot_ReturnValue(const QString& sModule, const QString& sDescribe, const QString& sVariable, const QVariant& extra) { return -1; };
	qint32 slot_GetCMD(const QString& sModule, const QString& sDescribe, const QString& sVariable, const QVariant& extra) { return -1; };

private:
	TSG_Enum_Space::DeviceState mdeviceState = DeviceState::DS_None;
	TSG_Enum_Space::DeviceOpen mdeviceOpen = TSG_Enum_Space::DeviceOpen::DO_Close;
	QString mDeviceName;
};


class TSG_Device_Controller : public TSG_Framework {
	//输入当前进程的路径作为根路径，用于初始化控制类，完全是交由自己控制的
	virtual bool Init(const QString& ApplicationPath) = 0;

	virtual QString getDeviceType() = 0;
	virtual bool PreStartMission() = 0;
	virtual bool StartMission() = 0;
	virtual bool PauseMission() = 0;
	virtual bool EndMission() = 0;
	virtual WorkState Get_WorkState() = 0;

	qint32 slot_ReturnValue(const QString& sModule, const QString& sDescribe, const QString& sVariable, const QVariant& extra) { return -1; };
	qint32 slot_GetCMD(const QString& sModule, const QString& sDescribe, const QString& sVariable, const QVariant& extra) { return -1; };

	virtual QList<QString> getDeviceNames() = 0;
};


class TSG_Params :public QObject, public TSG_Framework {

public:
	QMap<QString, QString> getKeys() {
		QMap<QString, QString> ret;
		const QMetaObject* metaObject = this->metaObject();
		int propertyCount = metaObject->propertyCount();
		for (int i = 0; i < propertyCount; ++i)
		{
			QMetaProperty property = metaObject->property(i);
			if (property.isValid() && property.isReadable() && property.isWritable())
			{
				qDebug() << "Property name: " << property.name();
				qDebug() << "Property type: " << property.typeName();
				ret.insert(property.typeName(), property.name());
			}
		}
		return ret;
	}

	QString toJson() {
		return Lev_Json::JsonSerialization(this);
	}
	bool isValid(QString strMessage) {
		return Lev_Json::ValidateJsonKeys(strMessage, this);
	}


};
#endif
