#include <array>
#include <bit>
#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>

auto f(unsigned j, unsigned x, unsigned y, unsigned z) {
  if (j < 16) {
    return (x ^ y ^ z);
  }

  if (j < 32) {
    assert(j >= 16);
    return (x & y) | (~x & z);
  }

  if (j < 48) {
    assert(j >= 32);
    return (x | ~y) ^ z;
  }

  if (j < 64) {
    assert(j >= 48);
    return x ^ (y | ~z);
  }

  assert(j >= 64);
  return x ^ (y | ~z);
}

unsigned K(unsigned j) {
  if (j < 16) {
    return 0x00000000u;
  }

  if (j < 32) {
    return 0x5A827999u;
  }

  if (j < 48) {
    return 0x6ED9EBA1u;
  }

  if (j < 64) {
    return 0x8F1BBCDCu;
  }

  return 0xA953FD4Eu;
}

unsigned K_prime(unsigned j) {
  if (j < 16) {
    return 0x50A28BE6u;
  }

  if (j < 32) {
    return 0x5C4DD124u;
  }

  if (j < 48) {
    return 0x6D703EF3u;
  }

  if (j < 64) {
    return 0x7A6D76E9u;
  }

  return 0x00000000u;
}

auto r(unsigned j) {
  if (j < 16) {
    return j;
  }

  unsigned r16_31[16] = {7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8};
  unsigned r32_47[16] = {3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12};
  unsigned r48_63[16] = {1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2};
  unsigned r64_79[16] = {4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13};

  if (j < 32) {
    return r16_31[j % 16];
  }

  if (j < 48) {
    return r32_47[j % 16];
  }

  if (j < 64) {
    return r48_63[j % 16];
  }

  return r64_79[j % 16];
}

auto r_prime(unsigned j) {
  unsigned r0_15[16] = {5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12};
  unsigned r16_31[16] = {6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2};
  unsigned r32_47[16] = {15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13};
  unsigned r48_63[16] = {8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14};
  unsigned r64_79[16] = {12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11};

  if (j < 16) {
    return r0_15[j % 16];
  }

  if (j < 32) {
    return r16_31[j % 16];
  }

  if (j < 48) {
    return r32_47[j % 16];
  }

  if (j < 64) {
    return r48_63[j % 16];
  }

  return r64_79[j % 16];
}

unsigned s(unsigned j) {
  unsigned s0_15[16] = {11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8};
  unsigned s16_31[16] = {7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12};
  unsigned s32_47[16] = {11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5};
  unsigned s48_63[16] = {11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12};
  unsigned s64_79[16] = {9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6};

  if (j < 16) {
    return s0_15[j % 16];
  }

  if (j < 32) {
    return s16_31[j % 16];
  }

  if (j < 48) {
    return s32_47[j % 16];
  }

  if (j < 64) {
    return s48_63[j % 16];
  }

  return s64_79[j % 16];
}

unsigned s_prime(unsigned j) {
  unsigned s0_15[16] = {8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6};
  unsigned s16_31[16] = {9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11};
  unsigned s32_47[16] = {9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5};
  unsigned s48_63[16] = {15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8};
  unsigned s64_79[16] = {8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11};

  if (j < 16) {
    return s0_15[j % 16];
  }

  if (j < 32) {
    return s16_31[j % 16];
  }

  if (j < 48) {
    return s32_47[j % 16];
  }

  if (j < 64) {
    return s48_63[j % 16];
  }

  return s64_79[j % 16];
}

std::array<unsigned char, 20> ripemd_160(char const* X, std::size_t n) {
  std::array<unsigned char, 20> out = {};

  unsigned h0 = 0x67452301;
  unsigned h1 = 0xEFCDAB89;
  unsigned h2 = 0x98BADCFE;
  unsigned h3 = 0x10325476;
  unsigned h4 = 0xC3D2E1F0;

  unsigned concat[5] = {};

  for (unsigned i = 0; i < n; ++i) {
    auto const* X_i = X + 64 * i;

    auto A = h0;
    auto B = h1;
    auto C = h2;
    auto D = h3;
    auto E = h4;

    auto A_prime = h0;
    auto B_prime = h1;
    auto C_prime = h2;
    auto D_prime = h3;
    auto E_prime = h4;

    for (unsigned j = 0; j < 80; ++j) {
      auto T = std::rotl(A + f(j, B, C, D) + X_i[r(j)] + K(j), s(j)) + E;
      A = E;
      E = D;
      D = std::rotl(C, 10);
      C = B;
      B = T;

      T = std::rotl(A_prime + f(79 - j, B_prime, C_prime, D_prime) + X_i[r_prime(j)] + K_prime(j), s_prime(j)) +
          E_prime;

      A_prime = E_prime;
      E_prime = D_prime;
      D_prime = std::rotl(C_prime, 10);
      C_prime = B_prime;
      B_prime = T;
    }

    auto T = h1 + C + D_prime;
    h1 = h2 + D + E_prime;
    h2 = h3 + E + A_prime;
    h3 = h4 + A + B_prime;
    h4 = h0 + B + C_prime;
    h0 = T;
  }

  concat[0] = h0;
  concat[1] = h1;
  concat[2] = h2;
  concat[3] = h3;
  concat[4] = h4;

  std::memcpy(out.data(), reinterpret_cast<unsigned char const*>(concat), out.size());

  return out;
}

std::string string_to_hex(std::string_view input) {
  static const char hex_digits[] = "0123456789ABCDEF";

  std::string output;
  output.reserve(input.length() * 2);
  for (unsigned char c : input) {
    output.push_back(hex_digits[c >> 4]);
    output.push_back(hex_digits[c & 15]);
  }
  return output;
}

int main() {
  char X[64] = {};
  X[0] = 0b10000000;

  auto digest = ripemd_160(X, 1);

  std::string_view msg(reinterpret_cast<char const*>(digest.data()), digest.size());
  std::cout << string_to_hex(msg) << std::endl;
}
