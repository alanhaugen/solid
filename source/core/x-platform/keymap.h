#ifndef KEYMAP
#define KEYMAP

#ifndef __MINGW32__
#include <SDL.h>
struct KeyMap
{
    enum
    {
       NONE,
       A = SDLK_a,
       B = SDLK_b,
       C = SDLK_c,
       D = SDLK_d,
       E = SDLK_e,
       F = SDLK_f,
       G = SDLK_g,
       H = SDLK_h,
       I = SDLK_i,
       J = SDLK_j,
       K = SDLK_k,
       L = SDLK_l,
       M = SDLK_m,
       N = SDLK_n,
       O = SDLK_o,
       P = SDLK_p,
       Q = SDLK_q,
       R = SDLK_r,
       S = SDLK_s,
       T = SDLK_t,
       U = SDLK_u,
       V = SDLK_v,
       W = SDLK_w,
       X = SDLK_x,
       Y = SDLK_y,
       Z = SDLK_z,

       KEY_0 = SDLK_0,
       KEY_1 = SDLK_1,
       KEY_2 = SDLK_2,
       KEY_3 = SDLK_3,
       KEY_4 = SDLK_4,
       KEY_5 = SDLK_5,
       KEY_6 = SDLK_6,
       KEY_7 = SDLK_7,
       KEY_8 = SDLK_8,
       KEY_9 = SDLK_9,

       PAD_0 = SDLK_0,
       PAD_1 = SDLK_1,
       PAD_2 = SDLK_2,
       PAD_3 = SDLK_3,
       PAD_4 = SDLK_4,
       PAD_5 = SDLK_5,
       PAD_6 = SDLK_6,
       PAD_7 = SDLK_7,
       PAD_8 = SDLK_8,
       PAD_9 = SDLK_9,

       F1 = SDLK_F1,
       F2 = SDLK_F2,
       F3 = SDLK_F3,
       F4 = SDLK_F4,
       F5 = SDLK_F5,
       F6 = SDLK_F6,
       F7 = SDLK_F7,
       F8 = SDLK_F8,
       F9 = SDLK_F9,
       F10 = SDLK_F10,
       F11 = SDLK_F11,
       F12 = SDLK_F12,

       ESCAPE = SDLK_ESCAPE,
       TILDE,
       MINUS = SDLK_MINUS,
       EQUALS = SDLK_EQUALS,
       BACKSPACE = SDLK_BACKSPACE,
       TAB = SDLK_TAB,
       OPENBRACE = SDLK_KP_LEFTBRACE,
       CLOSEBRACE = SDLK_KP_RIGHTBRACE,
       ENTER = SDLK_RETURN,
       SEMICOLON = SDLK_SEMICOLON,
       QUOTE = SDLK_QUOTE,
       BACKSLASH = SDLK_BACKSLASH,
       BACKSLASH2, /* DirectInput calls this DIK_OEM_102: "< > | on UK/Germany keyboards" */
       COMMA = SDLK_COMMA,
       FULLSTOP = SDLK_PERIOD,
       SLASH = SDLK_SLASH,
       SPACE = SDLK_SPACE,

       INSERT = SDLK_INSERT,
       DELETE_KEY = SDLK_DELETE,
       HOME = SDLK_HOME,
       END = SDLK_END,
       PGUP = SDLK_PAGEUP,
       PGDN = SDLK_PAGEDOWN,
       LEFT = SDLK_LEFT,
       RIGHT = SDLK_RIGHT,
       UP = SDLK_UP,
       DOWN = SDLK_DOWN,

       PAD_SLASH = SDLK_SLASH,
       PAD_ASTERISK = SDLK_ASTERISK,
       PAD_MINUS = SDLK_MINUS,
       PAD_PLUS = SDLK_PLUS,
       PAD_DELETE = SDLK_DELETE,
       PAD_ENTER = SDLK_KP_ENTER,

       PRINTSCREEN = SDLK_PRINTSCREEN,
       PAUSE = SDLK_PAUSE,

