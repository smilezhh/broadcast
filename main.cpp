#include "UI/device/device_manager.h"
#include "UI/device/group_manager.h"
#include "broadcastmain.h"
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
// #include<device_manager.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BroadcastMain *w = BroadcastMain::getBroadcastMain();

    device_manager d;
    d.show();

    return a.exec();
}
