
#include "SerialPort.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

int main(int argc, TCHAR* argv[])
{
    SERIAL serial;

    int dRet = serial.InitPort(5);
    if (dRet == false)
    {
        std::cout << "initPort fail !" << std::endl;
    }
    else
    {
        std::cout << "initPort success !" << std::endl;
    }
    dRet = serial.StartListening();
    if (dRet == false)
    {
        std::cout << "start listening fail !" << std::endl;
    }
    else
    {
        std::cout << "start listening success !" << std::endl;
    }

    while (1)
    {

        char szCmd[16];
        int cmdlen = 0;
        /*cin.getline(szCmd, 16);
        serial.WriteData((unsigned char*)szCmd, strlen(szCmd));*/

        clock_t clk = clock();

        while (serial.GetBufferLength() == 0)
        {
            /*if (clock() - clk > 10000)
            {
                printf("time out!\n");
                break;
            }*/
        }
        printf("RecvMsg: %s\n", serial.GetBuffer().c_str());

        Sleep(1000);
    }
    return 0;
}
