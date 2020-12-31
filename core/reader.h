// Copyright (c) 2020 smarsufan. All Rights Reserved.

#pragma once
#include <cstdio>

namespace smcc {

/// The code reader.
class Reader {
 public:
  Reader() = default;

  virtual ~Reader() = default;

  // Read one char.
  virtual int getchar() = 0;
 private:
};

class ReaderStdio : public Reader {
 public:
  ReaderStdio(FILE *fp);

  ~ReaderStdio();

  virtual int getchar();

 private:
  FILE *fp_{nullptr};
};

class ReaderMem : public Reader {
 public:
  ReaderMem(const char *mem, size_t size);

  ~ReaderMem();

  virtual int getchar();

 private:
  const char *mem_{nullptr};
  size_t size_{0};
  const char *end_{nullptr};
};

}  // namespace smcc
