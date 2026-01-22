#ifndef CVAR_H
#define CVAR_H

#include <string>

class CVar
{
public:
    typedef void (*CVarCallback)(CVar*);


    CVar(const std::string& name, const std::string& default_value, bool archive = false, CVarCallback callback = NULL);
    //~CVar();

    void SetValue(float value);
    void SetValue(int value);
    void SetValue(const std::string& value);

	//Only use these to get values, as actual values might be outdated if multiple instances exist.
	int GetInt() const { return pOwner->iValue; }
    float GetFloat() const { return pOwner->fValue; }
    std::string GetString() const { return pOwner->strValue; }

	CVar* pOwner; //Pointer to real Cvar if multiple instances exist. If unique, points to this.
    std::string     name;
    std::string     strValue;
    float           fValue;
    int             iValue;
    bool            bArchive;
    CVarCallback    callback;
    CVar* next;
};

extern CVar* g_CVarChain;

#endif
