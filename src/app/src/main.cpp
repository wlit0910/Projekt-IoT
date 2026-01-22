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
#include <MQTTClient.h>

#define ADAFRUIT_ADDRESS "tcp://io.adafruit.com:1883"
#define ADAFRUIT_QOS     1
#define ADAFRUIT_TIMEOUT 5000L



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


CVar AIO_Username("aio_username", "", true);
CVar AIO_Key("aio_key", "", true);

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

//Global handle for MQTT client
MQTTClient client;

void ConnectToAdafruitIO(const std::string& username, const std::string& aio_key)
{
	// Create and connect the MQTT client
	MQTTClient_create(&client, ADAFRUIT_ADDRESS, username.c_str(),
		MQTTCLIENT_PERSISTENCE_NONE, NULL);
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	//conn_opts.cleansession = 0;
	conn_opts.username = username.c_str();
	conn_opts.password = aio_key.c_str();
	int rc;
	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
		ConMsg("Failed to connect to Adafruit IO, return code %d\n", rc);
	}
	else
	{
		ConMsg("Successfully connected to Adafruit IO!\n");
		LayerPanel* layerMenus = (LayerPanel*)g_rootPanel->GetLayerPanel(LAYER_MENUS);
		Panel* mm_panel = layerMenus->GetChildByName("mm_panel");
		if(mm_panel)
		{
			Label* lb_aio_status = dynamic_cast<Label*>(mm_panel->GetChildByName("lb_aio_status"));
			if(lb_aio_status)
			{
				lb_aio_status->SetText("AIO Status: Connected");
			}

		}
	}
}
void DisconnectFromAdafruitIO()
{
	MQTTClient_disconnect(client, 1000);
	MQTTClient_destroy(&client);
	ConMsg("Disconnected from Adafruit IO\n");

	LayerPanel* layerMenus = (LayerPanel*)g_rootPanel->GetLayerPanel(LAYER_MENUS);
	Panel* mm_panel = layerMenus->GetChildByName("mm_panel");
	if (mm_panel)
	{
		Label* lb_aio_status = dynamic_cast<Label*>(mm_panel->GetChildByName("lb_aio_status"));
		if (lb_aio_status)
		{
			lb_aio_status->SetText("AIO Status: Disconnected");
		}
	}
}



class AdaIOConnectionMenu : public Form
{
	public:
	AdaIOConnectionMenu(std::string name) : Form(name)
	{
		SetPos(150, 150);
		SetSize(400, 300);
		SetMinSize(200, 150);
		SetFormName("Adafruit IO Settings");
		SetBackgroundColor({ 200, 200, 200, 255 });

		tbUsername = new TextBox("tbUsername");
		tbUsername->SetParent(this);
		tbUsername->SetPos(20, 50);
		tbUsername->SetSize(200, 30);
		tbUsername->SetText(AIO_Username.GetString());
		tbAIOKey = new TextBox("tbAIOKey");
		tbAIOKey->SetParent(this);
		tbAIOKey->SetPos(20, 100);
		tbAIOKey->SetSize(200, 30);
		tbAIOKey->SetText(AIO_Key.GetString());
		btnSaveSettings = new Button("btnSaveSettings");
		btnSaveSettings->SetParent(this);
		btnSaveSettings->SetPos(20, 150);
		btnSaveSettings->SetSize(150, 30);
		btnSaveSettings->SetText("Save Settings");
		btnSaveSettings->panelCommand = "save_aio_settings";


		testConnection = new Button("testConnection");
		testConnection->SetParent(this);
		testConnection->SetPos(200, 150);
		testConnection->SetSize(150, 30);
		testConnection->SetText("Test Connection");
		testConnection->panelCommand = "test_aio_connection";

		btnConnect = new Button("btnConnect");
		btnConnect->SetParent(this);
		btnConnect->SetPos(20, 200);
		btnConnect->SetSize(100, 30);
		btnConnect->SetText("Connect");
		btnConnect->panelCommand = "connect_aio";
		btnDisconnect = new Button("btnDisconnect");
		btnDisconnect->SetParent(this);
		btnDisconnect->SetPos(200, 200);
		btnDisconnect->SetSize(100, 30);
		btnDisconnect->SetText("Disconnect");
		btnDisconnect->panelCommand = "disconnect_aio";

	}

