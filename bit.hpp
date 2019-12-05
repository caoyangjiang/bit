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
#include <exception>
#include <limits>
#include <type_traits>
#include <vector>

namespace jcy {
template <class T = unsigned char, bool MSB_TO_LSB = true,
          class = typename std::enable_if<std::is_unsigned<T>::value>::type>
class bit {
 public:
  typedef T value_type;
  typedef T& reference;
  typedef const T& const_reference;
  typedef size_t size_type;
  typedef std::vector<T> container_type;

  static const value_type ONE = value_type(1);
  static const value_type ZERO = value_type(0);
  static const value_type VALUE_MAX = std::numeric_limits<value_type>::max();
  static const size_t T_BYTE_SIZE = sizeof(value_type);
  static const size_t T_BIT_SIZE = 8 * sizeof(value_type);

 public:
  static constexpr value_type create_bit_pattern(size_t i) {
    return ONE << (MSB_TO_LSB ? (T_BIT_SIZE - 1) - i : i);
  }

  template <size_t... i>
  static constexpr auto create_bit_pattern_array(std::index_sequence<i...>) {
    return std::array<value_type, sizeof...(i)>{{create_bit_pattern(i)...}};
  }

  static constexpr auto create_bit_pattern_array() {
    return create_bit_pattern_array(std::make_index_sequence<T_BIT_SIZE>{});
  }

  static constexpr value_type create_mask_pattern(size_t i) {
    value_type mask = ZERO;
    for (size_t d = 0; d <= i; d++) mask |= create_bit_pattern(d);
    return mask;
  }

  template <size_t... i>
  static constexpr auto create_mask_pattern_array(std::index_sequence<i...>) {
    return std::array<value_type, sizeof...(i)>{{create_mask_pattern(i)...}};
  }

  static constexpr auto create_mask_pattern_array() {
    return create_mask_pattern_array(std::make_index_sequence<T_BIT_SIZE>{});
  }

 public:
  /**
   * @brief BIT_PATTERNS are
   *
   * For MSB to LSB,
   *    "10000000", "01000000", "00100000", "00010000",  "00001000", "00000100",
   *    "00000010", "00000001"
   *
   * For LSB to MSB,
   *    "00000001", "00000010", "00000100", "00001000",  "00010000", "00100000",
   *    "01000000", "10000000"
   */
  static constexpr auto BIT_PATTERNS = create_bit_pattern_array();

  /**
   * @brief MASK_PATTERNS are
   *
   * For MSB to LSB,
   *    "10000000", "11000000", "11100000", "11110000",  "11111000", "11111100",
   *    "11111110", "11111111"
   *
   * For LSB to MSB,
   *    "00000001", "00000011", "00000111", "00001111",  "00011111", "00111111",
   *    "01111111", "11111111"
   */
  static constexpr auto MASK_PATTERNS = create_mask_pattern_array();

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
  void resize(size_type n, const value_type& val);
  void reserve(size_type n);
  size_type buffer_size() const noexcept;
  bool empty() const noexcept;

  // element access
  value_type operator[](size_type n) const;
  value_type at(size_type n) const;
  value_type front() const noexcept;
  value_type back() const noexcept;
  const value_type* data() const noexcept;
  value_type* data() noexcept;
  // bit sub_range(size_type begin, size_type end) const;

  // modifiers
  void push(value_type value);
  void pop();
  void replace(size_type position, value_type value);
  void align(value_type value = ZERO);
  void push_byte(unsigned char value);
  void push_bytes(const unsigned char* data, size_type n);

  // template <class InputIterator>
  // void push_bytes(InputIterator first, InputIterator last);

  void clear() noexcept;

 public:
  struct bit_holder {
    explicit bit_holder(container_type* container) { container_ = container; }

    void initialize() {
      container_->resize(0);
      next_bit_position_ = T_BIT_SIZE;
    }

    T* last_byte_holder_ = nullptr;
    container_type* container_ = nullptr;
    size_type next_bit_position_ = 0;
  };

  inline void push_with_resize(bit_holder* holder, value_type value);
  inline void pop_with_resize(bit_holder* holder);
  inline void clear(bit_holder* holder);
  inline value_type bit_value_at(const bit_holder& holder,
                                 size_type position) const;
  inline value_type bit_value(const bit_holder& holder,
                              size_type position) const;

  inline size_type bit_count(const bit_holder& holder) const noexcept;
  inline size_type bit_remainder(const bit_holder& holder) noexcept;

