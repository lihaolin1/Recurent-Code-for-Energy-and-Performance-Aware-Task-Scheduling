task number: (as a example: 10) (int number) 
This input will tell our code how many  tasks we have, in order to build a matrix to save the time consumption of each task in each core.  

core number: (as a example: 3) (int number) 
This input will tell our code how many  core we can use to process our tasks, in order to build a matrix to save the time consumption of each task in each core and generate the final result.  
time consumption matrix: (all elements should be int number) 
9 7 5   
8 6 5   
6 5 4   
7 5 3   
5 4 2   
7 6 4   
8 5 3   
6 4 2   
5 3 2   
7 4 2   
This is will tell the code how many time to finish different tasks in different core, each row means the time cost of a task running in different core(from core 1 to core 3 as a example), and each column means the time cost of different tasks running in a same core. If the time cost is a decimal number, we can time 10 or 100 to all element to make the matrix become a integer matrix. 

sending time: (as a example: 3) (int number) 
 This input means the time cost when we send a task to cloud. 

process time: (as a example: 1) (int number) 
 This input means the time cost when we process some task on cloud 

receive time: (as a example: 1) (int number) 
This input means the time cost when the cloud finish the task and send it back. 

number of edge: (as a example: 15) (int number) 
 This is the number of edges in the “rule” graph. 

order graph:(can cpoy this directly) 
(as a example shown below)
1 2   
1 3   
1 4   
1 5   
1 6   
2 8   
2 9   
3 7   
4 8   
4 9   
5 9   
6 8   
7 10   
8 10   
9 10   
This input matrix means the graph; for each row in the matrix shown above, the left and right elements means an arrow from the left element task to the right element task in the graph, and this means task on the right can only start to process after task on the left finish.  

power of sending: (as a example: 0.5) (double number) 
 This means the power when we send something to cloud. 

power of core 1:  (as a example: 1) (int number) 
 This means the power when we process some task in core 1. 

power of core 2: (as a example: 2) (int number) 
 This means the power when we process some task in core 2. 

power of core 3: (as a example: 4) (int number) 
 This means the power when we process some task in core 1. 
  
