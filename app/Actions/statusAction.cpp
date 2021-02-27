#include <JsonObjectStream.h>
#include "statusAction.h"
#include "../Services/StatusProvider.h"

void statusAction(HttpRequest &request, HttpResponse &response) {
	auto info = StatusProvider::getStatus(false);

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject json = stream->getRoot();

	auto infoSize = info->count();
	for(int i=0; i<infoSize; i++) {
		json[info->keyAt(i)] = info->valueAt(i);
	}
	delete info;
	response.sendNamedStream(stream);
}
