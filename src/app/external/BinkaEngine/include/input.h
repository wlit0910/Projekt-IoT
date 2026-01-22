#ifndef INPUT_H
#define INPUT_H

#include <string>

enum ScrollState
{
    NO_SCROLL = 0,
    SCROLL_UP,
    SCROLL_DOWN
};

#define INTERFACEVER_INPUTSYSTEM "InputSystemV1"

class IInputSystem
{
public:
    virtual ~IInputSystem() {}

    virtual void CaptureMouse() = 0;
    virtual void ReleaseMouse() = 0;
    virtual void SetMouseCapture(bool capture) = 0;
    virtual bool IsMouseCaptured() = 0;

    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Shutdown() = 0;

    bool g_bInTextInput;
    bool g_bForceCloseTextInput;

    bool keyState[SDL_NUM_SCANCODES];
    bool keyEventPressRepeat[SDL_NUM_SCANCODES];
    bool keyEventPressNoRepeat[SDL_NUM_SCANCODES];
    Uint32 mouseButtons;
    SDL_Point mousePos;
    SDL_Point relativeMousePos;
    ScrollState scrollState;
    std::string g_globalTextBuffer;
};

extern IInputSystem* g_inputsystem;



void Init_Input();
void Update_Input();
void Shutdown_Input();

#endif