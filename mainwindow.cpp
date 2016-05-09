#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>

void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    char tempbuf[256] = {0};
    switch(dwType)
    {
    case EXCEPTION_RECONNECT:    //预览时重连
        qDebug("----------reconnect--------%d\n", time(NULL));
    break;
    default:
    break;
    }
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    NET_DVR_Init();
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);
}
void MainWindow::PsDataCallBack(LONG lRealHandle, DWORD dwDataType,BYTE *pPacketBuffer,DWORD nPacketSize, void* pUser)
{
    qDebug("PsDataCallBack");
}
void MainWindow::on_pushButton_clicked()
{
    qDebug("button clicked");
    // 注册设备
    LONG lUserID;
    NET_DVR_DEVICEINFO_V30 struDeviceInfo;
    lUserID = NET_DVR_Login_V30("192.168.13.20", 8000, "admin", "1qaz2wsx", &struDeviceInfo);
    if (lUserID < 0)
    {
         qDebug("Login error, %d\n", NET_DVR_GetLastError());
         NET_DVR_Cleanup();
         return;
    }

    //---------------------------------------
    //设置异常消息回调函数
    NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);

    //---------------------------------------
    //启动预览并设置回调数据流
    LONG lRealPlayHandle;
    HWND hWnd = (HWND)ui->frame->winId();     //获取窗口句柄
    NET_DVR_PREVIEWINFO struPlayInfo = {0};
    struPlayInfo.hPlayWnd = hWnd;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
    struPlayInfo.lChannel     = 1;       //预览通道号
    struPlayInfo.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
    struPlayInfo.dwLinkMode   = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP

    lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, /*PsDataCallBack*/NULL, NULL);
    if (lRealPlayHandle < 0)
    {
        DWORD error = NET_DVR_GetLastError();
        qDebug("NET_DVR_RealPlay_V40 error, %d\n" , error);
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
        return;
    }

    qDebug("NET_DVR_RealPlay_V40 ok!");
    //QThread::sleep(10000);
    ////---------------------------------------
    ////关闭预览
    //NET_DVR_StopRealPlay(lRealPlayHandle);
    ////注销用户
    //NET_DVR_Logout(lUserID);
    ////释放SDK资源
    //NET_DVR_Cleanup();

    return;
}
MainWindow::~MainWindow()
{
    delete ui;
}
