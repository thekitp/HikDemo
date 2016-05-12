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
LONG lPort; //全局的播放库port号
struct Movie/*可以指定类型名也可以不指定*/
{
    //成员都是public的
    HWND hWnd;
} movie;
void MainWindow::PsDataCallBack(LONG lRealHandle, DWORD dwDataType,BYTE *pBuffer,DWORD dwBufSize, void* pUser)
{
    HWND hWnd=((Movie*)pUser)->hWnd;
    //qDebug("hWnd(pUser):%d", *(HWND*)pUser);

	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD: //系统头
		if (!PlayM4_GetPort(&lPort))  //获取播放库未使用的通道号
		{
            qDebug("PlayM4_GetPort");
			break;
		}
		//m_iPort = lPort; //第一次回调的是系统头，将获取的播放库port号赋值给全局port，下次回调数据时即使用此port号播放
		if (dwBufSize > 0)
		{
			if (!PlayM4_SetStreamOpenMode(lPort, STREAME_REALTIME))  //设置实时流播放模式
			{
                qDebug("PlayM4_SetStreamOpenMode");
				break;
			}

			if (!PlayM4_OpenStream(lPort, pBuffer, dwBufSize, 1024*1024)) //打开流接口
			{
                qDebug("PlayM4_OpenStream");
				break;
			}
            qDebug("hWnd(pUser):%d", hWnd);
			if (!PlayM4_Play(lPort, hWnd)) //播放开始
			{
                qDebug("PlayM4_Play");
				break;
			}
		}
        break;
	case NET_DVR_STREAMDATA:   //码流数据
		if (dwBufSize > 0 && lPort != -1)
		{
			if (!PlayM4_InputData(lPort, pBuffer, dwBufSize))
			{
                qDebug("PlayM4_InputData");
				break;
			}
        }
    }
}
void MainWindow::on_pushButton_2_clicked()
{
    qDebug("button2 clicked");
}
void MainWindow::on_pushButton_clicked()
{
    qDebug("button clicked");
    // 注册设备
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
    //HWND hWnd = (HWND)ui->frame->winId();     //获取窗口句柄
    movie.hWnd = (HWND)ui->frame->winId();     //获取窗口句柄
    qDebug("hWnd:%d", movie.hWnd);
    NET_DVR_PREVIEWINFO struPlayInfo = {0};
    //struPlayInfo.hPlayWnd = hWnd;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
    struPlayInfo.lChannel     = 1;       //预览通道号
    struPlayInfo.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
    struPlayInfo.dwLinkMode   = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP

    lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, PsDataCallBack, &movie);
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
    // 停止解码
    PlayM4_Stop(lPort);

    //关闭流，回收源数据缓冲
    PlayM4_CloseStream(lPort);

    //释放播放库端口号
    PlayM4_FreePort(lPort);

    NET_DVR_StopRealPlay(lRealPlayHandle);
    //注销用户
    NET_DVR_Logout(lUserID);
    //释放SDK资源
    NET_DVR_Cleanup();
    delete ui;
}
