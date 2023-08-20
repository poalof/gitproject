#include "record/row.h"

/**
 * TODO: Student Implement
 */
uint32_t Row::SerializeTo(char *buf, Schema *schema) const {
  char* p = buf;
  MACH_WRITE_UINT32(buf, fields_.size()); buf += 4;
  char* bitmap = new char[(fields_.size()+7)/8]; //null bitmap
  memset(bitmap, 0, (fields_.size()+7)/8);
  for(uint32_t i = 0; i < fields_.size(); i++)
  {
    if(fields_[i]->IsNull()) bitmap[i/8] |= (0x1<<(7-i%8));
  }
  memcpy(buf, bitmap, (fields_.size()+7)/8); buf += (fields_.size()+7)/8; //null bitmap
  for(uint32_t i = 0; i < fields_.size(); i++)
  {
    MACH_WRITE_TO(TypeId, buf, fields_[i]->GetTypeId()); buf += sizeof(TypeId);
    buf += fields_[i]->SerializeTo(buf);
  }
  
  return buf-p;
}
uint32_t Row::DeserializeFrom(char *buf, Schema *schema) {
  char *p = buf;
  uint32_t cnt = MACH_READ_FROM(uint32_t, buf); buf += sizeof(uint32_t);
  char *btmp = new char[(cnt+7)/8];
  memcpy(btmp, buf, (cnt+7)/8); buf += (cnt+7)/8;
  for(uint32_t i = 0; i < cnt; i++)
  {
    TypeId type = MACH_READ_FROM(TypeId, buf); buf += sizeof(TypeId);
    bool isnull = (btmp[cnt/8] & (0x1<<(7-cnt%8)));
    Field *f = NULL;
    buf += Field::DeserializeFrom(buf, type, &f, isnull);
    fields_.push_back(f);
  }
  return buf-p;
}
uint32_t Row::GetSerializedSize(Schema *schema) const {
  int ofs = sizeof(uint32_t) + (fields_.size()+7)/8;
  for(long unsigned int i = 0; i < fields_.size(); i++)
    ofs += fields_[i]->GetSerializedSize();
  ofs += fields_.size() * sizeof(TypeId);
  return ofs;
}

void Row::GetKeyFromRow(const Schema *schema, const Schema *key_schema, Row &key_row) {
  auto columns = key_schema->GetColumns();
  std::vector<Field> fields;
  uint32_t idx;
  for (auto column : columns) {
    schema->GetColumnIndex(column->GetName(), idx);
    fields.emplace_back(*this->GetField(idx));
  }
  key_row = Row(fields);
}
