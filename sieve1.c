#ifndef __SIEVE1_C__
#define __SIEVE1_C__

#include "include.h"

void sieve1(unsigned long long *global_count,unsigned long long n,int pnum,int pid)
{
    // Adjust range to only consider odd numbers
    unsigned long long low_value = 3 + pid * (n-1)/pnum; // Start from the smallest odd number
    unsigned long long high_value = 2 + (pid + 1) * (n-1)/pnum; // End at the largest odd number
    // unsigned long long size = (high_value - low_value) / 2 + 1; // Number of odd numbers in range
    low_value = low_value + (low_value + 1) % 2;
    high_value = high_value - (high_value + 1) % 2;
    unsigned long long size = (high_value - low_value) / 2 + 1;

    // Check if process 0's range can hold all small primes
    if (2 + ((n - 1) / pnum) < (int)sqrt((double)n)) {
        if (pid == 0)
            printf("Error: Too many processes.\n");
        MPI_Finalize();
        exit(0);
    }

    // Allocate memory for odd numbers only
    char *marked = (char *)malloc(size);
    if (marked == NULL) {
        printf("Error: Cannot allocate enough memory.\n");
        MPI_Finalize();
        exit(0);
    }
    memset(marked, 0, size);

    unsigned long long prime = 3; // Start with the first odd prime
    // unsigned long long first;
    unsigned long long index = 0;

    do {
        // Calculate the first odd multiple of prime in this process's range
        unsigned long long first;
        if (prime * prime > low_value)
            first = (prime * prime - low_value) / 2;
        else {
            unsigned long long smallest = low_value + ((prime - (low_value % prime)) % prime);
            if(smallest % 2 == 0) smallest += prime;
            first = (smallest - low_value) / 2;
        }

        // Mark all multiples of the current prime
        for (unsigned long long i = first; i < size; i += prime)
            marked[i] = 1;
        index++;
        // Process 0 finds the next prime
        if (pid == 0) {
            // unsigned long long index = (prime - 3) / 2;
            while (marked[index] == 1) index++;
            prime = 3 + index * 2; // Convert index back to actual odd number
        }

        // Broadcast the next prime to all processes
        MPI_Bcast(&prime, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    } while (prime * prime <= n);

    // Count primes in the local range
    unsigned long long count = 0;
    for (unsigned long long i = 0; i < size; i++) {
        if (marked[i] == 0)
            count++;
    }
    if (pid == 0)
        count++; // Add the prime 2 (handled globally)
    // Reduce the global count across all processes
    MPI_Reduce(&count, global_count, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
}

#endif