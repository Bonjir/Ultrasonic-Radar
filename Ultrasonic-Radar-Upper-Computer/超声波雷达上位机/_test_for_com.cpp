
#include "SerialPort.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
int main(int argc, TCHAR* argv[])
{
    CSerialPort mySerialPort;
    if (!mySerialPort.InitPort(5))
    {
        std::cout << "initPort fail !" << std::endl;
    }
    else
    {
        std::cout << "initPort success !" << std::endl;
    }
    if (!mySerialPort.OpenListenThread())
    {
        std::cout << "OpenListenThread fail !" << std::endl;
    }
    else
    {
        std::cout << "OpenListenThread success !" << std::endl;
    }

    while (1)
    {

        char szCmd[100];
        int cmdlen = 0;
        /*cin.getline(szCmd, 16);
        mySerialPort.WriteData((unsigned char*)szCmd, strlen(szCmd));

        clock_t clk = clock();*/

        while (mySerialPort.GetBytesInCOM() == 0)
        {
           /* if (clock() - clk > 10000)
            {
                printf("time out!\n");
                break;
            }*/
        }
        memset(szCmd, 0, sizeof(szCmd));
        while (mySerialPort.GetBytesInCOM())
        {
            mySerialPort.ReadChar(szCmd[cmdlen++]);
        }
        printf("RecvMsg: %s\n", szCmd);
    }
    return 0;
}
