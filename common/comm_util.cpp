#include "comm_util.h"


void Util::put_unknowfieldset_to_json(const google::protobuf::UnknownFieldSet& ufs, Json::Value& jnode) {
    std::map<int, std::vector<Json::Value> > kvs;
    for(int i = 0; i < ufs.field_count(); ++i) {
        const google::protobuf::UnknownField& uf = ufs.field(i);
        google::protobuf::UnknownField::Type type = uf.type();
        switch(type) {
        case google::protobuf::UnknownField::TYPE_VARINT:
            kvs[uf.number()].push_back((Json::Int64)uf.varint());
            //jnode[std::to_string(uf.number())] = (Json::Int64)uf.varint();
            break;
        case google::protobuf::UnknownField::TYPE_FIXED32:
            kvs[uf.number()].push_back((Json::UInt)uf.fixed32());
            //jnode[std::to_string(uf.number())] = (Json::Int)uf.fixed32();
            break;
        case google::protobuf::UnknownField::TYPE_FIXED64:
            kvs[uf.number()].push_back((Json::UInt64)uf.fixed64());
            //jnode[std::to_string(uf.number())] = (Json::Int64)uf.fixed64();
            break;
        case google::protobuf::UnknownField::TYPE_LENGTH_DELIMITED:
        {
            google::protobuf::UnknownFieldSet tmp;
            auto& v = uf.length_delimited();
            if(!v.empty() && tmp.ParseFromString(v)) {
                Json::Value vv;
                put_unknowfieldset_to_json(tmp, vv);
                kvs[uf.number()].push_back(vv);
                //jnode[std::to_string(uf.number())] = vv;
            } else {
                //jnode[std::to_string(uf.number())] = v;
                kvs[uf.number()].push_back(v);
            }
        }
            break;
        case google::protobuf::UnknownField::TYPE_GROUP:
            break;
        }
    }

    for(auto& i : kvs) {
        if(i.second.size() > 1) {
            for(auto& n : i.second) {
                jnode[std::to_string(i.first)].append(n);
            }
        } else {
            jnode[std::to_string(i.first)] = i.second[0];
        }
    }
}

void Util::convert_proto_to_json(const google::protobuf::Message& message, Json::Value& json_value) {
    const google::protobuf::Descriptor* descriptor = message.GetDescriptor();
    const google::protobuf::Reflection* reflection = message.GetReflection();
    Json::Value jnode;
    for(int i = 0; i < descriptor->field_count(); ++i) {
        const google::protobuf::FieldDescriptor* field = descriptor->field(i);

        if(field->is_repeated()) {
            if(!reflection->FieldSize(message, field)) {
                continue;
            }
        } else {
            if(!reflection->HasField(message, field)) {
                continue;
            }
        }

        if(field->is_repeated()) {
            switch(field->cpp_type()) {
#define XX(cpptype, method, valuetype, jsontype) \
            case google::protobuf::FieldDescriptor::CPPTYPE_##cpptype: { \
    int size = reflection->FieldSize(message, field); \
    for(int n = 0; n < size; ++n) { \
    jnode[field->name()].append((jsontype)reflection \
    ->GetRepeated##method(message, field, n)); \
            } \
    break; \
            }
            XX(INT32, Int32, int32_t, Json::Int);
            XX(UINT32, UInt32, uint32_t, Json::UInt);
            XX(FLOAT, Float, float, double);
            XX(DOUBLE, Double, double, double);
            XX(BOOL, Bool, bool, bool);
            XX(INT64, Int64, int64_t, Json::Int64);
            XX(UINT64, UInt64, uint64_t, Json::UInt64);
#undef XX
            case google::protobuf::FieldDescriptor::CPPTYPE_ENUM: {
                int size = reflection->FieldSize(message, field);
                for(int n = 0; n < size; ++n) {
                    jnode[field->name()].append(reflection->GetRepeatedEnum(message, field, n)->number());
                }
                break;
            }
            case google::protobuf::FieldDescriptor::CPPTYPE_STRING: {
                int size = reflection->FieldSize(message, field);
                for(int n = 0; n < size; ++n) {
                    jnode[field->name()].append(reflection->GetRepeatedString(message, field, n));
                }
                break;
            }
            case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE: {
                int size = reflection->FieldSize(message, field);
                for(int n = 0; n < size; ++n) {
                    Json::Value vv;
                    convert_proto_to_json(reflection->GetRepeatedMessage(message, field, n), vv);
                    jnode[field->name()].append(vv);
                }
                break;
            }
            }
            continue;
        }

        switch(field->cpp_type()) {
#define XX(cpptype, method, valuetype, jsontype) \
        case google::protobuf::FieldDescriptor::CPPTYPE_##cpptype: { \
    jnode[field->name()] = (jsontype)reflection->Get##method(message, field); \
    break; \
        }
        XX(INT32, Int32, int32_t, Json::Int);
        XX(UINT32, UInt32, uint32_t, Json::UInt);
        XX(FLOAT, Float, float, double);
        XX(DOUBLE, Double, double, double);
        XX(BOOL, Bool, bool, bool);
        XX(INT64, Int64, int64_t, Json::Int64);
        XX(UINT64, UInt64, uint64_t, Json::UInt64);
#undef XX
        case google::protobuf::FieldDescriptor::CPPTYPE_ENUM: {
            jnode[field->name()] = reflection->GetEnum(message, field)->number();
            break;
        }
        case google::protobuf::FieldDescriptor::CPPTYPE_STRING: {
            jnode[field->name()] = reflection->GetString(message, field);
            break;
        }
        case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE: {
            convert_proto_to_json(reflection->GetMessage(message, field), jnode[field->name()]);
            break;
        }
        }

    }
    const auto& ufs = reflection->GetUnknownFields(message);
    put_unknowfieldset_to_json(ufs, jnode);
    //succeed
    json_value[descriptor->name()] = jnode;
}

