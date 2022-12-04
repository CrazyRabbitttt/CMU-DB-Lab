//===----------------------------------------------------------------------===//
//
//                         CMU-DB Project (15-445/645)
//                         ***DO NO SHARE PUBLICLY***
//
// Identification: src/include/page/b_plus_tree_page.h
//
// Copyright (c) 2018, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//
#pragma once

#include <cassert>
#include <climits>
#include <cstdlib>
#include <string>

#include "buffer/buffer_pool_manager.h"
#include "storage/index/generic_key.h"

namespace bustub {

#define MappingType std::pair<KeyType, ValueType>

#define INDEX_TEMPLATE_ARGUMENTS template <typename KeyType, typename ValueType, typename KeyComparator>

// define page type enum
enum class IndexPageType { INVALID_INDEX_PAGE = 0, LEAF_PAGE, INTERNAL_PAGE };

/**
 * Both internal and leaf page are inherited from this page.
 *
 * It actually serves as a header part for each B+ tree page and
 * contains information shared by both leaf page and internal page.                     // 服务用于作为: header part of B+ tree page & internal page 和 leaf page 共享信息
 *
 * Header format (size in byte, 24 bytes in total):
 * ----------------------------------------------------------------------------
 * | PageType (4) | LSN (4) | CurrentSize (4) | MaxSize (4) |       ==> 16 byte
 * ----------------------------------------------------------------------------
 * | ParentPageId (4) | PageId(4) |                                 ==>  8 byte
 * ----------------------------------------------------------------------------
 */
class BPlusTreePage {
 public:
  auto IsLeafPage() const -> bool;
  auto IsRootPage() const -> bool;
  void SetPageType(IndexPageType page_type);

  auto GetSize() const -> int;
  void SetSize(int size);
  void IncreaseSize(int amount);

  auto GetMaxSize() const -> int;
  void SetMaxSize(int max_size);
  auto GetMinSize() const -> int;

  auto GetParentPageId() const -> page_id_t;
  void SetParentPageId(page_id_t parent_page_id);

  auto GetPageId() const -> page_id_t;
  void SetPageId(page_id_t page_id);

  void SetLSN(lsn_t lsn = INVALID_LSN);

 private:
  // member variable, attributes that both internal and leaf page share
  IndexPageType page_type_ __attribute__((__unused__));             /** Internal Page or leaf page */
  lsn_t lsn_ __attribute__((__unused__));                           /** 日志的序列号，用于版本控制？ */
  int size_ __attribute__((__unused__));                            /** KV 对的数量 */
  int max_size_ __attribute__((__unused__));
  page_id_t parent_page_id_ __attribute__((__unused__));            /** Child pointer, aka page_id */
  page_id_t page_id_ __attribute__((__unused__));                   /** Parent pointer, */
};

}  // namespace bustub
