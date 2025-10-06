#!/bin/bash -l
#SBATCH --partition=batch
#SBATCH -o sieve1_3.o
#SBATCH -e sieve1_3.err
#SBATCH -J sieve1_3
#SBATCH --nodes=3
#SBATCH --wait-all-nodes=1
#SBATCH -t 00:10:00
#SBATCH --exclusive

module load gcc/gcc-5.1.0
module load mpich-3.2.1/gcc-4.8.5
mpirun -np 96 ../main sieve1 10000000000