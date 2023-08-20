//
// Created by njz on 2023/1/30.
//

#include "executor/executors/update_executor.h"

UpdateExecutor::UpdateExecutor(ExecuteContext *exec_ctx, const UpdatePlanNode *plan,
                               std::unique_ptr<AbstractExecutor> &&child_executor)
    : AbstractExecutor(exec_ctx), plan_(plan), child_executor_(std::move(child_executor)) {}

/**
* TODO: Student Implement
*/
void UpdateExecutor::Init() {
  exec_ctx_->GetCatalog()->GetTableIndexes(plan_->GetTableName(), index_info_);
  child_executor_->Init();
}

bool UpdateExecutor::Next([[maybe_unused]] Row *row, RowId *rid) {
  Row update_row;
  RowId update_rid;
  if(child_executor_->Next(&update_row, &update_rid) == false) return false;
  //printf("123\n");
  Row new_row = GenerateUpdatedTuple(update_row);
  // update table heap
  TableInfo* table_info;
  exec_ctx_->GetCatalog()->GetTable(plan_->GetTableName(), table_info);
  TableHeap* th = table_info->GetTableHeap();

  std::vector<Row> key_list;
  std::vector<Row> old_key_list;
  // delete an index, insert an index
  for(auto info: index_info_)
  {
    Index* index = info->GetIndex();
    Schema* schema = info->GetIndexKeySchema();
    std::vector<Field> key;
    std::vector<Field> old_key;
    for(auto col: schema->GetColumns())
    {
      key.push_back(*(new_row.GetField(col->GetTableInd())));
      old_key.push_back(*(update_row.GetField(col->GetTableInd())));
    }
    Row key_row(key);
    Row old_key_row(old_key);
    // 有重复索引，且rid不同 不满足唯一约束
    std::vector<RowId> res;
    index->ScanKey(key_row,res,nullptr);
    if(res.size() && res[0].Get()!=update_rid.Get()) return false;
    key_list.push_back(key_row);
    old_key_list.push_back(old_key_row);
  }

  int i = 0;
  for(auto info: index_info_)
  {
    Index* index = info->GetIndex();
    if(index->RemoveEntry(old_key_list[i], update_rid, nullptr) != DB_SUCCESS) return false;
    if(index->InsertEntry(key_list[i], new_row.GetRowId(), nullptr) != DB_SUCCESS) return false;
    i++;
  }

  if(th->UpdateTuple(new_row, update_row.GetRowId(), nullptr) == false)
    return false;
  *row = new_row;
  *rid = new_row.GetRowId();
  return true;
}

Row UpdateExecutor::GenerateUpdatedTuple(const Row &src_row) {
  std::vector<Field> new_fields;
  Row new_row(src_row);
  // init
  for(auto it: plan_->GetUpdateAttr())
  {
    // 这个map表示要更新的列和更新后的值，是常数，所以不需要row
    Field new_field = it.second->Evaluate(&src_row);
    *(new_row.GetField(it.first)) = new_field;
  }
  return new_row;
}