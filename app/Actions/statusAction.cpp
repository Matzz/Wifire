#include <JsonObjectStream.h>
#include "statusAction.h"
#include "../Services/StatusProvider.h"

void statusAction(HttpRequest &request, HttpResponse &response) {
	Status status = StatusProvider::getStatus(false);


	DynamicJsonDocument doc(JSON_MAX_SIZE);
	CodecHelpers::encodeDoc<Status>(Codec<Status>::getInstance(), doc, status);
	String body;
	serializeJson(doc, body);

	response.setContentType(MIME_JSON);
	response.sendString(body);
}
