#include "StringVectorCodec.h"

void StringVectorCodec::encode(JsonObject& json, const Vector<String>& vector, const String& key) {
    JsonArray jsonArr = json.createNestedArray(key);
    for(int j=0; j<vector.size(); j++) {
        jsonArr.add(vector[j]);
    }
}

Vector<String> StringVectorCodec::decode(JsonObject& json, const String& key) {
    JsonArray jsonArr = json[key].as<JsonArray>();
    unsigned int jsonArraySize = jsonArr.size();
    
    Vector<String> vector(jsonArraySize,  10);
    for(int j=0; j<jsonArraySize; j++) {
        String element = jsonArr[j].as<String>();
        vector.add(element);
    }
    return vector;
}