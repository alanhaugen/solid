#include "input.h"

Input::Input()
    : controls(DEFAULT_HASH_SIZE, Hash<String, unsigned int>::StringHash)
{
    for (unsigned int i = 0; i < KEYS_QUANTITY; i++)
    {
        keys[i] = false;
        oldKeys[i] = false;
        keyChanges[i] = false;
    }

    // See http://www.music.mcgill.ca/~gary/rtmidi/index.html
    /*unsigned int nPorts = midiin.getPortCount();
    std::cout << "There are " << nPorts << " MIDI input sources available" << std::endl;

    std::string portName;

    for (unsigned int i=0; i<nPorts; i++)
    {
        portName = midiin.getPortName(i);
        std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
    }

    if (nPorts > 0)
    {
        midiin.openPort(0);
        midiin.ignoreTypes(false, false, false); // Don't ignore sysex, timing, or active sensing messages.
    }*/
}

Input::~Input()
{
}

void Input::Define(const String& controlName, unsigned int key)
{
    controls.Insert(controlName, key);
}

void Input::Update()
{
    for (int i = 0; i < 256; i++)
    {
        keyChanges[i] = keys[i] ^ oldKeys[i];
        oldKeys[i] = keys[i];
    }

    // Read from MIDI controller
    /*double stamp;
    stamp = midiin.getMessage(&message);
    int nBytes = message.size();

    bool state;

    for (int i = 0; i < nBytes; i++)
    {
        // std::cout << "Byte " << i << " = " << (int)message[i] << ", ";

        if (i == 0)
        {
            if ((int)message[i] == 144)
            {
                state = true;
            }
            else
            {
                state = false;
            }
        }

        if (i == 1)
        {
            keys[(int)message[i]] = state;
        }
    }
    if (nBytes > 0)
    {
        //std::cout << "stamp = " << stamp << std::endl;
    }*/
}

bool Input::Held(const unsigned int key)
{
    return keys[key];
}

bool Input::Held(String controlName)
{
    return Held(controls[controlName]->data_);
}

bool Input::Pressed(const unsigned int key)
{
    return keyChanges[key] & keys[key];
}

bool Input::Pressed(String controlName)
{
    return Pressed(controls[controlName]->data_);
}

bool Input::Released(const unsigned int key)
{
    return keyChanges[key] | keys[key];
}

bool Input::Released(String controlName)
{
    return Released(controls[controlName]->data_);
}

void Input::OnButton(const unsigned int button, const bool isDown)
{
    if (button < KEYS_QUANTITY)
    {
        keys[button] = isDown;
    }
}

Input::HeadStruct::HeadStruct()
{
    x = 0;
    y = 0;
    z = 0;

    pitch = 0;
    yaw = 0;
    roll = 0;

    active = false;
}

Input::MouseStruct::MouseStruct()
{
    x = 0;
    y = 0;

    dx = 0;
    dy = 0;

    Up = true;
    Down = false;
    Pressed = false;
    Released = false;

    Hidden = true;
    Dragging = false;
}
