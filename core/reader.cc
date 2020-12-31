// Copyright (c) 2020 smarsufan. All Rights Reserved.

#include "reader.h"

namespace smcc {

ReaderStdio::ReaderStdio(FILE *fp) : fp_(fp) {
}

ReaderStdio::~ReaderStdio() {
}

int ReaderStdio::getchar() {
  char ch;
  int ok = fread(&ch, 1, 1, fp_);
  if (ok <= 0) {
    return EOF;
  }
  return ch;
}

ReaderMem::ReaderMem(const char *mem, size_t size) 
    : mem_(mem), size_(size) {
  end_ = mem_ + size_;
}

ReaderMem::~ReaderMem() {
}

int ReaderMem::getchar() {
  if (mem_ >= end_) {
    return EOF;
  }
  return *(mem_++);
}

}  // namespace smcc
