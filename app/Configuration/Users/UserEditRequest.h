#pragma once

#include <SmingCore.h>
#include "../Codec.h"

#define MAX_USERS 100
#define MAX_ROLES 10

class UserEditRequest {
public:
    bool enabled;
	String login;
	String password;
	Vector<String> roles;
};

template<> class Codec<UserEditRequest> {
	public:
        static Codec<UserEditRequest>& getInstance() {
            static Codec<UserEditRequest> instance;
            return instance;
        }

	void encode(JsonObject& json, UserEditRequest userToEdit) {
        json["enabled"] = userToEdit.enabled;
        json["login"] = userToEdit.login;
        json["password"] = userToEdit.password;
        JsonArray rolesArr = json.createNestedArray("roles");
        for(int j=0; j<userToEdit.roles.size(); j++) {
            rolesArr.add(userToEdit.roles[j]);
        }
	}

	Either<String, UserEditRequest> decode(JsonObject& json) {
		UserEditRequest cfg;
        cfg.enabled = json["enabled"].as<bool>();
        cfg.login = json["login"].as<String>();
        cfg.password = json["password"].as<String>();
        
		JsonArray usersArr = json["users"].as<JsonArray>();
		int usersArrSize = usersArr.size();

        JsonArray rolesArr = json["roles"].as<JsonArray>();
        unsigned int rolesArrSize = rolesArr.size();
        if(rolesArrSize>MAX_ROLES) {
            return {left_tag_t(), "Too many roles!"};
        }
        Vector<String> roles(rolesArrSize,  10);
        for(int j=0; j<rolesArrSize; j++) {
            String role = rolesArr[j].as<String>();
            if(role.length() > 0) {
                roles.addElement(role);
            }
        }
        cfg.roles = roles;
		
		return {right_tag_t(), std::move(cfg)};
	}
};