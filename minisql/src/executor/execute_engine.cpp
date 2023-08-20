#include "executor/execute_engine.h"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <chrono>

#include "common/result_writer.h"
#include "executor/executors/delete_executor.h"
#include "executor/executors/index_scan_executor.h"
#include "executor/executors/insert_executor.h"
#include "executor/executors/seq_scan_executor.h"
#include "executor/executors/update_executor.h"
#include "executor/executors/values_executor.h"
#include "glog/logging.h"
#include "planner/planner.h"
#include "utils/utils.h"
#include "parser/syntax_tree_printer.h"
#include "utils/tree_file_mgr.h"

extern "C" {
int yyparse(void);
#include "parser/minisql_lex.h"
#include "parser/parser.h"
}

#include <set>

ExecuteEngine::ExecuteEngine() {
  char path[] = "./databases";
  DIR *dir;
  if((dir = opendir(path)) == nullptr) {
    mkdir("./databases", 0777);
    dir = opendir(path);
  }
  /** When you have completed all the code for
   *  the test, run it using main.cpp and uncomment
   *  this part of the code.
   */
  
  struct dirent *stdir;
  while((stdir = readdir(dir)) != nullptr) {
    if( strcmp( stdir->d_name , "." ) == 0 ||
        strcmp( stdir->d_name , "..") == 0 ||
        stdir->d_name[0] == '.')
      continue;
    dbs_[stdir->d_name] = new DBStorageEngine(stdir->d_name, false);
  }

  closedir(dir);
}

std::unique_ptr<AbstractExecutor> ExecuteEngine::CreateExecutor(ExecuteContext *exec_ctx,
                                                                const AbstractPlanNodeRef &plan) {
  switch (plan->GetType()) {
    // Create a new sequential scan executor
    case PlanType::SeqScan: {
      return std::make_unique<SeqScanExecutor>(exec_ctx, dynamic_cast<const SeqScanPlanNode *>(plan.get()));
    }
    // Create a new index scan executor
    case PlanType::IndexScan: {
      return std::make_unique<IndexScanExecutor>(exec_ctx, dynamic_cast<const IndexScanPlanNode *>(plan.get()));
    }
    // Create a new update executor
    case PlanType::Update: {
      auto update_plan = dynamic_cast<const UpdatePlanNode *>(plan.get());
      auto child_executor = CreateExecutor(exec_ctx, update_plan->GetChildPlan());
      return std::make_unique<UpdateExecutor>(exec_ctx, update_plan, std::move(child_executor));
    }
      // Create a new delete executor
    case PlanType::Delete: {
      auto delete_plan = dynamic_cast<const DeletePlanNode *>(plan.get());
      auto child_executor = CreateExecutor(exec_ctx, delete_plan->GetChildPlan());
      return std::make_unique<DeleteExecutor>(exec_ctx, delete_plan, std::move(child_executor));
    }
    case PlanType::Insert: {
      auto insert_plan = dynamic_cast<const InsertPlanNode *>(plan.get());
      auto child_executor = CreateExecutor(exec_ctx, insert_plan->GetChildPlan());
      return std::make_unique<InsertExecutor>(exec_ctx, insert_plan, std::move(child_executor));
    }
    case PlanType::Values: {
      return std::make_unique<ValuesExecutor>(exec_ctx, dynamic_cast<const ValuesPlanNode *>(plan.get()));
    }
    default:
      throw std::logic_error("Unsupported plan type.");
  }
}

dberr_t ExecuteEngine::ExecutePlan(const AbstractPlanNodeRef &plan, std::vector<Row> *result_set, Transaction *txn,
                                   ExecuteContext *exec_ctx) {
  // Construct the executor for the abstract plan node
  auto executor = CreateExecutor(exec_ctx, plan);

  try {
    executor->Init();
    RowId rid{};
    Row row{};
    while (executor->Next(&row, &rid)) {
      if (result_set != nullptr) {
        result_set->push_back(row);
      }
    }
  } catch (const exception &ex) {
    std::cout << "Error Encountered in Executor Execution: " << ex.what() << std::endl;
    if (result_set != nullptr) {
      result_set->clear();
    }
    return DB_FAILED;
  }
  return DB_SUCCESS;
}

