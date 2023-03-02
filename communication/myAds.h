//
// Created by tim on 23-2-23.
//

#ifndef QT_ADS_H
#define QT_ADS_H
typedef int BOOL;

#include <iostream>
#include <string>
#include <vector>
#include "C:\TwinCAT\AdsApi\TcAdsDll\Include\TcAdsDef.h"
#include "C:\TwinCAT\AdsApi\TcAdsDll\Include\TcAdsAPI.h"

using ulong = unsigned long;
/// 设置 PLC 的地址
char DeviceAddr[] = { "MAIN.Machine" };
char ForwardAddr[] = { "MAIN.Machine.EMVehicle.IO.bForward.Force" };
char ForwardLeftAddr[] = { "MAIN.Machine.EMVehicle.IO.bLeft.Force" };
char ForwardRightAddr[] = { "MAIN.Machine.EMVehicle.IO.bRight.Force" };
char BackwardAddr[] = { "MAIN.Machine.EMVehicle.IO.bBackward.Force" };
char BackwardLeftAddr[] = { "MAIN.Machine.EMVehicle.IO.bLeftBack.Force" };
char BackwardRightAddr[] = { "MAIN.Machine.EMVehicle.IO.bRightBack.Force" };

/// 句柄
ulong Device;         //前进句柄
ulong Forward;        //前进句柄
ulong ForwardLeft;    //左前句柄
ulong ForwardRight;   //右前句柄
ulong Backward;       //前进句柄
ulong BackwardLeft;   //前进句柄
ulong BackwardRight;  //前进句柄

class TCAds
{
public:
  TCAds()
  {
    pAddr = &Addr;          //定义端口地址变量
    nPort = AdsPortOpen();  //打开ADS通讯端口
    if (nPort == 0)
    {
      std::cout << "Error: AdsGetLocalAddress" << std::endl;  // 检查端口是否打开
    }

    nErr = AdsGetLocalAddress(pAddr);  //得到本地端口地址
    if (nErr)
    {
      std::cout << "Error: AdsGetLocalAddress: " << nErr << std::endl;  //检查获取地址的操作是否执行成功
    }
    pAddr->port = 851;  //将指针指向851端口，注意在TwinCAT3中通信端口为851而不是801
  };

  /// 测试机器人每个部件的通信情况
  void InitAds()
  {
    AdsNotificationAttrib adsNotificationAttribStateDevice;  //定义一个通知
    //设备状态变换通知事件
    adsNotificationAttribStateDevice.cbLength = 143;  //从PLC获取到的数据Byte长度，传递给回调函数
    adsNotificationAttribStateDevice.nTransMode = ADSTRANS_SERVERONCHA;  //当PLC变量变化后，通知回调函数被调用
    adsNotificationAttribStateDevice.nMaxDelay = 0;  //检测到PLC变量改变后通知回调函数被调用的延时间隔
    adsNotificationAttribStateDevice.nCycleTime = 2000000;  // ADS服务器检查PLC变量的周期

    nErr =
        AdsSyncReadWriteReq(pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(Device), &Device, sizeof(DeviceAddr), DeviceAddr);
    if (nErr)
      std::cout << DeviceAddr << " Create Error: AdsSyncReadWriteReq: " << nErr << std::endl;

    nErr = AdsSyncReadWriteReq(pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(Forward), &Forward, sizeof(ForwardAddr),
                               ForwardAddr);
    if (nErr)
      std::cout << ForwardAddr << " Create Error: AdsSyncReadWriteReq: " << nErr << std::endl;

    nErr = AdsSyncReadWriteReq(pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(ForwardLeft), &ForwardLeft,
                               sizeof(ForwardLeftAddr), ForwardLeftAddr);
    if (nErr)
      std::cout << ForwardLeftAddr << " Create Error: AdsSyncReadWriteReq: " << nErr << std::endl;

    nErr = AdsSyncReadWriteReq(pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(ForwardRight), &ForwardRight,
                               sizeof(ForwardRightAddr), ForwardRightAddr);
    if (nErr)
      std::cout << ForwardRightAddr << " Create Error: AdsSyncReadWriteReq: " << nErr << std::endl;

    nErr = AdsSyncReadWriteReq(pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(Backward), &Backward, sizeof(BackwardAddr),
                               BackwardAddr);
    if (nErr)
      std::cout << BackwardAddr << " Create Error: AdsSyncReadWriteReq: " << nErr << std::endl;

    nErr = AdsSyncReadWriteReq(pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(BackwardLeft), &BackwardLeft,
                               sizeof(BackwardLeftAddr), BackwardLeftAddr);
    if (nErr)
      std::cout << BackwardLeftAddr << " Create Error: AdsSyncReadWriteReq: " << nErr << std::endl;

    nErr = AdsSyncReadWriteReq(pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(BackwardRight), &BackwardRight,
                               sizeof(BackwardRightAddr), BackwardRightAddr);
    if (nErr)
      std::cout << BackwardRightAddr << " Create Error: AdsSyncReadWriteReq: " << nErr << std::endl;
  };

