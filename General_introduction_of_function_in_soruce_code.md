1.main  
Input: 
We use two linked list array to save the graph of the rule, one of them is “vertex_from”, and the other one is ”vertex_come”, each index of each element in “vertex_from” is “task number -1”,and the linked list  in each element means the task will be process only after “element task” finish; ”vertex_come” is same like ”vertex_from”, just the linked list in each element means the task need to be finish before “element task” start.  Use “time array” to save the running time of different task run in the different core. “array_a” is used to save the power of each core when it is working, and index zero means the power when we send a task to cloud, and others means the power of core “index”. “T_s”,”T_c”,”T_r” are respective used to save the sending time, cloud processing time, and receiving time when we want to send a task to cloud. 
Output: 
“core_result” is the initial scheduling of all the task, it should have the minimum processing time compare with other permutation and combination of those tasks. 
“S_k” is the scheduling result after each time when we run “step 2” in the paper. And at the end “S_k” will be the best scheduling. 
  
2.primary_assignment: 
This function is used to determine which task should be processed on the cloud when we do the initial scheduling.   We can compare the time of the task running in each core and cloud process time plus receive time to determine if this task should be process in cloud, if cloud process time plus receive time smaller than the minimum processing time on local, then we use cloud to process this task, otherwise, use local to process. 

3.prioriting: 
This function is used to generate a vector “priority”, in each element, we use a struct to save the the task number and the ”priority” of each task(is just a number), which is shown in the paper as section C, “TaskPrecedence Requirements in the MCC Environment”. After we finish function prioriting, we get a vector “priority”, and we can find the “priority” of each task, and there are no doubt that a task have bigger “priority” will process first. 

4.unit_selection 
This function is used to generate the initial scheduling, there have few steps to do this: 1. Order the task with “priority” 2. According to priority chose task from the result of step 1, and put it in the minimum time cost place(core or cloud), and make the start time 
of this task equals to the maximum end time of the precedent task.(can be known from “vertex_from”) 3. Use “for loop” to do step2 until we finish schedule all tasks. 

5.copy_vec 
This function is used to copy a linked list array to another, because linked list array is a pointer, so if we use equal sign “=” copy a linked list array to another one, they be same, so every time when we change something in the copy one, the original array will also change, that not what we want. 

6.kernel 
This function is used to move one of the task to another core or cloud, and continue do this until we move all tasks to another core or cloud. And each time after we move the task, we’ll use “prepare_time_energy” to reschedule it and finally use “chose_best_result”  to find the best scheduling in all of those (core number * task number)  numbers scheduling. To do this, we can: 1. Based on “priority” to chose which task we need to move 2. Chose which core or cloud we need to move our task to. 3. Move the target task later than the one which have larger or equal start time to our target task in the target core or cloud. 4. Use “prepare_time_energy” to rescheduling our task. 

7. prepare_time_energy 
This function is used to rescheduling the task, from kernel, we already know position of each task and the order in each core, so this function will scheduling all the task to make them obey the rule shown in the graph. 

8.chose_best_result 
This function is used to chose the best scheduling from all of those (core number * task number)  numbers scheduling. we can do this through. 1. If we can find some scheduling which have same or smaller time than original scheduling , then we’ll chose the most smallest energy consumption scheduling from them as our result. 2. If we can’t find a scheduling in step1, then we’ll use function:       (kth energy – original energy)/(kth time – original time) calculate the rate of each scheduling, and then chose the one which have biggest rate as our result. 
