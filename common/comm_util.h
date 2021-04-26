#ifndef COMM_UTIL_H
#define COMM_UTIL_H

#include <json/json.h>
#include <google/protobuf/message.h>
#include <google/protobuf/unknown_field_set.h>
#include "def.h"

class Util
{
    static void put_unknowfieldset_to_json(const google::protobuf::UnknownFieldSet& ufs, Json::Value& jnode);
    static void convert_proto_to_json(const google::protobuf::Message& message, Json::Value& json_value);
    static void json_to_proto(const Json::Value &json_object, google::protobuf::Message &message);
    static void convert_json_to_proto(const Json::Value &json_value, google::protobuf::Message &message);
    static void convert_unknown_json_to_proto(const Json::Value &json_value, google::protobuf::Message **new_message);
    static std::string json_as_string(const Json::Value& json);
};

#endif // COMM_UTIL_H
