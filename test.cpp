#include <iostream>

#include "bit.hpp"

int main() {
  jcy::bit<uint64_t, true> bit2;

  bit2.push_with_resize(&bit2.bit_holder_, 1);
  bit2.push_with_resize(&bit2.bit_holder_, 0);
  bit2.push_with_resize(&bit2.bit_holder_, 1);
  bit2.push_with_resize(&bit2.bit_holder_, 0);
  std::cout << bit2.bit_value_at(&bit2.bit_holder_, 0) << std::endl;
  std::cout << bit2.bit_value_at(&bit2.bit_holder_, 1) << std::endl;
  std::cout << bit2.bit_value_at(&bit2.bit_holder_, 2) << std::endl;
  std::cout << bit2.bit_value_at(&bit2.bit_holder_, 3) << std::endl;
  std::cout << std::hex << static_cast<uint64_t>(*bit2.data()) << std::endl;
  bit2.pop_with_resize(&bit2.bit_holder_);
  bit2.clear(&bit2.bit_holder_);

  return 0;
}
