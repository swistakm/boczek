#include "../Bacon.h"
#include "../BaconInternal.h"
#include "Platform.h"
#include "Controller.h"
#include <Windows.h>

static ControllerProvider s_Controllers[MaxControllerCount];

Bacon_ControllerConnectedEventHandler g_ControllerConnectedHandler = nullptr;
Bacon_ControllerButtonEventHandler g_ControllerButtonHandler = nullptr;
Bacon_ControllerAxisEventHandler g_ControllerAxisHandler = nullptr;

void Controller_Init()
{
    for (int i = 0; i < MaxControllerCount; ++i)
        s_Controllers[i] = ControllerProvider_None;

    DirectInputController_Init();
    XInputController_Init();
}

void Controller_Shutdown()
{
    DirectInputController_Shutdown();
    XInputController_Shutdown();
}

void Controller_RegisterDeviceNotifications()
{
    RAWINPUTDEVICE devices[] = {
        { 0x1, 0x4, RIDEV_DEVNOTIFY, g_hWnd }, // Generic Desktop : Joystick
        { 0x1, 0x5, RIDEV_DEVNOTIFY, g_hWnd }, // Generic Desktop : Game Pad
        { 0x1, 0x8, RIDEV_DEVNOTIFY, g_hWnd }, // Generic Desktop : Multi-axis Controller
    };
    RegisterRawInputDevices(devices, BACON_ARRAY_COUNT(devices), sizeof(RAWINPUTDEVICE));
}

void Controller_EnumDevices()
{
    DirectInputController_EnumDevices();
    XInputController_EnumDevices();
}

void Controller_Update()
{
    DirectInputController_Update();
    XInputController_Update();
}

int Controller_GetAvailableIndex(int preferredIndex)
{
    if (s_Controllers[preferredIndex] == ControllerProvider_None)
        return preferredIndex;

    for (int i = 0; i < MaxControllerCount; ++i)
    {
        if (s_Controllers[i] == ControllerProvider_None)
            return i;
    }

    return -1;
}

void Controller_SetProvider(int controller, ControllerProvider provider)
{
    if (s_Controllers[controller] == provider)
        return;

    s_Controllers[controller] = provider;
    if (g_ControllerConnectedHandler)
        g_ControllerConnectedHandler(controller, provider != ControllerProvider_None);
}

int Bacon_SetControllerConnectedEventHandler(Bacon_ControllerConnectedEventHandler handler)
{
    g_ControllerConnectedHandler = handler;
    return Bacon_Error_None;
}

int Bacon_SetControllerButtonEventHandler(Bacon_ControllerButtonEventHandler handler)
{
    g_ControllerButtonHandler = handler;
    return Bacon_Error_None;
}

int Bacon_SetControllerAxisEventHandler(Bacon_ControllerAxisEventHandler handler)
{
    g_ControllerAxisHandler = handler;
    return Bacon_Error_None;
}

int Bacon_GetControllerPropertyInt(int controller, int property, int* outValue)
{
    if (controller < 0 || controller >= MaxControllerCount)
        return Bacon_Error_InvalidHandle;

    switch (s_Controllers[controller])
    {
    case ControllerProvider_DirectInput:
        return DirectInputController_GetControllerPropertyInt(controller, property, outValue);
    case ControllerProvider_XInput:
        return XInputController_GetControllerPropertyInt(controller, property, outValue);
    default:
        return Bacon_Error_InvalidHandle;
    }
    return Bacon_Error_None;
}

int Bacon_GetControllerPropertyString(int controller, int property, char* outBuffer, int* inOutBufferSize)
{
    switch (s_Controllers[controller])
    {
    case ControllerProvider_DirectInput:
        return DirectInputController_GetControllerPropertyString(controller, property, outBuffer, inOutBufferSize);
    case ControllerProvider_XInput:
        return XInputController_GetControllerPropertyString(controller, property, outBuffer, inOutBufferSize);
    default:
        return Bacon_Error_InvalidHandle;
    }
    return Bacon_Error_None;
}