 public:
  container_type buffer_;
  bit_holder bit_holder_;
};

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::bit() : bit_holder_(&buffer_) {
  bit_holder_.initialize();
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::~bit() {}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::push_with_resize(
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

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::pop_with_resize(
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

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::clear(
    bit_holder* holder) {
  holder->initialize();
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::bit_value_at(
    const bit_holder& holder, size_type position) const {
  if (position >= (holder.container_->size() - 1) * T_BIT_SIZE +
                      holder.next_bit_position_) {
    throw std::out_of_range("bit query position is out of range.");
  }

  return bit_value(holder, position);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::bit_value(
    const bit_holder& holder, size_type position) const {
  size_type byte_position = position / T_BIT_SIZE;
  size_type bit_position = position % T_BIT_SIZE;
  value_type byte = holder.container_->at(byte_position);
  return (byte >>
          (MSB_TO_LSB ? ((T_BIT_SIZE - 1) - bit_position) : bit_position)) &
         ONE;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::bit_count(
    const bit_holder& holder) const noexcept {
  return (holder.container_->size() - 1) * T_BIT_SIZE +
         holder.next_bit_position_;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::bit_remainder(
    const bit_holder& holder) noexcept {
  return T_BIT_SIZE - holder.next_bit_position_;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size() const noexcept {
  return bit_count(bit_holder_);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::reserve(size_type n) {
  buffer_.reserve((n + T_BIT_SIZE - 1) / T_BIT_SIZE);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::resize(size_type n) {
  buffer_.resize((n + T_BIT_SIZE - 1) / T_BIT_SIZE);
  bit_holder_.last_byte_holder_ = &(bit_holder_.container->back());
  bit_holder_.next_bit_position_ =
      (n % T_BIT_SIZE) == 0 ? T_BIT_SIZE : (n % T_BIT_SIZE);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::resize(
    size_type n, const value_type& value) {
  if ((value != ZERO) && (value != ONE)) {
    throw std::invalid_argument("input argument is not one or zero.");
  }

  const value_type element_value = value == ONE ? VALUE_MAX : ZERO;
  buffer_.resize((n + T_BIT_SIZE - 1) / T_BIT_SIZE, element_value);
  bit_holder_.last_byte_holder_ = &(bit_holder_.container_->back());
  bit_holder_.next_bit_position_ =
      (n % T_BIT_SIZE) == 0 ? T_BIT_SIZE : (n % T_BIT_SIZE);

  // keep unused bits zero
  *bit_holder_.last_byte_holder_ =
      MASK_PATTERNS[bit_holder_.next_bit_position_ - 1];
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::buffer_size() const noexcept {
  return buffer_.size();
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bool bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::empty() const noexcept {
  return bit_count(bit_holder_) == 0;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
    bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::operator[](
        size_type n) const {
  return bit_value(bit_holder_, n);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::at(size_type n) const {
  return bit_value_at(bit_holder_, n);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::front() const noexcept {
  return (MSB_TO_LSB ? buffer_[0] >> (T_BIT_SIZE - 1) : buffer_[0]) & ONE;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::back() const noexcept {
  // std::cout << "count" << bit_count(bit_holder_) << std::endl;
  return bit_value(bit_holder_, bit_count(bit_holder_) - 1);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
const typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type*
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::data() const noexcept {
  return buffer_.data();
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type*
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::data() noexcept {
  return buffer_.data();
}
template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::push(value_type value) {
  push_with_resize(&bit_holder_, value);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::pop() {
  pop_with_resize(&bit_holder_);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::replace(
    size_type position, value_type value) {
  size_type byte_position = position / T_BIT_SIZE;
  size_type bit_position = position % T_BIT_SIZE;

  if (value == ONE) {
    bit_holder_.container_->at(byte_position) |= BIT_PATTERNS[bit_position];
  }

  if (value == ZERO) {
    bit_holder_.container_->at(byte_position) &= ~BIT_PATTERNS[bit_position];
  }
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::align(value_type value) {
  int b = 0;
  while (bit_holder_.next_bit_position_ != T_BIT_SIZE) {
    push((value & BIT_PATTERNS[b]) == ZERO ? ZERO : ONE);
    b++;
  }
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::push_byte(
    unsigned char value) {
  if (std::is_same<BIT_CONTAINER_TYPE, unsigned char>::value &&
      (bit_holder_.next_bit_position_ == T_BIT_SIZE)) {
    bit_holder_.container_->resize(bit_holder_.container_->size() + 1);
    bit_holder_.last_byte_holder_ = &(bit_holder_.container_->back());
    *(bit_holder_.last_byte_holder_) = value;
  } else {
    for (size_type i = 0; i < 8; i++) {
      push(((value & (0x80 >> i)) == 0u) ? ZERO : ONE);
    }
  }
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::push_bytes(
    const unsigned char* data, size_type n) {
  if (std::is_same<BIT_CONTAINER_TYPE, unsigned char>::value &&
      (bit_holder_.next_bit_position_ == T_BIT_SIZE)) {
    bit_holder_.container_->resize(bit_holder_.container_->size() + n);
    bit_holder_.last_byte_holder_ = &(bit_holder_.container_->back());
    std::memcpy(
        bit_holder_.container_->data() + bit_holder_.container_->size() - n,
        data, n);
  } else {
    for (size_type j = 0; j < n; j++) {
      for (size_type i = 0; i < 8; i++) {
        push(((data[j] & BIT_PATTERNS[i]) == 0u) ? ZERO : ONE);
      }
    }
  }
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::clear() noexcept {
  clear(&bit_holder_);
}

}  // namespace jcy

#endif  // BIT_HPP_
