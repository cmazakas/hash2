
#include <boost/core/detail/string_view.hpp>
#include <boost/core/span.hpp>

#include <boost/core/lightweight_test.hpp>

#include <array>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

unsigned rotl(unsigned x, unsigned n) { return (x << n) | (x >> (32 - n)); }

unsigned f1(unsigned x, unsigned y, unsigned z) { return (x ^ y ^ z); }
unsigned f2(unsigned x, unsigned y, unsigned z) { return (x & y) | (~x & z); }
unsigned f3(unsigned x, unsigned y, unsigned z) { return (x | ~y) ^ z; }
unsigned f4(unsigned x, unsigned y, unsigned z) { return (x & z) | (y & ~z); }
unsigned f5(unsigned x, unsigned y, unsigned z) { return x ^ (y | ~z); }

void md_init(boost::span<unsigned, 5> md_buf) {
  md_buf[0] = 0x67452301U;
  md_buf[1] = 0xefcdab89U;
  md_buf[2] = 0x98badcfeU;
  md_buf[3] = 0x10325476U;
  md_buf[4] = 0xc3d2e1f0U;

  return;
}

void compress(boost::span<unsigned, 5> MDbuf, boost::span<unsigned, 16> X) {
  unsigned aa = MDbuf[0], bb = MDbuf[1], cc = MDbuf[2], dd = MDbuf[3], ee = MDbuf[4];
  unsigned aaa = MDbuf[0], bbb = MDbuf[1], ccc = MDbuf[2], ddd = MDbuf[3], eee = MDbuf[4];

  /* round 1 */
  for (unsigned j = 0; j < 16; ++j) {
    unsigned K = 0x00000000u;
    unsigned r[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    unsigned s[16] = {11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8};

    auto T = rotl(aa + f1(bb, cc, dd) + X[r[j]] + K, s[j]) + ee;
    aa = ee;
    ee = dd;
    dd = rotl(cc, 10);
    cc = bb;
    bb = T;
  }

  /* round 2 */
  for (unsigned j = 0; j < 16; ++j) {
    unsigned K = 0x5A827999u;
    unsigned r[16] = {7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8};
    unsigned s[16] = {7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12};

    auto T = rotl(aa + f2(bb, cc, dd) + X[r[j]] + K, s[j]) + ee;
    aa = ee;
    ee = dd;
    dd = rotl(cc, 10);
    cc = bb;
    bb = T;
  }

  /* round 3 */
  for (unsigned j = 0; j < 16; ++j) {
    unsigned K = 0x6ED9EBA1u;
    unsigned r[16] = {3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12};
    unsigned s[16] = {11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5};

    auto T = rotl(aa + f3(bb, cc, dd) + X[r[j]] + K, s[j]) + ee;
    aa = ee;
    ee = dd;
    dd = rotl(cc, 10);
    cc = bb;
    bb = T;
  }

  /* round 4 */
  for (unsigned j = 0; j < 16; ++j) {
    unsigned K = 0x8F1BBCDCu;
    unsigned r[16] = {1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2};
    unsigned s[16] = {11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12};

    auto T = rotl(aa + f4(bb, cc, dd) + X[r[j]] + K, s[j]) + ee;
    aa = ee;
    ee = dd;
    dd = rotl(cc, 10);
    cc = bb;
    bb = T;
  }

  /* round 5 */
  for (unsigned j = 0; j < 16; ++j) {
    unsigned K = 0xA953FD4Eu;
    unsigned r[16] = {4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13};
    unsigned s[16] = {9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6};

    auto T = rotl(aa + f5(bb, cc, dd) + X[r[j]] + K, s[j]) + ee;
    aa = ee;
    ee = dd;
    dd = rotl(cc, 10);
    cc = bb;
    bb = T;
  }

  /* parallel round 1 */
  for (unsigned j = 0; j < 16; ++j) {
    unsigned K = 0x50a28be6u;
    unsigned r[16] = {5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12};
    unsigned s[16] = {8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6};

    auto T = rotl(aaa + f5(bbb, ccc, ddd) + X[r[j]] + K, s[j]) + eee;
    aaa = eee;
    eee = ddd;
    ddd = rotl(ccc, 10);
    ccc = bbb;
    bbb = T;
  }

  /* parallel round 2 */
  for (unsigned j = 0; j < 16; ++j) {
    unsigned K = 0x5C4DD124u;
    unsigned r[16] = {6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2};
    unsigned s[16] = {9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11};

    auto T = rotl(aaa + f4(bbb, ccc, ddd) + X[r[j]] + K, s[j]) + eee;
    aaa = eee;
    eee = ddd;
    ddd = rotl(ccc, 10);
    ccc = bbb;
    bbb = T;
  }

  /* parallel round 3 */
  for (unsigned j = 0; j < 16; ++j) {
    unsigned K = 0x6D703EF3u;
    unsigned r[16] = {15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13};
    unsigned s[16] = {9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5};

    auto T = rotl(aaa + f3(bbb, ccc, ddd) + X[r[j]] + K, s[j]) + eee;
    aaa = eee;
    eee = ddd;
    ddd = rotl(ccc, 10);
    ccc = bbb;
    bbb = T;
  }

  /* parallel round 4 */
  for (unsigned j = 0; j < 16; ++j) {
    unsigned K = 0x7A6D76E9u;
    unsigned r[16] = {8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14};
    unsigned s[16] = {15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8};

    auto T = rotl(aaa + f2(bbb, ccc, ddd) + X[r[j]] + K, s[j]) + eee;
    aaa = eee;
    eee = ddd;
    ddd = rotl(ccc, 10);
    ccc = bbb;
    bbb = T;
  }

  /* parallel round 5 */
  for (unsigned j = 0; j < 16; ++j) {
    unsigned K = 0x00000000u;
    unsigned r[16] = {12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11};
    unsigned s[16] = {8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11};

    auto T = rotl(aaa + f1(bbb, ccc, ddd) + X[r[j]] + K, s[j]) + eee;
    aaa = eee;
    eee = ddd;
    ddd = rotl(ccc, 10);
    ccc = bbb;
    bbb = T;
  }

  auto T = MDbuf[1] + cc + ddd;
  MDbuf[1] = MDbuf[2] + dd + eee;
  MDbuf[2] = MDbuf[3] + ee + aaa;
  MDbuf[3] = MDbuf[4] + aa + bbb;
  MDbuf[4] = MDbuf[0] + bb + ccc;
  MDbuf[0] = T;

  return;
}

/********************************************************************/

void finish(boost::span<unsigned, 5> md_buf, unsigned char const* strptr, std::uint64_t input_len) {
  unsigned X[16] = {};
  std::memcpy(X, strptr, input_len % 64);

  // append the trailing 1 to the input
  // we know the last byte written to is :
  // input_len / 4 => number of ints
  // % 16 => constrain to our 16 element-wide array
  //
  // we then need to find out which byte offset to append the 1 to
  // example:
  // input_len = 75
  // number of bytes to write: 75 % 64 = 11
  // number of total complete ints: 75 / 4 = 18
  // number of complete ints we can write: 18 % 16 = 2
  // leftover bytes: 75 % 4 = 3
  //
  // 2 is a valid index here because we've written 11 bytes, which is 4 + 4 + 3 bytes
  // so 2 complete ints, but 3 remnant bytes
  //
  // we unconditionally set the 7th bit
  //
  // therefore we know that at array index 2, we must write past the 3rd byte
  //
  X[(input_len / 4) % 16] ^= (unsigned)1 << (8 * (input_len % 4) + 7);

  /* must reserve 8 bytes (aka 2 ints) for storing input len */
  if ((input_len % 64) >= 56) {
    /* length goes to next block */
    compress(md_buf, X);
    memset(X, 0, 16 * sizeof(unsigned));
  }

  /* append length in bits*/
  input_len *= 8;
  std::memcpy(X + 14, &input_len, sizeof(input_len));
  compress(md_buf, X);

  return;
}

std::array<unsigned char, 20> ripemd_160(boost::span<unsigned char const> message) {
  std::array<unsigned char, 20> out = {};

  unsigned md_buf[5];   /* contains (A, B, C, D(, E))   */
  unsigned X[16] = {}; /* current 16-word chunk        */

  md_init(md_buf);

  /* process message in 16-word chunks */
  auto* pos = message.data();
  for (unsigned nbytes = message.size(); nbytes > 63; nbytes -= 64) {
    std::memcpy(X, pos, 64);
    compress(md_buf, X);
    pos += 4;
  } /* length mod 64 bytes left */

  /* finish: */
  finish(md_buf, pos, message.size());
  std::memcpy(out.data(), md_buf, out.size());
  return out;
}

void testsuite() {

  auto test_digest = [](boost::core::string_view input, boost::core::string_view expected_digest) {
    boost::span<unsigned char const> buf(reinterpret_cast<unsigned char const*>(input.data()), input.size());
    auto out = ripemd_160(buf);

    std::array<char, 40 + 1> hex_digest = {};
    auto* pos = hex_digest.data();
    for (auto c : out) {
      pos += sprintf(pos, "%02x", c);
    }
    boost::core::string_view digest(hex_digest.data(), 40);
    BOOST_TEST_EQ(digest, expected_digest);
  };

  std::vector<char> digits;
  for (int i = 0; i < 8; ++i) {
    boost::core::string_view tmp = "1234567890";
    digits.insert(digits.end(), tmp.begin(), tmp.end());
  }

  std::string str(1000000, 'a');

  test_digest("", "9c1185a5c5e9fc54612808977ee8f548b2258d31");
  test_digest("a", "0bdc9d2d256b3ee9daae347be6f4dc835a467ffe");
  test_digest("abc", "8eb208f7e05d987a9b044a8e98c6b087f15a0bfc");
  test_digest("message digest", "5d0689ef49d2fae572b881b123a85ffa21595f36");
  test_digest("abcdefghijklmnopqrstuvwxyz", "f71c27109c692c1b56bbdceb5b9d2865b3708dbc");
  test_digest("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "12a053384a9c0c88e405a06c27dcf49ada62eb2b");
  test_digest("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
              "b0e20b6e3116640286ed3a87a5713079b21f5189");
  test_digest({digits.data(), digits.size()}, "9b752e45573d4b39f4dbd3323cab82bf63326bfb");
  test_digest(str, "52783243c1697bdbe16d37f97f68f08325dc1528");

  return;
}

int main() {
  testsuite();
  return boost::report_errors();
}
