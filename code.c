#include "types.h"
#include "stat.h"
#include "user.h"

const int P = 15;
/*

This file is used to test the scheduler in proc.c. It will run several bubble sorts
in different processes with noTickets varying from argv[1], argv[1] + 10, ..., 100

Spected result: Process with most tickets finishes first, then second most, and so on.

*/

void reverse(char *array, int start, int end)
{
    int temp;
    while (start < end)
    {
        temp = array[start];
        array[start] = array[end];
        array[end] = temp;
        start++;
        end--;
    }
}

char *itoa(int value){
    char *str = (char *)malloc(sizeof(char) * 11);
    int i = 0;
    while(value > 0){
        str[i++] = (value % 10) + '0';
        value /= 10;
    }
    str[i] = '\0';
    reverse(str, 0, i - 1);
    
    return str;
}

int main(int argc, char *argv[])
{
    int noTickets = 10;
    int biggestPID = 0;
    if (argc > 1)
        noTickets = atoi(argv[1]);


    for (int i = 0; i < P; i++)
    {
        int pid = fork(noTickets);
        
        char *args[2];
        args[0] = "bubble\0";
        args[1] = itoa(noTickets);
        
        if (pid == 0){
            
            exec("bubble", args);
        }
        else if (i == P - 1)
            biggestPID = pid;

        noTickets += 100;
    }
    for (int i = 0; i < P; i++)
        wait();

    printf(1, "\nBIGGEST PID IS %d\n", biggestPID);

    exit();
}