       ABNT_C1,
       YEN,
       KANA,
       CONVERT,
       NOCONVERT,
       AT,
       CIRCUMFLEX,
       COLON2,
       KANJI,

       KEY_PAD_EQUALS,	/* MacOS X */
       KEY_BACKQUOTE,	/* MacOS X */
       KEY_SEMICOLON2,	/* MacOS X */
       KEY_COMMAND,	/* MacOS X */
       KEY_UNKNOWN,

       /////////////////////////////////////////////

       KEY_MODIFIERS,

       LSHIFT = SDLK_LSHIFT,
       RSHIFT = SDLK_RSHIFT,
       LCTRL = SDLK_LCTRL,
       RCTRL = SDLK_RCTRL,
       ALT = SDLK_LALT,
       ALTGR,
       LWIN = SDLK_LGUI,
       RWIN = SDLK_RGUI,
       MENU = SDLK_MENU,
       SCROLLLOCK = SDLK_SCROLLLOCK,
       NUMLOCK = SDLK_NUMLOCKCLEAR,
       CAPSLOCK = SDLK_CAPSLOCK,

       SHIFT = SDLK_LSHIFT,
       CTRL = SDLK_LCTRL,
       COMMAND,
       INALTSEQ,
       ACCENT1,
       ACCENT2,
       ACCENT3,
       ACCENT4
    };
};

#else

// from winuser.h
#define VK_LBUTTON 0x01
#define VK_RBUTTON 0x02
#define VK_CANCEL 0x03
#define VK_MBUTTON 0x04
#define VK_XBUTTON1 0x05
#define VK_XBUTTON2 0x06
#define VK_BACK 0x08
#define VK_TAB 0x09
#define VK_CLEAR 0x0C
#define VK_RETURN 0x0D
#define VK_SHIFT 0x10
#define VK_CONTROL 0x11
#define VK_MENU 0x12
#define VK_PAUSE 0x13
#define VK_CAPITAL 0x14
#define VK_KANA 0x15
#define VK_HANGEUL 0x15
#define VK_HANGUL 0x15
#define VK_JUNJA 0x17
#define VK_FINAL 0x18
#define VK_HANJA 0x19
#define VK_KANJI 0x19
#define VK_ESCAPE 0x1B
#define VK_CONVERT 0x1C
#define VK_NONCONVERT 0x1D
#define VK_ACCEPT 0x1E
#define VK_MODECHANGE 0x1F
#define VK_SPACE 0x20
#define VK_PRIOR 0x21
#define VK_NEXT 0x22
#define VK_END 0x23
#define VK_HOME 0x24
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define VK_SELECT 0x29
#define VK_PRINT 0x2A
#define VK_EXECUTE 0x2B
#define VK_SNAPSHOT 0x2C
#define VK_INSERT 0x2D
#define VK_DELETE 0x2E
#define VK_HELP 0x2F

