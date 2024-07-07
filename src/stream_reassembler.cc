#include "stream_reassembler.hh"

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    :_output(capacity),buffer(capacity, '\0'),bitmap(capacity, false)
{
    this->lastByte=false;
    this->substringSize=0;
    this->unorderedIndex=0;
    this->maxBytes=capacity;
}


//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::checkContiguous(){
    string str="";
    while(this->bitmap.front()==true){
        str+=this->buffer.front();
        this->buffer.pop_front();
        this->bitmap.pop_front();
        this->buffer.push_back('\0');
        this->bitmap.push_back(false);
    }
    int len=str.length();
    if(len>0){
        this->_output.write(str);
        this->unorderedIndex+=len;
        this->substringSize-=len;
    }
}

void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if(eof==true){
        this->lastByte=true;
    }
    int len=data.length();
    if(this->lastByte==true && len==0 && this->substringSize==0){
        this->_output.end_input();
        return;
    }
    if(index>=this->unorderedIndex + this->maxBytes){
        return;
    }

    int rem=index-this->unorderedIndex;
    int lo;
    
    if(index>=this->unorderedIndex){
        int val=this->maxBytes-rem-this->_output.buffer_size();
        lo=min(val,len);
        if(len>lo){
            this->lastByte=false;
        }
        for(int i=0;i<lo;i++){
            if(this->bitmap[i+rem]==true){
                continue;
            }
            else{
                this->buffer[i+rem]=data[i];
                this->bitmap[i+rem]=true;
                this->substringSize++;
            }
        }
    }
    else if(index+len>this->unorderedIndex){
        rem=(-1)*rem;
        int val1=len-rem;
        int val2=this->maxBytes-this->_output.buffer_size();
        lo=min(val1,val2);
        if(lo<val1){
            this->lastByte=false;
        }
        for(int i=0; i<lo; i++){
            if(this->bitmap[i]==true){
                continue;
            }
            else{
                this->buffer[i]=data[i+rem];
                this->bitmap[i]=true;
                this->substringSize++;
            }
        }
    }
    
    checkContiguous();
    if(this->lastByte==true && this->substringSize==0){
        this->_output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { 
    return this->substringSize; 
    }

bool StreamReassembler::empty() const { 
    if(this->substringSize==0){
    	return true;
    }
    return false; 
    }

size_t StreamReassembler::ack_index() const { 
    return this->unorderedIndex; 
    }
