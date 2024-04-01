#include "userlogin.h"
#include "connect/dispatcher.h"
#include "connect/dispatcher.h"
#include "qjsonobject.h"
#include "qsqlquery.h"
#include "userlogin.h"
userlogin* userlogin::userlogin1 = new userlogin;

userlogin::userlogin(QObject *parent)
    : QObject{parent}
{
    registerMethods();


}

void userlogin::registerMethods(){
    Dispatcher *dispatcher = Dispatcher::getDispatcher();

    dispatcher->Register("mobileLogin",std::bind(&userlogin::login, this,std::placeholders::_1));
    dispatcher->Register("mobileRegist",std::bind(&userlogin::regist, this,std::placeholders::_1));
    dispatcher->Register("mobileModify",std::bind(&userlogin::modify, this,std::placeholders::_1));


}
QJsonObject userlogin::login(QJsonObject jsonObj){

    // 从 jsonObj 中提取 data 对象
    QJsonObject data = jsonObj["data"].toObject();

    // 从 data 对象中提取用户名和密码
    QString username = data["username"].toString();
    QString password = data["password"].toString();
    QString role = QString::number(data["role"].toInt());

    QJsonObject userInfo = getUserInfo(username,password);

    if (!userInfo.isEmpty()) {
        // 如果匹配，返回用户信息
        qDebug()<<"登陆成功";
        return QJsonObject{
            {"success", true},
            {"data", userInfo},
        };
    } else {
        // 如果不匹配，返回错误信息
        return QJsonObject{
            {"code", 401},
            {"message", "用户名或密码错误"},
            {"success", false}  // 添加一个字段标识登录失败
        };
    }
}


QJsonObject userlogin::getUserInfo(const QString& accountNumber, const QString& password) {

    QSqlQuery query;
    query.prepare("SELECT * FROM mobile_user WHERE username = :username ");
    query.bindValue(":username", accountNumber);
    query.exec();

    if (query.next()) {
         QString dbPassword = query.value("password").toString();
         if (dbPassword == password) {
        QJsonObject userInfo;
        userInfo["id"] = query.value("Id").toInt();
        userInfo["username"] = query.value("username").toString();
        userInfo["password"] = query.value("password").toString();
        userInfo["avatar"] = query.value("avatar").toString();
        userInfo["phone"] = query.value("phone").toString();
        userInfo["email"] = query.value("email").toString();
        userInfo["role"] = query.value("userrole").toInt();
        return userInfo;
         }
         else{
             qDebug() << "密码错误";
             return QJsonObject();
         }
    } else {
        qDebug() << "用户名或密码错误";
        return QJsonObject();
    }
}
QJsonObject userlogin::regist(QJsonObject jsonObj){
    QString username = jsonObj["username"].toString();
    QString password = jsonObj["password"].toString();
    QString phone = QString::number(jsonObj["phone"].toInt());
    QString email = jsonObj["email"].toString();

    QSqlQuery query;
    query.prepare("INSERT INTO mobile_user (username,password,phone,email) VALUES (':username',':password',':phone',':email')");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":phone", phone);
    query.bindValue(":email", email);
    query.exec();

    QJsonObject userInfo = getUserInfo(username,password);

    return QJsonObject{
     {"code", 200},
     {"message", "成功"},
     {"data", userInfo}
    };
}

QJsonObject userlogin::modify(QJsonObject jsonObj)
{
    QString id = QString::number(jsonObj["id"].toInt());
    QString username = jsonObj["username"].toString();
    QString password = jsonObj["password"].toString();
    QString avatar = jsonObj["avatar"].toString();
    QString phone = QString::number(jsonObj["phone"].toInt());
    QString email = jsonObj["email"].toString();
    QString role = QString::number(jsonObj["role"].toInt());
    if (!password.isEmpty()){
        QSqlQuery query;
        query.exec("UPDATE mobile_user SET password = '"+password+"' WHERE Id = '"+id+"'");
    }
    if (!username.isEmpty()){
        QSqlQuery query;
        query.exec("UPDATE mobile_user SET username = '"+username+"' WHERE Id = '"+id+"'");
    }
    if (!avatar.isEmpty()){
        QSqlQuery query;
        query.exec("UPDATE mobile_user SET avatar = '"+avatar+"' WHERE Id = '"+id+"'");
    }
    if (!email.isEmpty()){
        QSqlQuery query;
        query.exec("UPDATE mobile_user SET email = '"+email+"' WHERE Id = '"+id+"'");
    }
    if (!role.isEmpty()){
        QSqlQuery query;
        query.exec("UPDATE mobile_user SET userRole = '"+role+"' WHERE Id = '"+id+"'");
    }
    if (!phone.isEmpty()){
        QSqlQuery query;
        query.exec("UPDATE mobile_user SET phone = '"+phone+"' WHERE Id = '"+id+"'");
    }

    QJsonObject userInfo = getUserInfo(username,password);

    return QJsonObject{
     {"code", 200},
     {"message", "成功"},
     {"data", userInfo}
    };
}
