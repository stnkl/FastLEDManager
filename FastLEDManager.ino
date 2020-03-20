#include <Arduino.h>
#include <ESPEssentials.h>

#include "Animation.h"
#include "Config.h"
#include "Fade.h"
#include "Hardware.h"
#include "WebSocket.h"
#include "Webserver.h"

// Include animations here:
#include "Animations/color.h"

void setup()
{
  initESPEssentials("Lightstrip");
  Config.init();
  initHardware();

  if (WiFi.status() == WL_CONNECTED)
  {
    initWebsocket();
    initWebserverCommands();
    initFade();
  }

  // Register animations here:
  registerAnimation(new Color("Color"));

  if (Config.startupAnimation != "")
    getAnimation(Config.startupAnimation)->begin();
}

void loop()
{
  if (status == RUNNING && currentAnimation)
    currentAnimation->loop();

  handleESPEssentials();
  handleFade();
  webSocket.loop();
}