#define VK_LWIN 0x5B
#define VK_RWIN 0x5C
#define VK_APPS 0x5D
#define VK_SLEEP 0x5F
#define VK_NUMPAD0 0x60
#define VK_NUMPAD1 0x61
#define VK_NUMPAD2 0x62
#define VK_NUMPAD3 0x63
#define VK_NUMPAD4 0x64
#define VK_NUMPAD5 0x65
#define VK_NUMPAD6 0x66
#define VK_NUMPAD7 0x67
#define VK_NUMPAD8 0x68
#define VK_NUMPAD9 0x69
#define VK_MULTIPLY 0x6A
#define VK_ADD 0x6B
#define VK_SEPARATOR 0x6C
#define VK_SUBTRACT 0x6D
#define VK_DECIMAL 0x6E
#define VK_DIVIDE 0x6F
#define VK_F1 0x70
#define VK_F2 0x71
#define VK_F3 0x72
#define VK_F4 0x73
#define VK_F5 0x74
#define VK_F6 0x75
#define VK_F7 0x76
#define VK_F8 0x77
#define VK_F9 0x78
#define VK_F10 0x79
#define VK_F11 0x7A
#define VK_F12 0x7B
#define VK_F13 0x7C
#define VK_F14 0x7D
#define VK_F15 0x7E
#define VK_F16 0x7F
#define VK_F17 0x80
#define VK_F18 0x81
#define VK_F19 0x82
#define VK_F20 0x83
#define VK_F21 0x84
#define VK_F22 0x85
#define VK_F23 0x86
#define VK_F24 0x87
#define VK_NUMLOCK 0x90
#define VK_SCROLL 0x91
#define VK_OEM_NEC_EQUAL 0x92
#define VK_OEM_FJ_JISHO 0x92
#define VK_OEM_FJ_MASSHOU 0x93
#define VK_OEM_FJ_TOUROKU 0x94
#define VK_OEM_FJ_LOYA 0x95
#define VK_OEM_FJ_ROYA 0x96
#define VK_LSHIFT 0xA0
#define VK_RSHIFT 0xA1
#define VK_LCONTROL 0xA2
#define VK_RCONTROL 0xA3
#define VK_LMENU 0xA4
#define VK_RMENU 0xA5
#define VK_BROWSER_BACK 0xA6
#define VK_BROWSER_FORWARD 0xA7
#define VK_BROWSER_REFRESH 0xA8
#define VK_BROWSER_STOP 0xA9
#define VK_BROWSER_SEARCH 0xAA
#define VK_BROWSER_FAVORITES 0xAB
#define VK_BROWSER_HOME 0xAC
#define VK_VOLUME_MUTE 0xAD
#define VK_VOLUME_DOWN 0xAE
#define VK_VOLUME_UP 0xAF
#define VK_MEDIA_NEXT_TRACK 0xB0
#define VK_MEDIA_PREV_TRACK 0xB1
#define VK_MEDIA_STOP 0xB2
#define VK_MEDIA_PLAY_PAUSE 0xB3
#define VK_LAUNCH_MAIL 0xB4
#define VK_LAUNCH_MEDIA_SELECT 0xB5
#define VK_LAUNCH_APP1 0xB6
#define VK_LAUNCH_APP2 0xB7
#define VK_OEM_1 0xBA
#define VK_OEM_PLUS 0xBB
#define VK_OEM_COMMA 0xBC
#define VK_OEM_MINUS 0xBD
#define VK_OEM_PERIOD 0xBE
#define VK_OEM_2 0xBF
#define VK_OEM_3 0xC0
#define VK_OEM_4 0xDB
#define VK_OEM_5 0xDC
#define VK_OEM_6 0xDD
#define VK_OEM_7 0xDE
#define VK_OEM_8 0xDF
#define VK_OEM_AX 0xE1
#define VK_OEM_102 0xE2
#define VK_ICO_HELP 0xE3
#define VK_ICO_00 0xE4
#define VK_PROCESSKEY 0xE5
#define VK_ICO_CLEAR 0xE6
#define VK_PACKET 0xE7
#define VK_OEM_RESET 0xE9
#define VK_OEM_JUMP 0xEA
#define VK_OEM_PA1 0xEB
#define VK_OEM_PA2 0xEC
#define VK_OEM_PA3 0xED
#define VK_OEM_WSCTRL 0xEE
#define VK_OEM_CUSEL 0xEF
#define VK_OEM_ATTN 0xF0
#define VK_OEM_FINISH 0xF1
#define VK_OEM_COPY 0xF2
#define VK_OEM_AUTO 0xF3
#define VK_OEM_ENLW 0xF4
#define VK_OEM_BACKTAB 0xF5
#define VK_ATTN 0xF6
#define VK_CRSEL 0xF7
#define VK_EXSEL 0xF8
#define VK_EREOF 0xF9
#define VK_PLAY 0xFA
#define VK_ZOOM 0xFB
#define VK_NONAME 0xFC
#define VK_PA1 0xFD
#define VK_OEM_CLEAR 0xFE

