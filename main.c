#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>

struct Process
{
    int P_NO;                 // Just for storing Process number
    int Arrival_Time;         // time when process arrives
    int Burst_Time;           // time needed by the process to get completed
    int Burst_Time_Remaining; // amount of time remained to be successfully executed
    int Completion_Time;      // time after which it finally gets executed
    int Turn_around_Time;     // total time the process exists in the queue
    int Waiting_Time;         // time spent in the queue not being executed
    int Process_ID;           // unique process id
    int Priority;             // priority of the task (useful when same arrival time)
    bool Execution_State;     // true for active else false
    bool is_Complete;         // checks if the process is completed
};

void sort_by_arrival_time(struct Process *P, int n)
{

    struct Process temp;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if ((P + j)->Arrival_Time < (P + i)->Arrival_Time)
            {
                temp = *(P + i);
                *(P + i) = *(P + j);
                *(P + j) = temp;
            }

            else if ((P + j)->Arrival_Time == (P + i)->Arrival_Time)
            {
                if ((P + j)->Priority < (P + i)->Priority)
                {
                    temp = *(P + i);
                    *(P + i) = *(P + j);
                    *(P + j) = temp;
                }
            }
        }
    }
}

int queue[(int)1e7] = {-1}; // creating global queue to avoid errors.
int front = -1, rear = -1;  // handling of queue
void enqueue(int element, const int n)
{
    if (front == -1 && rear == -1)
    {
        front = 0;
        rear = 0;
        queue[front] = element;
        return;
    }

    if (rear == n - 1)
        rear = 0;
    else
        rear++;

    queue[rear] = element;
}

int dequeue(const int n)
{
    int current = queue[front];
    if (front == rear)
    {
        front = -1;
        rear = -1;
        return current;
    }

    if (front == n - 1)
        front = 0;
    else
        front++;

    return current;
}

int current_Time = 0;      // holds the current time after each process has been executed
int completed_process = 0; // holds the number of programs executed so far

/**
 * Reference of GFG.
 */
void new_process_arrival(struct Process **p1, const int n)
{

    for (int i = 0; i < n; i++)
    {
        // checking if any processes has arrived
        // if so, push them in the ready Queue.
        if ((*(p1) + i)->Arrival_Time <= current_Time && !(*(p1) + i)->Execution_State && !(*(p1) + i)->is_Complete)
        {
            (*(p1) + i)->Execution_State = true;
            enqueue(i, n);
        }
    }
}

int output_queue[(int)1e7], k = 0;
/**
 * Reference of GFG.
 */
int queue_updation(struct Process **p1, const int n, const int quantum)
{

    int i = dequeue(n);
    int flag = -1;
    // Checking if the current process can be completed under the time quant or not
    if ((*(p1) + i)->Burst_Time_Remaining <= quantum)
    {
        (*(p1) + i)->is_Complete = true;
        current_Time += (*(p1) + i)->Burst_Time_Remaining;
        (*(p1) + i)->Completion_Time = current_Time;
        (*(p1) + i)->Waiting_Time = (*(p1) + i)->Completion_Time - (*(p1) + i)->Arrival_Time - (*(p1) + i)->Burst_Time;
        (*(p1) + i)->Turn_around_Time = (*(p1) + i)->Burst_Time + (*(p1) + i)->Waiting_Time;

        if ((*(p1) + i)->Waiting_Time < 0)
            (*(p1) + i)->Waiting_Time = 0;

        (*(p1) + i)->Burst_Time_Remaining = 0;
        completed_process++;
    }

    else
    {
        // this means process is not done yet.
        (*(p1) + i)->Burst_Time_Remaining -= quantum;
        current_Time += quantum;

        flag = i;
    }
    output_queue[k] = (*(p1) + i)->P_NO;
    k++;
    return flag;
}

void RoundRobinSchedule(struct Process *p, int n, int quantum)
{

    enqueue(0, n); // initially, pushing the first process which arrived first
    p->Execution_State = true;

    while (completed_process != n)
    {
        int x = queue_updation(&p, n, quantum);
        if (x == -1)
            // Checking for further availbale process.
            new_process_arrival(&p, n);

        else
        {
            new_process_arrival(&p, n);
            enqueue(x, n);
        }
    }
}

void Output(struct Process *p, int n)
{

    printf("Process\t Process ID\tArrival Time\tCompletion Time\t Idle Time\tTurnAround Time\n");

    for (int i = 0; i < n; i++)
    {
        printf("P%d\t %d\t\t%d\t\t%d\t\t %d\t\t%d\n", (p + i)->P_NO, (p + i)->Process_ID, (p + i)->Arrival_Time, (p + i)->Completion_Time, (p + i)->Waiting_Time, (p + i)->Turn_around_Time);
    }

    printf("\nExecution Sequence of Processes: \n");
    for (int j = 0; j < k; j++)
        printf("+----");
    printf("+\n");
    for (int j = 0; j < k; j++)
        printf("| P%d ", output_queue[j]);
    printf("|\n");
    for (int j = 0; j < k; j++)
        printf("+----");
    printf("+\n");

    double avg_wait = 0, avg_turnAround = 0;
    for (int i = 0; i < n; i++)
    {
        avg_wait += (p + i)->Waiting_Time;
        avg_turnAround += (p + i)->Turn_around_Time;
    }

    printf("\nAverage waiting Time: %lf", (avg_wait / n));
    printf("\nAverage Turn Around Time: %lf\n", (avg_turnAround / n));
}

void main()
{
    srand(0);
    int Number_Of_Processes;
    int Time_Quant = 2; // in milliseconds (ms)
                        // Can be made user accesible/input

    printf("\n--------------------------- Welcome to Round Robin Scheduling Program -----------------------\n");
    printf("\nEnter number of processes to execute: ");
    scanf("%d", &Number_Of_Processes);
    printf("\n");

    struct Process P[Number_Of_Processes]; // array of processes.

    for (int i = 0; i < Number_Of_Processes; i++)
    {
        P[i].P_NO = i + 1;
        printf("Enter arrival time of P%d: ", (i + 1));
        scanf("%d", &P[i].Arrival_Time);

        printf("Enter burst time of P%d: ", (i + 1));
        scanf("%d", &P[i].Burst_Time);

        printf("Enter priority of P%d: ", (i + 1));
        scanf("%d", &P[i].Priority);

        P[i].Burst_Time_Remaining = P[i].Burst_Time; // intially both are same
        P[i].Process_ID = rand() % 1000000 + 1;      // assigning process id by random function

        printf("\n");
    }

    /*
        Now we sort the Process P array according to the the arrival time and priority
        of the processes. This gives the CPU a required order to execute the processes.
    */
    sort_by_arrival_time(P, Number_Of_Processes);

    // Creating a Round Robin Scheduler for the processes
    RoundRobinSchedule(P, Number_Of_Processes, Time_Quant);

    // Displaying the output
    Output(P, Number_Of_Processes);
}