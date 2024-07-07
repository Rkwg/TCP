#include "tcp_receiver.hh"

#include <algorithm>


using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    const TCPHeader head = seg.header();
    // ...

    // --- Hint ------
        // convert the seqno into absolute seqno
    // uint64_t checkpoint = _reassembler.ack_index();
    // uint64_t abs_seqno = unwrap(head.seqno, _isn, checkpoint);
    // uint64_t stream_idx = abs_seqno - _synReceived;
    // --- Hint ------  

    // ... 

    if(this->_synReceived==false && head.syn==false){
        return;
    }
    string str=seg.payload().copy();
    bool eof=false;
    if(head.syn==true && this->_synReceived==false){
        this->_synReceived=true;
        _isn=head.seqno;
        if(head.fin==true){
            this->_finReceived=true;
            eof=true;
        }
        this->_reassembler.push_substring(str, 0, eof);
        return;
    }
    if(this->_synReceived ==true && head.fin==true){
        this->_finReceived=true;
        eof=true;
    }
    uint64_t checkpoint = _reassembler.ack_index();
    uint64_t abs_seqno = unwrap(head.seqno, _isn, checkpoint);
    uint64_t stream_idx = abs_seqno - _synReceived;
    this->_reassembler.push_substring(str, stream_idx, eof);
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if(this->_synReceived==false){
        return nullopt;
    }
    return wrap(this->_reassembler.ack_index() +(this->_reassembler.empty() && _finReceived), _isn) +1;
}

size_t TCPReceiver::window_size() const { 
    return this->_capacity - (this->_reassembler.stream_out().buffer_size());
    }

