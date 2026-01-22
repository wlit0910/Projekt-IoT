#ifndef CVAR_INTERFACE_H
#define CVAR_INTERFACE_H

#include <string>
#include "cvar.h"

#define INTERFACEVER_CVAR "CVarV1"

// Interface wrapper (similar style to filesystem.h)
class ICVarInterface
{
public:
	virtual ~ICVarInterface() {}

	virtual void FillCVarList(CVar* cvarchain) = 0;
	virtual CVar* FindCVar(const std::string& name) = 0;
	virtual void LoadCVarSettings(const std::string& filename) = 0;
	virtual void SaveCVarSettings() = 0;
	virtual void ListCVars() = 0;
	virtual bool CheckCVar(const std::string& command) = 0;
};

extern ICVarInterface* g_cvarInterface;

#ifdef ENGINE_DLL
void Init_CVar();
void Shutdown_CVar();
#endif


#endif
