#include <JsonObjectStream.h>
#include "gpioSwitchAction.h"
#include "../Services/GPIOStateManager.h"
#include "../Services/Injector.h"


void gpioSwitchAction(HttpRequest &request, HttpResponse &response) {
	Injector &di = Injector::getInstance();
	GPIOStateManager& gpioState = di.getGPIOStateManager();
	String name = request.getQueryParameter("name");
    bool result = gpioState.turnOn(name, 1000);

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();
	if(result) {
		json["status"] = "successful";
	} else {
		json["status"] = "failed";
	}
	response.sendNamedStream(stream);
}