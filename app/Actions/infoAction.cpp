#include "infoAction.h"

#include <SmingCore/SmingCore.h>
#include "../Services/InfoProvider.h"

void infoAction(HttpRequest &request, HttpResponse &response) {
	auto info = InfoProvider::getInfo(false);

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();
	auto infoSize = info->count();
	for(int i=0; i<infoSize; i++) {
		json[info->keyAt(i)] = info->valueAt(i);
	}
	delete info;
	response.sendJsonObject(stream);
}
