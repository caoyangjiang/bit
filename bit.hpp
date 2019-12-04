/**
 * @file bit.hpp
 * @author Caoyang Jiang
 * @brief
 * @version 0.1
 * @date 2019-12-03
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef BIT_HPP_
#define BIT_HPP_

#include <array>
#include <cstdint>
#include <exception>
#include <type_traits>
#include <vector>

namespace jcy {
template <class T = uint8_t, bool MSB_FIRST = true,
          class = typename std::enable_if<std::is_unsigned<T>::value>::type>
class bit {
 public:
  typedef T value_type;
  typedef T& reference;
  typedef const T& const_reference;
  typedef size_t size_type;
  typedef std::vector<T> container_type;
  static const T ONE = T(1);
  static const T ZERO = T(0);
  static const size_t T_BYTE_SIZE = sizeof(T);
  static const size_t T_BIT_SIZE = 8 * sizeof(T);

 public:
  static constexpr value_type create_bit_pattern(size_t i) {
    return ONE << (MSB_FIRST ? (T_BIT_SIZE - 1) - i : i);
  }

  template <size_t... i>
  static constexpr auto create_bit_pattern_array(std::index_sequence<i...>) {
    return std::array<value_type, sizeof...(i)>{{create_bit_pattern(i)...}};
  }

  static constexpr auto create_bit_pattern_array() {
    return create_bit_pattern_array(std::make_index_sequence<T_BIT_SIZE>{});
  }

 public:
  static constexpr auto BIT_PATTERNS = create_bit_pattern_array();

  // public:
  //  class iterator;
  //  class const_iterator;
  //  class reverse_iterator;
  //  class const_reverse_iterator;

 public:
  // constructor
  bit();

  explicit bit(size_type n);

  template <typename K,
            typename std::enable_if<std::is_integral<K>::value>::type>
  bit(size_type n, const K& val);

  bit(const bit& x);

  bit(bit&& x);

  template <typename K>
  bit(std::initializer_list<K> il);

  template <typename K>
  bit(const K* data, size_type size);

  template <class InputIterator>
  bit(InputIterator first, InputIterator last);

  // destructor
  ~bit();

  // assignment operator
  bit& operator=(const bit& x);
  bit& operator=(bit&& x);
  bit& operator=(std::initializer_list<value_type> il);

  // concatenate operator
  bit& operator+(const bit& x);
  bit& operator+(bit&& x);

  // iterator
  // iterator begin();
  // iterator end();
  // const_iterator cbegin() const;
  // const_iterator cend() const;
  // reverse_iterator rbegin();
  // reverse_iterator rend();
  // const_reverse_iterator crbegin() const;
  // const_reverse_iterator crend() const;

  // capacity
  size_type size() const noexcept;
  void resize(size_type n);
  size_type capacity() const noexcept;
  bool empty() const noexcept;
  void reserve(size_type n);

  // element access
  value_type operator[](size_type n) const;
  value_type at(size_type n) const;
  value_type front() const noexcept;
  value_type back() const noexcept;
  const value_type* data() const noexcept;
  value_type* data() noexcept;
  bit sub_range(size_type begin, size_type end) const;

  // modifiers
  template <typename K,
            typename std::enable_if<std::is_integral<K>::value>::type>
  void push_bit(K value);

  template <typename K,
            typename std::enable_if<std::is_integral<K>::value>::type>
  void replace_bit(size_type position, K value);

  template <typename K,
            typename std::enable_if<std::is_integral<K>::value>::type>
  void push_trailing_bit(K pattern);

  template <typename K>
  void push_bytes(K value);

  template <typename K>
  void push_bytes(const K* data, size_type n);

  template <class InputIterator>
  void push_bytes(InputIterator first, InputIterator last);

  void clear() noexcept;

 public:
  struct bit_holder {
    explicit bit_holder(container_type* container) { container_ = container; }

    void initialize() {
      container_->resize(1);
      last_byte_holder_ = &(container_->back());
      next_bit_position_ = 0;
    }

    T* last_byte_holder_ = nullptr;
    container_type* container_ = nullptr;
    size_type next_bit_position_ = 0;
  };

  inline void push_with_resize(bit_holder* holder, value_type value);
  inline void pop_with_resize(bit_holder* holder);
  inline void clear(bit_holder* holder);
  inline value_type bit_value_at(bit_holder* holder, size_type position);
  inline size_type bit_count(const bit_holder& holder) const noexcept;
  inline size_type bit_remainder(size_type n) noexcept;

 public:
  container_type buffer_;
  bit_holder bit_holder_;
};

template <class BIT_CONTAINER_TYPE, bool MSB_FIRST, class TYPE_CHECK>
bit<BIT_CONTAINER_TYPE, MSB_FIRST, TYPE_CHECK>::bit() : bit_holder_(&buffer_) {
  bit_holder_.initialize();
}

template <class BIT_CONTAINER_TYPE, bool MSB_FIRST, class TYPE_CHECK>
bit<BIT_CONTAINER_TYPE, MSB_FIRST, TYPE_CHECK>::~bit() {}

template <class BIT_CONTAINER_TYPE, bool MSB_FIRST, class TYPE_CHECK>
const typename bit<BIT_CONTAINER_TYPE, MSB_FIRST, TYPE_CHECK>::value_type*
bit<BIT_CONTAINER_TYPE, MSB_FIRST, TYPE_CHECK>::data() const noexcept {
  return buffer_.data();
}

template <class BIT_CONTAINER_TYPE, bool MSB_FIRST, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_FIRST, TYPE_CHECK>::value_type*
bit<BIT_CONTAINER_TYPE, MSB_FIRST, TYPE_CHECK>::data() noexcept {
  return buffer_.data();
}

template <class BIT_CONTAINER_TYPE, bool MSB_FIRST, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_FIRST, TYPE_CHECK>::push_with_resize(
    bit_holder* holder, value_type value) {
  if ((value != ZERO) && (value != ONE)) {
    throw std::invalid_argument("input argument is not one or zero.");
  }

  if (holder->next_bit_position_ == T_BIT_SIZE) {
    holder->container_->resize(holder->container_->size() + 1);
    holder->last_byte_holder_ = &(holder->container_->back());
    *(holder->last_byte_holder_) = ZERO;
    holder->next_bit_position_ = 0;
  }

  if (value == ONE) {
    *(holder->last_byte_holder_) =
        *(holder->last_byte_holder_) | BIT_PATTERNS[holder->next_bit_position_];
  }

  holder->next_bit_position_++;
}

template <class BIT_CONTAINER_TYPE, bool MSB_FIRST, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_FIRST, TYPE_CHECK>::pop_with_resize(
    bit_holder* holder) {
  if (holder->next_bit_position_ == 0) {
    if (holder->container_->size() > 1) {
      holder->container_->pop_back();
      holder->last_byte_holder_ = &(holder->container_->back());
      holder->next_bit_position_ = T_BIT_SIZE;
    }
  }

  holder->next_bit_position_--;
}

template <class BIT_CONTAINER_TYPE, bool MSB_FIRST, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_FIRST, TYPE_CHECK>::clear(bit_holder* holder) {
  holder->initialize();
}

template <class BIT_CONTAINER_TYPE, bool MSB_FIRST, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_FIRST, TYPE_CHECK>::value_type
bit<BIT_CONTAINER_TYPE, MSB_FIRST, TYPE_CHECK>::bit_value_at(
    bit_holder* holder, size_type position) {
  if (position >= (holder->container_->size() - 1) * T_BIT_SIZE +
                      holder->next_bit_position_) {
    throw std::out_of_range("bit query position is out of range.");
  }

  size_type byte_position = position / T_BIT_SIZE;
  size_type bit_position = position % T_BIT_SIZE;
  value_type byte = holder->container_->at(byte_position);
  return (byte >>
          (MSB_FIRST ? ((T_BIT_SIZE - 1) - bit_position) : bit_position)) &
         ONE;
}

}  // namespace jcy

#endif  // BIT_HPP_
