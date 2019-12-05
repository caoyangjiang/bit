#include <iostream>

#include "bit.hpp"

int main() {
  jcy::bit bit2;

  std::cout << "empty " << bit2.empty() << std::endl;
  bit2.push_with_resize(&bit2.bit_holder_, 1);

  std::cout << bit2[0] << " " << bit2.bit_count(bit2.bit_holder_) << " "
            << bit2.bit_remainder(bit2.bit_holder_) << std::endl;
  bit2.push_with_resize(&bit2.bit_holder_, 0);
  std::cout << bit2[1] << " " << bit2.bit_count(bit2.bit_holder_) << " "
            << bit2.bit_remainder(bit2.bit_holder_) << std::endl;
  bit2.replace(1, 1);
  std::cout << bit2[1] << " " << bit2.bit_count(bit2.bit_holder_) << " "
            << bit2.bit_remainder(bit2.bit_holder_) << std::endl;
  bit2.push_with_resize(&bit2.bit_holder_, 1);

  bit2.push_with_resize(&bit2.bit_holder_, 0);
  std::cout << "front " << bit2.front() << std::endl;
  std::cout << "back " << bit2.back() << std::endl;
  std::cout << std::hex << static_cast<uint64_t>(*bit2.data()) << std::endl;
  bit2.pop_with_resize(&bit2.bit_holder_);
  bit2.clear(&bit2.bit_holder_);

  bit2.resize(101, 1);
  std::cout << bit2.buffer_size() << std::endl;
  std::cout << std::dec << bit2.bit_count(bit2.bit_holder_) << std::endl;
  std::cout << std::hex << static_cast<uint64_t>(bit2.data()[1]) << std::endl;
  std::cout << "front " << bit2.front() << std::endl;
  std::cout << "back " << bit2.back() << std::endl;
  bit2.push_byte(0xFE);
  std::cout << "back " << bit2.back() << std::endl;
  std::cout << std::dec << "size " << bit2.size() << std::endl;
  bit2.align(0xFF);
  std::cout << std::dec << "size " << bit2.size() << std::endl;

  std::vector<uint8_t> bytes = {0x81, 0x91};
  bit2.push_bytes(bytes.data(), bytes.size());
  std::cout << std::dec << "size " << bit2.size() << std::endl;

  for (size_t i = 0; i < bit2.buffer_size(); i++)
    std::cout << "value " << std::hex << static_cast<uint64_t>(bit2.data()[i])
              << std::endl;
  return 0;
}
