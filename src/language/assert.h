#ifndef Assert_h
#define Assert_h

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Assert framework configuration settings
#define ASSERT_CONFIG_PRINT_PASS        true
#define ASSERT_CONFIG_PRINT_FAIL        true
#define ASSERT_CONFIG_ABORT_ON_FAIL     false

#ifndef NDEBUG
    // Assert framework definitions
    #define ASSERT_EXPRESSION(P)            (#P)
    #define ASSERT_PASS(P)                  (!!(P))
    #define ASSERT_FAIL(P)                  (!(P))
    #define ASSERT_NUL                      "\0"
    #define ASSERT_NIL                      0
    #define ASSERT_ZILCH                    { ((void)0;) }

    // Assert framework diagnostic symbols
    #define ASSERT_VERUM_SYMBOL             "T"
    #define ASSERT_FALSUM_SYMBOL            "F"
    #define ASSERT_PASSED_SYMBOL            "passed"
    #define ASSERT_FAILED_SYMBOL            "failed"
    #define ASSERT_OK_SYMBOL                "ok"
    #define ASSERT_NOT_OK_SYMBOL            "not ok"
    #define ASSERT_ABORT_SYMBOL             "aborting..."

    #if ((!ASSERT_CONFIG_PRINT_FAIL) && (!ASSERT_CONFIG_PRINT_PASS))
        #define ASSERT(P)                   ASSERT_ZILCH
    #else
        #define ASSERT_PRINT(P,exp,fn,f,ln)     printf("# <Assert: %s> [%s=%s,%s=%s,%s=%s]::assertion(%s):function(%s):file(%s):line(%d) -> [%s]\n", \
                                                        (ASSERT_PASS(P) ? ASSERT_PASSED_SYMBOL : ASSERT_FAILED_SYMBOL ), \
                                                        "print_pass", (ASSERT_CONFIG_PRINT_PASS ? ASSERT_VERUM_SYMBOL : ASSERT_FALSUM_SYMBOL), \
                                                        "print_fail", (ASSERT_CONFIG_PRINT_FAIL ? ASSERT_VERUM_SYMBOL : ASSERT_FALSUM_SYMBOL), \
                                                        "abort_fail", (ASSERT_CONFIG_ABORT_ON_FAIL ? ASSERT_VERUM_SYMBOL : ASSERT_FALSUM_SYMBOL), \
                                                        exp, fn, f, ln, \
                                                        (ASSERT_PASS(P) ? ASSERT_OK_SYMBOL : (ASSERT_CONFIG_ABORT_ON_FAIL ? ASSERT_ABORT_SYMBOL : ASSERT_NOT_OK_SYMBOL )))

        #define ASSERT(P,exp)           {if(ASSERT_FAIL(P)) { \
                                            if(ASSERT_CONFIG_PRINT_FAIL)        { ASSERT_PRINT(P,exp,__FUNCTION__,__FILE__,__LINE__); } \
                                            if(ASSERT_CONFIG_ABORT_ON_FAIL)     { fflush(stdout); abort(); } \
                                        } else { \
                                            if(ASSERT_CONFIG_PRINT_PASS)        { ASSERT_PRINT(P,exp,__FUNCTION__,__FILE__,__LINE__); } \
                                        }}

        #define ASSERT_EQUAL(VALUE,EXPECTED)            ASSERT(VALUE == EXPECTED, "VALUE == EXPECTED")
        #define ASSERT_NOT_EQUAL(VALUE,EXPECTED)        ASSERT(VALUE != EXPECTED, "VALUE != EXPECTED")
        #define ASSERT_LESS_THAN(VALUE,EXPECTED)        ASSERT(VALUE < EXPECTED, "VALUE < EXPECTED")
        #define ASSERT_LESS_OR_EQUAL(VALUE,EXPECTED)    ASSERT(VALUE <= EXPECTED, "VALUE <= EXPECTED")
        #define ASSERT_GREATER_THAN(VALUE,EXPECTED)     ASSERT(VALUE > EXPECTED, "VALUE > EXPECTED")
        #define ASSERT_GREATER_OR_EQUAL(VALUE,MIN,MAX)  ASSERT(VALUE >= EXPECTED, "VALUE >= EXPECTED")
        #define ASSERT_RANGE(VALUE,MIN,MAX)             ASSERT(true == ((MIN <= VALUE) && (VALUE <= MAX)), "((MIN <= VALUE) && (VALUE <= MAX))")
        #define ASSERT_MATCH(A,B,SIZE)                  ASSERT(true == (A == B || (0 == memcmp((void*)A, (void*)B, SIZE))), "(A == B || (0 == memcmp((void*)A, (void*)B, SIZE)))")

    #endif
#else
    #define ASSERT(P)       ASSERT_ZILCH
#endif

#endif
