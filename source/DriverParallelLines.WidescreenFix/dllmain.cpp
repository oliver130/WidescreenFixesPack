#include "stdafx.h"

#define _USE_MATH_DEFINES
#include "math.h"
#define DEGREE_TO_RADIAN(fAngle) ((fAngle)* (float)M_PI / 180.0f)
#define RADIAN_TO_DEGREE(fAngle) ((fAngle)* 180.0f / (float)M_PI)
#define SCREEN_AR_NARROW (4.0f / 3.0f)
#define SCREEN_FOV_HORIZONTAL 90.0f
#define SCREEN_FOV_VERTICAL (2.0f * RADIAN_TO_DEGREE(atan(tan(DEGREE_TO_RADIAN(SCREEN_FOV_HORIZONTAL * 0.5f)) / SCREEN_AR_NARROW)))

HWND hWnd;

struct Screen
{
    int32_t DesktopResW;
    int32_t DesktopResH;
    int32_t Width;
    int32_t Height;
    float fWidth;
    float fHeight;
    float fFieldOfView;
    float fDynamicScreenFieldOfViewScale;
    float fAspectRatio;
    float fHudOffset;
    int32_t Width43;
    float fWidth43;
} Screen;

enum ScreenModes
{
    LETTERBOX = 0,
    UNKNOWN = 1,
    WIDE16BY9 = 2
};

void __fastcall sub_5F3627(uintptr_t _this, uint32_t edx, uintptr_t a2, uintptr_t a3)
{
    //DWORD* unk_933E1C = (DWORD*)0x933E1C;
    //uintptr_t v3 = unk_933E1C[(*(uintptr_t*)(_this + 4)) + 257];
    size_t j = 0;
    //float w = *(float *)(v3 + 0x3F4);
    // float h = *(float *)(v3 + 0x3F8);
    float v4 = 1.0f / Screen.fWidth;
    float v5 = 1.0f / Screen.fHeight;
    uintptr_t v6 = *(uintptr_t*)(a2 + 8);
    if (*(uint32_t*)a2)
    {
        do
        {
            *(float*)(a3 + 00) = *(float*)(v6 + 16);
            *(float*)(a3 + 04) = *(float*)(v6 + 20) + *(float *)(v6 + 28);
            *(float*)(a3 + 16) = *(float*)(v6 + 32);
            *(float*)(a3 + 20) = *(float*)(v6 + 44);
            *(float*)(a3 + 24) = *(float*)(v6 + 16);
            *(float*)(a3 + 28) = *(float*)(v6 + 20);
            *(float*)(a3 + 40) = *(float*)(v6 + 32);
            *(float*)(a3 + 44) = *(float*)(v6 + 36);
            *(float*)(a3 + 48) = *(float*)(v6 + 24) + *(float *)(v6 + 16);
            *(float*)(a3 + 52) = *(float*)(v6 + 20) + *(float *)(v6 + 28);
            *(float*)(a3 + 64) = *(float*)(v6 + 40);
            *(float*)(a3 + 68) = *(float*)(v6 + 44);
            *(float*)(a3 + 72) = *(float*)(v6 + 24) + *(float *)(v6 + 16);
            *(float*)(a3 + 76) = *(float*)(v6 + 20);
            *(float*)(a3 + 88) = *(float*)(v6 + 40);
            *(float*)(a3 + 92) = *(float*)(v6 + 36);

            uintptr_t v8 = a3 + 4;

            struct flt
            {
                float f1;
                float f2;
            };

            flt floats[4];

            for (size_t i = 0; i < 4; i++)
            {
                floats[i].f1 = *(float*)(v8 - 4);
                floats[i].f2 = *(float*)(v8 - 0);
                *(float*)(v8 - 4) = (((*(float*)(v8 - 4) * 2.0f) - 1.0f) - v4);
                *(float*)(v8 + 0) = ((1.0f - (*(float*)v8 * 2.0f)) - (0.0f - v5));
                uint32_t v9 = (uint32_t)(*(float*)(v6 + 8) * 255.0f);
                uint32_t v10 = (uint32_t)(*(float*)(v6 + 4) * 255.0f) & 0xFF | ((((uint32_t)(*(float*)(v6 + 12) * 255.0f) << 8) | (uint32_t)(*(float*)v6 * 255.0) & 0xFF) << 8);
                *(uint32_t*)(v8 + 8) = (uint8_t)v9 | (v10 << 8);
                *(uint32_t*)(v8 + 4) = 0;
                v8 += 24;
            }

            uintptr_t v8a = a3 + 4;
            for (size_t i = 0; i < 4; i++)
            {
                if ((floats[0].f1 == 0.0f && floats[0].f2 == 1.0f) && (floats[1].f1 == 0.0f && floats[1].f2 == 0.0f) &&
                    (floats[2].f1 == 1.0f && floats[2].f2 == 1.0f) && (floats[3].f1 == 1.0f && floats[3].f2 == 0.0f))
                {
                    //to-do: fmv fix / radar
                    //*(float *)(v8a - 4) -= 0.5f;
                }
                else
                {
                    *(float *)(v8a - 4) /= (Screen.fAspectRatio / (16.0f / 9.0f));
                }
                v8a += 24;
            }

            ++j;
            a3 += 96;
            v6 += 48;
        } while (j < *(uint32_t*)a2);
    }
}

