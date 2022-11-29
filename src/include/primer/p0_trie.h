//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// p0_trie.h
//
// Identification: src/include/primer/p0_trie.h
//
// Copyright (c) 2015-2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "common/logger.h"
#include "common/exception.h"
#include "common/rwlatch.h"

namespace bustub {

/**
 * TrieNode is a generic container for any node in Trie.
 */
class TrieNode {
 public:
  /**
   * TODO(P0): Add implementation
   *
   * @brief Construct a new Trie Node object with the given key char.
   * is_end_ flag should be initialized to false in this constructor.
   *
   * @param key_char Key character of this trie node
   */
  explicit TrieNode(char key_char) : key_char_(key_char), is_end_(false) {}
  // This constructor function has not assign value to the map, it will 自动生成
  /**
   * TODO(P0): Add implementation
   *
   * @brief Move constructor for trie node object. The unique pointers stored
   * in children_ should be moved from other_trie_node to new trie node.
   *
   * @param other_trie_node Old trie node.
   */
  TrieNode(TrieNode &&other_trie_node) noexcept {
    this->is_end_ = other_trie_node.is_end_;
    this->key_char_ = other_trie_node.key_char_;
    this->children_ = std::move(other_trie_node.children_);
  }

  /**
   * @brief Destroy the TrieNode object.
   */
  virtual ~TrieNode() = default;        // 不需要实现，因为都是 unique_ptr

  /**
   * TODO(P0): Add implementation
   *
   * @brief Whether this trie node has a child node with specified key char.
   *
   * @param key_char Key char of child node.
   * @return True if this trie node has a child with given key, false otherwise.
   */
  bool HasChild(char key_char) const { return children_.count(key_char) != 0; }

  /**
   * TODO(P0): Add implementation
   *
   * @brief Whether this trie node has any children at all. This is useful
   * when implementing 'Remove' functionality.
   *
   * @return True if this trie node has any child node, false if it has no child node.
   */
  bool HasChildren() const { return children_.size() != 0; }

  /**
   * TODO(P0): Add implementation
   *
   * @brief Whether this trie node is the ending character of a key string.
   *
   * @return True if is_end_ flag is true, false if is_end_ is false.
   */
  bool IsEndNode() const { return is_end_; }

  /**
   * TODO(P0): Add implementation
   *
   * @brief Return key char of this trie node.
   *
   * @return key_char_ of this trie node.
   */
  char GetKeyChar() const { return key_char_; }

  /**
   * TODO(P0): Add implementation
   *
   * @brief Insert a child node for this trie node into children_ map, given the key char and
   * unique_ptr of the child node. If specified key_char already exists in children_,
   * return nullptr. If parameter `child`'s key char is different than parameter
   * `key_char`, return nullptr.
   *
   * Note that parameter `child` is rvalue and should be moved when it is
   * inserted into children_map.
   *
   * The return value is a pointer to unique_ptr because pointer to unique_ptr can access the
   * underlying data without taking ownership of the unique_ptr. Further, we can set the return
   * value to nullptr when error occurs.
   *
   * @param key Key of child node
   * @param child Unique pointer created for the child node. This should be added to children_ map.
   * @return Pointer to unique_ptr of the inserted child node. If insertion fails, return nullptr.
   */
  std::unique_ptr<TrieNode> *InsertChildNode(char key_char, std::unique_ptr<TrieNode> &&child) {
    if (children_.count(key_char)) return nullptr;          // There already has key in children
    if (key_char != child->GetKeyChar()) return nullptr;
    // 注意传进来的 child 是 rvalue, 赋值使用 std::move()
    children_[key_char] = std::move(child);
    // Return value is a pointer to unique_ptr ==> 能够不获得 所有权 的情况下得到 内部的 value
    return &children_[key_char];
  }

  /**
   * TODO(P0): Add implementation
   *
   * @brief Get the child node given its key char. If child node for given key char does
   * not exist, return nullptr.
   *
   * @param key Key of child node
   * @return Pointer to unique_ptr of the child node, nullptr if child
   *         node does not exist.
   */
  std::unique_ptr<TrieNode> *GetChildNode(char key_char) {
      if (children_.count(key_char) == 0) return nullptr;
      return &children_[key_char];
  }

  /**
   * TODO(P0): Add implementation
   *
   * @brief Remove child node from children_ map.
   * If key_char does not exist in children_, return immediately.
   *
   * @param key_char Key char of child node to be removed
   */
  void RemoveChildNode(char key_char) {
    if (children_.count(key_char) == 0) return;
    children_.erase(key_char);      // Erase element by Value
  }

  bool IfHasChildren() {
    return children_.size() != 0;
  }


  /**
   * TODO(P0): Add implementation
   *
   * @brief Set the is_end_ flag to true or false.
   *
   * @param is_end Whether this trie node is ending char of a key string
   */
  void SetEndNode(bool is_end) { is_end_ = is_end; }