void Util::json_to_proto(const Json::Value &json_object, google::protobuf::Message &message)
{
    const google::protobuf::Descriptor* descriptor = message.GetDescriptor();
    const google::protobuf::Reflection* reflection = message.GetReflection();
    for (Json::Value::const_iterator itr = json_object.begin(); itr != json_object.end(); ++itr)
    {
        const std::string &member_name = itr.name();
        const Json::Value &node = *itr;
        if (node.isNull())
            continue;
        if (node.isArray() && node.empty())
            continue;
        const google::protobuf::FieldDescriptor *field = descriptor->FindFieldByName(member_name);
        if (field->is_repeated())
        {
            switch(field->cpp_type()) {
            case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddInt32(&message, field, node[i].asInt());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddUInt32(&message, field, node[i].asUInt());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddBool(&message, field, node[i].asBool());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddEnumValue(&message, field, node[i].asInt());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddInt64(&message, field, node[i].asInt64());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddUInt64(&message, field, node[i].asUInt64());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddFloat(&message, field, node[i].asFloat());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddDouble(&message, field, node[i].asDouble());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddString(&message, field, node[i].asString());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i) {
                    google::protobuf::Message* mutable_message = reflection->AddMessage(&message, field, nullptr);
                    json_to_proto(node[i], *mutable_message);
                }
                break;
            default:
                break;
            } //switch
            continue;
        } //if
        switch(field->cpp_type()) {
        case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
            reflection->SetInt32(&message, field, node.asInt());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
            reflection->SetUInt32(&message, field, node.asUInt());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
            reflection->SetBool(&message, field, node.asBool());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
            reflection->SetEnumValue(&message, field, node.asInt());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
            reflection->SetInt64(&message, field, node.asInt64());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
            reflection->SetUInt64(&message, field, node.asUInt64());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
            reflection->SetFloat(&message, field, node.asFloat());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
            reflection->SetDouble(&message, field, node.asDouble());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
            reflection->SetString(&message, field, node.asString());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
        {
            google::protobuf::Message* mutable_message = reflection->MutableMessage(&message, field, nullptr);
            json_to_proto(node, *mutable_message);
        }
            break;
        default:
            break;
        }//switch
    }//for
}
void Util::convert_json_to_proto(const Json::Value &json_value, google::protobuf::Message &message)
{
    if (json_value.isNull() && json_value.empty())
        return;
    std::string message_name = json_value.begin().name();
    const Json::Value &json_object = *json_value.begin();
    if (!json_object.isObject())
        return;
    const google::protobuf::Descriptor* descriptor = message.GetDescriptor();
    const google::protobuf::Reflection* reflection = message.GetReflection();
    for (Json::Value::const_iterator itr = json_object.begin(); itr != json_object.end(); ++itr)
    {
        const std::string &member_name = itr.name();
        const Json::Value &node = *itr;
        if (node.isNull())
            continue;
        if (node.isArray() && node.empty())
            continue;
        const google::protobuf::FieldDescriptor *field = descriptor->FindFieldByName(member_name);
        if (field->is_repeated())
        {
            switch(field->cpp_type()) {
            case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddInt32(&message, field, node[i].asInt());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddUInt32(&message, field, node[i].asUInt());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddBool(&message, field, node[i].asBool());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddEnumValue(&message, field, node[i].asInt());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddInt64(&message, field, node[i].asInt64());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddUInt64(&message, field, node[i].asUInt64());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddFloat(&message, field, node[i].asFloat());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddDouble(&message, field, node[i].asDouble());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i)
                    reflection->AddString(&message, field, node[i].asString());
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
                for (Json::ArrayIndex i = 0 ; i < node.size(); ++i) {
                    google::protobuf::Message* mutable_message = reflection->AddMessage(&message, field, nullptr);
                    convert_json_to_proto(node[i], *mutable_message);
                }
                break;
            default:
                break;
            } //switch
            continue;
        } //if
        switch(field->cpp_type()) {
        case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
            reflection->SetInt32(&message, field, node.asInt());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
            reflection->SetUInt32(&message, field, node.asUInt());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
            reflection->SetBool(&message, field, node.asBool());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
            reflection->SetEnumValue(&message, field, node.asInt());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
            reflection->SetInt64(&message, field, node.asInt64());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
            reflection->SetUInt64(&message, field, node.asUInt64());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
            reflection->SetFloat(&message, field, node.asFloat());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
            reflection->SetDouble(&message, field, node.asDouble());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
            reflection->SetString(&message, field, node.asString());
            break;
        case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
        {
            google::protobuf::Message* mutable_message = reflection->MutableMessage(&message, field, nullptr);
            convert_json_to_proto(node, *mutable_message);
        }
            break;
        default:
            break;
        }//switch
    }//for
}
void Util::convert_unknown_json_to_proto(const Json::Value &json_value, google::protobuf::Message **new_message)
{
    if (json_value.empty())
        return;
    std::string message_name = json_value.begin().name();
    const google::protobuf::Descriptor * descriptor = google::protobuf::DescriptorPool::generated_pool()
            ->FindMessageTypeByName(message_name);
    const google::protobuf::Message* prototype = google::protobuf::MessageFactory::generated_factory()
            ->GetPrototype(descriptor);
    if (!descriptor || !prototype)
        return;
    *new_message = prototype->New();
    if (*new_message)
        return;
    convert_json_to_proto(json_value, **new_message);
}

std::string Util::json_as_string(const Json::Value& json)
{
    Json::FastWriter w;
    return w.write(json);
}

