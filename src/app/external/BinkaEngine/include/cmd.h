#ifndef CMD_H
#define CMD_H

#include <SDL.h>
#include <string>
#include "enginecommoninterface.h"

extern std::string g_LogBuffer;

ENGINE_API void ConMsg(const char* fmt, ...);
ENGINE_API void SysErrorMsgBox(const char* fmt, ...);
ENGINE_API void CommandGlobal(const char* command);

#define INTERFACEVER_COMMANDSYSTEM "CommandSystemV1"

typedef void (*fnOnCommandGlobalClient)(const char* command);

class ICommandSystem
{
public:
    virtual ~ICommandSystem() {}

    virtual void ExecuteGlobalCommands() = 0;
    virtual void ListCommands() = 0;
    virtual void OnCommandGlobal(const char* command) = 0;
    virtual void SetClientCommandCallback(fnOnCommandGlobalClient callback) = 0;
};

extern ICommandSystem* g_cmdsystem;


#endif
