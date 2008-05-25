/*!
    @file   KeyCode.cpp
    @author dasyprocta
 */

#include "KeyCode.h"
#include <windows.h>
#include <dinput.h>
#include <boost/array.hpp>

BEGIN_NAMESPACE_NYANCO

BEGIN_NO_NAMESPACE

typedef boost::array<uint8, 256> SysCode;

SysCode const& setupDiMap();
SysCode const& setupVkMap();

SysCode const&                          g_diMap = setupDiMap();
SysCode const&                          g_vkMap = setupVkMap();

// ----------------------------------------------------------------------------
SysCode const& setupDiMap()
{
    static SysCode table;
#if 0
    table[DIK_ESCAPE]       = KeyCode::Escape;
    table[DIK_1]            = KeyCode::Num1;
    table[DIK_2]            = KeyCode::Num2;
    table[DIK_3]            = KeyCode::Num3;
    table[DIK_4]            = KeyCode::Num4;
    table[DIK_5]            = KeyCode::Num5;
    table[DIK_6]            = KeyCode::Num6;
    table[DIK_7]            = KeyCode::Num7;
    table[DIK_8]            = KeyCode::Num8;
    table[DIK_9]            = KeyCode::Num9;
    table[DIK_0]            = KeyCode::Num0;
    table[DIK_MINUS]        = KeyCode::Minus;
    table[DIK_EQUALS]       = KeyCode::Equals;
    table[DIK_BACK]         = KeyCode::Back;

    table[DIK_TAB]          = KeyCode::Tab;
    table[DIK_Q]            = KeyCode::CharQ;
    table[DIK_W]            = KeyCode::CharW;
    table[DIK_E]            = KeyCode::CharE;
    table[DIK_R]            = KeyCode::CharR;
    table[DIK_T]            = KeyCode::CharT;
    table[DIK_Y]            = KeyCode::CharY;
    table[DIK_U]            = KeyCode::CharU;
    table[DIK_I]            = KeyCode::CharI;
    table[DIK_O]            = KeyCode::CharO;
    table[DIK_P]            = KeyCode::CharP;
    table[DIK_LBRACKET]     = KeyCode::BracketL;
    table[DIK_RBRACKET]     = KeyCode::BracketR;
    table[DIK_RETURN]       = KeyCode::Return;

    table[DIK_LCONTROL]     = KeyCode::ControlL;
    table[DIK_A]            = KeyCode::CharA;
    table[DIK_S]            = KeyCode::CharS;
    table[DIK_D]            = KeyCode::CharD;
    table[DIK_F]            = KeyCode::CharF;
    table[DIK_G]            = KeyCode::CharG;
    table[DIK_H]            = KeyCode::CharH;
    table[DIK_J]            = KeyCode::CharJ;
    table[DIK_K]            = KeyCode::CharK;
    table[DIK_L]            = KeyCode::CharL;
    table[DIK_SEMICOLON]    = KeyCode::SemiColon;
    table[DIK_APOSTROPHE]   = KeyCode::Apostrophe;
    table[DIK_GRAVE]        = KeyCode::Grave;

    table[DIK_LSHIFT]       = KeyCode::ShiftL;
    table[DIK_BACKSLASH]    = KeyCode::BackSlash;
    table[DIK_Z]            = KeyCode::CharZ;
    table[DIK_X]            = KeyCode::CharX;
    table[DIK_C]            = KeyCode::CharC;
    table[DIK_V]            = KeyCode::CharV;
    table[DIK_B]            = KeyCode::CharB;
    table[DIK_N]            = KeyCode::CharN;
    table[DIK_M]            = KeyCode::CharM;
    table[DIK_COMMA]        = KeyCode::Comma;
    table[DIK_PERIOD]       = KeyCode::Period;
    table[DIK_SLASH]        = KeyCode::Slash;
    table[DIK_RSHIFT]       = KeyCode::ShiftR;

    table[DIK_MULTIPLY]     = KeyCode::Multiply;
    table[DIK_LMENU]        = KeyCode::MenuL;
    table[DIK_SPACE]        = KeyCode::Space;
    table[DIK_CAPITAL]      = KeyCode::Capital;
    table[DIK_F1]           = KeyCode::F1;
    table[DIK_F2]           = KeyCode::F2;
    table[DIK_F3]           = KeyCode::F3;
    table[DIK_F4]           = KeyCode::F4;
    table[DIK_F5]           = KeyCode::F5;
    table[DIK_F6]           = KeyCode::F6;
    table[DIK_F7]           = KeyCode::F7;
    table[DIK_F8]           = KeyCode::F8;
    table[DIK_F9]           = KeyCode::F9;
    table[DIK_F10]          = KeyCode::F10;
    table[DIK_NUMLOCK]      = KeyCode::NumLock;
    table[DIK_SCROLL]       = KeyCode::Scroll;
    table[DIK_NUMPAD7]      = KeyCode::Numpad7;
    table[DIK_NUMPAD8]      = KeyCode::Numpad8;
    table[DIK_NUMPAD9]      = KeyCode::Numpad9;
    table[DIK_SUBTRACT]     = KeyCode::Subtract;
    table[DIK_NUMPAD4]      = KeyCode::Numpad4;
    table[DIK_NUMPAD5]      = KeyCode::Numpad5;
    table[DIK_NUMPAD6]      = KeyCode::Numpad6;
    table[DIK_ADD]          = KeyCode::Add;
    table[DIK_NUMPAD1]      = KeyCode::Numpad1;
    table[DIK_NUMPAD2]      = KeyCode::Numpad2;
    table[DIK_NUMPAD3]      = KeyCode::Numpad3;
    table[DIK_NUMPAD0]      = KeyCode::Numpad0;
    table[DIK_DECIMAL]      = KeyCode::Decimal;
    table[DIK_OEM_102]      = KeyCode::Oem102;
    table[DIK_F11]          = KeyCode::F11;
    table[DIK_F12]          = KeyCode::F12;
    table[DIK_F13]          = KeyCode::F13;
    table[DIK_F14]          = KeyCode::F14;
    table[DIK_F15]          = KeyCode::F15;
    table[DIK_KANA]         = KeyCode::Kana;
    table[DIK_ABNT_C1]      = KeyCode::AbntC1;
    table[DIK_CONVERT]      = KeyCode::Convert;
    table[DIK_NOCONVERT]    = KeyCode::NoConvert;
    table[DIK_YEN]          = KeyCode::Yen;
    table[DIK_ABNT_C2]      = KeyCode::AbntC2;
    table[DIK_NUMPADEQUALS] = KeyCode::NumpadEquals;
    table[DIK_PREVTRACK]    = KeyCode::PrevTrack;
    table[DIK_AT]           = KeyCode::At;
    table[DIK_COLON]        = KeyCode::Colon;
    table[DIK_UNDERLINE]    = KeyCode::UnderLine;
    table[DIK_KANJI]        = KeyCode::Kanji;
    table[DIK_STOP]         = KeyCode::Stop;
    table[DIK_AX]           = KeyCode::Ax;
    table[DIK_UNLABELED]    = KeyCode::Unlabeled;
    table[DIK_NEXTTRACK]    = KeyCode::NextTrack;
    table[DIK_NUMPADENTER]  = KeyCode::NumpadEnter;
    table[DIK_RCONTROL]     = KeyCode::ControlR;
    table[DIK_MUTE]         = KeyCode::Mute;
    table[DIK_CALCULATOR]   = KeyCode::Calculator;
    table[DIK_PLAYPAUSE]    = KeyCode::PlayPause;
    table[DIK_MEDIASTOP]    = KeyCode::MediaStop;
    table[DIK_VOLUMEDOWN]   = KeyCode::VolumeDown;
    table[DIK_VOLUMEUP]     = KeyCode::VolumeUp;
    table[DIK_WEBHOME]      = KeyCode::WebHome;
    table[DIK_NUMPADCOMMA]  = KeyCode::NumpadComma;
    table[DIK_DIVIDE]       = KeyCode::Divide;
    table[DIK_SYSRQ]        = KeyCode::SysRq;
    table[DIK_RMENU]        = KeyCode::MenuR;
    table[DIK_PAUSE]        = KeyCode::Pause;
    table[DIK_HOME]         = KeyCode::Home;
    table[DIK_UP]           = KeyCode::Up;
    table[DIK_PRIOR]        = KeyCode::Prior;
    table[DIK_LEFT]         = KeyCode::Left;
    table[DIK_RIGHT]        = KeyCode::Right;
    table[DIK_END]          = KeyCode::End;
    table[DIK_DOWN]         = KeyCode::Down;
    table[DIK_NEXT]         = KeyCode::Next;
    table[DIK_INSERT]       = KeyCode::Insert;
    table[DIK_DELETE]       = KeyCode::Delete;
    table[DIK_LWIN]         = KeyCode::WinL;
    table[DIK_RWIN]         = KeyCode::WinR;
    table[DIK_APPS]         = KeyCode::Apps;
    table[DIK_POWER]        = KeyCode::Power;
    table[DIK_SLEEP]        = KeyCode::Sleep;
    table[DIK_WAKE]         = KeyCode::Wake;
    table[DIK_WEBSEARCH]    = KeyCode::WebSearch;
    table[DIK_WEBFAVORITES] = KeyCode::WebFavorites;
    table[DIK_WEBREFRESH]   = KeyCode::WebRefresh;
    table[DIK_WEBSTOP]      = KeyCode::WebStop;
    table[DIK_WEBFORWARD]   = KeyCode::WebForward;
    table[DIK_WEBBACK]      = KeyCode::WebBack;
    table[DIK_MYCOMPUTER]   = KeyCode::MyComputer;
    table[DIK_MAIL]         = KeyCode::Mail;
    table[DIK_MEDIASELECT]  = KeyCode::MediaSelect;
#endif
    return table;
}

