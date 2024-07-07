#include "wrapping_integers.hh"

#include <iostream>
// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) { return isn + uint32_t(n); }

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    uint64_t x = 0;
    uint64_t y = 0;
    if (n - isn < 0) {
        x = uint64_t(n - isn + (1l << 32));
    } else {
        x = uint64_t(n - isn);
    }
    if (x >= checkpoint)
        return x;
    x = x|((checkpoint >> 32) << 32);
    while (x <= checkpoint)
        x += (1ll << 32);
    y = x - (1ll << 32);
    //return (checkpoint - tmp1 < tmp - checkpoint) ? tmp1 : tmp;
    if(x- checkpoint>= checkpoint -y){
    	return y;
    }
    else{
    	return x;
    }
}
