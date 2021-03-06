/*
 * Msg_Struct.cpp
 *
 *  Created on: 2016年8月2日
 *      Author: zhangyalei
 */

#include "Log.h"
#include "Msg_Struct.h"
#include "Robot_Manager.h"

Msg_Struct::Msg_Struct(Xml &xml, TiXmlNode *node) : Base_Struct(xml, node) {}

Msg_Struct::~Msg_Struct() {}

void Msg_Struct::print_msg(Block_Buffer &buffer) {
	for(std::vector<Field_Info>::const_iterator iter = field_vec().begin();
			iter != field_vec().end(); iter++) {
		if((*iter).field_label == "arg") {
			print_msg_arg((*iter), buffer);
		}
		else if((*iter).field_label == "vector" || (*iter).field_label == "map") {
			print_msg_vector((*iter), buffer);
		}
		else if((*iter).field_label == "struct") {
			print_msg_struct((*iter), buffer);
		}
	}
}

void Msg_Struct::print_msg_arg(const Field_Info &field_info, Block_Buffer &buffer) {
	if(field_info.field_type == "int8") {
		int8_t value = 0;
		buffer.read_int8(value);
		LOG_INFO("struct_name:%s, field_type:%s, field_name:%s, field_value:%d",
				struct_name().c_str(), field_info.field_type.c_str(), field_info.field_name.c_str(), value);
	}
	else if(field_info.field_type == "int16") {
		int16_t value = 0;
		buffer.read_int16(value);
		LOG_INFO("struct_name:%s, field_type:%s, field_name:%s, field_value:%d",
				struct_name().c_str(), field_info.field_type.c_str(), field_info.field_name.c_str(), value);
	}
	else if(field_info.field_type == "int32") {
		int32_t value = 0;
		buffer.read_int32(value);
		LOG_INFO("struct_name:%s, field_type:%s, field_name:%s, field_value:%d",
				struct_name().c_str(), field_info.field_type.c_str(), field_info.field_name.c_str(), value);
	}
	else if(field_info.field_type == "int64") {
		int64_t value = 0;
		buffer.read_int64(value);
		LOG_INFO("struct_name:%s, field_type:%s, field_name:%s, field_value:%ld",
				struct_name().c_str(), field_info.field_type.c_str(), field_info.field_name.c_str(), value);
	}
	else if(field_info.field_type == "double") {
		double value = 0;
		buffer.read_double(value);
		LOG_INFO("struct_name:%s, field_type:%s, field_name:%s, field_value:%lf",
				struct_name().c_str(), field_info.field_type.c_str(), field_info.field_name.c_str(), value);
	}
	else if(field_info.field_type == "bool") {
		bool value = false;
		buffer.read_bool(value);
		LOG_INFO("struct_name:%s, field_type:%s, field_name:%s, field_value:%d",
				struct_name().c_str(), field_info.field_type.c_str(), field_info.field_name.c_str(), value);
	}
	else if(field_info.field_type == "string") {
		std::string value = "";
		buffer.read_string(value);
		LOG_INFO("struct_name:%s, field_type:%s, field_name:%s, field_value:%s",
				struct_name().c_str(), field_info.field_type.c_str(), field_info.field_name.c_str(), value.c_str());
	}
	else {
		LOG_ERROR("Can not find the field_type:%s, struct_name:%s", field_info.field_type.c_str(), struct_name().c_str());
	}
}

void Msg_Struct::print_msg_vector(const Field_Info &field_info, Block_Buffer &buffer) {
	uint16_t vec_size = 0;
	buffer.read_uint16(vec_size);
	LOG_INFO("struct_name:%s, field_type:%s, field_name:%s, vec_size:%d",
			struct_name().c_str(), field_info.field_type.c_str(), field_info.field_name.c_str(), vec_size);

	if(is_struct(field_info.field_type)) {
		for(uint16_t i = 0; i < vec_size; ++i) {
			print_msg_struct(field_info, buffer);
		}
	}
	else{
		for(uint16_t i = 0; i < vec_size; ++i) {
			print_msg_arg(field_info, buffer);
		}
	}
}

void Msg_Struct::print_msg_struct(const Field_Info &field_info, Block_Buffer &buffer) {
	Robot_Manager::Struct_Name_Map::iterator iter = ROBOT_MANAGER->msg_struct_name_map().find(field_info.field_type);
	if(iter == ROBOT_MANAGER->msg_struct_name_map().end()) {
		LOG_ERROR("Can not find the struct_name:%s", field_info.field_type.c_str());
		return;
	}

	LOG_INFO("struct_name:%s, field_type:%s, field_name:%s",
			struct_name().c_str(), field_info.field_type.c_str(), field_info.field_name.c_str());
	Base_Struct *db_struct  = iter->second;
	std::vector<Field_Info> field_vec = db_struct->field_vec();
	for(std::vector<Field_Info>::const_iterator iter = field_vec.begin();
			iter != field_vec.end(); iter++) {
		if((*iter).field_label == "arg"){
			print_msg_arg(*iter, buffer);
		}
		else if((*iter).field_label == "vector" || (*iter).field_label == "map") {
			print_msg_vector(*iter, buffer);
		}
		else if((*iter).field_label == "struct") {
			print_msg_struct(*iter, buffer);
		}
	}
}
