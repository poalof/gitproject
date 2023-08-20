//
// Created by njz on 2023/1/29.
//

#include "executor/executors/delete_executor.h"

/**
* TODO: Student Implement
*/

DeleteExecutor::DeleteExecutor(ExecuteContext *exec_ctx, const DeletePlanNode *plan,
                               std::unique_ptr<AbstractExecutor> &&child_executor)
    : AbstractExecutor(exec_ctx), plan_(plan), child_executor_(std::move(child_executor)) {}

void DeleteExecutor::Init() {
  child_executor_->Init();
}

bool DeleteExecutor::Next([[maybe_unused]] Row *row, RowId *rid) {
  Row delete_row;
  RowId delete_rid;
  if(child_executor_->Next(&delete_row, &delete_rid) == false) return false;
  TableInfo* table_info;
  std::vector<IndexInfo*> indexes;
  GetExecutorContext()->GetCatalog()->GetTable(plan_->table_name_, table_info);
  GetExecutorContext()->GetCatalog()->GetTableIndexes(plan_->table_name_, indexes);
  TableHeap* th = table_info->GetTableHeap();
  // 到底要不要真的删除
  if(th->MarkDelete(delete_rid, nullptr) == false) return false;
  for (auto index : indexes) 
  {
    std::vector<Field> key;
    for (auto col : index->GetIndexKeySchema()->GetColumns()) 
    {
      key.push_back(*(delete_row.GetField(col->GetTableInd())));
    }
    index->GetIndex()->RemoveEntry(Row(key), delete_row.GetRowId(), nullptr);
  }
  return true;
}