#ifndef INPUT_H
#define INPUT_H

//#include "core/components/sprite.h"
#include "core/x-platform/keymap.h"
#include "core/containers/hash.h"

#if PLATFORM == SDLApplication
const unsigned int KEYS_QUANTITY = 1073742106; // FIXME: SDL platform has keys going up to 1073742106...
#else
const unsigned int KEYS_QUANTITY = 256;
#endif

//#include "3rdparty/RtMidi.h"

/// Input
/*!
     Example usage:
        if (input.Held(input.Key.C)) { ... }
        input.Define("A mapped key", Key.N);
        if (input.Held("A mapped key")) { ... }

 */
class Input
{
private:
    class Joystick
    {
    private:
        struct Stick {
            float x, y;
            //Button Trigger;
        };
    public:
        Joystick();

        Stick Left;
        Stick Right;

        void Rumble(float intensity, float duration = 0);
        void Rumble(bool active, float duration = 0);
    };

    /*RtMidiIn midiin;
    std::vector<unsigned char> message;*/

    struct HeadStruct
    {
        float x,y,z;
        float pitch, yaw, roll;

        bool active;

        HeadStruct();
    };

    struct MouseStruct
    {
        // Position of the mouse
        int x,y;
        int dx,dy;

        // To store mouse state
        bool Up, Down, Pressed, Released;
        bool Hidden, Dragging;

        MouseStruct();

        //void Hide();
        //void Show();
        //void PointerGraphics(Sprite pointer);
        //void MoveTo(unsigned int x, unsigned int y);
    };
    //deque<Button> Input;
    Hash<String, unsigned int> controls;
    bool oldKeys[KEYS_QUANTITY];
    bool keyChanges[KEYS_QUANTITY];
    bool keys[KEYS_QUANTITY];

public:
    Input();
    ~Input();

    //! Key Map
    KeyMap Key;

    /// Define controls, map keys to a label
    void Define(const String &controlName, unsigned int key);

    /// Called each frame
    void Update();

    /// Check if input key is held down
    bool Held(const unsigned int key);
    /// Check if defined control name is held down
    bool Held(String controlName);

    /// Check if input key was pressed this frame
    bool Pressed(const unsigned int key);
    /// Check if defined control name was pressed this frame
    bool Pressed(String controlName);

    /// Check if input key was released this frame
    bool Released(const unsigned int key);
    /// Check if defined control name was released this frame
    bool Released(String controlName);

    //! Called when a mouse/controller button is pressed/released.
    void OnButton(const unsigned int button, const bool isDown);

    /// Mouse location and trigger states
    MouseStruct Mouse;

    /// Head position location and rotation in space
    HeadStruct Head;
};

#endif
