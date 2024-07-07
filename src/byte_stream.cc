#include "byte_stream.hh"

#include <algorithm>

// You will need to add private members to the class declaration in `byte_stream.hh`

/* Replace all the dummy definitions inside the methods in this file. */


using namespace std;

ByteStream::ByteStream(const size_t capa)
{
  this->sz=capa;
  this->endread=false;
  this->endwrite=false;
  this->popcnt=0;
  this->writecnt=0;
}

size_t ByteStream::write(const string &data) {
  int strsize=data.length();
  int empty=(this->sz)-(this->buffer.size());
  int limit=min(strsize,empty);
  size_t written=0;
  for(int i=0; i<limit; i++){
    if(empty==0){
      return written;
    }
    empty--;
    this->buffer.push_back(data[i]);
    this->writecnt++;
    written++;
  }
  return written;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
  string ans="";
  int length=len;
  int currsize=this->buffer.size();
  int limit=min(length,currsize);
  //int ind=this->buffer.size()-1;
  for(int i=0;i<limit;i++){
    ans+=this->buffer[i];
  }
  return ans;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
  int length=len;
  int currsize=this->buffer.size();
  //int limit=min(length,currsize);
  if(currsize<length){
    set_error();
    return;
  }
  for(int i=0;i<length;i++){
    this->buffer.pop_front();
    this->popcnt++;
  }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
  string ans="";
  int length=len;
  int currsize=this->buffer.size();
  //int limit=min(length,currsize);
  if(currsize<length){
    set_error();
    return ans;
  }
  for(int i=0;i<length;i++){
    ans+=this->buffer.front();
    this->buffer.pop_front();
    this->popcnt++;
  }
  return ans;
}

void ByteStream::end_input() {
  this->endwrite=true;
 }

bool ByteStream::input_ended() const {
   return this->endwrite;
   }

size_t ByteStream::buffer_size() const {
  return this->buffer.size(); 
  }

bool ByteStream::buffer_empty() const {
  if(this->buffer.empty()){
    return true;
  }
  return false; 
  }

bool ByteStream::eof() const { 
  if(this->buffer.size()==0 && endwrite==true){
    return true;
  }
  return false; 
  }

size_t ByteStream::bytes_written() const {
   return this->writecnt; 
   }

size_t ByteStream::bytes_read() const { 
  return this->popcnt;  
  }

size_t ByteStream::remaining_capacity() const { 
  return (this->sz)-(this->buffer.size()); 
  }

