#ifndef BOOST_HASH2_RIPEMD_HPP_INCLUDED
#define BOOST_HASH2_RIPEMD_HPP_INCLUDED

// Copyright 2017, 2018 Peter Dimov.
// Copyright 2024 Christian Mazakas
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
//
// SHA1 message digest algorithm, https://tools.ietf.org/html/rfc3174

#include <boost/hash2/detail/read.hpp>
#include <boost/hash2/detail/write.hpp>
#include <boost/hash2/detail/rot.hpp>
#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <cstdint>
#include <array>
#include <cstring>
#include <cstddef>

namespace boost
{
namespace hash2
{

class ripemd_160
{
private:

    std::uint32_t state_[ 5 ];

    static const int N = 64;

    unsigned char buffer_[ N ];
    std::size_t m_; // == n_ % N

    std::uint64_t n_;

private:

    void init()
    {
        state_[ 0 ] = 0x67452301u;
        state_[ 1 ] = 0xefcdab89u;
        state_[ 2 ] = 0x98badcfeu;
        state_[ 3 ] = 0x10325476u;
        state_[ 4 ] = 0xc3d2e1f0u;
    }

    static BOOST_FORCEINLINE std::uint32_t f1(std::uint32_t x, std::uint32_t y, std::uint32_t z)
    {
        return (x ^ y ^ z);
    }

    static BOOST_FORCEINLINE std::uint32_t f2(std::uint32_t x, std::uint32_t y, std::uint32_t z)
    {
        return (x & y) | (~x & z);
    }

    static BOOST_FORCEINLINE std::uint32_t f3(std::uint32_t x, std::uint32_t y, std::uint32_t z)
    {
        return (x | ~y) ^ z;
    }

    static BOOST_FORCEINLINE std::uint32_t f4(std::uint32_t x, std::uint32_t y, std::uint32_t z)
    {
        return (x & z) | (y & ~z);
    }

    static BOOST_FORCEINLINE std::uint32_t f5(std::uint32_t x, std::uint32_t y, std::uint32_t z)
    {
        return x ^ (y | ~z);
    }


