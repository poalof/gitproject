//
// Created by njz on 2023/1/17.
//
#include "executor/executors/seq_scan_executor.h"

/**
* TODO: Student Implement
*/
SeqScanExecutor::SeqScanExecutor(ExecuteContext *exec_ctx, const SeqScanPlanNode *plan)
    : AbstractExecutor(exec_ctx),
      plan_(plan){}

void SeqScanExecutor::Init() {
  std::string table_name = plan_->GetTableName();
  TableInfo* table_info = nullptr;
  exec_ctx_->GetCatalog()->GetTable(table_name, table_info);
  TableHeap* table_heap = table_info->GetTableHeap();
  it = table_heap->Begin(nullptr);
  end = table_heap->End();
}

bool SeqScanExecutor::Next(Row *row, RowId *rid) {
  std::string table_name = plan_->GetTableName();
  TableInfo* table_info = nullptr;
  exec_ctx_->GetCatalog()->GetTable(table_name, table_info);
  TableHeap* table_heap = table_info->GetTableHeap();
  AbstractExpressionRef pred = plan_->GetPredicate();
  while(it != end)
  {
    
    if(plan_->filter_predicate_ == nullptr || Field(TypeId::kTypeInt, 1).CompareEquals(plan_->filter_predicate_->Evaluate(&(*it))))
    {
      std::vector<Field> output; 
      for (auto col : plan_->OutputSchema()->GetColumns()) 
      {
        //uint32_t col_index;
        //table_info->GetSchema()->GetColumnIndex(col->GetName(), col_index);
        output.push_back(*(*it).GetField(col->GetTableInd()));
      }
      *row = Row(output);
      row->SetRowId(*(it).GetRowId());
      *rid = row->GetRowId();
      ++it;
      return true;
    }
    ++it;
  }
  return false;
}
