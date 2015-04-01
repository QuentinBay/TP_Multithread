#!/bin/sh
#-*-mode:c-*-
(echo "#line 3 \"$0\"";echo;tail -n +4 $0) >/tmp/cs.$$.c && gcc -Wall -o /tmp/cs.$$ /tmp/cs.$$.c && /tmp/cs.$$ $*;rm -f /tmp/cs.$$*; exit

/*

Usage: ./generator.cs QUANTITY MAGNITUDE REDUNDANCY

Generates an arbitraty QUANTITY of "random" numbers. Each number is
scaled down to a certain binary MAGNITUDE (e.g. if you set this
parameter to 10, then all your numbers will be smaller than 1024).

The script also include a REDUNDANCY parameter to artificially control
the "variety" of the results. Setting this value to 100 will cause all
numbers to be the same. Setting this value to 0 will cause all numbers
to be really random (even though we make no attempt to eliminate
fortuitous repetitions)

You should redirect the ouput in to a file, like in the examples below:

    ./generator.cs 50 32 0 > fifty_distinct_smallish_numbers.txt

    ./generator.cs 10 64 100 > one_large_number_repeated_ten_times.txt

*/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    uint64_t  number ;
    uint32_t *word = (void*) &number ;
    uint64_t *previous_numbers;
    
    // how many numbers to generate
    int quantity = 20;
    if( argc > 1)
        quantity=atoi(argv[1]);

    // maximum magnitude of numbers, in bits (0..64)
    int magnitude= 64;
    if( argc > 2)
        magnitude=atoi(argv[2]);

    // percentage of redundancy (0..100)
    // 30% means each number only has 2/3 chance to be a brand new one
    int redundancy=50;
    if( argc > 3)
        redundancy=atoi(argv[3]);

    // we seed the the generator with a constant value so as to get
    // reproducible results.
    srandom(0);

    previous_numbers=malloc(quantity*sizeof(uint64_t));
    
    int i;
    for(i=0; i<quantity; i++)
    {
        if( i==0 || random() % 100 > redundancy)
        {
            // let's generate a new number
            word[0] = random();
            word[1] = random();
            
            // shift right to reduce magnitude
            number >>= 64-magnitude ;
        }
        else
        {
            // let's pick from previously generated numbers
            number = previous_numbers[ random() % i ];
        }
            
        previous_numbers[i] = number;
        printf("%ju\n",(intmax_t)number);
    }
    
    return 0;
}
