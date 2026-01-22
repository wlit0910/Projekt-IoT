#ifndef ENGINE_COMMON_INTERFACE_H
#define ENGINE_COMMON_INTERFACE_H

#include <string>

#if defined(_WIN32)
#if defined(ENGINE_DLL_EXPORT)
#define ENGINE_API extern "C" __declspec(dllexport)
#else
#define ENGINE_API extern "C"
#endif
#else
#define ENGINE_API extern "C" __attribute__((visibility("default")))
#endif


class RootPanel;

struct ClientCallbacks
{
    int (*OnInit)();
    void (*OnFrame)();
    void (*OnShutdown)();
};

class IEngineCommon
{
public:
    virtual ~IEngineCommon() {}

    virtual void RegisterClientCallbacks(const ClientCallbacks& cb) = 0;
    virtual void* GetInterface(const char* name) = 0;

	virtual void OnInit() = 0;
	virtual void OnFrame() = 0;
	virtual void OnShutdown() = 0;

public:
	RootPanel* pRootPanel; //Temporary
};

// ------------------------------------------------------------
// Exported functions
// ------------------------------------------------------------
ENGINE_API IEngineCommon* GetEngineCommonInterface();
ENGINE_API int EngineMain(int argc, char* argv[]);

extern IEngineCommon* g_pIEngineCommon;

#endif