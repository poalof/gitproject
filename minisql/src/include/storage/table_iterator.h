#ifndef MINISQL_TABLE_ITERATOR_H
#define MINISQL_TABLE_ITERATOR_H

#include "common/rowid.h"
#include "record/row.h"
#include "transaction/transaction.h"
#include "storage/table_heap.h"

class TableHeap;

class TableIterator {
public:
  // you may define your own constructor based on your member variables
  TableIterator(TableHeap*);

  TableIterator(const TableIterator &other);

  virtual ~TableIterator();

  bool operator==(const TableIterator &itr) const;

  bool operator!=(const TableIterator &itr) const;

  const Row &operator*();

  Row *operator->();

  TableIterator &operator=(const TableIterator &itr) noexcept;

  TableIterator &operator++();

  TableIterator operator++(int);

  RowId *GetRowId() { return &row_id; }
private:
  // add your own private member variables here
  RowId row_id{};
  TableHeap* table_heap;
};

#endif  // MINISQL_TABLE_ITERATOR_H
