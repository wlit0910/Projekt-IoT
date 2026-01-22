#include <SDL.h>
#include <string>
#include "enginecommoninterface.h"
#include "renderer.h"
#include "filesystem.h"
#include "rootpanel.h"
#include "form.h"
#include "cmd.h"
#include "cvar_interface.h"
#include "cvar.h"
#include "util.h"
#include "input.h"
#include "font.h"
#include "resizebar.h"
#include "texturemgr.h"
#include "modelmgr.h"
#include "modelviewpanel.h"
#include "common_controls.h"

IEngineCommon* g_pIEngineCommon = NULL;
IRenderer* renderer_main = NULL;
IFileSystem* g_filesystem = NULL;
ICVarInterface* g_cvarInterface;
RootPanel* g_rootPanel = NULL;
ICommandSystem* g_cmdsystem = NULL;
IInputSystem* g_inputsystem = NULL;
IFontSystem* g_fontsystem = NULL;
ITextureMgr* g_texturemgr = NULL;
IModelMgr* g_modelmgr = NULL;

bool LoadAllInterfaces()
{
    if ((g_filesystem = (IFileSystem*)g_pIEngineCommon->GetInterface(INTERFACEVER_FILESYSTEM)) == NULL)
        return false;
	if ((renderer_main = (IRenderer*)g_pIEngineCommon->GetInterface(INTERFACEVER_RENDERER)) == NULL)
		return false;
	if ((g_cvarInterface = (ICVarInterface*)g_pIEngineCommon->GetInterface(INTERFACEVER_CVAR)) == NULL)
		return false;
	if ((g_cmdsystem = (ICommandSystem*)g_pIEngineCommon->GetInterface(INTERFACEVER_COMMANDSYSTEM)) == NULL)
		return false;
	if ((g_inputsystem = (IInputSystem*)g_pIEngineCommon->GetInterface(INTERFACEVER_INPUTSYSTEM)) == NULL)
		return false;
	if ((g_fontsystem = (IFontSystem*)g_pIEngineCommon->GetInterface(INTERFACEVER_FONTSYSTEM)) == NULL)
		return false;
	if ((g_texturemgr = (ITextureMgr*)g_pIEngineCommon->GetInterface(INTERFACEVER_TEXTUREMGR)) == NULL)
		return false;
	if ((g_modelmgr = (IModelMgr*)g_pIEngineCommon->GetInterface(INTERFACEVER_MODELMGR)) == NULL)
		return false;

    // Add more interfaces here...

    return true;
}

class DeviceForm : public Form
{
public:
	DeviceForm(std::string name) : Form(name)
	{
		SetPos(100, 100);
		SetSize(300, 200);
		SetMinSize(100, 100);
		//SetFormName("App3");
		SetBackgroundColor({ 150, 150, 150, 255 });
		resizeBar = new ResizeBar("ResizeBar");
		resizeBar->SetParent(this);
		resizeBar->SetPos(280, 180);
		resizeBar->SetSize(20, 20);
		resizeBar->SetZOrder(-10);

		deviceIPAddress = new TextBox("DeviceIPAddress");
		deviceIPAddress->SetParent(this);
		deviceIPAddress->SetPos(20, 50);
		deviceIPAddress->SetSize(200, 30);

		btnConnect = new Button("BtnConnect");
		btnConnect->SetParent(this);
		btnConnect->SetPos(20, 100);
		btnConnect->SetSize(100, 30);
		btnConnect->SetText("Connect");




	}
	ResizeBar* resizeBar;
	TextBox* deviceIPAddress;
	Button* btnConnect;
};

void OnCommandGlobalClient(const char* command)
{
	KeyValue kv = ParseKeyValue(command);
	std::string cmd = kv.key;

	if (cmd == "testclientcmd")
	{
		ConMsg("Test command Client Global\n");
	}
	else if (cmd == "print_root")
	{
		ConMsg("Root panel: %s\n", g_rootPanel->GetPanelName());
	}
	else if(cmd == "create_device_form")
	{
		DeviceForm* deviceForm = new DeviceForm("DeviceForm");
		deviceForm->SetParent(g_rootPanel->GetLayerPanel(LAYER_GAMEUI));
	}
	else
	{
		ConMsg("Unknown command: %s\n", cmd.c_str());
	}
}





int ClientInit()
{
	if (LoadAllInterfaces() == false)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to load engine interfaces!", NULL);
		return 1;
	}

	g_cmdsystem->SetClientCommandCallback(OnCommandGlobalClient);

	g_rootPanel = g_pIEngineCommon->pRootPanel;
	if(g_rootPanel == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Root panel is NULL!", NULL);
		return 1;
	}

	g_cvarInterface->FillCVarList(g_CVarChain);
	g_cvarInterface->LoadCVarSettings(GetFullPath("test/scripts/cvars.txt"));

	LayerPanel* layerMenus = (LayerPanel*)g_rootPanel->GetLayerPanel(LAYER_MENUS);
	if (layerMenus)
	{
		layerMenus->should_draw_background = false;
		layerMenus->is_background_nonsolid = true;
	}


	LayerPanel* layerGameui = (LayerPanel*)g_rootPanel->GetLayerPanel(LAYER_GAMEUI);

	//In here we're adding all forms.

	Button* btnAddDevice = new Button("btnAddDevice");
	btnAddDevice->SetParent(layerGameui);
	btnAddDevice->SetPos(10, 10);
	btnAddDevice->SetText("Add Device");
	btnAddDevice->globalCommand = "create_device_form";


	
	ConMsg("Client initialized\n");


	return 0;
}

void ClientFrame()
{

}

int main(int argc, char* argv[])
{
	//Setup engine interface
	g_pIEngineCommon = GetEngineCommonInterface();
	if(g_pIEngineCommon == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to get engine common interface!", NULL);
		return 1;
	}

	g_pIEngineCommon->RegisterClientCallbacks({ ClientInit, ClientFrame, nullptr });




	EngineMain(argc, argv);


	return 0;
}
