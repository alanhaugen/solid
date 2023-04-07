#ifndef KEYMAP
#define KEYMAP

struct KeyMap
{
    enum
    {
       NONE,
       A = KEYMAP_FIRSTKEY,
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

#endif // KEYMAP

