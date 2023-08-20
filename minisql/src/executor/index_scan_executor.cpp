#include "executor/executors/index_scan_executor.h"
#include <algorithm>
#include <set>
#include <iterator>

/**
* TODO: Student Implement
*/
IndexScanExecutor::IndexScanExecutor(ExecuteContext *exec_ctx, const IndexScanPlanNode *plan)
    : AbstractExecutor(exec_ctx), plan_(plan) {}

// 我逐渐理解一切......
// 不我不理解
// 1. 给定的谓词关系只能有and，如果索引数量大于1，查找结果一定需要取交集
// 2. 为了遍历所有嵌套的谓词需要递归child node，储存scan用到的变量
// 3. 规则是：<column> <compare_op> <constant> 其它一律略过（逻辑只有与，单独的其他节点没有意义）
// 4. 规则只对索引生效，即如果b不是索引，b<2 满足条件也不能用索引搜索
// 5. 取set_intersection后，如果need_filter，直接调用expr的evaluate（这个函数好像是自动递归执行的）

void IndexScanExecutor::GetMap(const AbstractExpressionRef &filter_predicate_, std::map<uint32_t, std::pair<Field, std::string>>& col_map) 
{
  // e.g. a > 1
  if (filter_predicate_->GetType() == ExpressionType::ComparisonExpression &&
      filter_predicate_->GetChildAt(0)->GetType() == ExpressionType::ColumnExpression &&
      filter_predicate_->GetChildAt(1)->GetType() == ExpressionType::ConstantExpression) 
  {
    auto column_value_expr = std::dynamic_pointer_cast<ColumnValueExpression>(filter_predicate_->GetChildAt(0));
    auto constant_value_expr = std::dynamic_pointer_cast<ConstantValueExpression>(filter_predicate_->GetChildAt(1));
    auto comparison_expr = std::dynamic_pointer_cast<ComparisonExpression>(filter_predicate_);
    uint32_t col_idx = column_value_expr->GetColIdx();
    std::pair<Field, std::string> pair_(constant_value_expr->val_, comparison_expr->GetComparisonType());
    //m0.insert(map<uint32_t, pair<string, Field>>::value_type(col_idx, pair_));
    col_map.insert(std::map<uint32_t, std::pair<Field, std::string>>::value_type(col_idx, pair_)); // add mapping
  }
  else
    if(filter_predicate_->GetChildAt(0) != nullptr) 
    {
      GetMap(filter_predicate_->GetChildAt(0), col_map);
      GetMap(filter_predicate_->GetChildAt(1), col_map);
    }
}
// ScanKey(const Row &key, std::vector<RowId> &result, Transaction *txn,
//                          string compare_operator = "=") = 0;
// index, field, comp_op
void IndexScanExecutor::Init() {
  std::map<uint32_t, std::pair<Field, std::string>> col_map; // a(col_num) >(op) 1(field)
  GetMap(plan_->GetPredicate(), col_map);
  std::vector<uint32_t> index_col; // index[0] -> col_num[0]
  for (uint32_t i = 0; i < plan_->indexes_.size(); ++i) 
  {
    std::string index_name_ = plan_->indexes_[i]->GetIndexName();
    uint32_t col_idx = (plan_->indexes_[i]->GetIndexKeySchema()->GetColumns())[0]->GetTableInd();
    index_col.push_back(col_idx);
  }
  // init result set
  std::vector<Field> key_field;
  key_field.push_back((col_map.at(index_col[0])).first); // key_row
  Row key_row = Row(key_field);
  std::string compare_op = (col_map.at(index_col[0])).second;
  (plan_->indexes_)[0]->GetIndex()->ScanKey(key_row, results, nullptr, compare_op);
  sort(results.begin(), results.end(), cmp);
  for (int i = 1; i < plan_->indexes_.size(); i++) 
  {
    std::vector<RowId> tmp_res;
    key_field.clear();
    key_field.push_back((col_map.at(index_col[i])).first);
    key_row = Row(key_field);
    compare_op = (col_map.at(index_col[i])).second;
    (plan_->indexes_)[i]->GetIndex()->ScanKey(key_row, tmp_res, nullptr, compare_op);
    sort(tmp_res.begin(), tmp_res.end(), cmp);
    auto iter = set_intersection(tmp_res.begin(), tmp_res.end(), results.begin(), results.end(), results.begin(), cmp);
    results.resize(iter - results.begin());
  }

  if(plan_->need_filter_) 
    for(int i = 0; i < results.size(); i++)
    {
      TableInfo* table_info;
      exec_ctx_->GetCatalog()->GetTable(plan_->GetTableName(), table_info);
      Row row(results[i]);
      table_info->GetTableHeap()->GetTuple(&row, nullptr);
      if(plan_->GetPredicate()->Evaluate(&row).CompareEquals(Field(kTypeInt, 1)) == kFalse)
      {
        results.erase(results.begin()+i); 
        i--;
      }
    }
    
}

bool IndexScanExecutor::Next(Row *row, RowId *rid) 
{
  if (count == results.size()) return false;
  else 
  {
    TableInfo *table_info_;
    GetExecutorContext()->GetCatalog()->GetTable(plan_->GetTableName(), table_info_);
    row->SetRowId(results[count]);
    row->GetFields().clear();
    table_info_->GetTableHeap()->GetTuple(row, nullptr);
    *rid = results[count];
    count++;
  }
  return true;
}
