#include "record/schema.h"

/**
 * TODO: Student Implement
 */
uint32_t Schema::SerializeTo(char *buf) const {
  char* p = buf;
  MACH_WRITE_UINT32(buf, SCHEMA_MAGIC_NUM); buf += 4;
  MACH_WRITE_UINT32(buf, columns_.size()); buf += 4;
  for(uint32_t i = 0; i < columns_.size(); i++)
    buf += columns_[i]->SerializeTo(buf);
  MACH_WRITE_UINT32(buf, is_manage_); buf += 4;
  
  //std::cout << GetSerializedSize() << std::endl;
  //std::cout << buf-p << std::endl;
  return buf-p;
}
uint32_t Schema::GetSerializedSize() const {
  uint32_t size = 12;
  for(uint32_t i = 0; i < columns_.size(); i++)
    size += columns_[i]->GetSerializedSize();
  return size;
}
uint32_t Schema::DeserializeFrom(char *buf, Schema *&schema) {
  char* p = buf;
  uint32_t MAGIC = MACH_READ_UINT32(buf); buf += 4;
  if(MAGIC != SCHEMA_MAGIC_NUM) return -1;
  uint32_t col_size = MACH_READ_UINT32(buf); buf += 4;
  std::vector<Column *> columns;
  for(uint32_t i = 0; i < col_size; i++)
  {
    Column* col = nullptr;
    buf += Column::DeserializeFrom(buf, col);
    columns.push_back(col);
  }
  bool manage = MACH_READ_UINT32(buf); buf += 4;
  schema = new Schema(columns, manage);
  return buf-p;
}