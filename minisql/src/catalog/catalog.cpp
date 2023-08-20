#include "catalog/catalog.h"

/* 不写注释是坏文明，但估计我也不会再改了，你让让我。。。 */

void CatalogMeta::SerializeTo(char *buf) const {
    ASSERT(GetSerializedSize() <= PAGE_SIZE, "Failed to serialize catalog metadata to disk.");
    MACH_WRITE_UINT32(buf, CATALOG_METADATA_MAGIC_NUM);
    buf += 4;
    MACH_WRITE_UINT32(buf, table_meta_pages_.size());
    buf += 4;
    MACH_WRITE_UINT32(buf, index_meta_pages_.size());
    buf += 4;
    for (auto iter : table_meta_pages_) {
        MACH_WRITE_TO(table_id_t, buf, iter.first);
        buf += 4;
        MACH_WRITE_TO(page_id_t, buf, iter.second);
        buf += 4;
    }
    for (auto iter : index_meta_pages_) {
        MACH_WRITE_TO(index_id_t, buf, iter.first);
        buf += 4;
        MACH_WRITE_TO(page_id_t, buf, iter.second);
        buf += 4;
    }
}

CatalogMeta *CatalogMeta::DeserializeFrom(char *buf) {
    // check valid
    uint32_t magic_num = MACH_READ_UINT32(buf);
    buf += 4;
    ASSERT(magic_num == CATALOG_METADATA_MAGIC_NUM, "Failed to deserialize catalog metadata from disk.");
    // get table and index nums
    uint32_t table_nums = MACH_READ_UINT32(buf);
    buf += 4;
    uint32_t index_nums = MACH_READ_UINT32(buf);
    buf += 4;
    // create metadata and read value
    CatalogMeta *meta = new CatalogMeta();
    for (uint32_t i = 0; i < table_nums; i++) {
        auto table_id = MACH_READ_FROM(table_id_t, buf);
        buf += 4;
        auto table_heap_page_id = MACH_READ_FROM(page_id_t, buf);
        buf += 4;
        meta->table_meta_pages_.emplace(table_id, table_heap_page_id);
    }
    for (uint32_t i = 0; i < index_nums; i++) {
        auto index_id = MACH_READ_FROM(index_id_t, buf);
        buf += 4;
        auto index_page_id = MACH_READ_FROM(page_id_t, buf);
        buf += 4;
        meta->index_meta_pages_.emplace(index_id, index_page_id);
    }
    return meta;
}

/**
 * TODO: Student Implement
 */
uint32_t CatalogMeta::GetSerializedSize() const {
  return 12 + 8 * (table_meta_pages_.size()+index_meta_pages_.size());
}

CatalogMeta::CatalogMeta() {}

/**
 * TODO: Student Implement
 */
CatalogManager::CatalogManager(BufferPoolManager *buffer_pool_manager, LockManager *lock_manager,
                               LogManager *log_manager, bool init)
    : buffer_pool_manager_(buffer_pool_manager), lock_manager_(lock_manager), log_manager_(log_manager) {
  if(init)
    catalog_meta_ = CatalogMeta::NewInstance();
  else
  {
    char *buf = reinterpret_cast<char *>(buffer_pool_manager_->FetchPage(CATALOG_META_PAGE_ID)->GetData());
    catalog_meta_ = CatalogMeta::DeserializeFrom(buf);
    //从metadata里取出table_meta_data
    auto table_pages = catalog_meta_->GetTableMetaPages();
    for (auto &page : *table_pages) 
    {
      auto table_page = buffer_pool_manager_->FetchPage(page.second);  //取page
      auto table_info = TableInfo::Create();                      //创建table_info
      // table_meta
      TableMetadata *table_meta = nullptr;
      TableMetadata::DeserializeFrom(table_page->GetData(), table_meta);
      // table_heap
      auto table_heap = TableHeap::Create(buffer_pool_manager_, (page_id_t)table_meta->GetFirstPageId(), 
        table_meta->GetSchema(), log_manager_, lock_manager_);
      table_info->Init(table_meta, table_heap);

      table_names_.emplace(std::make_pair(table_meta->GetTableName(), page.first));
      tables_.emplace(std::make_pair(page.first, table_info));

      buffer_pool_manager_->UnpinPage(page.second, false);
    }
    //取出index_meta_data
    auto index_pages = catalog_meta_->GetIndexMetaPages();
    for (auto &page : *index_pages) 
    {
      auto index_page = buffer_pool_manager_->FetchPage(page.second);
      //index_page->RLatch();
      IndexMetadata *index_meta = nullptr;
      IndexMetadata::DeserializeFrom(index_page->GetData(), index_meta);

      auto index_info = IndexInfo::Create();
      index_info->Init(index_meta, tables_[index_meta->GetTableId()], buffer_pool_manager_);
      auto table_name = tables_[index_meta->GetTableId()]->GetTableName();

      index_names_[table_name].insert(std::make_pair(index_meta->GetIndexName(), page.first));
      indexes_.emplace(page.first, index_info);
      //index_page->RUnlatch();
      buffer_pool_manager_->UnpinPage(page.second, false);
    }
    buffer_pool_manager_->UnpinPage(CATALOG_META_PAGE_ID,false);
  }
}

