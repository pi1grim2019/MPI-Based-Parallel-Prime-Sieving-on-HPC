#!/bin/bash -l
#SBATCH --partition=batch
#SBATCH -o sieve3_3.o
#SBATCH -e sieve3_3.err
#SBATCH -J sieve3_3
#SBATCH --nodes=3
#SBATCH --wait-all-nodes=1
#SBATCH -t 00:10:00
#SBATCH --exclusive

module load gcc/gcc-5.1.0
module load mpich-3.2.1/gcc-4.8.5
mpirun -np 96 ../main sieve3 10000000000