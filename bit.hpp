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
template <class BUFFER_ELEMENT_TYPE = unsigned char, bool MSB_TO_LSB = true,
          class = typename std::enable_if<
              std::is_unsigned<BUFFER_ELEMENT_TYPE>::value>::type>
class bit {
 public:
  typedef BUFFER_ELEMENT_TYPE value_type;
  typedef BUFFER_ELEMENT_TYPE& reference;
  typedef const BUFFER_ELEMENT_TYPE& const_reference;
  typedef size_t size_type;
  typedef std::vector<value_type> container_type;

  static const value_type ONE = value_type(1);
  static const value_type ZERO = value_type(0);
  static const value_type VALUE_MAX = std::numeric_limits<value_type>::max();
  static const size_t T_BYTE_SIZE = sizeof(value_type);
  static const size_t T_BIT_SIZE = 8 * sizeof(value_type);

 public:
  template <class A, bool B, class C>
  friend bit<A, B, C> operator+(const bit<A, B, C>& lhs,
                                const bit<A, B, C>& rhs);

 private:
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

 private:
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
  bit(size_type n, const value_type& val);
  bit(const bit& x);
  bit(bit&& x);
  bit(std::initializer_list<value_type> il);
  bit(const value_type* data, size_type size);

  // template <class InputIterator>
  // bit(InputIterator first, InputIterator last);

  // destructor
  ~bit();

  // assignment operator
  bit& operator=(const bit& x);
  bit& operator=(bit&& x);
  bit& operator=(std::initializer_list<value_type> il);

  // concatenate operator
  // bit& operator+(const bit& x);
  // bit& operator+(bit&& x);

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
  size_type buffer_element_count() const noexcept;
  size_type buffer_element_size() const noexcept;
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

 private:
  inline void push_with_resize(value_type value);
  inline void pop_with_resize();
  inline value_type bit_value_at(size_type position) const;
  inline value_type bit_value(size_type position) const;
  inline size_type bit_count() const noexcept;
  inline size_type bit_remainder() noexcept;
  inline void initialize_from(const std::initializer_list<value_type>& type);