dberr_t ExecuteEngine::Execute(pSyntaxNode ast) {
  if (ast == nullptr) {
    return DB_FAILED;
  }
  auto start_time = std::chrono::system_clock::now();
  unique_ptr<ExecuteContext> context(nullptr);
  if(!current_db_.empty())
    context = dbs_[current_db_]->MakeExecuteContext(nullptr);
  switch (ast->type_) {
    case kNodeCreateDB:
      return ExecuteCreateDatabase(ast, context.get());
    case kNodeDropDB:
      return ExecuteDropDatabase(ast, context.get());
    case kNodeShowDB:
      return ExecuteShowDatabases(ast, context.get());
    case kNodeUseDB:
      return ExecuteUseDatabase(ast, context.get());
    case kNodeShowTables:
      return ExecuteShowTables(ast, context.get());
    case kNodeCreateTable:
      return ExecuteCreateTable(ast, context.get());
    case kNodeDropTable:
      return ExecuteDropTable(ast, context.get());
    case kNodeShowIndexes:
      return ExecuteShowIndexes(ast, context.get());
    case kNodeCreateIndex:
      return ExecuteCreateIndex(ast, context.get());
    case kNodeDropIndex:
      return ExecuteDropIndex(ast, context.get());
    case kNodeTrxBegin:
      return ExecuteTrxBegin(ast, context.get());
    case kNodeTrxCommit:
      return ExecuteTrxCommit(ast, context.get());
    case kNodeTrxRollback:
      return ExecuteTrxRollback(ast, context.get());
    case kNodeExecFile:
      return ExecuteExecfile(ast, context.get());
    case kNodeQuit:
      return ExecuteQuit(ast, context.get());
    default:
      break;
  }
  // Plan the query.
  Planner planner(context.get());
  std::vector<Row> result_set{};
  try {
    planner.PlanQuery(ast);
    // Execute the query.
    ExecutePlan(planner.plan_, &result_set, nullptr, context.get());
  } catch (const exception &ex) {
    std::cout << "Error Encountered in Planner: " << ex.what() << std::endl;
    return DB_FAILED;
  }
  auto stop_time = std::chrono::system_clock::now();
  double duration_time =
      double((std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time)).count());
  // Return the result set as string.
  std::stringstream ss;
  ResultWriter writer(ss);

  if (planner.plan_->GetType() == PlanType::SeqScan || planner.plan_->GetType() == PlanType::IndexScan) {
    auto schema = planner.plan_->OutputSchema();
    auto num_of_columns = schema->GetColumnCount();
    if (!result_set.empty()) {
      // find the max width for each column
      vector<int> data_width(num_of_columns, 0);
      for (const auto &row : result_set) {
        for (uint32_t i = 0; i < num_of_columns; i++) {
          data_width[i] = max(data_width[i], int(row.GetField(i)->toString().size()));
        }
      }
      int k = 0;
      for (const auto &column : schema->GetColumns()) {
        data_width[k] = max(data_width[k], int(column->GetName().length()));
        k++;
      }
      // Generate header for the result set.
      writer.Divider(data_width);
      k = 0;
      writer.BeginRow();
      for (const auto &column : schema->GetColumns()) {
        writer.WriteHeaderCell(column->GetName(), data_width[k++]);
      }
      writer.EndRow();
      writer.Divider(data_width);

      // Transforming result set into strings.
      for (const auto &row : result_set) {
        writer.BeginRow();
        for (uint32_t i = 0; i < schema->GetColumnCount(); i++) {
          writer.WriteCell(row.GetField(i)->toString(), data_width[i]);
        }
        writer.EndRow();
      }
      writer.Divider(data_width);
    }
    writer.EndInformation(result_set.size(), duration_time, true);
  } else {
    writer.EndInformation(result_set.size(), duration_time, false);
  }
  std::cout << writer.stream_.rdbuf();
  return DB_SUCCESS;
}