 protected:
  /** Key character of this trie node */
  char key_char_;
  /** whether this node marks the end of a key */
  bool is_end_{false};
  /** A map of all child nodes of this trie node, which can be accessed by each
   * child node's key char. */
  std::unordered_map<char, std::unique_ptr<TrieNode>> children_;
};

/**
 * TrieNodeWithValue is a node that marks the ending of a key, and it can
 * hold a value of any type T.
 */
template <typename T>
class TrieNodeWithValue : public TrieNode {
 private:
  /* Value held by this trie node. */     // 增加了一个 Value 的属性
  T value_;

 public:
  /**
   * TODO(P0): Add implementation
   *
   * @brief Construct a new TrieNodeWithValue object from a TrieNode object and specify its value.
   * This is used when a non-terminal TrieNode is converted to terminal TrieNodeWithValue.
   *
   * The children_ map of TrieNode should be moved to the new TrieNodeWithValue object.         // 1. 转移 map 到新的 object
   * Since it contains unique pointers, the first parameter is a rvalue reference.
   *
   * You should:
   * 1) invoke TrieNode's move constructor to move data from TrieNode to
   * TrieNodeWithValue.
   * 2) set value_ member variable of this node to parameter `value`.
   * 3) set is_end_ to true
   *
   * @param trieNode TrieNode whose data is to be moved to TrieNodeWithValue
   * @param value
   */

  // 这个带 value 的 node 就代表string到达了结尾处了, 所以说 endvalue 一直都是 true
  TrieNodeWithValue(TrieNode &&trieNode, T value) : TrieNode(std::move(trieNode)), value_(value) { SetEndNode(true);}

  /**
   * TODO(P0): Add implementation
   *
   * @brief Construct a new TrieNodeWithValue. This is used when a new terminal node is constructed. // 构建一个terminal节点的时候会用到
   *
   * You should:
   * 1) Invoke the constructor for TrieNode with the given key_char.
   * 2) Set value_ for this node.
   * 3) set is_end_ to true.
   *
   * @param key_char Key char of this node
   * @param value Value of this node
   */

  // Create from scratch, from scratch, first construct the base class [TrieNode], then set some private attribute arguments
  TrieNodeWithValue(char key_char, T value) : TrieNode(key_char), value_(value) { SetEndNode(true); }

  /**
   * @brief Destroy the Trie Node With Value object
   */
  ~TrieNodeWithValue() override = default;

  /**
   * @brief Get the stored value_.
   *
   * @return Value of type T stored in this node
   */
  T GetValue() const {
    return value_; }
};

/**
 * Trie is a concurrent key-value store. Each key is a string and its corresponding
 * value can be any type.
 */
class Trie {
 private:
  /* Root node of the trie */
  std::unique_ptr<TrieNode> root_;
  /* Read-write lock for the trie */
  ReaderWriterLatch latch_;
  std::mutex mutex_;

 public:
  /**
   * TODO(P0): Add implementation
   *
   * @brief Construct a new Trie object. Initialize the root node with '\0'
   * character.
   */
  Trie() : root_(std::make_unique<TrieNode>('\0')) {}

  /**
   * TODO(P0): Add implementation
   *
   * @brief Insert key-value pair into the trie.
   *
   * If the key is an empty string, return false immediately.
   *
   * If the key already exists, return false. Duplicated keys are not allowed and
   * you should never overwrite value of an existing key.
   *
   * When you reach the ending character of a key:
   * 1. If TrieNode with this ending character does not exist, create new TrieNodeWithValue
   * and add it to parent node's children_ map.
   * 2. If the terminal node is a TrieNode, then convert it into TrieNodeWithValue by           需要删除掉原来的 TrieNode & 创建新的 TrieNodeWithValue
   * invoking the appropriate constructor.
   * 3. If it is already a TrieNodeWithValue,
   * then insertion fails and returns false. Do not overwrite existing data with new data.
   *
   * You can quickly check whether a TrieNode pointer holds TrieNode or TrieNodeWithValue
   * by checking the is_end_ flag. If is_end_ == false, then it points to TrieNode. If
   * is_end_ == true, it points to TrieNodeWithValue.
   *
   * @param key Key used to traverse the trie and find the correct node
   * @param value Value to be inserted
   * @return True if insertion succeeds, false if the key already exists
   */
  template <typename T>
  bool Insert(const std::string &key, T value) {
    std::scoped_lock<std::mutex> lock(mutex_);
    // 如果说插入的节点是重复的，直接返回 false; 怎么判断是重复的呢？ Is_end is true to judge is duplicated
    if (key.size() == 0) return false;
    auto node = &root_;
    int n = key.size();
    for (int i = 0; i < n-1; i++) {
      char ch = key[i];
      if (!(*node)->HasChild(ch)) {
        (*node)->InsertChildNode(ch, std::make_unique<TrieNode>(ch));
      }
      node = (*node)->GetChildNode(ch);       // node指针的移动
    }
//    LOG_INFO("目前已经遍历完TrieNode对应的string了");
    char final_char = key.back();
    // 1. 不存在末尾的节点，直接插入 TrieNodeWithValue
    if (node->get()->HasChild(final_char) == false) {       // 最后一个节点是不存在的， 直接创建然后 Return True 就好了
      std::unique_ptr<TrieNodeWithValue<T>> newnode = std::make_unique<TrieNodeWithValue<T>>(final_char, value);
      node->get()->InsertChildNode(final_char, std::move(newnode));
      return true;
    }

    node = (*node)->GetChildNode(final_char);

    // 2. 如果说是一个 TrieNodeWithValue, 那么就是重复插入数据，直接返回 false
    if ((*node)->IsEndNode()) return false;

    // 3. 如果说是一个 TrieNode, 那么直接提升为 TrieNodeWithValue
    *node = std::make_unique<TrieNodeWithValue<T>>(std::move(*(*node)), value);
    return true;
  }

