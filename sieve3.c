#ifndef __SIEVE3_C__
#define __SIEVE3_C__

#include "include.h"

void sieve3(unsigned long long *global_count, unsigned long long n, int pnum, int pid)
{
    unsigned long long sqrt_n = (unsigned long long)sqrt(n);

    // Sieve small primes up to sqrt(n)
    unsigned long long sqrt_n_odd = (sqrt_n - 1) / 2;

    char *small_primes = (char *)malloc(sqrt_n_odd + 1);
    if (small_primes == NULL)
    {
        if (pid == 0)
            printf("Error: Cannot allocate enough memory for small_primes.\n");
        MPI_Finalize();
        exit(0);
    }
    memset(small_primes, 0, sqrt_n_odd + 1);

    // Simple sieve to find small primes up to sqrt(n)
    for (unsigned long long i = 0; i <= sqrt_n_odd; i++)
    {
        if (small_primes[i] == 0)
        {
            unsigned long long p = 2 * i + 3;
            unsigned long long start = (p * p - 3) / 2;
            for (unsigned long long j = start; j <= sqrt_n_odd; j += p)
            {
                small_primes[j] = 1;
            }
        }
    }

    // Collect the small primes
    unsigned long long num_small_primes = 0;
    for (unsigned long long i = 0; i <= sqrt_n_odd; i++)
    {
        if (small_primes[i] == 0)
            num_small_primes++;
    }

    unsigned long long *primes = (unsigned long long *)malloc(num_small_primes * sizeof(unsigned long long));
    if (primes == NULL)
    {
        if (pid == 0)
            printf("Error: Cannot allocate enough memory for primes.\n");
        MPI_Finalize();
        exit(0);
    }

    unsigned long long index = 0;
    for (unsigned long long i = 0; i <= sqrt_n_odd; i++)
    {
        if (small_primes[i] == 0)
        {
            primes[index++] = 2 * i + 3;
        }
    }
    free(small_primes);

    // Compute the range for this process
    unsigned long long n_odds = (n - 1) / 2;                    // Number of odd numbers >= 3 and <= n
    unsigned long long chunk_size = (n_odds + pnum - 1) / pnum; // Ceiling division
    unsigned long long low_index = pid * chunk_size;
    unsigned long long high_index = (pid + 1) * chunk_size - 1;
    if (high_index >= n_odds)
        high_index = n_odds - 1;

    if (low_index > high_index)
    {
        // No numbers to process
        unsigned long long count = 0;
        MPI_Reduce(&count, global_count, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
        free(primes);
        return;
    }

    unsigned long long block_size = 32768; // Adjust block size to fit into cache
    unsigned long long count = 0;

    for (unsigned long long block_low_index = low_index; block_low_index <= high_index; block_low_index += block_size)
    {
        unsigned long long block_high_index = block_low_index + block_size - 1;
        if (block_high_index > high_index)
            block_high_index = high_index;

        unsigned long long block_size_current = block_high_index - block_low_index + 1;

        char *block = (char *)malloc(block_size_current);
        if (block == NULL)
        {
            if (pid == 0)
                printf("Error: Cannot allocate enough memory for block.\n");
            MPI_Finalize();
            exit(0);
        }
        memset(block, 0, block_size_current);

        unsigned long long block_low_value = 2 * block_low_index + 3;

        // Sieve the block with small primes
        for (unsigned long long i = 0; i < num_small_primes; i++)
        {
            unsigned long long p = primes[i];

            // Find the first multiple of p within the block
            unsigned long long first;

            if (p * p > block_low_value)
                first = p * p;
            else
            {
                unsigned long long rem = block_low_value % p;
                if (rem == 0)
                    first = block_low_value;
                else
                    first = block_low_value + (p - rem);
            }

            if ((first & 1) == 0)
                first += p;
                
            unsigned long long j = (first - block_low_value) / 2;
            
            while (j < block_size_current)
            {
                block[j] = 1;
                j += p;
            }
        }

        // Count primes in the block
        for (unsigned long long i = 0; i < block_size_current; i++)
        {
            if (block[i] == 0)
                count++;
        }
        free(block);
    }

    if (pid == 0)
        count++; // Include prime number 2

    MPI_Reduce(&count, global_count, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    free(primes);
}

#endif