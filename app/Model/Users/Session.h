#pragma once

#include <SmingCore.h>
#include "../../Utils/Either.h"
#include "../Codec.h"

class Session {
public:
    String login;
    String sessionId;
	Vector<String> roles;
    uint64_t lastUsed;
    Session();
    Session(String login, String sessionId, const Vector<String> &roles, uint64_t lastUsed = 0);
    Session& operator=(const Session &session);
    
    inline void markUsed() {
        lastUsed = micros();
    }
};

template<> class Codec<Vector<Session>> {
	public:
	static Codec<Vector<Session>>& getInstance() {
		static Codec<Vector<Session>> instance;
		return instance;
	}

	void encode(JsonObject& json, const Vector<Session> &sessions) {
		JsonArray sessionsArr = json.createNestedArray("sessions");
		int sessionsSize = sessions.size();
		for(int i=0; i<sessionsSize; i++) {
			auto session = sessions[i];
			JsonObject sessionObj = sessionsArr.createNestedObject();
            sessionObj["login"] = session.login;
            sessionObj["sessionId"] = session.sessionId;
            sessionObj["lastUsed"] = session.lastUsed;
            // We dont encode roles here as they will be provided by UsersConfig
		}

	}

	Either<String, Vector<Session>> decode(JsonObject& json) {
		Vector<Session> sessions;
		JsonArray sessionsArr = json["sessions"].as<JsonArray>();
		int sessionsArrSize = sessionsArr.size();

		for(int i=0; i<sessionsArrSize; i++) {
			JsonObject sessionObj = sessionsArr[i].as<JsonObject>();
			Session session(
				sessionObj["login"].as<String>(),
				sessionObj["sessionId"].as<String>(),
                Vector<String>(),
				sessionObj["lastUsed"].as<uint64_t>()
			);

        	sessions.add(session);
		}
		
		return {RightTagT(), std::move(sessions)};
	}
};