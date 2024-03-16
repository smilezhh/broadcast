#ifndef USER_MANAGE_H
#define USER_MANAGE_H


#include <QWidget>
#include <QScrollBar>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
namespace Ui {
class User_Manage;
}

class BroadcastMain;
class AddUser;
class User_Manage : public QWidget
{
    Q_OBJECT

public:

    ~User_Manage();

    static User_Manage* getUser_Manage(){
        static User_Manage instance;
        return &instance;
    };

signals:
    void getdata();
private slots:
    void on_modify_clicked();

    void on_delete_2_clicked();

private:
    QTimer* timer;
    AddUser* au;
    void setTable();
    void update_data();//更新表格数据
    QJsonObject getUserList(QJsonObject &obj);
    QJsonObject addUser(QJsonObject &obj);
    QJsonObject setUserStatus(QJsonObject &obj);
    QJsonObject resetUserPw(QJsonObject &obj);
    explicit User_Manage(QWidget *parent = nullptr);
    Ui::User_Manage *ui;
};

#endif // USER_MANAGE_H