    void transform( unsigned char const block[ 64 ] )
    {
        std::uint32_t aa = state_[ 0 ];
        std::uint32_t bb = state_[ 1 ];
        std::uint32_t cc = state_[ 2 ];
        std::uint32_t dd = state_[ 3 ];
        std::uint32_t ee = state_[ 4 ];

        std::uint32_t aaa = state_[ 0 ];
        std::uint32_t bbb = state_[ 1 ];
        std::uint32_t ccc = state_[ 2 ];
        std::uint32_t ddd = state_[ 3 ];
        std::uint32_t eee = state_[ 4 ];

        std::uint32_t X[ 16 ];

        for( int i = 0; i < 16; ++i )
        {
        X[ i ] = detail::read32le( block + i * 4 );
        }

        for( int i = 0; i < 16; ++i )
        {
        // round 1

        std::uint32_t K = 0x00000000u;
        std::uint32_t r[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        std::uint32_t s[16] = {11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8};

        auto T = detail::rotl(aa + f1(bb, cc, dd) + X[r[i]] + K, s[i]) + ee;
        aa = ee;
        ee = dd;
        dd = detail::rotl(cc, 10);
        cc = bb;
        bb = T;
        }

        for( int i = 0; i < 16; ++i )
        {
        // round 2

        std::uint32_t K = 0x5a827999u;
        std::uint32_t r[16] = {7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8};
        std::uint32_t s[16] = {7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12};

        auto T = detail::rotl(aa + f2(bb, cc, dd) + X[r[i]] + K, s[i]) + ee;
        aa = ee;
        ee = dd;
        dd = detail::rotl(cc, 10);
        cc = bb;
        bb = T;
        }

        for( int i = 0; i < 16; ++i )
        {
        // round 3

        std::uint32_t K = 0x6ed9eba1u;
        std::uint32_t r[16] = {3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12};
        std::uint32_t s[16] = {11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5};

        auto T = detail::rotl(aa + f3(bb, cc, dd) + X[r[i]] + K, s[i]) + ee;
        aa = ee;
        ee = dd;
        dd = detail::rotl(cc, 10);
        cc = bb;
        bb = T;
        }

        for( int i = 0; i < 16; ++i )
        {
        // round 4

        std::uint32_t K = 0x8f1bbcdcu;
        std::uint32_t r[16] = {1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2};
        std::uint32_t s[16] = {11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12};

        auto T = detail::rotl(aa + f4(bb, cc, dd) + X[r[i]] + K, s[i]) + ee;
        aa = ee;
        ee = dd;
        dd = detail::rotl(cc, 10);
        cc = bb;
        bb = T;
        }

        for( int i = 0; i < 16; ++i )
        {
        // round 5

        std::uint32_t K = 0xa953fd4eu;
        std::uint32_t r[16] = {4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13};
        std::uint32_t s[16] = {9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6};

        auto T = detail::rotl(aa + f5(bb, cc, dd) + X[r[i]] + K, s[i]) + ee;
        aa = ee;
        ee = dd;
        dd = detail::rotl(cc, 10);
        cc = bb;
        bb = T;
        }

        for( int i = 0; i < 16; ++i )
        {
        // parallel round 1

        std::uint32_t K = 0x50a28be6u;
        std::uint32_t r[16] = {5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12};
        std::uint32_t s[16] = {8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6};

        auto T = detail::rotl(aaa + f5(bbb, ccc, ddd) + X[r[i]] + K, s[i]) + eee;
        aaa = eee;
        eee = ddd;
        ddd = detail::rotl(ccc, 10);
        ccc = bbb;
        bbb = T;
        }

        for( int i = 0; i < 16; ++i )
        {
        // parallel round 2

        std::uint32_t K = 0x5c4dd124u;
        std::uint32_t r[16] = {6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2};
        std::uint32_t s[16] = {9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11};

        auto T = detail::rotl(aaa + f4(bbb, ccc, ddd) + X[r[i]] + K, s[i]) + eee;
        aaa = eee;
        eee = ddd;
        ddd = detail::rotl(ccc, 10);
        ccc = bbb;
        bbb = T;
        }

        for( int i = 0; i < 16; ++i )
        {
        // parallel round 3

        std::uint32_t K = 0x6d703ef3u;
        std::uint32_t r[16] = {15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13};
        std::uint32_t s[16] = {9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5};

        auto T = detail::rotl(aaa + f3(bbb, ccc, ddd) + X[r[i]] + K, s[i]) + eee;
        aaa = eee;
        eee = ddd;
        ddd = detail::rotl(ccc, 10);
        ccc = bbb;
        bbb = T;
        }

        for( int i = 0; i < 16; ++i )
        {
        // parallel round 4

        std::uint32_t K = 0x7a6d76e9u;
        std::uint32_t r[16] = {8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14};
        std::uint32_t s[16] = {15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8};

        auto T = detail::rotl(aaa + f2(bbb, ccc, ddd) + X[r[i]] + K, s[i]) + eee;
        aaa = eee;
        eee = ddd;
        ddd = detail::rotl(ccc, 10);
        ccc = bbb;
        bbb = T;
        }

        for( int i = 0; i < 16; ++i )
        {
        // parallel round 5

        std::uint32_t K = 0x00000000u;
        std::uint32_t r[16] = {12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11};
        std::uint32_t s[16] = {8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11};

        auto T = detail::rotl(aaa + f1(bbb, ccc, ddd) + X[r[i]] + K, s[i]) + eee;
        aaa = eee;
        eee = ddd;
        ddd = detail::rotl(ccc, 10);
        ccc = bbb;
        bbb = T;
        }

        auto T = state_[ 1 ] + cc + ddd;

        state_[ 1 ] = state_[ 2 ] + dd + eee;
        state_[ 2 ] = state_[ 3 ] + ee + aaa;
        state_[ 3 ] = state_[ 4 ] + aa + bbb;
        state_[ 4 ] = state_[ 0 ] + bb + ccc;
        state_[ 0 ] = T;
    }

public:

    typedef std::array<unsigned char, 20> result_type;
    typedef std::uint64_t size_type;

    static const int block_size = 64;

    ripemd_160(): m_( 0 ), n_( 0 )
    {
        init();
    }

    explicit ripemd_160( std::uint64_t seed ): m_( 0 ), n_( 0 )
    {
        init();

        if( seed != 0 )
        {
            unsigned char tmp[ 8 ];
            detail::write64le( tmp, seed );

            update( tmp, 8 );
            result();
        }
    }

    ripemd_160( unsigned char const * p, std::size_t n ): m_( 0 ), n_( 0 )
    {
        init();

        if( n != 0 )
        {
            update( p, n );
            result();
        }
    }

    void update( void const * pv, std::size_t n )
    {
        unsigned char const* p = static_cast<unsigned char const*>( pv );

        BOOST_ASSERT( m_ == n_ % N );

        if( n == 0 ) return;

        n_ += n;

        if( m_ > 0 )
        {
            std::size_t k = N - m_;

            if( n < k )
            {
                k = n;
            }

            std::memcpy( buffer_ + m_, p, k );

            p += k;
            n -= k;
            m_ += k;

            if( m_ < N ) return;

            BOOST_ASSERT( m_ == N );

            transform( buffer_ );
            m_ = 0;

            std::memset( buffer_, 0, N );
        }

        BOOST_ASSERT( m_ == 0 );

        while( n >= N )
        {
            transform( p );

            p += N;
            n -= N;
        }

        BOOST_ASSERT( n < N );

        if( n > 0 )
        {
            std::memcpy( buffer_, p, n );
            m_ = n;
        }

        BOOST_ASSERT( m_ == n_ % N );
    }

    result_type result()
    {
        BOOST_ASSERT( m_ == n_ % N );

        unsigned char bits[ 8 ];

        detail::write64le( bits, n_ * 8 );

        std::size_t k = m_ < 56? 56 - m_: 120 - m_;

        unsigned char padding[ 64 ] = { 0x80 };

        update( padding, k );

        update( bits, 8 );

        BOOST_ASSERT( m_ == 0 );

        result_type digest;

        for( int i = 0; i < 5; ++i )
        {
            detail::write32le( &digest[ i * 4 ], state_[ i ] );
        }

        return digest;
    }
};

} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_RIPEMD_HPP_INCLUDED
