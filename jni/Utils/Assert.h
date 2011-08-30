#ifndef ASSERT_H
#define ASSERT_H

//#if !defined(NDEBUG)
#define ASSERT(condition, message) if (!(condition)) { Assert(__FILE__, __LINE__, message); }
//#else
//#define ASSERT(condition, message)
//#endif

void Assert(const char* fileName, const int line, const char* message);


#endif // ASSERT_H
