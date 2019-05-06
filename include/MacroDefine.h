#ifndef MACRO_DEFINE
#define MACRO_DEFINE

#include<assert.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

#define ERRRET(x) { if(UNLIKELY(x)){\
					fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
__LINE__, __FILE__, errno, strerror(errno)); exit(-1); }}


/*#ifdef CHECK_PTHREAD_RETURN_VALUE

#ifdef NDEBUG
__BEGIN_DECLS
extern void __assert_perror_fail (int errnum,
                                  const char *file,
                                  unsigned int line,
                                  const char *function)
    noexcept __attribute__ ((__noreturn__));
__END_DECLS
#endif

#define RETCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       if (UNLIKELY(x))    \
                         __assert_perror_fail (errnum, __FILE__, __LINE__, __func__);})

#else */ // CHECK_PTHREAD_RETURN_VALUE

#define RETCHECK(ret) { __typeof__ (ret) errnum = (ret);         \
                       assert(errnum == 0); (void) errnum;}

//#endif // CHECK_PTHREAD_RETURN_VALUE







#endif
