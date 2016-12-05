#include <ESPEssentials.h>

#include "WebSocket.h"
#include "EffectController.h"
#include "Time.h"
#include "Hardware.h"
#include "Config.h"

#include "Effects/Wakeup.h"
#include "Effects/StaticSingleColor.h"

void setup()
{
	Serial.begin(115200);
	Serial.println("");
	
	Wifi.autoConnect("Lightstrip AP (192.168.4.1)");
	WebServer.init();
	if(Wifi.isAP())
		return;
	
	OTA.init("Lightstrip");
	websocket_init();
	Config.init();

	initHardware();
	initTime();
}

void loop()
{
	if(Wifi.isAP())
	{
		WebServer.handleClient();
	}
	else
	{
		WebServer.handleClient();
		OTA.handle();
		webSocket.loop();
		
		handleAlarm();
	}
}