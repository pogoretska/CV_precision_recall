#ifndef _OBJ_CLASS_DEFINED
#define _OBJ_CLASS_DEFINED

#include <map>
#include <string>

/* enum to store detected object classes */
enum class ObjectClass : char { 
  bus, 
  bycicle, 
  person, 
  car 
};

/* simple map string -> enum */
static std::map<std::string, ObjectClass> class_map = {
  {"bus", ObjectClass::bus}, 
  {"bycicle", ObjectClass::bycicle}, 
  {"person", ObjectClass::person}, 
  {"car", ObjectClass::car}
};

#endif