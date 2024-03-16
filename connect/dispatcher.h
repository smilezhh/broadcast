#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <functional>
#include<QJsonObject>
#include <map>
class Dispatcher
{

private:
    typedef std::function<QJsonObject(QJsonObject&)> FuncType;

    std::map<std::string, FuncType> funcMap;



    static inline Dispatcher *dispatch = nullptr;


public:



    void Register(const std::string& funcName, FuncType func) {
        funcMap[funcName] = func;
    }
    static Dispatcher* getDispatcher(){


        if (!dispatch) {
            dispatch = new Dispatcher();
        }
        return dispatch;
    }

    QJsonObject Dispatch(const QString &path, QJsonObject &obj)
    {
        std::string funcName = path.toStdString();

        // 检查函数是否已注册
        if (funcMap.find(funcName) == funcMap.end()) {
            qDebug() << "Function not found: " << path;
            return QJsonObject();
        }

        // 调用函数并返回结果
        FuncType func = funcMap[funcName];
        return func(obj);
    }




};

#endif // DISPATCHER_H