CatalogManager::~CatalogManager() {
 /** After you finish the code for the CatalogManager section,
 *  you can uncomment the commented code. Otherwise it will affect b+tree test
 */
  FlushCatalogMetaPage();
  delete catalog_meta_;
  
  for (auto iter : tables_) {
    delete iter.second;
  }
  
  for (auto iter : indexes_) {
    delete iter.second;
  }
}

/**
* TODO: Student Implement
*/
dberr_t CatalogManager::CreateTable(const string &table_name, TableSchema *schema,
                                    Transaction *txn, TableInfo *&table_info) {
  if(table_names_.find(table_name) != table_names_.end()) return DB_TABLE_ALREADY_EXIST;

  // table name id
  table_id_t new_table_id = catalog_meta_->GetNextTableId();
  // table_names_
  table_names_.insert({table_name, new_table_id});
  // table
  page_id_t new_page_id;  //新页
  Page *new_page = buffer_pool_manager_->NewPage(new_page_id);
  // table_info分配内存
  table_info = TableInfo::Create();
  // 这里确实要用深拷贝，schema指针是外部传来的，你不知道他什么时候释放
  Schema* new_schema = Schema::DeepCopySchema(schema);
  TableMetadata *new_table_metadata = TableMetadata::Create(new_table_id, table_name, new_page_id, new_schema);
  TableHeap *new_table_heap = TableHeap::Create(buffer_pool_manager_, new_schema, txn, log_manager_, lock_manager_);
  table_info->Init(new_table_metadata, new_table_heap);
  // tables_
  tables_.insert({new_table_id, table_info});

  // table_meta_data   catalog_meta序列化
  char *buf = reinterpret_cast<char *>(new_page->GetData());
  new_table_metadata->SerializeTo(buf);
  catalog_meta_->table_meta_pages_.insert({new_table_id, new_page_id});
  catalog_meta_->SerializeTo(buffer_pool_manager_->FetchPage(CATALOG_META_PAGE_ID)->GetData());

  buffer_pool_manager_->UnpinPage(CATALOG_META_PAGE_ID, true);
  buffer_pool_manager_->UnpinPage(new_page_id, true);

  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */
dberr_t CatalogManager::GetTable(const string &table_name, TableInfo *&table_info) {
  if(table_names_.find(table_name) == table_names_.end()) return DB_TABLE_NOT_EXIST;
  return GetTable(table_names_[table_name], table_info);
}

/**
 * TODO: Student Implement
 */
dberr_t CatalogManager::GetTables(vector<TableInfo *> &tables) const {
  //if(tables_.size() == 0) return DB_TABLE_NOT_EXIST;
  for(auto &table: tables_)
  {
    tables.emplace_back(table.second);
  }
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */
dberr_t CatalogManager::CreateIndex(const std::string &table_name, const string &index_name,
                                    const std::vector<std::string> &index_keys, Transaction *txn,
                                    IndexInfo *&index_info, const string &index_type) {
  if(table_names_.find(table_name) == table_names_.end()) return DB_TABLE_NOT_EXIST;
  if(index_names_[table_name].count(index_name)!=0) return DB_INDEX_ALREADY_EXIST;
  
  auto schema = tables_.at(table_names_.at(table_name))->GetSchema();
  std::vector<uint32_t> key_map;
  for(auto col_name: index_keys)
  {
    uint32_t val;
    if(schema->GetColumnIndex(col_name, val) == DB_COLUMN_NAME_NOT_EXIST) return DB_COLUMN_NAME_NOT_EXIST;
    key_map.emplace_back(val);
  }

  table_id_t new_index_id = catalog_meta_->GetNextIndexId();
  std::unordered_map<std::string, index_id_t> index_map;
  index_map[index_name] = new_index_id;
  index_names_[table_name] = index_map;
  page_id_t new_page_id;
  Page *new_page = buffer_pool_manager_->NewPage(new_page_id);
  index_info = IndexInfo::Create();

  IndexMetadata *new_index_metadata = IndexMetadata::Create(new_index_id, index_name, table_names_[table_name], key_map);
  index_info->Init(new_index_metadata, tables_[table_names_[table_name]], buffer_pool_manager_);
  indexes_.insert({new_index_id, index_info});

  char *buf = reinterpret_cast<char *>(new_page->GetData());
  new_index_metadata->SerializeTo(buf);
  catalog_meta_->index_meta_pages_.insert({new_index_id, new_page_id});
  catalog_meta_->SerializeTo(buffer_pool_manager_->FetchPage(CATALOG_META_PAGE_ID)->GetData());

  buffer_pool_manager_->UnpinPage(CATALOG_META_PAGE_ID, true);
  buffer_pool_manager_->UnpinPage(new_page_id, true);

  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */
dberr_t CatalogManager::GetIndex(const std::string &table_name, const std::string &index_name,
                                 IndexInfo *&index_info) const {
  if(index_names_.find(table_name) == index_names_.end()) return DB_INDEX_NOT_FOUND;
  auto index_map = index_names_.at(table_name);
  if(index_map.find(index_name) == index_map.end()) return DB_INDEX_NOT_FOUND;
  index_id_t index_id = index_map[index_name];
  if(indexes_.find(index_id) == indexes_.end()) return DB_INDEX_NOT_FOUND;
  index_info = indexes_.find(index_id)->second;
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */
dberr_t CatalogManager::GetTableIndexes(const std::string &table_name, std::vector<IndexInfo *> &indexes) const {
  if(table_names_.count(table_name) == 0) return DB_TABLE_NOT_EXIST;
  if(index_names_.count(table_name) == 0) return DB_INDEX_NOT_FOUND;
  auto index_map = index_names_.at(table_name);
  for(auto index: index_map)
  {
    indexes.emplace_back(indexes_.at(index.second));
  }
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */
dberr_t CatalogManager::DropTable(const string &table_name) {
  if(table_names_.count(table_name) == 0) return DB_TABLE_NOT_EXIST;
  table_id_t table_id = table_names_[table_name];
  TableInfo* table_info = tables_[table_id];

  if (catalog_meta_->table_meta_pages_.count(table_id) == 0) return DB_FAILED;
  buffer_pool_manager_->DeletePage(catalog_meta_->table_meta_pages_[table_id]);
  catalog_meta_->table_meta_pages_.erase(table_id);
  table_names_.erase(table_name);
  tables_.erase(table_id);
  table_info->GetTableHeap()->FreeTableHeap();
  delete table_info;
  auto page = buffer_pool_manager_->FetchPage(CATALOG_META_PAGE_ID);
  catalog_meta_->SerializeTo(page->GetData());
  buffer_pool_manager_->UnpinPage(CATALOG_META_PAGE_ID, true);
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */
dberr_t CatalogManager::DropIndex(const string &table_name, const string &index_name) {
  if(table_names_.count(table_name) == 0) return DB_TABLE_NOT_EXIST;
  if(index_names_[table_name].count(index_name) == 0) return DB_INDEX_NOT_FOUND;
  index_id_t index_id = index_names_[table_name][index_name];
  IndexInfo* index_info = indexes_[index_id];
  if(!catalog_meta_->DeleteIndexMetaPage(buffer_pool_manager_, index_id)) return DB_FAILED;
  index_names_[table_name].erase(index_name);
  if(index_names_[table_name].size() == 0)
    index_names_.erase(table_name);
  indexes_.erase(index_id);
  index_info->GetIndex()->Destroy();
  delete index_info;
  auto page = buffer_pool_manager_->FetchPage(CATALOG_META_PAGE_ID);
  catalog_meta_->SerializeTo(page->GetData());
  buffer_pool_manager_->UnpinPage(CATALOG_META_PAGE_ID, true);
  return DB_SUCCESS; 
}

/**
 * TODO: Student Implement
 */
dberr_t CatalogManager::FlushCatalogMetaPage() const {
  auto page = buffer_pool_manager_->FetchPage(CATALOG_META_PAGE_ID);
  catalog_meta_->SerializeTo(page->GetData());
  buffer_pool_manager_->UnpinPage(CATALOG_META_PAGE_ID, true);
  buffer_pool_manager_->FlushPage(CATALOG_META_PAGE_ID);
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */
dberr_t CatalogManager::LoadTable(const table_id_t table_id, const page_id_t page_id) {
 // is this just an auxiliary method? 
}

/**
 * TODO: Student Implement
 */
dberr_t CatalogManager::LoadIndex(const index_id_t index_id, const page_id_t page_id) {
  
}

/**
 * TODO: Student Implement
 */
dberr_t CatalogManager::GetTable(const table_id_t table_id, TableInfo *&table_info) {
  if(tables_.find(table_id) == tables_.end()) return DB_TABLE_NOT_EXIST;

  table_info = tables_[table_id];
  return DB_SUCCESS;
}