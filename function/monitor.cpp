#include "monitor.h"
#include "connect/dispatcher.h"
#include "lib/NAudioServerLib.h"
#include "qjsonobject.h"

MonitorModule *MonitorModule::getInstance()
{
    static MonitorModule monitorModule;
    return &monitorModule;
}

MonitorModule::MonitorModule(QObject *parent)
    : QObject{parent}
{

}

void MonitorModule::InitMapper()
{
    Dispatcher *dispatcher =  Dispatcher::getDispatcher();

}


/** 函数名称 :设置自动触发参数
  * 函数功能 :设置设备自动触发采集参数，只对采集设备有效
  * 输入参数 :devno:设置参数的设备编号
  *          para:自动触发参数{
  *             unsigned char trig_enable;  //自动触发开关
                unsigned char priority;     //自动采集时音源的优先级
                unsigned short trig_val;    //触发值
  *          }
  * 返 回 值 :status 记录参数设置状态
  * 函数作者 :张昊
  * 完成日期 :2024-3-25
  * 修改记录 :
  */
QJsonObject MonitorModule::setAutoTrigPara(QJsonObject &data){
    int   devNo      =  data.value ("devNo").toInt ();
    unsigned char  trigEnable =  (unsigned char)data.value("trigEnable").toInt();
    unsigned char  priority   =  (unsigned char)data.value("priority").toInt();
    unsigned short trig_val    =  (unsigned short)data.value ("trig_va").toInt ();

    _auto_trig_para trig_para ;
    trig_para.trig_enable = trigEnable;
    trig_para.priority    = priority  ;
    trig_para.trig_val    = trig_val  ;

    m_autotrigpara.insert({devNo,trig_para});
    bool status = !na_set_auto_trig_para (devNo,&trig_para);
    QJsonObject obj;
    obj.insert("response", "reSetAutoTrigPara");
    obj.insert("status", status);
    return obj;
}

/** 函数名称 :获取自动触发参数
  * 函数功能 :获取给设备设置的自动触发参数
  * 输入参数 :devno:设置参数的设备编号
  * 返 回 值 :paraList:{
  *             int devNo;                  //设备编号
  *             unsigned char trig_enable;  //自动触发开关
                unsigned char priority;     //自动采集时音源的优先级
                unsigned short trig_val;    //触发值
  *          }
  * 函数作者 :张昊
  * 完成日期 :2024-3-25
  * 修改记录 :
  */
QJsonObject MonitorModule::getAutoPara(QJsonObject &data){
    int devNo = data.value("devNo").toInt();
    QJsonObject response;

    for(auto &it : m_autotrigpara){
        if(it.first == devNo){
            _auto_trig_para para = it.second;
            QJsonObject paraList;
            paraList.insert("trigEnable", QJsonValue::fromVariant(para.trig_enable));
            paraList.insert("devNo", QJsonValue::fromVariant(it.first));
            paraList.insert("priority", QJsonValue::fromVariant(para.priority));
            paraList.insert("trigVal", QJsonValue::fromVariant(para.trig_val));

            response.insert("response", "reGetAutoPara");
            response.insert("paraList", paraList);
            break;
        }
    }

    return response;
}

/**
 * 函数名称 : setMonitorPara
 * 函数功能 : 设置设备的监听参数
 * 输入参数 : devNo       : 设置参数的设备编号
 *            autoMonotor : 上线自动监听状态
 *            saveData    : 音频数据保存状态
 *            saveDir     : 监听文件保存目录
 *            fileName    : 保存文件名
 *            fileSize    : 保存单个文件的大小
 *
 * 返 回 值 : status      : 设置参数的状态
 * 函数作者 : 张昊
 * 完成日期 : 2024-3-25
 * 修改记录 :
 */
QJsonObject MonitorModule::setMonitorPara(QJsonObject &data){
    int           devNo       = data.value("devNo").toInt();
    unsigned char autoMonotor = data.value("autoMonotor").toInt();
    unsigned char saveData    = data.value("saveData").toInt();
    QString       saveDir     = data.value("dirList").toString();
    QString       fileName    = data.value("fileNameList").toString();
    int           fileSize    = data.value("fileSize").toInt();

    _mon_para para ;
    para.auto_mon             = autoMonotor;
    para.save_audio_data      = saveData;
    para.file_size            = fileSize;
    strncpy(para.save_dir,  saveDir .toStdString().c_str(), sizeof(para.save_dir));
    strncpy(para.file_name, fileName.toStdString().c_str(), sizeof(para.file_name));


    m_monitorpara.insert({devNo, para});

    bool status = !na_set_monitor_para(devNo, &para);

    QJsonObject response;
    response.insert("response", "reSetMonitorPara");
    response.insert("status", status);


    return response;
}

/**
 * 函数名称 : setMonitorPara
 * 函数功能 : 设置设备的监听参数
 * 输入参数 :  devNo       : 设置参数的设备编号
 *            autoMonotor : 上线自动监听状态
 *            saveData    : 音频数据保存状态
 *            saveDir     : 监听文件保存目录
 *            fileName    : 保存文件名
 *            fileSize    : 保存单个文件的大小
 *
 * 返 回 值 : status      : 设置参数的状态
 * 函数作者 : 张昊
 * 完成日期 : 2024-3-25
 * 修改记录 :
 */
QJsonObject MonitorModule::getMonitorPara(QJsonObject &data){
    int devNo = data.value("devNo").toInt();
    QJsonObject response;

    auto it = m_monitorpara.find(devNo);
    if(it != m_monitorpara.end()){
        _mon_para para = it->second;
        QJsonObject paraList;
        paraList.insert("autoMonotor", QJsonValue::fromVariant(para.auto_mon));
        paraList.insert("devNo", QJsonValue::fromVariant(it->first));
        paraList.insert("fileName", QJsonValue::fromVariant(QString::fromStdString(para.file_name)));
        paraList.insert("fileSize", QJsonValue::fromVariant(para.file_size));
        paraList.insert("saveData", QJsonValue::fromVariant(para.save_audio_data));

        response.insert("response", "reGetMonitorPara");
        response.insert("paraList", paraList);
    }

    return response;
}


QJsonObject MonitorModule::startDevMonitor(QJsonObject &data){

}

QJsonObject MonitorModule::stopDevMonitor(QJsonObject &data){

}

QJsonObject MonitorModule::startMonitorDecode(QJsonObject &data){

}

QJsonObject MonitorModule::stopMonitorDecode(QJsonObject &data){

}
