#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "HCNetSDK.h"
#include "PlayM4.h"
#include <QMainWindow>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    LONG lRealPlayHandle;
    LONG lUserID;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
private:
    Ui::MainWindow *ui;
    void static PsDataCallBack(LONG lRealHandle, DWORD dwDataType,BYTE *pPacketBuffer,DWORD nPacketSize, void* pUser);
};

#endif // MAINWINDOW_H
