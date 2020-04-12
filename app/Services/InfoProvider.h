#ifndef APP_INFOPROVIDER_H_
#define APP_INFOPROVIDER_H_

#include <SmingCore.h>
#include <Wiring/WHashMap.h>

class InfoProvider {
public:
	static HashMap<String, String>* getInfo(bool showPassword);
};

#endif