DWORD WINAPI Init(LPVOID bDelay)
{
    auto pattern = hook::pattern("55 8B EC 83 EC 60 53 56 57");

    if (pattern.empty() && !bDelay)
    {
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&Init, (LPVOID)true, 0, NULL);
        return 0;
    }

    if (bDelay)
        while (pattern.clear().count_hint(1).empty()) { Sleep(0); };


    HMONITOR monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO info;
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);
    Screen.DesktopResW = info.rcMonitor.right - info.rcMonitor.left;
    Screen.DesktopResH = info.rcMonitor.bottom - info.rcMonitor.top;
 
    //uncapping resolutions
    pattern = hook::pattern("68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? FF 75 F8 BF"); //4C5A89
    injector::WriteMemory(pattern.get_first<int32_t*>(1 +  0), INT_MAX, true);
    injector::WriteMemory(pattern.get_first<int32_t*>(1 +  5), INT_MAX, true);
    injector::WriteMemory(pattern.get_first<int32_t*>(1 + 10), 0, true);
    injector::WriteMemory(pattern.get_first<int32_t*>(1 + 15), 0, true);

    //default to desktop res
    pattern = hook::pattern("C7 46 4C ? ? ? ? C7 46 50 ? ? ? ? C7 46 54 ? ? ? ? C7"); //4C5077
    injector::WriteMemory(pattern.get_first<int32_t*>(3 + 0), Screen.DesktopResW, true);
    injector::WriteMemory(pattern.get_first<int32_t*>(3 + 7), Screen.DesktopResH, true);
    
    static auto pGameHwnd = *hook::get_pattern<HWND*>("A1 ? ? ? ? 85 C0 8D 7E 14 89", 1); //0x933E18
    static auto pAspectRatio = *hook::get_pattern<float*>("D9 05 ? ? ? ? 53 8B 5D 08 8B 03 57 51", 2); //64B238
    static auto pFOV = *hook::get_pattern<float*>("D9 05 ? ? ? ? 8B 03 51 8B CB D9 1C 24", 2); //64B234
    static auto dword_71D4B4 = *hook::get_pattern<uintptr_t*>("A1 ? ? ? ? 8B 08 6A 00 50 FF 91 ? ? ? ? 8D 4E 18", 1);
    static auto pGameResWidth = dword_71D4B4 + 2;
    static auto pGameResHeight = dword_71D4B4 + 3;

    pattern = hook::pattern("89 86 6C 01 00 00 8B 45 F4"); //0x5E4C55
    struct SetWindowedResHook
    {
        void operator()(injector::reg_pack& regs)
        {
            *(uint32_t*)(regs.esi + 0x16C) = Screen.DesktopResW;
            *(uint32_t*)(regs.esi + 0x170) = Screen.DesktopResH;
            SetWindowPos(*pGameHwnd, NULL, 0, 0, Screen.DesktopResW, Screen.DesktopResH, SWP_NOACTIVATE | SWP_NOZORDER);
        }
    }; injector::MakeInline<SetWindowedResHook>(pattern.get_first(0), pattern.get_first(15));

    pattern = hook::pattern("C6 01 01 33 C0 C2 0C 00"); //0x5E4A6A
    struct GetResHook
    {
        void operator()(injector::reg_pack& regs)
        {
            Screen.Width = *pGameResWidth;
            Screen.Height = *pGameResHeight;
            Screen.fWidth = static_cast<float>(Screen.Width);
            Screen.fHeight = static_cast<float>(Screen.Height);
            Screen.fAspectRatio = (Screen.fWidth / Screen.fHeight);
            Screen.Width43 = static_cast<uint32_t>(Screen.fHeight * (4.0f / 3.0f));
            Screen.fWidth43 = static_cast<float>(Screen.Width43);
            Screen.fDynamicScreenFieldOfViewScale = 2.0f * RADIAN_TO_DEGREE(atan(tan(DEGREE_TO_RADIAN(SCREEN_FOV_VERTICAL * 0.5f)) * (Screen.fWidth / Screen.fHeight))) * (1.0f / SCREEN_FOV_HORIZONTAL);

            *pAspectRatio = Screen.fAspectRatio;
            *pFOV = Screen.fDynamicScreenFieldOfViewScale * 1.308f;

            static tagRECT REKT;
            REKT.left = (LONG)(((float)Screen.DesktopResW / 2.0f) - (Screen.fWidth / 2.0f));
            REKT.top = (LONG)(((float)Screen.DesktopResH / 2.0f) - (Screen.fHeight / 2.0f));
            REKT.right = (LONG)Screen.Width;
            REKT.bottom = (LONG)Screen.Height;
            SetWindowPos(*pGameHwnd, NULL, REKT.left, REKT.top, REKT.right, REKT.bottom, SWP_NOACTIVATE | SWP_NOZORDER);
            *(uint8_t*)regs.ecx = 1;
            regs.eax = 0;
        }
    }; injector::MakeInline<GetResHook>(pattern.get_first(0));

    pattern = hook::pattern("8B 44 24 04 89 41 10 B0 01 C2 04 00"); //0x57B954
    struct MenuAspectRatioSwitchHook
    {
        void operator()(injector::reg_pack& regs)
        {
            *(uint32_t*)(regs.ecx + 0x10) = WIDE16BY9; //*(uint32_t*)(regs.esp + 0x04);
        }
    }; injector::MakeInline<MenuAspectRatioSwitchHook>(pattern.get_first(0), pattern.get_first(7));

    //2D
    pattern = hook::pattern("E8 ? ? ? ? 8B 45 08 8B 4D 0C 89 45 E8 8B"); //0x5F75AD
    injector::MakeCALL(pattern.get_first(0), sub_5F3627, true);

    //Radar
    pattern = hook::pattern("F3 0F 11 86 44 07 00 00 D9 40 14 51"); //0x4B874A
    struct RadarPosHook
    {
        void operator()(injector::reg_pack& regs)
        {
            *(float*)(regs.esi + 0x744) = ((*(float *)(regs.eax + 0x10) + (1.0f / 640.0f / 2.0f)) / (Screen.fAspectRatio / (16.0f / 9.0f)));
        }
    }; injector::MakeInline<RadarPosHook>(pattern.get_first(0), pattern.get_first(8));


    pattern = hook::pattern("F3 0F 11 86 4C 07 00 00 D9 40 1C D9 1C 24 "); //0x4B877E
    struct RadarScaleHook
    {
        void operator()(injector::reg_pack& regs)
        {
            *(float*)(regs.esi + 0x74C) = ((*(float *)(regs.eax + 0x18) - (1.0f / 640.0f / 2.0f)) / (Screen.fAspectRatio / (16.0f / 9.0f)));
        }
    }; injector::MakeInline<RadarScaleHook>(pattern.get_first(0), pattern.get_first(8));

    return 0;
}


BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        Init(NULL);
    }
    return TRUE;
}