#include "func_call.c"

int main (int argc, char *argv[])
{
    if (argc!=3)
    {
        printf("Usage: mpirun -np {pnum} ./main {func_name} {n}\n");
        exit(0);
    }
    char *func_name=argv[1];
    unsigned long long n=atoll(argv[2]);
    int pid,pnum;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&pid);
    MPI_Comm_size(MPI_COMM_WORLD,&pnum);
    MPI_Barrier(MPI_COMM_WORLD);
    unsigned long long global_count=0;
    double time;
    func_call(&global_count,&time,func_name,n,pnum,pid);
    if (!pid)
        printf("func=%s, pnum=%d, count=%lld, time=%lf\n",func_name,pnum,global_count,time);
    MPI_Finalize();
}