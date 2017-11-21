#include <stdio.h>
#include <string.h>

#include "esp_system.h"

#include "esp_log.h"

#include "string.h"

#include "mruby.h"
#include "mruby/error.h"
#include "mruby/string.h"
#include "mruby/data.h"

#include "nvs_flash.h"

void mrb_nvs_get_struct_data(mrb_state* mrb, mrb_value data, void* _t, nvs_handle** nvs) {
  Data_Get_Struct(mrb, data, NULL, *nvs);
}

static mrb_value 
mruby_esp32_nvs_init(mrb_state* mrb, mrb_value self) {
    nvs_flash_init();	
	
	return mrb_true_value();
}

static mrb_value 
mruby_esp32_nvs_open(mrb_state* mrb, mrb_value self) {
	mrb_int mode;
	char* ns;
	
	mrb_get_args(mrb, "zi", &ns, &mode);
	
	static nvs_handle nvs;
	nvs_open(ns, mode, &nvs);
	
	return mrb_obj_value(Data_Wrap_Struct(mrb, mrb->object_class, NULL, &nvs));
}

static mrb_value 
mruby_esp32_nvs_set_numeric(mrb_state* mrb, mrb_value self) {
	mrb_value ins;
	char* key;
	mrb_int value, type;
	
	mrb_get_args(mrb, "ozii", &ins, &key, &value, &type);
	
	nvs_handle* nvs;
	mrb_nvs_get_struct_data(mrb, ins, NULL, &nvs);;
	
	esp_err_t err;
	
	switch ((int)type) {
		case 0:
		  err = nvs_set_i8((*nvs),  key, (int8_t)value);
		  break;
		case 1:
		  err = nvs_set_i16((*nvs), key, (int16_t)value);
		  break;
		case 2:
		  err = nvs_set_i32((*nvs), key, (int32_t)value);
		  break;
		case 3:
		  err = nvs_set_i64((*nvs), key, (int64_t)value);
		  break;
		case 4:
		  err = nvs_set_u8((*nvs),  key, (uint8_t)value);
		  break;
		case 5:
		  err = nvs_set_u16((*nvs), key, (uint16_t)value);
		  break;
		case 6:
		  err = nvs_set_u32((*nvs), key, (uint32_t)value);
		  break;  
		case 7:
		  err = nvs_set_u64((*nvs), key, (uint64_t)value);
		  break;
		default:
		  return mrb_nil_value();
	}
	
	if (ESP_OK == err) {
	  return mrb_true_value();
    }
    
    return mrb_false_value();
}

static mrb_value 
mruby_esp32_nvs_get_numeric(mrb_state* mrb, mrb_value self) {
	mrb_value ins, result;
	char* key;
	mrb_int type;
	
	mrb_get_args(mrb, "ozi", &ins, &key, &type);
	
	nvs_handle* nvs;
	mrb_nvs_get_struct_data(mrb, ins, NULL, &nvs);
	
	switch ((int)type) {
		case 0:
		  ;
		  int8_t i8value = 0;
		  if (ESP_OK == nvs_get_i8((*nvs),  key, &i8value)) {
		    return mrb_fixnum_value((mrb_int)i8value);
		  }
		  break;
		case 1:
		  ;
		  int16_t i16value = 0;
		  if (ESP_OK == nvs_get_i16((*nvs),  key, &i16value)) {
		    return mrb_fixnum_value((mrb_int)i16value);
		  }
		  break;
		case 2:
		  ;
		  int32_t i32value = 0;
		  if (ESP_OK == nvs_get_i32((*nvs),  key, &i32value)) {
		    return mrb_fixnum_value((mrb_int)i32value);
		  }
		  break;
		case 3:
		  ;
		  int64_t i64value = 0;
		  if (ESP_OK == nvs_get_i64((*nvs),  key, &i64value)) {
		    return mrb_fixnum_value((mrb_int)i64value);
	      }
		  break;
		case 4:
		  ;
		  uint8_t u8value = 0;
		  if (ESP_OK == nvs_get_u8((*nvs),  key, &u8value)) {
		    return mrb_fixnum_value((mrb_int)u8value);
	      }
		  break;
		case 5:
		  ;
		  uint16_t u16value = 0;
		  if (ESP_OK == nvs_get_u16((*nvs),  key, &u16value)) {
		    return mrb_fixnum_value((mrb_int)u16value);
		  }
		  break;
		case 6:
		  ;
		  uint32_t u32value = 0;
		  if (ESP_OK == nvs_get_u32((*nvs),  key, &u32value)) {
		    return mrb_fixnum_value((mrb_int)u32value);
	      }
		  break;  
		case 7:
		  ;
		  uint64_t u64value = 0;
		  if (ESP_OK == nvs_get_u64((*nvs),  key, &u64value)) {
		    return mrb_fixnum_value((mrb_int)u64value);
	      }
		  break;
		default:
		  return mrb_nil_value();
	}
	
	// err != ESP_OK
	// failed 
	return mrb_false_value();
}