	void OnCommand(const std::string& command)
	{
		if (command == "save_aio_settings")
		{
			std::string username = tbUsername->GetText();
			std::string aio_key = tbAIOKey->GetText();
			AIO_Username.SetValue(username);
			AIO_Key.SetValue(aio_key);
			ConMsg("Saved AIO Settings\n");
		}
		else if (command == "test_aio_connection")
		{
			std::string username = AIO_Username.GetString();
			std::string aio_key = AIO_Key.GetString();
			if (username.empty() || aio_key.empty())
			{
				ConMsg("AIO Username or Key is empty!\n");
				return;
			}
			MQTTClient client;
			MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
			int rc;
			MQTTClient_create(&client, ADAFRUIT_ADDRESS, username.c_str(),
				MQTTCLIENT_PERSISTENCE_NONE, NULL);
			conn_opts.username = username.c_str();
			conn_opts.password = aio_key.c_str();
			if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
			{
				ConMsg("Failed to connect to Adafruit IO, return code %d\n", rc);
			}
			else
			{
				ConMsg("Successfully connected to Adafruit IO!\n");
				MQTTClient_disconnect(client, 1000);
			}
			MQTTClient_destroy(&client);
		}
		else if (command == "connect_aio")
		{
			ConnectToAdafruitIO(AIO_Username.GetString(), AIO_Key.GetString());
		}
		else if (command == "disconnect_aio")
		{
			DisconnectFromAdafruitIO();
		}
		else
		{
			Panel::OnCommand(command);
		}

	}

	TextBox* tbUsername;
	TextBox* tbAIOKey;
	Button* btnSaveSettings;
	Button* testConnection;
	Button* btnConnect;
	Button* btnDisconnect;
};

class AdaIOFeedMenu : public Form
{
public:
	AdaIOFeedMenu(std::string name) : Form(name)
	{
		SetPos(200, 200);
		SetSize(400, 400);
		SetMinSize(200, 200);
		SetFormName("Adafruit IO Feed");
		SetBackgroundColor({ 220, 220, 220, 255 });

		feedNameBox = new TextBox("feedNameBox");
		feedNameBox->SetParent(this);
		feedNameBox->SetPos(20, 50);
		feedNameBox->SetSize(200, 30);
		feedInputValueBox = new TextBox("feedInputValueBox");
		feedInputValueBox->SetParent(this);
		feedInputValueBox->SetPos(20, 100);
		feedInputValueBox->SetSize(200, 30);
		publishButton = new Button("publishButton");
		publishButton->SetParent(this);
		publishButton->SetPos(20, 150);
		publishButton->SetSize(150, 30);
		publishButton->SetText("Publish to Feed");
		publishButton->panelCommand = "publish_feed";
		lb_feedName = new Label("lb_feedName");
		lb_feedName->SetParent(this);
		lb_feedName->SetPos(240, 50);
		lb_feedName->SetSize(120, 30);
		lb_feedName->SetText("Feed Name");
		lb_feedValue = new Label("lb_feedValue");
		lb_feedValue->SetParent(this);	
		lb_feedValue->SetPos(240, 100);
		lb_feedValue->SetSize(120, 30);
		lb_feedValue->SetText("Feed Value");



	}

	void OnCommand(const std::string& command)
	{
		if (command == "publish_feed")
		{
			std::string feedName = feedNameBox->GetText();
			std::string feedValue = feedInputValueBox->GetText();
			std::string username = AIO_Username.GetString();
			std::string aio_key = AIO_Key.GetString();
			if (username.empty() || aio_key.empty())
			{
				ConMsg("AIO username or key is empty\n");
				return;
			}
			// Build topic
			std::string topic = username + "/feeds/" + feedName;
			// Publish using global client
			MQTTClient_message pubmsg = MQTTClient_message_initializer;
			pubmsg.payload = (void*)feedValue.c_str();
			pubmsg.payloadlen = (int)feedValue.length();
			pubmsg.qos = ADAFRUIT_QOS;
			pubmsg.retained = 0;
			MQTTClient_deliveryToken token;
			int rc = MQTTClient_publishMessage(client, topic.c_str(), &pubmsg, &token);
			if (rc != MQTTCLIENT_SUCCESS)
			{
				ConMsg("Failed to publish message, rc=%d\n", rc);
				return;
			}
			rc = MQTTClient_waitForCompletion(client, token, ADAFRUIT_TIMEOUT);
			ConMsg("Message with delivery token %d delivered\n", token);
		}
		else
		{
			Form::OnCommand(command);
		}
	}

	TextBox* feedNameBox;
	TextBox* feedInputValueBox;
	Button* publishButton;
	Label* lb_feedName;
	Label* lb_feedValue;

};


class AdaIOFeedSubscriber : public Form
{
public:
	AdaIOFeedSubscriber(std::string name) : Form(name)
	{
		SetPos(250, 250);
		SetSize(400, 400);
		SetMinSize(200, 200);
		SetFormName("Adafruit IO Feed Subscriber");
		SetBackgroundColor({ 240, 240, 240, 255 });

		feedNameBox = new TextBox("feedNameBox");
		feedNameBox->SetParent(this);
		feedNameBox->SetPos(20, 50);
		feedNameBox->SetSize(200, 30);

		feedValueLabel = new Label("feedValueLabel");
		feedValueLabel->SetParent(this);
		feedValueLabel->SetPos(20, 100);
		feedValueLabel->SetSize(300, 30);

		subscribeButton = new Button("subscribeButton");
		subscribeButton->SetParent(this);
		subscribeButton->SetPos(20, 150);
		subscribeButton->SetSize(150, 30);
		subscribeButton->SetText("Subscribe to Feed");
		subscribeButton->panelCommand = "subscribe_feed";

		subscribed = false;
		lastValue = "";
	}

