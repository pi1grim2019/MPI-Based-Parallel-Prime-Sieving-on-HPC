# MPI-Based Parallel Prime Sieving on HPC

## Introduction
This project implements and optimizes the Sieve of Eratosthenes algorithm using C and MPI on a multi-node High-Performance Computing cluster. It explores parallelization strategies and algorithmic optimizations across four versions (sieve0 – sieve3), achieving near-linear scalability up to 160 CPU cores and approximately 17× speedup.

## Running the code  
This program requires multi-node parallel computation. Please run this program on a high-performance computing (HPC) cluster (reference environment: 5 nodes, each equipped with 32 CPU cores) to ensure you have access to the necessary computational resources.

You can simply run the code with:  
```bash
python3 starter.py
```

The script will submit all tests (sieve0 to sieve3) to SLURM. If you want to test specific cases rather than the entire algorithm, you can modify the following list variables in the starter.py:

``N_list`` specifies the number of nodes to use in the test.

``ver_list`` defines the sieving algorithms to test. For example, to test only sieve3.c, set ver_list = [3]. 

You can find your execution results in .o files under the sh directory.