static mrb_value 
mruby_esp32_nvs_set_string(mrb_state* mrb, mrb_value self) {
	mrb_value ins;
	char* key, *value;
	
	mrb_get_args(mrb, "ozz", &ins, &key, &value);
	
	nvs_handle* nvs;
	mrb_nvs_get_struct_data(mrb, ins, NULL, &nvs);
	
    if (ESP_OK == nvs_set_str((*nvs), key, value)) {
	  return mrb_true_value();
    }
    
    return mrb_false_value();
}

static mrb_value 
mruby_esp32_nvs_get_string(mrb_state* mrb, mrb_value self) {
	mrb_value ins;
	char* key;
	
	mrb_get_args(mrb, "oz", &ins, &key);
	
	nvs_handle* nvs;
	mrb_nvs_get_struct_data(mrb, ins, NULL, &nvs);
		
    size_t len;
		
    if (ESP_OK == nvs_get_str((*nvs), key, NULL, &len)) {
	  char* str = malloc(len);
	  nvs_get_str((*nvs), key, str, &len);
	  return mrb_str_new_cstr(mrb, str);
    }
    
    return mrb_false_value();
}

static mrb_value 
mruby_esp32_nvs_set_data(mrb_state* mrb, mrb_value self) {
	
	return mrb_nil_value();
}

static mrb_value 
mruby_esp32_nvs_get_data(mrb_state* mrb, mrb_value self) {
	
	return mrb_nil_value();
}

void
mrb_mruby_esp32_nvs_gem_init(mrb_state* mrb)
{ 
  struct RClass *esp32, *nvs;
    
  esp32 = mrb_define_module(mrb, "ESP32");
  nvs   = mrb_define_class_under(mrb, esp32, "NVS", mrb->object_class);	
  
  mrb_define_module_function(mrb, nvs, "init",         mruby_esp32_nvs_init,        MRB_ARGS_NONE());  

  // instance
  mrb_define_module_function(mrb, nvs, "_open",        mruby_esp32_nvs_open,        MRB_ARGS_REQ(2));  

  mrb_define_module_function(mrb, nvs, "_set_numeric", mruby_esp32_nvs_set_numeric, MRB_ARGS_REQ(4));    	
  mrb_define_module_function(mrb, nvs, "_set_string",  mruby_esp32_nvs_set_string,  MRB_ARGS_REQ(3));      
  mrb_define_module_function(mrb, nvs, "_set_data",    mruby_esp32_nvs_set_data,    MRB_ARGS_REQ(3));    

  mrb_define_module_function(mrb, nvs, "_get_numeric", mruby_esp32_nvs_get_numeric, MRB_ARGS_REQ(3));    	
  mrb_define_module_function(mrb, nvs, "_get_string",  mruby_esp32_nvs_get_string,  MRB_ARGS_REQ(2));      
  mrb_define_module_function(mrb, nvs, "_get_data",    mruby_esp32_nvs_get_data,    MRB_ARGS_REQ(2));   

  // access modes
  mrb_define_const(mrb, nvs, "READ_ONLY",   mrb_fixnum_value(NVS_READONLY));
  mrb_define_const(mrb, nvs, "READ_WRITE",  mrb_fixnum_value(NVS_READWRITE));

  // numeric data types
  mrb_define_const(mrb, nvs, "INT8",    mrb_fixnum_value(0));
  mrb_define_const(mrb, nvs, "INT16",   mrb_fixnum_value(1));
  mrb_define_const(mrb, nvs, "INT32",   mrb_fixnum_value(2));
  mrb_define_const(mrb, nvs, "INT64",   mrb_fixnum_value(3));
  mrb_define_const(mrb, nvs, "UINT8",   mrb_fixnum_value(4));
  mrb_define_const(mrb, nvs, "UINT16",  mrb_fixnum_value(5));
  mrb_define_const(mrb, nvs, "UINT32",  mrb_fixnum_value(6));
  mrb_define_const(mrb, nvs, "UINT64",  mrb_fixnum_value(7));    
}

void
mrb_mruby_esp32_nvs_gem_final(mrb_state* mrb)
{
}
