#ifndef COMMON

#define COMMON

#define MAX_SELECT_CLIENTS 12

#define EPOLL_WATCH_SIZE 1024  //epoll 

#define EPOLL_EVENTS_MAX 100  


//申请内存管理类  
#define MEMGUARD(type, content) \
class MemGuard##content              \
{                           \
private:                    \
  type * memptr;             \
                            \
public:                     \
  MemGuard##content(type * content)       \
  { memptr = content;}          \
  ~MemGuard##content()               \
  {                         \
    if (memptr != NULL)     \
    {  delete memptr; memptr = NULL; } \
  }                         \
};                          \
MemGuard##content  memGuard##content(content);

//申请内存数组管理类  
#define MEMARRGUARD(type, content) \
class MemArrGuard##content  \
{                           \
private:                    \
  type * memptr;             \
                            \
public:                     \
  MemArrGuard##content(type * content)       \
  { memptr = content;}          \
  ~MemArrGuard##content()               \
  {                         \
    if (memptr != NULL)     \
    {  delete[] memptr; memptr = NULL; } \
  }                         \
};                          \
MemArrGuard##content  memArrGuard##content(content);

#endif //common.h