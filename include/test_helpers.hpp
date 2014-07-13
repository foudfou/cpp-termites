#include <cstdio>
#include "ext/log.h"

class LogCapture {
public:
  LogCapture() : buffer(nullptr), bufferSize(0)
  {
    bak = Output2FILE::Stream();
    ss = open_memstream(&buffer, &bufferSize);
    Output2FILE::Stream() = ss;
  }
  ~LogCapture()
  {
    Output2FILE::Stream() = bak;
    fflush(ss);
    fclose(ss);
    free(buffer);
  }

  char* getBuffer();

private:
  char* buffer;
  size_t bufferSize;
  FILE* bak;
  FILE* ss;
};

char* LogCapture::getBuffer()
{
  return buffer;
}


// /* http://stackoverflow.com/a/5419388/421846 */
// #include <iostream>
// #include <streambuf>
// struct CerrRedirect {
//   CerrRedirect(std::streambuf* newStream)
//     : oldStream(std::cerr.rdbuf(newStream))
//     { }
//   ~CerrRedirect() {
//     std::cerr.rdbuf(oldStream);
//   }
// private:
//   std::streambuf* oldStream;
// };
// #include <sstream>
// std::ostringstream buffer;
// CerrRedirect errRedirect(buffer.rdbuf());


// char* buffer = nullptr;
// size_t bufferSize = 0;
// FILE* ss = open_memstream(&buffer, &bufferSize);
// int bak = dup(STDERR_FILENO);
// dup2(fileno(ss), STDERR_FILENO);
// // do something...
// dup2(bak, STDERR_FILENO);
// close(bak);
// fclose(ss);
// free(buffer);
