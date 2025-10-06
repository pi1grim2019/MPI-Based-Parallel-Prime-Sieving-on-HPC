import os

N_list=[1,2,3,4,5]
# ver_list=[0,1,2,3]
ver_list=[2]


n=10000000000

os.system("module load mpich-3.2.1/gcc-4.8.5")
os.system("make")
if os.path.exists("main"):
    os.system("mkdir sh")
    for N in N_list:
        for ver in ver_list:
            f=open("sh/sieve"+str(ver)+"_"+str(N)+".sh","w")
            f.write("#!/bin/bash -l\n")
            f.write("#SBATCH --partition=batch\n")
            f.write("#SBATCH -o sieve"+str(ver)+"_"+str(N)+".o\n")
            f.write("#SBATCH -e sieve"+str(ver)+"_"+str(N)+".err\n")
            f.write("#SBATCH -J sieve"+str(ver)+"_"+str(N)+"\n")
            f.write("#SBATCH --nodes="+str(N)+"\n")
            f.write("#SBATCH --wait-all-nodes=1\n")
            f.write("#SBATCH -t 00:10:00\n")
            f.write("#SBATCH --exclusive\n")
            f.write("\n")
            f.write("module load gcc/gcc-5.1.0\n")
            f.write("module load mpich-3.2.1/gcc-4.8.5\n")
            f.write("mpirun -np "+str(N*32)+" ../main sieve"+str(ver)+" "+str(n))
            f.close()
    os.chdir("sh")
    for N in N_list:
        for ver in ver_list:
            os.system("sbatch sieve"+str(ver)+"_"+str(N)+".sh")
