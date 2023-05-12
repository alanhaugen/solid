#ifndef KEYMAP
#define KEYMAP

#if PLATFORM == SDLApplication
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

struct KeyMap
{
    enum
    {
       NONE,
       A = 65,
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

       KEY_0,
       KEY_1,
       KEY_2,
       KEY_3,
       KEY_4,
       KEY_5,
       KEY_6,
       KEY_7,
       KEY_8,
       KEY_9,

       PAD_0,
       PAD_1,
       PAD_2,
       PAD_3,
       PAD_4,
       PAD_5,
       PAD_6,
       PAD_7,
       PAD_8,
       PAD_9,

       F1,
       F2,
       F3,
       F4,
       F5,
       F6,
       F7,
       F8,
       F9,
       F10,
       F11,
       F12,

       ESCAPE,
       TILDE,
       MINUS,
       EQUALS,
       BACKSPACE,
       TAB,
       OPENBRACE,
       CLOSEBRACE,
       ENTER,
       SEMICOLON,
       QUOTE,
       BACKSLASH,
       BACKSLASH2, /* DirectInput calls this DIK_OEM_102: "< > | on UK/Germany keyboards" */
       COMMA,
       FULLSTOP,
       SLASH,
       SPACE = 0x20,

       INSERT,
       DELETE_KEY,
       HOME,
       END,
       PGUP,
       PGDN,
       LEFT = 0x25,
       RIGHT = 0x27,
       UP,
       DOWN,

       PAD_SLASH,
       PAD_ASTERISK,
       PAD_MINUS,
       PAD_PLUS,
       PAD_DELETE,
       PAD_ENTER,

       PRINTSCREEN,
       PAUSE,

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

       LSHIFT,
       RSHIFT,
       LCTRL,
       RCTRL,
       ALT,
       ALTGR,
       LWIN,
       RWIN,
       MENU,
       SCROLLLOCK,
       NUMLOCK,
       CAPSLOCK,

       SHIFT,
       CTRL,
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