// ----------------------------------------------------------------------------
SysCode const& setupVkMap()
{
    static SysCode table;

    table[VK_RETURN]        = KeyCode::Return;
    table[VK_TAB]           = KeyCode::Tab;
    table[VK_BACK]          = KeyCode::BackSpace;
    table[VK_SPACE]         = KeyCode::Space;

    table[VK_LEFT]          = KeyCode::Left;
    table[VK_UP]            = KeyCode::Up;
    table[VK_RIGHT]         = KeyCode::Right;
    table[VK_DOWN]          = KeyCode::Down;
    table[VK_DELETE]        = KeyCode::Delete;

    table[VK_LBUTTON]       = KeyCode::MouseButtonLeft;
    table[VK_RBUTTON]       = KeyCode::MouseButtonRight;
    table[VK_MBUTTON]       = KeyCode::MouseButtonMiddle;

    for (char c = '0', i = 0; c <= '9'; ++c, ++i)
        table[c] = KeyCode::Num0 + i;
    for (char c = 'A', i = 0; c <= 'Z'; ++c, ++i)
        table[c] = KeyCode::CharA + i;

    return table;
}

END_NO_NAMESPACE


// ----------------------------------------------------------------------------
KeyCode::Type mapFromDirectInput(uint8 diCode)
{
#if 0
    static BYTE table[256] =
    {
        0, // escape
        KeyCode::Num1,      // 1
        KeyCode::Num2,      // 2
        KeyCode::Num3,      // 3
        KeyCode::Num4,      // 4
        KeyCode::Num5,      // 5
        KeyCode::Num6,      // 6
        KeyCode::Num7,      // 7
        KeyCode::Num8,      // 8
        KeyCode::Num9,      // 9
        KeyCode::Num0,      // 0
        0,// minus
        0,// equals
        0,// backspace
        0,// tab
        KeyCode::CharQ,     // q
        KeyCode::CharW,     // w
        KeyCode::CharE,     // e
        KeyCode::CharR,     // r
        KeyCode::CharT,     // t
        KeyCode::CharY,     // y
        KeyCode::CharU,     // u
        KeyCode::CharI,     // i
        KeyCode::CharO,     // o
        KeyCode::CharP,     // p
        0,// lbracket
        0,// rbracket
        0,// return
        0,// lcontrol
        KeyCode::CharA,     // a
        KeyCode::CharS,     // s
        KeyCode::CharD,     // d
        KeyCode::CharF,     // f
        KeyCode::CharG,     // g
        KeyCode::CharH,     // h
        KeyCode::CharI,     // i
        KeyCode::CharJ,     // j
        KeyCode::CharK,     // k
        KeyCode::CharL,     // l
        0,// semicolon
        0,// apostrophe
        0,// grave
        0,// lshift
        0,// backslash
        KeyCode::CharZ,     // z
        KeyCode::CharX,     // x
        KeyCode::CharC,     // c
        KeyCode::CharV,     // v
        KeyCode::CharB,     // b
        KeyCode::CharN,     // n
        KeyCode::CharM,     // m
        0,// camma
        0,// period
        0,// slash
        0,// rshift
        0,// multiply
        0,// lmenu
        0,// space
        0,// capital
        KeyCode::F1,
        KeyCode::F2,
        KeyCode::F3,
        KeyCode::F4,
        KeyCode::F5,
        KeyCode::F6,
        KeyCode::F7,
        KeyCode::F8,
        KeyCode::F9,
        KeyCode::F10,
        0,// numlock
        0,// scroll
        0,// numpad7
        0,// numpad8
        0,// numpad9
        0,// subtract
        0,// numpad4
        0,// numpad5
        0,// numpad6
        0,// add
        0,// numpad1
        0,// numpad2
        0,// numpad3
        0,// numpad3
        0,// decimal
        0,// oem 102
        KeyCode::F11,
        KeyCode::F12,
        KeyCode::F13,
        KeyCode::F14,
        KeyCode::F15,
        0,// kana
        0,// abnt c1
        // convert
        // noconvert
        // yen
        // abnt c2
        // numpadequals
        // prevtrack
        // at
        // colon
        // underline
        // kanji
        // stop
        // ax
        // unlabeled
        // nexttrack
        // numpadenter
        // rcontrol
        // mute
        // calculator
        // playpause
        // mediastop
        // volumedown
        // volumeup
        // webhome
        // numpadcomma
        // divide
        // sysrq
        // rmenu
        // pause
        // home
        // up
        // prior
        // left
        // right
        // end
        // down
        // next
        // insert
        // delete
        // lwin
        // rwin
        // apps
        // power
        // sleep
        // wake
        // websearch
        // webfavorites
        // webrefresh
        // webstop
        // webforward
        // webback
        // mycomputer
        // mail
        // mediaselect
    };
#endif
    return static_cast<KeyCode::Type>(g_diMap[diCode]);
}

// ----------------------------------------------------------------------------
KeyCode::Type mapFromVirtualKey(uint8 vkCode)
{
    return static_cast<KeyCode::Type>(g_vkMap[vkCode]);
}

END_NAMESPACE_NYANCO
