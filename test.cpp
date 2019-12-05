#include <iostream>

#include "bit.hpp"

int main() {
  jcy::bit<uint64_t, true> bit2;

  bit2.push_with_resize(&bit2.bit_holder_, 1);
  std::cout << bit2.bit_count(bit2.bit_holder_) << std::endl;
  std::cout << bit2.bit_remainder(bit2.bit_holder_) << std::endl;
  bit2.push_with_resize(&bit2.bit_holder_, 0);

  std::cout << bit2.bit_count(bit2.bit_holder_) << std::endl;
  std::cout << bit2.bit_remainder(bit2.bit_holder_) << std::endl;
  bit2.push_with_resize(&bit2.bit_holder_, 1);

  std::cout << bit2.bit_count(bit2.bit_holder_) << std::endl;
  std::cout << bit2.bit_remainder(bit2.bit_holder_) << std::endl;
  bit2.push_with_resize(&bit2.bit_holder_, 0);

  std::cout << bit2.bit_count(bit2.bit_holder_) << std::endl;
  std::cout << bit2.bit_remainder(bit2.bit_holder_) << std::endl;

  std::cout << std::hex << static_cast<uint64_t>(*bit2.data()) << std::endl;
  bit2.pop_with_resize(&bit2.bit_holder_);
  bit2.clear(&bit2.bit_holder_);

  bit2.resize(101, 1);
  std::cout << bit2.buffer_size() << std::endl;
  std::cout << std::dec << bit2.bit_count(bit2.bit_holder_) << std::endl;
  std::cout << std::hex << static_cast<uint64_t>(bit2.data()[1]) << std::endl;
  return 0;
}
