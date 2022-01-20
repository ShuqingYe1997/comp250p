#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>

#define OP(x) { x; }

#define OP10(x)   OP(x) OP(x) OP(x) OP(x) \
                 OP(x) OP(x) OP(x) OP(x) \
                 OP(x) OP(x)

#define OP100(x)   OP10(x) OP10(x) OP10(x) OP10(x) \
                  OP10(x) OP10(x) OP10(x) OP10(x) \
                  OP10(x) OP10(x)

#define OP1000(x)   OP100(x) OP100(x) OP100(x) OP100(x) \
                   OP100(x) OP100(x) OP100(x) OP100(x) \
                   OP100(x) OP100(x)

#define OP10000(x)   OP1000(x) OP1000(x) OP1000(x) OP1000(x) \
                    OP1000(x) OP1000(x) OP1000(x) OP1000(x) \
                    OP1000(x) OP1000(x)

const unsigned long runs = 100000000;
const unsigned int runs2 = 10000;
const unsigned int array_size = 256;

uint64_t rdtsc(void) {

    unsigned cycles_low, cycles_high;

    asm volatile ( "rdtsc\n\t"
                   "mov %%edx, %0\n\t"
                   "mov %%eax, %1\n\t"
    : "=r" (cycles_high), "=r" (cycles_low)::
    "%rax", "%rbx", "%rcx", "%rdx");

    return ((uint64_t) cycles_high << 32) | cycles_low;
}

void warmup() {
    unsigned long i = 0;
    for(; i < 1000; i++) {
        asm volatile ("":::"%rax");
        OP10000({asm volatile ("add %rbx, %rax");});
    }
}

/******************************************************/
/**********          invocation            ***********/
/******************************************************/
int __attribute__ ((noinline)) foo(int a, int b) {
    return a + b;
}

int test_func_invocation() {
    uint64_t start;
    uint64_t end;
    volatile int ret = 0;

    start = rdtsc();
    int i = 0;
    for (; i < runs; i++) {
        ret = foo(ret, i);
    }
    end = rdtsc();

    printf("func invocation cycles:%lu\n", (end - start) / runs);
    return 0;
}
/******************************************************/
/**********          addition               ***********/
/******************************************************/
int test_add() {
    uint64_t start;
    uint64_t end;
    volatile int ret = 0;

    int i = 0;
    start = rdtsc();
    for (; i < runs; i++) {
        ret = ret + i;
    }
    end = rdtsc();

    printf("add cycles:%lu\n", (end - start)/runs);
    return ret;
}

int test_add_op() {
    uint64_t start;
    uint64_t end;
    volatile int ret = 0;

    start = rdtsc();

    asm volatile ("":::"%rax");
    OP10000({asm volatile ("add %rbx, %rax");});
    end = rdtsc();

    printf("add op cycles:%lu\n", (end - start) / runs2);
    return ret;
}

/******************************************************/
/**********          Division               ***********/
/******************************************************/
int test_div() {
    uint64_t start;
    uint64_t end;
    volatile int ret = 1;
    int i = 0;

    start = rdtsc();
    for (; i < runs; i++) {
        ret = ret / ret;  // todo
    }
    end = rdtsc();

    printf("div cycles:%lu\n", (end - start)/runs);
    return ret;
}

int test_div_op() {
    uint64_t start;
    uint64_t end;
    volatile int ret = 1;
    asm volatile ("mov $1, %ebx");

    start = rdtsc();

    asm volatile ("":::"%rax");
    asm volatile ("mov $0, %edx");
    asm volatile ("mov $1, %rax");
    OP10000({asm volatile ("idiv %ebx");});
    end = rdtsc();

    printf("div op cycles:%lu\n", (end - start) / runs2);
    return ret;
}

/******************************************************/
/**********        Multiplication           ***********/
/******************************************************/
int test_multi() {
    uint64_t start;
    uint64_t end;
    volatile int ret = 1;
    int i = 0;

    start = rdtsc();
    for (; i < runs; i++) {
        ret = ret * ret;
    }
    end = rdtsc();

    printf("multi cycles:%lu\n", (end - start)/runs);
    return ret;
}

int test_multi_op() {
    uint64_t start;
    uint64_t end;
    volatile int ret = 0;

    start = rdtsc();

    asm volatile ("":::"%rax");
    OP10000({asm volatile ("imul %rbx, %rax");});
    end = rdtsc();

    printf("multi op cycles:%lu\n", (end - start) / runs2);
    return ret;
}

/******************************************************/
/**********             Malloc              ***********/
/******************************************************/
int test_malloc() {
    uint64_t start;
    uint64_t end;
    volatile void* ret[runs2];
    int i = 0;

    start = rdtsc();
    for (; i < runs2; i++) {
        ret[i] = malloc(sizeof(int));
    }
    end = rdtsc();

    for (i = 0; i < runs2; i++) {
        free(ret[i]);
    }

    printf("malloc cycles:%lu\n", (end - start)/runs2);
    return 0;
}

/******************************************************/
/**********              Copy              ***********/
/******************************************************/
int test_copy() {
    uint64_t start;
    uint64_t end;
    volatile char charArray1[array_size];
    volatile char charArray2[array_size];

    int i = 0, j = 0;
    for (; i < array_size; i++) {
        charArray1[i] = '0';
    }

    start = rdtsc();
    for (j = 0; j < runs; j++) {
        for (i = 0; i < array_size; i++) {
            charArray2[i] = charArray1[i];
        }
    }
    end = rdtsc();

    printf("copy cycles:%lu\n", (end - start)/runs);
    return 0;
}

/******************************************************/
/**********             Memcpy              ***********/
/******************************************************/
int test_memcpy() {
    uint64_t start;
    uint64_t end;
    volatile char charArray1[array_size];
    volatile char charArray2[array_size];

    int i = 0;
    for (; i < array_size; i++) {
        charArray1[i] = '0';
    }

    start = rdtsc();
    for (i = 0; i < runs; i++) {
        memcpy(charArray2, charArray1, array_size);
    }
    end = rdtsc();

    printf("memcpy cycles:%lu\n", (end - start)/runs);
    return 0;
}


int main(void) {

    warmup();

    test_func_invocation();
    printf("--\n");

    test_add();
    test_add_op();
    printf("--\n");

    test_div();
    test_div_op();
    printf("--\n");

    test_multi();
    test_multi_op();
    printf("--\n");

    test_malloc();
    printf("--\n");

    test_copy();
    printf("--\n");

    test_memcpy();
    printf("--\n");

    return 0;
}

