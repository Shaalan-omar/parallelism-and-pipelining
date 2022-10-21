//
//  main.c
//  OS_Task_6
//
//  Created by Omar shaalan on 10/13/22.
//  Copyright © 2022 Omar shaalan. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

unsigned long long int sum(unsigned int N)
{
    unsigned long long int sum = 0;
    for (int i = 1; i<= N; i++)
    {
      sum = sum+i;
    }
    return sum;
};

unsigned long long int parallel_sum(unsigned int n_proc, unsigned int N)
{
    int steps = N/n_proc;
    int size_of_array_of_pipes = n_proc *2; //As each child needs its
    int pipes[2];
    if (pipe(pipes) == -1)
        exit(0);
    unsigned long long int tempSum =0,finalSum = 0;
    

        for(int i=0;i<n_proc;i++) // parent will create n_proc children, not 2^n
        {
            if(fork() == 0) //For preventing the parent to access the condition, ana el child
            {
                unsigned long long int sum = 0;
                
                int start = (i * steps) + 1, end = 0;
                
                if (i == n_proc - 1)
                {
                    end = N; //For the last child to carry any additional steps
                }
                else
                {
                    end = start + steps - 1;
                }
               
    
                for(; start <= end; start++ )
                {
                    sum += start;
                   
                }
                
                //close(piArr[i*2]);
                //printf("%lld \n", sum);
                write(pipes[1], &sum, sizeof(unsigned long long int));
                //close(piArr[i*2 + 1]);
                exit(0);//Killing the child to prevent the child from additional creations
            }
        }
   
         //wait(NULL);//To make sure each of the children have finsished summing
               unsigned long long int rsum = 0;
    for (int i=0; i < n_proc; i++)
    {
         wait(NULL);
    }
        for (int y = 0; y < n_proc; y++)
        {

            
            //close(piArr[(y*2)+1]);
            read(pipes[0], &rsum, sizeof(unsigned long long int));
            //printf("after: %lld\n", rsum);

            //close(piArr[y * 2]);
            
            finalSum+= rsum;
        }
    //closing all as we finished reading writing
      close(pipes[0]);
      close(pipes[1]);
    return finalSum;
}


void test1()
{

unsigned long long int Normal_sum,Parallel_sum;
clock_t start1,end1,start2,end2;
double cpu_time_used1,cpu_time_used2;
unsigned int N;
printf("The Normal sum\n" );
for(unsigned int i=100000;i<=300000;i=i+1000)
{
    start1 = clock();
    Normal_sum=sum(i);
    end1 = clock();
    cpu_time_used1=((double)(end1-start1)/CLOCKS_PER_SEC);
    
    printf("%d,%lld,%f\n" ,i,sum(i),cpu_time_used1);
    //printf("%f \n",cpu_time_used1);
 }
printf("The parallel sum\n" );
for(unsigned int i=100000;i<=300000;i=i+1000)
{
    start2 = clock();
    Parallel_sum=parallel_sum(7,i);
    end2 = clock();
    cpu_time_used2=((double)(end2-start2)/CLOCKS_PER_SEC);
    printf("%d,%lld,%f\n",i,Parallel_sum,cpu_time_used2);
    
}
}

void test2()
{
    const unsigned int N=1000000;

    unsigned long long int Normal_sum,Parallel_sum;
    clock_t start1,end1,start2,end2;
    double cpu_time_used1,cpu_time_used2;
    printf("The Normal sum\n" );
    //for (int i = 1; i < N; i++)
    //{
    start1 = clock();
    Normal_sum=sum(N);
    end1 = clock();
    cpu_time_used1=((double)(end1-start1)/CLOCKS_PER_SEC);
    printf("%lld,%f\n",sum(N),cpu_time_used1);
    //}
    
    printf("The parallel sum\n" );
    for(int i = 1; i < 200; i+=2)
    {
        start2 = clock();
        Parallel_sum=parallel_sum(i,N);
        end2 = clock();
        cpu_time_used2=((double)(end2-start2)/CLOCKS_PER_SEC);
        printf("%d,%lld,%f\n",i,Parallel_sum,cpu_time_used2);
    
    }
    
}

int main()
{
//test1();
//test2();
    
   printf("%lld \n", sum(2000000));
    printf("%lld\n",parallel_sum(31,2000000 ));
return 0;
}
