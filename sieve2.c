#ifndef __SIEVE2_C__
#define __SIEVE2_C__

#include "include.h"

void sieve2(unsigned long long *global_count, unsigned long long n, int pnum, int pid)
{
    // Adjust range to only consider odd numbers

    unsigned long long low_value = 3 + pid * (n - 1) / pnum;        // Start from the smallest odd number
    unsigned long long high_value = 2 + (pid + 1) * (n - 1) / pnum; // End at the largest odd number

    // Number of odd numbers in range
    low_value = low_value + (low_value + 1) % 2;
    high_value = high_value - (high_value + 1) % 2;
    unsigned long long size = (high_value - low_value) / 2 + 1;

    // Check if process 0's range can hold all small primes
    if (2 + ((n - 1) / pnum) < (int)sqrt((double)n))
    {
        if (pid == 0)
            printf("Error: Too many processes.\n");
        MPI_Finalize();
        exit(0);
    }

    // Allocate memory for odd numbers in this process's range
    char *marked = (char *)malloc(size);
    if (marked == NULL)
    {
        printf("Error: Cannot allocate enough memory.\n");
        MPI_Finalize();
        exit(0);
    }
    memset(marked, 0, size);

    // Allocate memory for local primes (to find primes up to sqrt(n))
    unsigned long long local_size = (unsigned long long)sqrt((double)n) - 1;
    char *local_marked = (char *)malloc(local_size);
    if (local_marked == NULL)
    {
        printf("Error: Cannot allocate enough memory for local primes.\n");
        MPI_Finalize();
        exit(0);
    }
    memset(local_marked, 0, local_size);

    // Step 1: Sieve locally to find primes up to sqrt(n)
    unsigned long long local_prime = 2;
    for (unsigned long long i = 0; i < local_size; i++) {
        local_marked[i] = 0; // Initialize local sieve
    }
    unsigned long long index = 0;
    do {
        unsigned long long local_first = local_prime * local_prime - 2;
        for (unsigned long long i = local_first; i < local_size; i += local_prime) {
            local_marked[i] = 1; // Mark multiples of local_prime
        }
        while (++index < local_size && local_marked[index] == 1);
        local_prime = index + 2; // Next local prime
    } while (local_prime * local_prime <= n);

    // Step 2: Mark multiples in the main range using local primes
    index = 0;
    memset(marked, 0, size);

    local_prime = 3;
    while (local_prime * local_prime <= n)
    {
        unsigned long long first;
        if (local_prime * local_prime > low_value)
            first = (local_prime * local_prime - low_value) / 2;
        else
        {
            unsigned long long smallest = low_value + ((local_prime - (low_value % local_prime)) % local_prime);
            if (smallest % 2 == 0)
                smallest += local_prime; // Ensure odd multiple
            first = (smallest - low_value) / 2;
        }

        // Mark multiples of the current prime in the range
        for (unsigned long long i = first; i < size; i += local_prime)
        {
            marked[i] = 1;
        }

        // Find next local prime
        while (++index < local_size && local_marked[index] == 1)
            ;
        local_prime = index + 2; // Next local prime
    }

    // Step 3: Count primes in the local range
    unsigned long long count = 0;
    for (unsigned long long i = 0; i < size; i++)
    {
        if (marked[i] == 0)
            count++;
    }

    // Add the prime 2 (handled globally)
    if (pid == 0)
        count++;

    // Reduce the global count across all processes
    MPI_Reduce(&count, global_count, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
}

#endif