#ifndef __FUNC_CALL_C__
#define __FUNC_CALL_C__

#include "sieve0.c"
#include "sieve1.c"
#include "sieve2.c"
#include "sieve3.c"

void func_call(unsigned long long *global_count,double *time,char *func_name,unsigned long long n,int pnum,int pid)
{
    if (strcmp(func_name,"sieve0")==0)
    {
        *time=-MPI_Wtime();
        sieve0(global_count,n,pnum,pid);
        *time+=MPI_Wtime();
        return; 
    }
    if (strcmp(func_name,"sieve1")==0)
    {
        *time=-MPI_Wtime();
        sieve1(global_count,n,pnum,pid);
        *time+=MPI_Wtime();
        return; 
    }
    if (strcmp(func_name,"sieve2")==0)
    {
        *time=-MPI_Wtime();
        sieve2(global_count,n,pnum,pid);
        *time+=MPI_Wtime();
        return; 
    }
    if (strcmp(func_name,"sieve3")==0)
    {
        *time=-MPI_Wtime();
        sieve3(global_count,n,pnum,pid);
        *time+=MPI_Wtime();
        return; 
    }
    printf("Error: Invalid function name\n");
    exit(0);
}

#endif