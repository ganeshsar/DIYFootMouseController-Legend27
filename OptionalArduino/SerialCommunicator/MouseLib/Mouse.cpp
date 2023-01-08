#include <Windows.h>
#include <WinUser.h>
#include <iostream>

extern "C" {
    bool down[3]{ false,false,false };
    WORD values[3]{ 0x01,0x02,0x04 };

	_declspec(dllexport) int MouseDown(int button) {
        
        INPUT Inputs[1] = { 0 };

        Inputs[0].type = INPUT_MOUSE;
        if (button==0&&GetKeyState(0x01)>=0)
        {
            Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            //std::cout << "DOWN";
        }
        if (button==1 && GetKeyState(0x02) >= 0)
        {

            Inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
        }
        down[button] = true;
        SendInput(1, Inputs, sizeof(INPUT));
        return 1;
	}

    _declspec(dllexport) int MouseUp(int button) {
        INPUT Inputs[1] = { 0 };

        Inputs[0].type = INPUT_MOUSE;
        if (button == 0 && GetKeyState(0x01) < 0)
        {
            Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
            //std::cout << "UP";
        }
        if (button == 1 && GetKeyState(0x02) < 0)
        {
            Inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        }

        SendInput(1, Inputs, sizeof(INPUT));
        return 1;
    }
}