	void OnCommand(const std::string& command)
	{
		if (command == "subscribe_feed")
		{
			if(client == NULL)
			{
				ConMsg("MQTT client is not connected. Please connect to Adafruit IO first.\n");
				return;
			}


			std::string feedName = feedNameBox->GetText();
			std::string username = AIO_Username.GetString();
			std::string aio_key = AIO_Key.GetString();

			if (username.empty() || aio_key.empty())
			{
				ConMsg("AIO username or key is empty\n");
				return;
			}

			// Build topic
			topic = username + "/feeds/" + feedName;

			// Subscribe using global client
			int rc = MQTTClient_subscribe(client, topic.c_str(), 1);

			if (rc != MQTTCLIENT_SUCCESS)
			{
				ConMsg("Failed to subscribe, rc=%d\n", rc);
				return;
			}

			subscribed = true;
			lastValue = "";

			ConMsg("Subscribed to feed %s\n", feedName.c_str());
		}
		else
		{
			Form::OnCommand(command);
		}
	}

	void Update()
	{
		Form::Update();

		if (subscribed)
		{
			MQTTClient_message* message = NULL;
			char* received_topic = NULL;
			int topic_len = 0;

			int rc = MQTTClient_receive(client,
				&received_topic,
				&topic_len,
				&message,
				0); // non-blocking

			if (rc == MQTTCLIENT_SUCCESS && message != NULL)
			{
				lastValue = std::string((char*)message->payload,
					message->payloadlen);

				MQTTClient_freeMessage(&message);
				MQTTClient_free(received_topic);
			}
		}

		if (lastValue.empty())
		{
			feedValueLabel->SetText("Feed Value: (waiting)");
		}
		else
		{
			feedValueLabel->SetText("Feed Value: " + lastValue);
		}
	}

	TextBox* feedNameBox;
	Label* feedValueLabel;
	Button* subscribeButton;

	bool subscribed;
	std::string lastValue;
	std::string topic;
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
	else if(cmd == "create_aio_settings_form")
	{
		AdaIOConnectionMenu* aioForm = new AdaIOConnectionMenu("AdaIOConnectionMenu");
		aioForm->SetParent(g_rootPanel->GetLayerPanel(LAYER_GAMEUI));
	}
	else if (cmd == "create_aio_feed_form")
	{
		AdaIOFeedMenu* aioFeedForm = new AdaIOFeedMenu("AdaIOFeedMenu");
		aioFeedForm->SetParent(g_rootPanel->GetLayerPanel(LAYER_GAMEUI));
	}
	else if (cmd == "create_aio_feed_subscriber_form")
	{
		AdaIOFeedSubscriber* aioFeedSubscriber = new AdaIOFeedSubscriber("AdaIOFeedSubscriber");
		aioFeedSubscriber->SetParent(g_rootPanel->GetLayerPanel(LAYER_GAMEUI));
	}
	else if (cmd == "connect_aio")
	{
		ConnectToAdafruitIO(AIO_Username.GetString(), AIO_Key.GetString());
	}
	else if (cmd == "disconnect_aio")
	{
		DisconnectFromAdafruitIO();
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

	Button* btnAIOSettings = new Button("btnAIOSettings");
	btnAIOSettings->SetParent(layerGameui);
	btnAIOSettings->SetPos(10, 50);
	btnAIOSettings->SetText("AIO Settings");
	btnAIOSettings->globalCommand = "create_aio_settings_form";

	Button* btnAIOFeed = new Button("btnAIOFeed");
	btnAIOFeed->SetParent(layerGameui);
	btnAIOFeed->SetPos(10, 90);
	btnAIOFeed->SetText("AIO Feed");
	btnAIOFeed->globalCommand = "create_aio_feed_form";


	Button* btnAddDevice = new Button("btnAddDevice");
	btnAddDevice->SetParent(layerGameui);
	btnAddDevice->SetPos(10, 10);
	btnAddDevice->SetText("Add Device");
	btnAddDevice->globalCommand = "create_device_form";

	Button* btnAIOSubscribeFeed = new Button("btnAIOSubscribeFeed");
	btnAIOSubscribeFeed->SetParent(layerGameui);
	btnAIOSubscribeFeed->SetPos(10, 130);
	btnAIOSubscribeFeed->SetText("Subscribe to Feed");
	btnAIOSubscribeFeed->globalCommand = "create_aio_feed_subscriber_form";



	
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