void ExecuteEngine::ExecuteInformation(dberr_t result) {
  switch (result) {
    case DB_ALREADY_EXIST:
      cout << "Database already exists." << endl;
      break;
    case DB_NOT_EXIST:
      cout << "Database not exists." << endl;
      break;
    case DB_TABLE_ALREADY_EXIST:
      cout << "Table already exists." << endl;
      break;
    case DB_TABLE_NOT_EXIST:
      cout << "Table not exists." << endl;
      break;
    case DB_INDEX_ALREADY_EXIST:
      cout << "Index already exists." << endl;
      break;
    case DB_INDEX_NOT_FOUND:
      cout << "Index not exists." << endl;
      break;
    case DB_COLUMN_NAME_NOT_EXIST:
      cout << "Column not exists." << endl;
      break;
    case DB_KEY_NOT_FOUND:
      cout << "Key not exists." << endl;
      break;
    case DB_QUIT:
      cout << "Bye." << endl;
      break;
    default:
      break;
  }
}
/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteCreateDatabase(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteCreateDatabase" << std::endl;
#endif
  pSyntaxNode tmp = ast->child_;
  std::string db_name = tmp->val_;
  if (dbs_.find(db_name) != dbs_.end()) {
    return DB_ALREADY_EXIST;
  } else {
    DBStorageEngine *engine = new DBStorageEngine(db_name, true, DEFAULT_BUFFER_POOL_SIZE);
    dbs_.insert({db_name, engine});
    printf("Create database %s.\n", db_name.c_str());
    return DB_SUCCESS;
  }
}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteDropDatabase(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteDropDatabase" << std::endl;
#endif
  pSyntaxNode tmp = ast->child_;
  std::string db_name = tmp->val_;
  if (dbs_.find(db_name) == dbs_.end()) {
    return DB_NOT_EXIST;
  } else {
    delete dbs_[db_name]; // 删除数据库文件实例
    std::string name = "./databases/" + db_name;
    remove(name.c_str()); // 删除磁盘上的数据库文件
    dbs_.erase(db_name);
    if(db_name == current_db_) current_db_ = "";
    printf("Drop database %s.\n", db_name.c_str());
    return DB_SUCCESS;
  }
}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteShowDatabases(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteShowDatabases" << std::endl;
#endif
  for(auto it: dbs_)
  {
    printf("%s\n", it.first.c_str());
  }
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteUseDatabase(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteUseDatabase" << std::endl;
#endif
  pSyntaxNode tmp = ast->child_;
  std::string db_name = tmp->val_;
  auto iter = dbs_.find(db_name);
  if (iter == dbs_.end()) {
    return DB_NOT_EXIST;
  } else {
    current_db_ = db_name;
    printf("Database changed.\n");
    return DB_SUCCESS;
  }
}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteShowTables(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteShowTables" << std::endl;
#endif
  if (current_db_ == "") 
  {
    printf("No database selected.\n");
    return DB_FAILED;
  }
  DBStorageEngine *engine = (dbs_.find(current_db_))->second;
  CatalogManager *cata = engine->catalog_mgr_;
  std::vector<TableInfo *> tables;
  cata->GetTables(tables);
  for (auto iter: tables)
    printf("%s\n", (*iter).GetTableName().c_str());
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteCreateTable(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteCreateTable" << std::endl;
#endif
  if (current_db_ == "") 
  {
    printf("No database selected.\n");
    return DB_FAILED;
  }
  DBStorageEngine *engine = (dbs_.find(current_db_))->second;
  CatalogManager *cata = engine->catalog_mgr_;
  std::string table_name = ast->child_->val_;
  TableInfo *table_info = nullptr;
  pSyntaxNode col_def = ast->child_->next_; // column definition
  std::vector<Column *> columns;
  std::unordered_set<std::string> primary;
  std::string column_name;
  TypeId type_;
  uint32_t index = 0;
  uint32_t len = 0;
  pSyntaxNode col = col_def->child_;
  while(col != nullptr) // scan for primary keys
  {
    if(col->type_ == kNodeColumnList)
    {
      pSyntaxNode key = col->child_;
      while(key != nullptr)
      {
        primary.insert(key->val_);
        key = key->next_;
      }
    }
    col = col->next_;
  }
  bool unique; // 很奇怪parser里没有nullable对应的语法，除主键全设置成true？
  bool nullable;
  std::vector<std::string> index_column;
  col = col_def->child_;
  while (col != nullptr)
  {
    if(col->type_ == kNodeColumnList)
    {
      col = col->next_;
      continue;
    }
    unique = false;
    nullable = true;
    if (col->val_ != NULL && strcmp(col->val_, "unique") == 0) 
      unique = true; // unique
    pSyntaxNode attr = col->child_; //属性名
    column_name = attr->val_;
    if(primary.count(column_name)!=0)
    {
      unique = true;
      nullable = false;
    }
    if(unique) // 要对unique键自动建立索引。。。
      index_column.push_back(column_name);
    //printf("index=%d\n", index_column.size());

    pSyntaxNode type = attr->next_; // 类型
    if (type->val_ == std::string("char")) // char
    {
      type_ = kTypeChar;
      pSyntaxNode size = type->child_;
      std::string check = size->val_;
      if(check.find("-")!=check.npos || check.find(".")!=check.npos)
      {
        printf("Invalid length of char type.\n");
        return DB_FAILED;
      }
      len = atoi(size->val_);
      if(len < 0)
      {
        printf("Invalid length of char type.\n");
        return DB_FAILED;
      } 
      Column *cur_col = new Column(column_name, type_, len, index, nullable, unique);
      columns.push_back(cur_col);
    }
    else 
    {
      if (type->val_ == std::string("int")) // int
        type_ = kTypeInt;
      else if (type->val_ == std::string("float")) //float
        type_ = kTypeFloat;
      Column *cur_col = new Column(column_name, type_, index, nullable, unique);
      columns.push_back(cur_col);
    }
    index++;
    col = col->next_;
  }
  Schema *schema = new Schema(columns);
  dberr_t state = cata->CreateTable(table_name, schema, nullptr, table_info);
  if (state == DB_SUCCESS) 
  {
    vector<std::string> primary_index_key;
    IndexInfo *index_info;
    for(auto column : index_column) // unique index
    {
      vector<std::string> index_key;
      if(primary.count(column)!=0)
      {
        primary_index_key.emplace_back(column);
        continue;
      }
      index_key.push_back(column);
      cata->CreateIndex(table_name, column, index_key, nullptr, index_info, "bptree");
    }
    //printf("primary_index_key.size()=%d\n", primary_index_key.size());
    if(primary_index_key.size()!=0) // 主键索引
      cata->CreateIndex(table_name, "primary_index", primary_index_key, nullptr, index_info, "bptree");
    printf("Create table %s.\n", table_name.c_str());
    return DB_SUCCESS;
  } 
  else 
  {
    return state;
  }
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteDropTable(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteDropTable" << std::endl;
#endif
  if (current_db_ == "") 
  {
    printf("No database selected.\n");
    return DB_FAILED;
  }
  std::string table_name = ast->child_->val_;
  CatalogManager* cata = context->GetCatalog();
  dberr_t state = cata->DropTable(table_name);;
  if(state != DB_SUCCESS) ExecuteInformation(state);
  return state;
}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteShowIndexes(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteShowIndexes" << std::endl;
#endif
  if (current_db_ == "") 
  {
    printf("No database selected.\n");
    return DB_FAILED;
  }
  CatalogManager *cata = context->GetCatalog();
  std::vector<TableInfo *> tables;
  std::vector<IndexInfo *> indexes;
  dberr_t state = cata->GetTables(tables);
  if (state != DB_SUCCESS) 
  {
    return state;
  }
  else 
  {
    for (auto& table : tables)
    {
      indexes.clear();
      auto code = cata->GetTableIndexes(table->GetTableName(), indexes);
      //if (code == DB_SUCCESS) 
      //{
        printf("Indices Of Table %s:\n", table->GetTableName().c_str());
        for (auto index : indexes)
          printf("%s\n", index->GetIndexName().c_str());
      //}
      //else
      //  return code;
    }
  }
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteCreateIndex(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteCreateIndex" << std::endl;
#endif
  if (current_db_ == "") 
  {
    printf("No database selected.\n");
    return DB_FAILED;
  }
  CatalogManager *cata = context->GetCatalog();
  // child0 index_name child1 table_name child2 index_key_node
  // index_key_node: val "index keys" child column_list
  // 只有b+树索引！
  std::string index_name = ast->child_->val_;
  std::string table_name = ast->child_->next_->val_;
  pSyntaxNode col_list = ast->child_->next_->next_->child_;
  std::vector<std::string> column_names;
  auto ptr = col_list;
  while(ptr != nullptr)
  {
    column_names.emplace_back(ptr->val_);
    ptr = ptr->next_;
  }
  IndexInfo* index_info = nullptr;
  auto state = cata->CreateIndex(table_name, index_name, column_names, nullptr, index_info, "bptree");

  if(state != DB_SUCCESS) 
  {
    return state;
  }
  else // 创建b+树。。。这一步是不是可以放到catalog manager里。。。
  {
    TableInfo *table_info = nullptr;
    cata->GetTable(table_name, table_info);
    TableHeap *table_heap = table_info->GetTableHeap();
    TableIterator iter = table_heap->Begin(nullptr);
    Index *index_ = index_info->GetIndex();
    Schema* key_schema = index_info->GetIndexKeySchema();
    while (iter != table_heap->End()) // 所有row拿出来建立b+树
    {
      std::vector<Field> fields;
      for (auto col : key_schema->GetColumns())
        fields.push_back(*(*iter).GetField(col->GetTableInd()));
      Row key_row(fields);
      auto code = index_->InsertEntry(key_row, *iter.GetRowId(), nullptr);
      if(code != DB_SUCCESS) return code;
      ++iter;
    }
    printf("Create index %s.\n", index_name.c_str());
  }
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteDropIndex(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteDropIndex" << std::endl;
#endif
  if (current_db_ == "") 
  {
    printf("No database selected.\n");
    return DB_FAILED;
  }
  CatalogManager *cata = context->GetCatalog();
  std::string index_name = ast->child_->val_;
  std::vector<TableInfo *> tables;
  cata->GetTables(tables);
  for (auto table : tables) 
  {
    IndexInfo *tmp;
    if (cata->GetIndex(table->GetTableName(), index_name, tmp) == DB_SUCCESS) 
    {
      cata->DropIndex(table->GetTableName(), index_name);
      printf("Drop index %s.\n", index_name.c_str());
      return DB_SUCCESS;
    }
  }
  return DB_INDEX_NOT_FOUND;
}

// wo bu xie
dberr_t ExecuteEngine::ExecuteTrxBegin(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteTrxBegin" << std::endl;
#endif
  return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteTrxCommit(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteTrxCommit" << std::endl;
#endif
  return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteTrxRollback(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteTrxRollback" << std::endl;
#endif
  return DB_FAILED;
}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteExecfile(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteExecfile" << std::endl;
#endif
  std::string file_name = ast->child_->val_;
  fstream stream;
  stream.open(file_name);
  std::cout << stream.is_open() << std::endl;
  // command buffer
  const int buf_size = 1024;
  char cmd[buf_size];
  TreeFileManagers syntax_tree_file_mgr("syntax_tree_");
  uint32_t syntax_tree_id = 0;
  int count = 0;
  while (1) 
  {
    stream.getline(cmd, 1025);
    count++;
    YY_BUFFER_STATE bp = yy_scan_string(cmd);
    if (bp == nullptr) 
    {
      LOG(ERROR) << "Failed to create yy buffer state." << std::endl;
      exit(1);
    }
    yy_switch_to_buffer(bp); // 我还以为我在写编译器，，
    // init parser module
    MinisqlParserInit();
    // parse
    yyparse();
    // parse result handle
    if (MinisqlParserGetError()) printf("%s\n", MinisqlParserGetErrorMessage());
    else {
#ifdef ENABLE_PARSER_DEBUG
      printf("[INFO] Sql syntax parse ok!\n");
      SyntaxTreePrinter printer(MinisqlGetParserRootNode());
      printer.PrintTree(syntax_tree_file_mgr[syntax_tree_id++]);
#endif
    }
    auto code = Execute(MinisqlGetParserRootNode());
    // clean memory after parse
    MinisqlParserFinish();
    yy_delete_buffer(bp);
    yylex_destroy();
    // quit condition
    if (code == DB_QUIT) 
    {
      ExecuteInformation(code);
      break;
    }
    if(code != DB_SUCCESS)
      return code;
    if (stream.eof())
      break;
  }
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement
 */
dberr_t ExecuteEngine::ExecuteQuit(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteQuit" << std::endl;
#endif
  return DB_QUIT; // 就这样？
}
