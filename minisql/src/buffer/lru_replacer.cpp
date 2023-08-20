#include "buffer/lru_replacer.h"

LRUReplacer::LRUReplacer(size_t num_pages)
{
  this->num_pages = num_pages;
}

LRUReplacer::~LRUReplacer() = default;

/**
 * TODO: Student Implement
 */
bool LRUReplacer::Victim(frame_id_t *frame_id) {
  if(list_map.size() == 0) return false;

  *frame_id = victim_list.front();
  list_map.erase(*frame_id);
  victim_list.pop_front();
  return true;
}

/**
 * TODO: Student Implement
 */
void LRUReplacer::Pin(frame_id_t frame_id) {
  if(list_map.find(frame_id) == list_map.end()) return ;

  victim_list.erase(list_map[frame_id]);
  list_map.erase(frame_id);
}

/**
 * TODO: Student Implement
 */
void LRUReplacer::Unpin(frame_id_t frame_id) {
  if(list_map.find(frame_id) != list_map.end()) return ;

  victim_list.push_back(frame_id);
  auto it = victim_list.end();
  it--;
  list_map[frame_id] = it;
}

/**
 * TODO: Student Implement
 */
size_t LRUReplacer::Size() {
  return list_map.size();
}