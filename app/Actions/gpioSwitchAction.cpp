#include <JsonObjectStream.h>
#include "gpioSwitchAction.h"
#include "../Services/GPIOStateManager.h"
#include "../Services/Injector.h"


void gpioSwitchAction(HttpRequest &request, HttpResponse &response) {
	Injector &di = Injector::getInstance();
	GPIOStateManager& gpioState = di.getGPIOStateManager();
	String name = request.getQueryParameter("name");
	uint32_t howLongMs = request.getQueryParameter("time_ms", "0").toInt();
	
	String stateName = request.getQueryParameter("state", "on");
	stateName.toLowerCase();
	uint8_t state = stateName == "on" ? HIGH : LOW;
	
    bool result = gpioState.switchPin(name, state, howLongMs);

}