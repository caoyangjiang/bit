#include <iostream>

#include "bit.hpp"

int main() {
  jcy::bit<uint64_t> bit2;

  // std::cout << "empty " << bit2.empty() << std::endl;
  // bit2.push_with_resize(1);

  // std::cout << bit2[0] << " " << bit2.bit_count() << " " <<
  // bit2.bit_remainder()
  //           << std::endl;
  // bit2.push_with_resize(0);
  // std::cout << bit2[1] << " " << bit2.bit_count() << " " <<
  // bit2.bit_remainder()
  //           << std::endl;
  // bit2.replace(1, 1);
  // std::cout << bit2[1] << " " << bit2.bit_count() << " " <<
  // bit2.bit_remainder()
  //           << std::endl;
  // bit2.push_with_resize(1);

  // bit2.push_with_resize(0);
  // std::cout << "front " << bit2.front() << std::endl;
  // std::cout << "back " << bit2.back() << std::endl;
  // std::cout << std::hex << static_cast<uint64_t>(*bit2.data()) << std::endl;
  // bit2.pop_with_resize();
  // bit2.clear();

  // bit2.resize(101, 1);
  // std::cout << bit2.buffer_size() << std::endl;
  // std::cout << std::dec << bit2.bit_count() << std::endl;
  // std::cout << std::hex << static_cast<uint64_t>(bit2.data()[1]) <<
  // std::endl; std::cout << "front " << bit2.front() << std::endl; std::cout <<
  // "back " << bit2.back() << std::endl;
  bit2.push_byte(0xFE);
  std::cout << "back " << bit2.back() << std::endl;
  std::cout << std::dec << "size " << bit2.size() << std::endl;
  bit2.align(0xFF);
  std::cout << std::dec << "size" << bit2.size() << std::endl;

  std::vector<uint8_t> bytes = {0x81, 0x91};
  bit2.push_bytes(bytes.data(), bytes.size());
  std::cout << std::dec << "size " << bit2.size() << std::endl;

  std::chrono::high_resolution_clock::time_point start, end;

  bit2.reserve(1000000);
  start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < 20000000; i++) {
    bit2.push(0);
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> dur = end - start;
  std::cout << dur.count() << std::endl;
  std::cout << "write " << 20000000 / dur.count() << " bits/s" << std::endl;

  start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < 20000000; i++) {
    (void)bit2[i];
  }
  end = std::chrono::high_resolution_clock::now();
  dur = end - start;
  std::cout << dur.count() << std::endl;
  std::cout << "read " << 20000000 / dur.count() << " bits/s" << std::endl;

  jcy::bit<uint64_t> value;
  value = {0, 1, 1};
  std::cout << value.size() << std::endl;
  std::cout << value[0] << std::endl;
  std::cout << value[1] << std::endl;
  std::cout << value[2] << std::endl;
  value = bit2;
  std::cout << value.size() << std::endl;
  std::cout << bit2.size() << std::endl;

  std::vector<uint64_t> input = {0xFE};
  jcy::bit<uint64_t> value2(input.data(), 64);
  std::cout << value2.size() << std::endl;
  std::cout << value2.back() << std::endl;

  jcy::bit<uint64_t> value3(input.data(), 63);
  std::cout << value3.size() << std::endl;
  std::cout << value3.back() << std::endl;

  jcy::bit<uint64_t> value4;
  value4 = value2 + value3;
  std::cout << value2.size() << std::endl;
  std::cout << value4.size() << std::endl;
  std::cout << value4.back() << std::endl;
  std::cout << value4.buffer_element_count() * value4.buffer_element_size()
            << std::endl;

  return 0;
}