  /**
   * TODO(P0): Add implementation
   *
   * @brief Remove key value pair from the trie.
   * This function should also remove nodes that are no longer part of another
   * key. If key is empty or not found, return false.
   *
   * You should:
   * 1) Find the terminal node for the given key.
   * 2) If this terminal node does not have any children, remove it from its
   * parent's children_ map.
   * 3) Recursively remove nodes that have no children and are not terminal node
   * of another key.                // not terminal node of any key && does not have children
   *
   * @param key Key used to traverse the trie and find the correct node
   * @return True if the key exists and is removed, false otherwise
   */

  bool Remove(const std::string &key) {
    std::scoped_lock<std::mutex> lock(mutex_);
    if (key.size() == 0) return false;
    std::vector<std::unique_ptr<TrieNode>*> vec;     // To store the parent node
    auto node = &root_;
    for (const char& ch : key) {    // root -> a -> a -> a
      if (!(*node)->HasChild(ch)) return false;       // 如果说压根就没有包含 给定的 key, 直接 Return false
      node = (*node)->GetChildNode(ch);
      vec.emplace_back(node);
    }
    // vector : [root, a, a, ]
    if ((*node)->HasChildren()) {
      (*node)->SetEndNode(false);                     // 将 endnode 的标识取消掉
      return true;                                    // 仍然是有children, 直接取消掉 end_node, return true
    }
    // 终端节点， 可以进行递归的删除节点了, 将终端节点的isend设置为false用于删除
    (*node)->SetEndNode(false);
    RemoveNode(key, key.size()-1, vec);
    return true;
  }

  // Recursion 的条件 : 没有子节点 & 不是 NodeWithValue 的终端节点
  void RemoveNode(const std::string& key, size_t curIndex, std::vector<std::unique_ptr<TrieNode>*>& vec) {
    std::unique_ptr<TrieNode>* curnode = vec.back(); vec.pop_back();     // unique_ptr* 的元素
    if (!(*curnode)->HasChildren() && !(*curnode)->IsEndNode()) {
      if (vec.size() == 0) {
        root_->RemoveChildNode((*curnode)->GetKeyChar());
        return;
      }
      (*vec.back())->RemoveChildNode((*curnode)->GetKeyChar());           // 从父节点中进行删除
      RemoveNode(key, curIndex-1, vec);
    } else {
      // 已经是不能够删除了
      return;
    }
  }
//
//  void ShowTreeNodes() {
//    auto node = &root_;
//    std::string str = "";
//    LOG_DEBUG("==================v============= Begin Debug =======================================");
//    while ((*node)->HasChildren()) {
//      if ((*node)->HasChild('a')) { LOG_INFO("Has child node 'a'"); str += "a"; }
//      node = (*node)->GetChildNode('a');
//      if ((*node)->IsEndNode()) LOG_INFO("本字符是一个单词的结尾:%s", str.c_str());
//    }
//    LOG_DEBUG("================================ End Debug =========================================");
//  }

  /**
   * TODO(P0): Add implementation
   *
   * @brief Get the corresponding value of type T given its key.
   * If key is empty, set success to false.
   * If key does not exist in trie, set success to false.
   * If the given type T is not the same as the value type stored in TrieNodeWithValue
   * (ie. GetValue<int> is called but terminal node holds std::string),
   * set success to false.
   *
   * To check whether the two types are the same, dynamic_cast
   * the terminal TrieNode to TrieNodeWithValue<T>. If the casted result
   * is not nullptr, then type T is the correct type.
   *
   * @param key Key used to traverse the trie and find the correct node
   * @param success Whether GetValue is successful or not
   * @return Value of type T if type matches
   */
  template <typename T>
  T GetValue(const std::string &key, bool *success) {     // get value,
    std::scoped_lock<std::mutex> lock(mutex_);
    if (key.size() == 0) {
      *success = false;
      return T{};
    }
    // 需要进行树的遍历，找到最终的 terminal node & judge the type if is equal
    auto node = &root_;
    for (const char& it : key) {
      if ((*node)->HasChild(it)) node = (*node)->GetChildNode(it);
      else { *success = false; return T{};}
    }
    *success = true;
    // 最后到达了 node with value [c]
    return dynamic_cast<TrieNodeWithValue<T> *>(&(*(*node)))->GetValue();
  }
};
}  // namespace bustub
