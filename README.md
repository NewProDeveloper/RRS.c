# RRS.c

This file gives a general perception of how a Round Robin Scheduliong works in a CPU.

I have made this algorithm time flexible, like user can enter the arrival time for different processes and if the arrival time for the process is same then the Priority comes into action. Lower the priority, first is its preference to execute.

I have kept a specific Time Quantum (can be made user defined too) which enables the CPU to allot a fix time to each process. If the process is completed in the Time Quant then it terminates, otherwise the process is sent at the back of the queue. 

I have used Queue data structure to impelement this algorithm which is used to maintain the currently active processes which are yet to be terminated or yet to be started.