#pragma once
#ifndef COMPAT_H
#define COMPAT_H

/// In this file we define standard methods / features that may not be available 
/// on all compilers

#include <algorithm>
#include <cassert>
#if !defined(__cpp_lib_clamp)
#pragma message "Defining std::clamp since not supported by this compiler c++ library"
// C__14 and previous versions code here
namespace std
{
    template<class T>
    constexpr const T& clamp( const T& v, const T& lo, const T& hi )
    {
        assert( !(hi < lo) );
        return (v < lo) ? lo : (hi < v) ? hi : v;
    }

    template<class T, class Compare>
    constexpr const T& clamp( const T& v, const T& lo, const T& hi, Compare comp )
    {
        assert( !comp(hi, lo) );
        return comp(v, lo) ? lo : comp(hi, v) ? hi : v;
    }
}
#endif // !defined(__cpp_lib_clamp)

#endif // COMPAT_H