 private:
  container_type buffer_;
  value_type* last_byte_holder_ = nullptr;
  size_type next_bit_position_ = 0;
};

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK> operator+(
    const bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>& lhs,
    const bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>& rhs) {
  bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK> concat;

  if (lhs.next_bit_position_ ==
      bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::T_BIT_SIZE) {
    concat.buffer_.resize(lhs.buffer_.size());
    concat.last_byte_holder_ = &(concat.buffer_.back());
  } else {
    for (typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type i =
             0;
         i < lhs.size(); i++) {
      concat.push(lhs[i]);
    }
  }

  if (rhs.next_bit_position_ ==
      bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::T_BIT_SIZE) {
    concat.buffer_.resize(rhs.buffer_.size() + concat.buffer_.size());
    concat.last_byte_holder_ = &(concat.buffer_.back());
  } else {
    for (typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type i =
             0;
         i < rhs.size(); i++) {
      concat.push(rhs[i]);
    }
  }

  return concat;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::bit() {
  clear();
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::bit(size_type n) {
  resize(n);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::bit(size_type n,
                                                     const value_type& val) {
  resize(n, val);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::bit(const bit& x) {
  *this = x;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::bit(bit&& x) {
  *this = x;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::bit(
    std::initializer_list<value_type> il) {
  initialize_from(il);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::bit(const value_type* data,
                                                     size_type size) {
  resize(size);
  std::memcpy(buffer_.data(), data,
              T_BYTE_SIZE * (size + T_BIT_SIZE - 1) / T_BIT_SIZE);

  // clean up tail if necessary
  if (next_bit_position_ != T_BIT_SIZE) {
    *last_byte_holder_ = MASK_PATTERNS[next_bit_position_ - 1];
  }
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::~bit() {}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>&
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::operator=(const bit& x) {
  buffer_ = x.buffer_;
  last_byte_holder_ = &(buffer_.back());
  next_bit_position_ = x.next_bit_position_;
  return *this;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>&
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::operator=(bit&& x) {
  buffer_ = std::move(x.buffer_);
  last_byte_holder_ = x.last_byte_holder_;
  next_bit_position_ = x.next_bit_position_;
  return *this;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>&
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::operator=(
    std::initializer_list<value_type> il) {
  initialize_from(il);
  return *this;
}

// template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
// bit& bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::operator+(bit&& x) {}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::push_with_resize(
    value_type value) {
  if ((value != ZERO) && (value != ONE)) {
    throw std::invalid_argument("input argument is not one or zero.");
  }

  if (next_bit_position_ == T_BIT_SIZE) {
    buffer_.resize(buffer_.size() + 1);
    last_byte_holder_ = &(buffer_.back());
    *(last_byte_holder_) = ZERO;
    next_bit_position_ = 0;
  }

  if (value == ONE) {
    *(last_byte_holder_) =
        *(last_byte_holder_) | BIT_PATTERNS[next_bit_position_];
  }

  next_bit_position_++;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::pop_with_resize() {
  if (next_bit_position_ == 0) {
    if (buffer_.size() > 1) {
      buffer_.pop_back();
      last_byte_holder_ = &(buffer_.back());
      next_bit_position_ = T_BIT_SIZE;
    }
  }

  next_bit_position_--;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::clear() noexcept {
  buffer_.clear();
  next_bit_position_ = T_BIT_SIZE;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::bit_value_at(
    size_type position) const {
  if (position >= (buffer_.size() - 1) * T_BIT_SIZE + next_bit_position_) {
    throw std::out_of_range("bit query position is out of range.");
  }

  return bit_value(position);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::bit_value(
    size_type position) const {
  size_type byte_position = position / T_BIT_SIZE;
  size_type bit_position = position % T_BIT_SIZE;
  value_type byte = buffer_.operator[](byte_position);
  return (byte & BIT_PATTERNS[bit_position]) == ZERO ? ZERO : ONE;
  // return (byte >>
  //         (MSB_TO_LSB ? ((T_BIT_SIZE - 1) - bit_position) : bit_position)) &
  //        ONE;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::bit_count() const noexcept {
  return (buffer_.size() - 1) * T_BIT_SIZE + next_bit_position_;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::bit_remainder() noexcept {
  return T_BIT_SIZE - next_bit_position_;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::initialize_from(
    const std::initializer_list<value_type>& il) {
  resize(il.size());

  size_type i = 0;
  for (auto& value : il) {
    std::cout << value << std::endl;
    if ((value != ZERO) && (value != ONE)) {
      throw std::invalid_argument("input argument is not one or zero.");
    }

    replace(i, value);
    i++;
  }
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size() const noexcept {
  return bit_count();
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::reserve(size_type n) {
  buffer_.reserve((n + T_BIT_SIZE - 1) / T_BIT_SIZE);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::resize(size_type n) {
  buffer_.resize((n + T_BIT_SIZE - 1) / T_BIT_SIZE);
  last_byte_holder_ = &(buffer_.back());
  next_bit_position_ = (n % T_BIT_SIZE) == 0 ? T_BIT_SIZE : (n % T_BIT_SIZE);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::resize(
    size_type n, const value_type& value) {
  if ((value != ZERO) && (value != ONE)) {
    throw std::invalid_argument("input argument is not one or zero.");
  }

  const value_type element_value = value == ONE ? VALUE_MAX : ZERO;
  buffer_.resize((n + T_BIT_SIZE - 1) / T_BIT_SIZE, element_value);
  last_byte_holder_ = &(buffer_.back());
  next_bit_position_ = (n % T_BIT_SIZE) == 0 ? T_BIT_SIZE : (n % T_BIT_SIZE);

  // keep unused bits zero
  *last_byte_holder_ = MASK_PATTERNS[next_bit_position_ - 1];
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::buffer_element_count() const
    noexcept {
  return buffer_.size();
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::size_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::buffer_element_size() const
    noexcept {
  return T_BYTE_SIZE;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
bool bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::empty() const noexcept {
  return bit_count() == 0;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
    bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::operator[](
        size_type n) const {
  return bit_value(n);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::at(size_type n) const {
  return bit_value_at(n);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::front() const noexcept {
  return (MSB_TO_LSB ? buffer_[0] >> (T_BIT_SIZE - 1) : buffer_[0]) & ONE;
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
typename bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::value_type
bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::back() const noexcept {
  return bit_value(bit_count() - 1);
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
  push_with_resize(value);
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::pop() {
  pop_with_resize();
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::replace(
    size_type position, value_type value) {
  size_type byte_position = position / T_BIT_SIZE;
  size_type bit_position = position % T_BIT_SIZE;

  if (value == ONE) {
    buffer_.at(byte_position) |= BIT_PATTERNS[bit_position];
  }

  if (value == ZERO) {
    buffer_.at(byte_position) &= ~BIT_PATTERNS[bit_position];
  }
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::align(value_type value) {
  int b = 0;
  while (next_bit_position_ != T_BIT_SIZE) {
    push((value & BIT_PATTERNS[b]) == ZERO ? ZERO : ONE);
    b++;
  }
}

template <class BIT_CONTAINER_TYPE, bool MSB_TO_LSB, class TYPE_CHECK>
void bit<BIT_CONTAINER_TYPE, MSB_TO_LSB, TYPE_CHECK>::push_byte(
    unsigned char value) {
  if (std::is_same<BIT_CONTAINER_TYPE, unsigned char>::value &&
      (next_bit_position_ == T_BIT_SIZE)) {
    buffer_.resize(buffer_.size() + 1);
    last_byte_holder_ = &(buffer_.back());
    *(last_byte_holder_) = value;
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
      (next_bit_position_ == T_BIT_SIZE)) {
    buffer_.resize(buffer_.size() + n);
    last_byte_holder_ = &(buffer_.back());
    std::memcpy(buffer_.data() + buffer_.size() - n, data, n);
  } else {
    for (size_type j = 0; j < n; j++) {
      for (size_type i = 0; i < 8; i++) {
        push(((data[j] & BIT_PATTERNS[i]) == 0u) ? ZERO : ONE);
      }
    }
  }
}

}  // namespace jcy

#endif  // BIT_HPP_
