#ifndef BOOST_HASH2_OPENSSL_HPP_INCLUDED
#define BOOST_HASH2_OPENSSL_HPP_INCLUDED

// Copyright 2017, 2018 Peter Dimov.
// Copyright 2024 Christian Mazakas
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
//
// SHA1 message digest algorithm, https://tools.ietf.org/html/rfc3174

#include <openssl/evp.h>

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
namespace openssl
{

class ripemd_160
{
private:
    EVP_MD_CTX* p_ctx_ = nullptr;

public:

    typedef std::array<unsigned char, 20> result_type;
    typedef std::uint64_t size_type;

    static const int block_size = 64;

    ripemd_160(): p_ctx_( EVP_MD_CTX_new() )
    {
      if( !EVP_DigestInit( p_ctx_, EVP_ripemd160() ) )
      {
        throw 1;
      }
    }

    ~ripemd_160()
    {
      EVP_MD_CTX_free( p_ctx_ );
    }

    ripemd_160( ripemd_160 const& ) = delete;
    ripemd_160& operator=( ripemd_160 const& ) = delete;

    void update( void const * pv, std::size_t n )
    {
        EVP_DigestUpdate( p_ctx_, pv, n );
    }

    result_type result()
    {
        result_type digest = {};
        EVP_DigestFinal( p_ctx_, digest.data(), nullptr );
        return digest;
    }
};

class md5_128
{
private:
    EVP_MD_CTX* p_ctx_ = nullptr;

public:

    typedef std::array<unsigned char, 16> result_type;
    typedef std::uint64_t size_type;

    md5_128(): p_ctx_( EVP_MD_CTX_new() )
    {
      if( !EVP_DigestInit( p_ctx_, EVP_md5() ) )
      {
        throw 1;
      }
    }

    ~md5_128()
    {
      EVP_MD_CTX_free( p_ctx_ );
    }

    md5_128( md5_128 const& ) = delete;
    md5_128& operator=( md5_128 const& ) = delete;

    void update( void const * pv, std::size_t n )
    {
        EVP_DigestUpdate( p_ctx_, pv, n );
    }

    result_type result()
    {
        result_type digest = {};
        EVP_DigestFinal( p_ctx_, digest.data(), nullptr );
        return digest;
    }
};

class sha1_160
{
private:
    EVP_MD_CTX* p_ctx_ = nullptr;

public:

    typedef std::array<unsigned char, 20> result_type;
    typedef std::uint64_t size_type;

    static const int block_size = 64;

    sha1_160(): p_ctx_( EVP_MD_CTX_new() )
    {
      if( !EVP_DigestInit( p_ctx_, EVP_sha1() ) )
      {
        throw 1;
      }
    }

    ~sha1_160()
    {
      EVP_MD_CTX_free( p_ctx_ );
    }

    sha1_160( sha1_160 const& ) = delete;
    sha1_160& operator=( sha1_160 const& ) = delete;

    void update( void const * pv, std::size_t n )
    {
        EVP_DigestUpdate( p_ctx_, pv, n );
    }

    result_type result()
    {
        result_type digest = {};
        EVP_DigestFinal( p_ctx_, digest.data(), nullptr );
        return digest;
    }
};

} // namespace openssl
} // namespace hash2
} // namespace boost

#endif // #ifndef BOOST_HASH2_OPENSSL_HPP_INCLUDED
