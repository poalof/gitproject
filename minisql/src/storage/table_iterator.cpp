#include "storage/table_iterator.h"

#include "common/macros.h"
#include "storage/table_heap.h"

/**
 * TODO: Student Implement
 */
TableIterator::TableIterator(TableHeap* th) {
  table_heap = th;
  row_id = INVALID_ROWID;
}
TableIterator::TableIterator(const TableIterator &other) {
  table_heap = other.table_heap;
  row_id = other.row_id;
}
TableIterator::~TableIterator() {
}
bool TableIterator::operator==(const TableIterator &itr) const {
  return table_heap==itr.table_heap && row_id.Get() == itr.row_id.Get();
}
bool TableIterator::operator!=(const TableIterator &itr) const {
  return table_heap!=itr.table_heap || row_id.Get() != itr.row_id.Get();
}
const Row &TableIterator::operator*() {
  Row* res = new Row(row_id);
  table_heap->GetTuple(res, nullptr);
  return *res;
}
Row *TableIterator::operator->() {
  Row* res = new Row(row_id);
  table_heap->GetTuple(res, nullptr);
  return res;
}
TableIterator &TableIterator::operator=(const TableIterator &itr) noexcept {
  table_heap = itr.table_heap;
  row_id = itr.row_id;
  return *this;
}
// ++iter
TableIterator &TableIterator::operator++() {
  if (*this == table_heap->End()) return *this;
  page_id_t pageid = row_id.GetPageId();
  auto page = reinterpret_cast<TablePage *>(table_heap->buffer_pool_manager_->FetchPage(pageid));

  if(page->GetNextTupleRid(row_id, &row_id))
  {
    table_heap->buffer_pool_manager_->UnpinPage(pageid, false);
    return *this;
  }
  table_heap->buffer_pool_manager_->UnpinPage(pageid, false);
  pageid = page->GetNextPageId();
  page = reinterpret_cast<TablePage *>(table_heap->buffer_pool_manager_->FetchPage(pageid));
  while(page != nullptr)
  {
    if(page->GetFirstTupleRid(&row_id))
    {
      table_heap->buffer_pool_manager_->UnpinPage(pageid, false);
      return *this;
    }
    table_heap->buffer_pool_manager_->UnpinPage(pageid, false);
    pageid = page->GetNextPageId();
    page = reinterpret_cast<TablePage *>(table_heap->buffer_pool_manager_->FetchPage(pageid));
  }
  row_id = INVALID_ROWID;
  return *this;
}
// iter++
TableIterator TableIterator::operator++(int) {
  page_id_t pageid = row_id.GetPageId();
  TableIterator old(*this);
  auto page = reinterpret_cast<TablePage *>(table_heap->buffer_pool_manager_->FetchPage(pageid));
  if(page->GetNextTupleRid(row_id, &row_id))
  {
    table_heap->buffer_pool_manager_->UnpinPage(pageid, false);
    return old;
  }
  table_heap->buffer_pool_manager_->UnpinPage(pageid, false);
  pageid = page->GetNextPageId();
  page = reinterpret_cast<TablePage *>(table_heap->buffer_pool_manager_->FetchPage(pageid));
  while(page != nullptr)
  {
    if(page->GetFirstTupleRid(&row_id))
    {
      table_heap->buffer_pool_manager_->UnpinPage(pageid, false);
      return old;
    }
    table_heap->buffer_pool_manager_->UnpinPage(pageid, false);
    pageid = page->GetNextPageId();
    page = reinterpret_cast<TablePage *>(table_heap->buffer_pool_manager_->FetchPage(pageid));
  }
  row_id = INVALID_ROWID;
  return old;
}
