#include <JsonObjectStream.h>
#include "statusAction.h"
#include "../Services/StatusProvider.h"
#include "actionsHelpers.h"

void statusAction(HttpRequest &request, HttpResponse &response) {
	Status status = StatusProvider::getStatus(false);


	handleConfigGet(request, response, status);
}
