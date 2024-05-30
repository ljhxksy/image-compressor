#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bitpack.h"

void print_binary(uint64_t n);
void print_binary_signed(int64_t n);

int main()
{
    int64_t n = -23;
    int64_t shiftu = n >> 4;
    // print_binary(n);
    // print_binary(shiftu);
    bool test = Bitpack_fitss(n, 0);
    printf("decimal: n - %ld, shiftu - %ld\n", n, shiftu);
    if (!test) printf("%ld << 4 = %ld doesn't fit!\n", n, shiftu);

    fprintf(stderr, "Result from getu: %lu\n", Bitpack_getu(0x3f4, 6, 2));

    int64_t result = Bitpack_gets(1200055143, 5, 13);
    
    fprintf(stderr, "Result from gets: %ld\n", result);

    printf("/// TEST RIGHT SIGNED SHIFT ////\n");
    int64_t x = -100;
    fprintf(stderr, "Before right shift: %ld\n", x);
    // print_binary(x);
    x = x >> 4;
    fprintf(stderr, "Result from right shift: %ld\n", x);
    // print_binary(x);


    printf("/////// TEST NEWS ///////\n");

    uint64_t testNewu = 3285;
    printf("testNewu: %lu\n", testNewu);
    printf("testNewu in binary: ");
    // print_binary(testNewu);
    printf("\n");

    printf("value: %ld\n", (int64_t)-23);
    printf("value in binary: ");
    // print_binary(-23);
    printf("\n");

    uint64_t res = Bitpack_news(testNewu, 3, 3, 4);
    printf("result: %lu\n", res);
    printf("result in binary: ");
    // print_binary(res);
    printf("\n");

    return 0;
}
