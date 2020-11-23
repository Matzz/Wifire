#include <JsonObjectStream.h>
#include "staticAction.h"

void staticAction(HttpRequest &request, HttpResponse &response)
{
	String file = request.getPath();
	if (file[0] == '/')
		file = file.substring(1);

	if (file[0] == '.' || file.startsWith("config")) {
		response.code = HTTP_STATUS_FORBIDDEN;
		return;
	}

	if(file.length()==0) {
		file = "index.htm";
	}

	response.setCache(86400, true);
	response.sendFile(file);
}