  void CloseAds()
  {
    nErr = AdsSyncWriteReq(pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(Device), &Device);
    if (nErr)
      std::cout << "Delete Ads Error: AdsSyncReadWriteReq: " << nErr;

    nErr = AdsSyncWriteReq(pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(Forward), &Forward);
    if (nErr)
      std::cout << "Delete Ads Error: AdsSyncReadWriteReq: " << nErr;

    nErr = AdsSyncWriteReq(pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(ForwardLeft), &ForwardLeft);
    if (nErr)
      std::cout << "Delete Ads Error: AdsSyncReadWriteReq: " << nErr;

    nErr = AdsSyncWriteReq(pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(ForwardRight), &ForwardRight);
    if (nErr)
      std::cout << "Delete Ads Error: AdsSyncReadWriteReq: " << nErr;

    nErr = AdsSyncWriteReq(pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(Backward), &Backward);
    if (nErr)
      std::cout << "Delete Ads Error: AdsSyncReadWriteReq: " << nErr;

    nErr = AdsSyncWriteReq(pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(BackwardLeft), &BackwardLeft);
    if (nErr)
      std::cout << "Delete Ads Error: AdsSyncReadWriteReq: " << nErr;

    nErr = AdsSyncWriteReq(pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(BackwardRight), &BackwardRight);
    if (nErr)
      std::cout << "Delete Ads Error: AdsSyncReadWriteReq: " << nErr;

    // 关闭ADS通信端口
    nErr = AdsPortClose();
    if (nErr)
      std::cout << "Error: AdsPortClose: " << nErr;
    else
      std::cout << "AdsPortClose Successfully" << nErr;
  };

  void FButtonToggle(bool value)
  {
    nErr = AdsSyncWriteReq(pAddr, ADSIGRP_SYM_VALBYHND, Forward, 0x1, &value);
    if (nErr)
      std::cout << "Ads Error!: " << nErr << std::endl << value;
  };

  void FLButtonToggle(bool value)
  {
    nErr = AdsSyncWriteReq(pAddr, ADSIGRP_SYM_VALBYHND, ForwardLeft, 0x1, &value);
    if (nErr)
      std::cout << "Ads Error!: " << nErr << std::endl << value;
  };

  void FRButtonToggle(bool value)
  {
    nErr = AdsSyncWriteReq(pAddr, ADSIGRP_SYM_VALBYHND, ForwardRight, 0x1, &value);
    if (nErr)
      std::cout << "Ads Error!: " << nErr << std::endl << value;
  };

  void BButtonToggle(bool value)
  {
    nErr = AdsSyncWriteReq(pAddr, ADSIGRP_SYM_VALBYHND, Backward, 0x1, &value);
    if (nErr)
      std::cout << "Ads Error!: " << nErr << std::endl << value;
  };
  void BLButtonToggle(bool value)
  {
    nErr = AdsSyncWriteReq(pAddr, ADSIGRP_SYM_VALBYHND, BackwardLeft, 0x1, &value);
    if (nErr)
      std::cout << "Ads Error!: " << nErr << std::endl << value;
  };
  void BRButtonToggle(bool value)
  {
    nErr = AdsSyncWriteReq(pAddr, ADSIGRP_SYM_VALBYHND, BackwardRight, 0x1, &value);
    if (nErr)
      std::cout << "Ads Error!: " << nErr << std::endl << value;
  };

private:
  /// Ads通信端口
  PAmsAddr pAddr;
  AmsAddr Addr;      //定义AMS地址变量
  long nErr, nPort;  //定义端口变量
};

#endif  // QT_ADS_H
