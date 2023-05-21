//  
/// COPYRIGHT NOTICE  
/// Copyright (c) 2009, 华中科技大学 tickTick Group  （版权声明）  
/// All rights reserved.  
///   
/// @file    SerialPort.h    
/// @brief   串口通信类头文件  
///  
/// 本文件完成串口通信类的声明  
///  
/// @version 1.0    
/// @author  卢俊   
/// @E-mail：lujun.hust@gmail.com  
/// @date    2010/03/19  
/// 
/// @修订作者：Bonjir
/// @修订时间：2023/5/13
/// @修订说明：增加了SERIAL类，在原有的CSerialPort基类上增添了多线程和缓冲区功能
/// 
//

#pragma once
#ifndef SERIALPORT_H_  
#define SERIALPORT_H_  
#include <Windows.h>
#include <thread>
#include <string>
#include <mutex>

using std::string;
using std::wstring;
using std::mutex;
using std::thread;

class CSerialPort
{
public:
    CSerialPort(void);
    ~CSerialPort(void);
public:
    bool InitPort(UINT  portNo = 1, UINT  baud = CBR_9600, char  parity = 'N', UINT  databits = 8, UINT  stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR);
    bool InitPort(UINT  portNo, const LPDCB& plDCB);
    bool OpenListenThread();
    bool CloseListenTread();
    bool WriteData(unsigned char* pData, unsigned int length);
    UINT GetBytesInCOM();
    bool ReadChar(char& cRecved);
    bool IsPortOpen();
    void ClosePort();
    wstring buffer;

private:
    bool openPort(UINT  portNo);
    static UINT WINAPI ListenThread(void* pParam);
private:
    HANDLE  m_hComm;
    static bool s_bExit;
    volatile HANDLE    m_hListenThread;
    CRITICAL_SECTION   m_csCommunicationSync;
};

class SERIAL : protected CSerialPort
{
public:
    using CSerialPort::InitPort;
    bool IsPortOpen();
    bool StartListening();
    bool IsListening();
    void ClosePort();

    bool WriteData(unsigned char* pData, unsigned int length);
    UINT GetBufferLength();
    wstring GetBuffer();
    TCHAR GetCharFromBuffer();
    TCHAR PeekCharFromBuffer();
    void ClearBuffer();

private:
    wstring buffer;
    mutex mtx_buffer;
    mutex mtx_serial;
    thread thd_serial;

    static void SerialThreadFunc(SERIAL *self);
};

#endif //SERIALPORT_H_ 
