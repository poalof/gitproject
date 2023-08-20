//
// Created by njz on 2023/1/27.
//

#include "executor/executors/insert_executor.h"

InsertExecutor::InsertExecutor(ExecuteContext *exec_ctx, const InsertPlanNode *plan,
                               std::unique_ptr<AbstractExecutor> &&child_executor)
    : AbstractExecutor(exec_ctx), plan_(plan), child_executor_(std::move(child_executor)) {}

// 插入前注意如果unique的字段是否重复 ScanKey
// 插入row后同时插入索引
// 貌似只支持一次插入一行。。。

void InsertExecutor::Init() {
  child_executor_->Init();
}

bool InsertExecutor::Next([[maybe_unused]] Row *row, RowId *rid) {
  if(exec_ctx_ == nullptr) 
  {
    printf("Error occurs.\n");
    return false;
  }
  
  TableInfo* table_info;
  std::vector<IndexInfo*> index_infos;
  GetExecutorContext()->GetCatalog()->GetTable(plan_->GetTableName(), table_info);
  GetExecutorContext()->GetCatalog()->GetTableIndexes(plan_->GetTableName(), index_infos);
  
  Row new_row;
  RowId new_rid;
  child_executor_->Next(&new_row, &new_rid);
  for(auto& index_info: index_infos)
  {
    Index* index = index_info->GetIndex();
    IndexSchema* schema = index_info->GetIndexKeySchema();
    std::vector<uint32_t> attrs;
    std::vector<Column*> columns = schema->GetColumns();
    std::vector<Field> key_fields;
    for(int i = 0; i < columns.size(); i++)
      key_fields.push_back(*(new_row.GetField(columns[i]->GetTableInd())));
    std::vector<RowId> result;
    index->ScanKey(Row(key_fields), result, nullptr);
    if(result.size() != 0) 
    {
      printf("PRIMARY KEY constraint or UNIQUE constraint conflict.\n");
      return false; // has duplicate key
    }
  }
  // insert into table
  TableHeap* th = table_info->GetTableHeap();
  if(th->InsertTuple(new_row, nullptr) == false) return false;
  
  // insert all indices
  new_rid = new_row.GetRowId();
  for (auto& index_info : index_infos) 
  {
    auto index = index_info->GetIndex();
    std::vector<Field> key_fields;
    for (auto idx : index_info->GetIndexKeySchema()->GetColumns()) 
    {
      key_fields.push_back(*(new_row.GetField(idx->GetTableInd())));
    }
    Row row_idx(key_fields);
    index_info->GetIndex()->InsertEntry(row_idx, new_rid, nullptr);
  }

  return false;
}