struct KeyMap
{
    enum
    {
       NONE,
       A = 0x41,
       B,
       C,
       D,
       E,
       F,
       G,
       H,
       I,
       J,
       K,
       L,
       M,
       N,
       O,
       P,
       Q,
       R,
       S,
       T,
       U,
       V,
       W,
       X,
       Y,
       Z,

       KEY_0 = 0x30,
       KEY_1,
       KEY_2,
       KEY_3,
       KEY_4,
       KEY_5,
       KEY_6,
       KEY_7,
       KEY_8,
       KEY_9,

       PAD_0 = VK_NUMPAD0,
       PAD_1 = VK_NUMPAD1,
       PAD_2 = VK_NUMPAD2,
       PAD_3 = VK_NUMPAD3,
       PAD_4 = VK_NUMPAD4,
       PAD_5 = VK_NUMPAD5,
       PAD_6 = VK_NUMPAD6,
       PAD_7 = VK_NUMPAD7,
       PAD_8 = VK_NUMPAD8,
       PAD_9 = VK_NUMPAD9,

       F1 = VK_F1,
       F2 = VK_F2,
       F3 = VK_F3,
       F4 = VK_F4,
       F5 = VK_F5,
       F6 = VK_F6,
       F7 = VK_F7,
       F8 = VK_F8,
       F9 = VK_F9,
       F10 = VK_F10,
       F11 = VK_F11,
       F12 = VK_F12,

       ESCAPE = VK_ESCAPE,
       TILDE = VK_OEM_3,
       MINUS = VK_OEM_MINUS,
       EQUALS,
       BACKSPACE = VK_BACK,
       TAB = VK_TAB,
       OPENBRACE = VK_OEM_4,
       CLOSEBRACE = VK_OEM_6,
       ENTER = VK_RETURN,
       SEMICOLON = VK_OEM_1,
       QUOTE = VK_OEM_7,
       BACKSLASH = VK_OEM_102,
       BACKSLASH2, /* DirectInput calls this DIK_OEM_102: "< > | on UK/Germany keyboards" */
       COMMA = VK_OEM_COMMA,
       FULLSTOP = VK_OEM_PERIOD,
       SLASH,
       SPACE = VK_SPACE,

       INSERT = VK_INSERT,
       DELETE_KEY = VK_DELETE,
       HOME = VK_HOME,
       END = VK_END,
       PGUP = VK_PRIOR,
       PGDN = VK_NEXT,
       LEFT = VK_LEFT,
       RIGHT = VK_RIGHT,
       UP = VK_UP,
       DOWN = VK_DOWN,

       PAD_SLASH,
       PAD_ASTERISK,
       PAD_MINUS,
       PAD_PLUS,
       PAD_DELETE,
       PAD_ENTER,

       PRINTSCREEN = VK_SNAPSHOT,
       PAUSE = VK_PAUSE,

       ABNT_C1,
       YEN,
       KANA,
       CONVERT,
       NOCONVERT,
       AT,
       CIRCUMFLEX,
       COLON2,
       KANJI,

       KEY_PAD_EQUALS,	/* MacOS X */
       KEY_BACKQUOTE,	/* MacOS X */
       KEY_SEMICOLON2,	/* MacOS X */
       KEY_COMMAND,	/* MacOS X */
       KEY_UNKNOWN,

       /////////////////////////////////////////////

       KEY_MODIFIERS,

       LSHIFT = VK_LSHIFT,
       RSHIFT = VK_RSHIFT,
       LCTRL = VK_LCONTROL,
       RCTRL = VK_RCONTROL,
       ALT = VK_LMENU,
       ALTGR = VK_RMENU,
       LWIN = VK_LWIN,
       RWIN = VK_RWIN,
       MENU,
       SCROLLLOCK = VK_SCROLL,
       NUMLOCK = VK_NUMLOCK,
       CAPSLOCK = VK_CAPITAL,

       SHIFT = VK_SHIFT,
       CTRL = VK_CONTROL,
       COMMAND,
       INALTSEQ,
       ACCENT1,
       ACCENT2,
       ACCENT3,
       ACCENT4
    };
};
#endif

#endif // KEYMAP

