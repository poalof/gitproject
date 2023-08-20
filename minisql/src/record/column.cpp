#include "record/column.h"
#include <iostream>
#include "glog/logging.h"

Column::Column(std::string column_name, TypeId type, uint32_t index, bool nullable, bool unique)
    : name_(std::move(column_name)), type_(type), table_ind_(index), nullable_(nullable), unique_(unique) {
  ASSERT(type != TypeId::kTypeChar, "Wrong constructor for CHAR type.");
  switch (type) {
    case TypeId::kTypeInt:
      len_ = sizeof(int32_t);
      break;
    case TypeId::kTypeFloat:
      len_ = sizeof(float_t);
      break;
    default:
      ASSERT(false, "Unsupported column type.");
  }
}

Column::Column(std::string column_name, TypeId type, uint32_t length, uint32_t index, bool nullable, bool unique)
    : name_(std::move(column_name)),
      type_(type),
      len_(length),
      table_ind_(index),
      nullable_(nullable),
      unique_(unique) {
  ASSERT(type == TypeId::kTypeChar, "Wrong constructor for non-VARCHAR type.");
}

Column::Column(const Column *other)
    : name_(other->name_),
      type_(other->type_),
      len_(other->len_),
      table_ind_(other->table_ind_),
      nullable_(other->nullable_),
      unique_(other->unique_) {}

/**
* TODO: Student Implement
*/
uint32_t Column::SerializeTo(char *buf) const {
  char* p = buf;
  MACH_WRITE_UINT32(buf, COLUMN_MAGIC_NUM); buf += 4;
  MACH_WRITE_UINT32(buf, name_.length()); buf += 4;
  MACH_WRITE_STRING(buf, name_); buf += name_.length();
  MACH_WRITE_UINT32(buf, type_); buf += 4;
  MACH_WRITE_UINT32(buf, len_); buf += 4;
  MACH_WRITE_UINT32(buf, table_ind_); buf += 4;
  MACH_WRITE_UINT32(buf, nullable_); buf += 4;
  MACH_WRITE_UINT32(buf, unique_); buf += 4;
  
  //std::cout << GetSerializedSize() << std::endl;
  //std::cout << buf-p << std::endl;

  return buf-p;
}

/**
 * TODO: Student Implement
 */
uint32_t Column::GetSerializedSize() const {
  uint32_t size0 = 28;
  uint32_t size_name = name_.length();
  return size0+size_name;
}

/**
 * TODO: Student Implement
 */
uint32_t Column::DeserializeFrom(char *buf, Column *&column) {
  if (column != nullptr) {
    LOG(WARNING) << "Pointer to column is not null in column deserialize." 									 << std::endl;
  }
  /* deserialize field from buf */
	char* p = buf;
  uint32_t MAGIC = MACH_READ_UINT32(buf); buf += 4;
  if(MAGIC != COLUMN_MAGIC_NUM) return -1;
  int name_len = MACH_READ_UINT32(buf); buf += 4;
  char temp[name_len+1]{0};
  memcpy(temp, buf, name_len);
  std::string column_name = temp; buf += name_len;
  uint32_t itype = MACH_READ_UINT32(buf); buf += 4;
  TypeId type = TypeId(itype);
  int col_len = MACH_READ_UINT32(buf); buf += 4;
  int col_ind = MACH_READ_UINT32(buf); buf += 4;
  bool nullable = MACH_READ_UINT32(buf); buf += 4;
  bool unique = MACH_READ_UINT32(buf); buf += 4;

  /* allocate object */
  if (type == kTypeChar) {
    column = new Column(column_name, type, col_len, col_ind, nullable, unique);
  } else {
    column = new Column(column_name, type, col_ind, nullable, unique);
  }
  return buf-p;
}
