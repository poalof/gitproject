#include "storage/table_heap.h"

#include <cassert>
/**
 * TODO: Student Implement
 */
bool TableHeap::InsertTuple(Row &row, Transaction *txn) {
  TablePage *page;
  if(first_page_id_==INVALID_PAGE_ID)
  {
    page = reinterpret_cast<TablePage *>(buffer_pool_manager_->NewPage(first_page_id_));
    page->Init(first_page_id_, INVALID_PAGE_ID, log_manager_,txn);
  }
  else page = reinterpret_cast<TablePage *>(buffer_pool_manager_->FetchPage(first_page_id_));
  while(page->GetTablePageId() < DEFAULT_BUFFER_POOL_SIZE)
  {
    if(page->InsertTuple(row, schema_, txn, lock_manager_, log_manager_))
    {
        buffer_pool_manager_->UnpinPage(page->GetTablePageId(), true);
        return true;
    }
    if(page->GetNextPageId()==INVALID_PAGE_ID)
    {
      page_id_t npage;
      auto new_page = reinterpret_cast<TablePage *>(buffer_pool_manager_->NewPage(npage));
      page->SetNextPageId(npage);
      new_page->Init(npage, page->GetTablePageId(), log_manager_,txn);
      buffer_pool_manager_->UnpinPage(page->GetTablePageId(), false);
      page = new_page;
    }
    else 
    {
      page_id_t tpage = page->GetNextPageId();
      buffer_pool_manager_->UnpinPage(page->GetTablePageId(), false);
      page = reinterpret_cast<TablePage *>(buffer_pool_manager_->FetchPage(tpage));
    }
  }
  return false;
}

bool TableHeap::MarkDelete(const RowId &rid, Transaction *txn) {
  // Find the page which contains the tuple.
  auto page = reinterpret_cast<TablePage *>(buffer_pool_manager_->FetchPage(rid.GetPageId()));
  // If the page could not be found, then abort the transaction.
  if (page == nullptr) {
    return false;
  }
  // Otherwise, mark the tuple as deleted.
  page->WLatch();
  page->MarkDelete(rid, txn, lock_manager_, log_manager_);
  page->WUnlatch();
  buffer_pool_manager_->UnpinPage(page->GetTablePageId(), true);
  return true;
}

/**
 * TODO: Student Implement
 */
bool TableHeap::UpdateTuple(Row &row, const RowId &rid, Transaction *txn) {
  auto page = reinterpret_cast<TablePage *>(buffer_pool_manager_->FetchPage(rid.GetPageId()));
  assert(page != nullptr);
  Row old_row(rid);
  int state = page->UpdateTuple(row, &old_row, schema_, txn, lock_manager_, log_manager_);
  buffer_pool_manager_->UnpinPage(rid.GetPageId(), true);
  if(state == FAIL_NO_SPACE)
  {
    ApplyDelete(rid, txn);
    return InsertTuple(row, txn);
  }
  else if(state != SUCCESS)
    return false;
  return true;
}

/**
 * TODO: Student Implement
 */
void TableHeap::ApplyDelete(const RowId &rid, Transaction *txn) {
  // Step1: Find the page which contains the tuple.
  auto page = reinterpret_cast<TablePage *>(buffer_pool_manager_->FetchPage(rid.GetPageId()));
  assert(page != nullptr);
  // Step2: Delete the tuple from the page.
  page->ApplyDelete(rid, txn, log_manager_);
  buffer_pool_manager_->UnpinPage(page->GetTablePageId(), true);
}

void TableHeap::RollbackDelete(const RowId &rid, Transaction *txn) {
  // Find the page which contains the tuple.
  auto page = reinterpret_cast<TablePage *>(buffer_pool_manager_->FetchPage(rid.GetPageId()));
  assert(page != nullptr);
  // Rollback to delete.
  page->WLatch();
  page->RollbackDelete(rid, txn, log_manager_);
  page->WUnlatch();
  buffer_pool_manager_->UnpinPage(page->GetTablePageId(), true);
}

/**
 * TODO: Student Implement
 */
bool TableHeap::GetTuple(Row *row, Transaction *txn) {
  page_id_t page_id = row->GetRowId().GetPageId();
  auto page = reinterpret_cast<TablePage *>(buffer_pool_manager_->FetchPage(page_id));
  assert(page != nullptr);

  bool res = page->GetTuple(row, schema_, txn, lock_manager_);

  buffer_pool_manager_->UnpinPage(page_id, false);
  return res;
}

void TableHeap::DeleteTable(page_id_t page_id) {
  if (page_id != INVALID_PAGE_ID) {
    auto temp_table_page = reinterpret_cast<TablePage *>(buffer_pool_manager_->FetchPage(page_id));  // 删除table_heap
    if (temp_table_page->GetNextPageId() != INVALID_PAGE_ID)
      DeleteTable(temp_table_page->GetNextPageId());
    buffer_pool_manager_->UnpinPage(page_id, false);
    buffer_pool_manager_->DeletePage(page_id);
  } else {
    DeleteTable(first_page_id_);
  }
}

/**
 * TODO: Student Implement
 */
TableIterator TableHeap::Begin(Transaction *txn) {
  TableIterator it(this);
  auto page = reinterpret_cast<TablePage *>(buffer_pool_manager_->FetchPage(first_page_id_));
  page_id_t pageid = first_page_id_;
  RowId rid;
  while(page != nullptr)
  {
    if(page->GetFirstTupleRid(&rid))
    {
      buffer_pool_manager_->UnpinPage(pageid, false);
      it.GetRowId()->Set(rid.GetPageId(), rid.GetSlotNum());
      return it;
    }
    buffer_pool_manager_->UnpinPage(pageid, false);
    pageid = page->GetNextPageId();
    page = reinterpret_cast<TablePage *>(buffer_pool_manager_->FetchPage(pageid));
  }
  *(it.GetRowId()) = INVALID_ROWID;
  return it;
}

/**
 * TODO: Student Implement
 */
TableIterator TableHeap::End() {
  TableIterator it(this);
  return it;
}
