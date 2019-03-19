#include<iostream>
#include<stdlib.h>
#include<vector>
#include<limits.h>
struct graph {
	int name;
	int end_time;  //use to schedule the task
	char finish;
	graph *next;

};

struct use_sort {
	float time;
	int num_task;
};

struct core_run {
	int task_num;
	int start;
	int end;
	core_run *next;
};

struct save_min {
	int T_min;
	int E_min;
};

struct final_result {
	std::vector<core_run> final;
	double Energy;
	int time;
};

std::vector<graph> vertex_from;
std::vector<graph> vertex_come;
std::vector<int> cloud;
std::vector<float> priority;
std::vector<double> a_energy; // save the energy consumption rate a_k
int small_time; //the smallest time of schedule
double ori_e;
double ori_e1;

void print_matrix(std::vector<std::vector<int> > A, int m, int n)
{
	int i;
	int j;
	std::cout << "Matrix is:" << std::endl;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			std::cout << " " << A[i][j];
		}
		std::cout << std::endl;
	}
}

save_min print_core(std::vector<core_run> A, int n, std::vector<std::vector<int> > time_array, int T_s, int time_r, double energy_r)		//print the schedule
{
	int i;
	int j;
	int time;
	int time_result;
	double energy;
	double energy_result;
	time = 0;
	time_result = 0;
	energy = 0;
	energy_result = 0;

	save_min result;
	std::cout << "The result should be:" << std::endl;
	for (i = 0; i < n + 1; i++)
	{
		time = 0;
		if (i == 0)
		{
			std::cout << "cloud: ";
		}
		else
		{
			std::cout << "core" << i << " :";
		}
		core_run* temp = new core_run;
		core_run* fore = new core_run;
		temp = A[i].next;
		fore = NULL;
		while (temp != NULL)
		{
			if (fore == NULL)
			{
				for (j = 0; j < temp->start; j++)
				{
					std::cout << "_ ";
					time = time + 1;
				}
			}
			else
			{
				for (j = fore->end; j < temp->start; j++)
				{
					std::cout << "_ ";
					time = time + 1;
				}

			}
			for (j = temp->start; j < temp->end; j++)
			{
				std::cout << temp->task_num << " "; //
				time = time + 1;
				if (i == 0) //cloud energy consumption
				{
					energy = energy + a_energy[0];  
				}
				else
				{
					energy = energy + a_energy[i];
				}
			}
			fore = temp;
			temp = temp->next;
		}
		std::cout << std::endl;
		if (time > time_result)
		{
			time_result = time; //calculate the time of running
		}
	}
	std::cout << "Time is: " << time_r << std::endl;
	std::cout << "Energy consumption is: " << energy_r << std::endl;
	result.T_min = time_result;
	result.E_min = energy;
	return result;
}

void primary_assignment(std::vector<std::vector<int> > A, int m, int n, int T_cloud)
{
	int i;
	int j;
	int min;
	for (i = 0; i < m; i++)
	{
		min = INT_MAX;
		cloud.push_back(0);
		for (j = 0; j < n; j++)
		{
			if (A[i][j] < min)
			{
				min = A[i][j];
			}
		}
		if (min > T_cloud)
		{
			cloud[i] = 1;
		}
	}
}

int zero_in()    //determine whether priority have 0
{
	int i;
	int j;
	for (i = 0; i < priority.size(); i++)
	{
		if (priority[i] == 0)
		{
			return 1;
		}
	}
	return 0;
}

void prioriting(std::vector<std::vector<int> > A, int m, int n, int Tcloud)
{
	int i;
	int j;
	int sum;
	int max;
	for (i = 0; i < m; i++)
	{
		if (vertex_come[i].next == NULL)
		{
			sum = 0;
			if (cloud[i] == 0)
			{
				for (j = 0; j < n; j++)
				{
					sum = sum + A[i][j];
				}
				priority[i] = sum / n;
			}
			else
			{
				priority[i] = Tcloud;
			}
			//finish.push_back(i);
			vertex_come[i].finish = 't';
		}
	}
	while (zero_in())
	{
		for (i = 0; i < m; i++)
		{
			if (priority[i] != 0)
			{
				continue;
			}
			sum = 0;
			max = 0;
			graph* temp = new graph;
			temp = vertex_come[i].next;
			while (temp != NULL)
			{
				if (vertex_come[temp->name - 1].finish == 't')
				{
					if (max < priority[temp->name - 1])
					{
						max = priority[temp->name - 1];
					}
					temp = temp->next;
				}
				else
				{
					break;
				}
			}
			if (temp == NULL && max != 0)
			{
				if (cloud[i] == 0)
				{
					for (j = 0; j < n; j++)
					{
						sum = sum + A[i][j];
					}
					priority[i] = sum / n + max;
				}
				else
				{
					priority[i] = Tcloud + max;
				}
				vertex_come[i].finish = 't';
			}
		}
	}

}

int max(std::vector<use_sort> A)
{
	int i;
	int max = 0;
	for (i = 0; i < A.size(); i++)
	{
		if (max < A[i].time)
		{
			max = A[i].time;
		}
	}
	return max;
}

std::vector<use_sort> counting_sort(std::vector<use_sort> A, int max)
{
	int i, j;
	std::vector<use_sort> B(A.size());
	std::vector<int> C(max + 2);
	for (i = 0; i < A.size(); i++)
	{
		B[i] = A[i];
	}
	for (j = 0; j < max + 2; j++)
	{
		C[j] = 0;
	}
	for (i = 0; i < B.size(); i++)
	{
		C[B[i].time] = C[B[i].time] + 1;
	}
	for (j = 1; j < max + 2; j++)
	{
		C[j] = C[j - 1] + C[j];
	}
	for (i = 0; i <= A.size() - 1; i++)
	{
		A[C[B[i].time] - 1] = B[i];
		C[B[i].time] = C[B[i].time] - 1;
	}
	return A;
}

std::vector<use_sort> order_priority(std::vector<use_sort> A) //order the priority vector
{
	int i;
	for (i = 0; i < priority.size(); i++)
	{
		A[i].time = priority[i];
		A[i].num_task = i + 1;
	}
	A = counting_sort(A, max(A)); //sort the priority
	return A;
}

std::vector<core_run> unit_selection(std::vector<use_sort> A, int m, int n, std::vector<std::vector<int> > time_array, int T_cloud, int T_s)
{
	int i;
	int j;
	int min_end;
	int min_start;
	int min_end_cloud;
	int index;
	int start_time = 0;
	graph* temp = new graph;
	graph on_cloud;
	on_cloud.end_time = 0;
	on_cloud.name = 0;
	on_cloud.next = NULL;
	std::vector<core_run> core_result(n + 1);  //use to save task in each core and cloud(index 0 means cloud), figure 3 in paper
	for (i = 0; i < n + 1; i++)			//use to initial the core_result
	{
		core_result[i].start = 0;
		core_result[i].end = 0;
		core_result[i].task_num = 0;
		core_result[i].next = NULL;
	}
	A = order_priority(A);
	//slected vertex should run after the vertex in it's vertex_from list.
	for (i = A.size() - 1; i >= 0; i--)
	{
		temp = vertex_from[A[i].num_task - 1].next;
		start_time = 0;
		while (temp != NULL && vertex_from[temp->name - 1].finish == 't')
		{
			if (start_time < vertex_from[temp->name - 1].end_time);
			{
				start_time = vertex_from[temp->name - 1].end_time;
			}
			temp = temp->next;
		}
		if (temp == NULL)
		{
			//core_result
			min_end = INT_MAX;
			core_run* sud = new core_run;
			core_run* v = new core_run;
			
			if (cloud[A[i].num_task - 1] == 0)    //if we need to schedule the task on local core
			{
				for (j = n; j >= 0; j--)
				{
					if (j != 0)
					{
						if (min_end > core_result[j].end + time_array[A[i].num_task - 1][j - 1])
						{
							if (core_result[j].end < start_time)
							{
								min_start = start_time;
								min_end = start_time + time_array[A[i].num_task - 1][j - 1];
								index = j;
							}
							else
							{
								min_start = core_result[j].end;
								min_end = core_result[j].end + time_array[A[i].num_task - 1][j - 1];
							}
							index = j;
						}
					}
					else   //if we need to schedule the task on cloud no matter it is a local task or cloud task
					{
						if (min_end > core_result[j].end + T_cloud)
						{
							if (core_result[j].end < start_time)
							{
								min_start = start_time;
								min_end = start_time + T_cloud;
								min_end_cloud = start_time + T_s;
							}
							else
							{
								min_start = core_result[j].end;
								min_end = core_result[j].end + T_cloud;
								min_end_cloud = start_time + T_s;
							}
							index = j;
						}
					}
				}
			}
			else         // if task is a cloud task
			{
				if (min_end > core_result[0].end + T_cloud)
				{
					if (core_result[0].end < start_time)
					{
						min_start = start_time;
						min_end = start_time + T_cloud;
						min_end_cloud = start_time + T_s;
					}
					else
					{
						min_start = core_result[0].end;
						min_end = core_result[0].end + T_cloud;
						min_end_cloud = start_time + T_s;
					}
					index = 0;
				}
			}
			sud = core_result[index].next;
			v->next = NULL;
			v->start = min_start;
			if (index != 0)
			{
				v->end = min_end;
			}
			else
			{
				v->end = min_end_cloud;
			}
			v->task_num = A[i].num_task;
			if (sud == NULL)
			{
				core_result[index].next = v;
			}
			else
			{
				while (sud->next != NULL)
				{
					sud = sud->next;
				}
				sud->next = v;
			}
			
			core_result[index].end = min_end;
			vertex_from[A[i].num_task - 1].finish = 't';
			vertex_from[A[i].num_task - 1].end_time = min_end;
		}
	}
	return core_result;
}

std::vector<core_run> copy_vec(std::vector<core_run> S_k, int n)
{
	int i;
	int j;
	std::vector<core_run> change_S;
	change_S.resize(n);
	for (i = 0; i < S_k.size(); i++)
	{
		core_run* tt;
		change_S[i].task_num = S_k[i].task_num;
		change_S[i].start = S_k[i].start;
		change_S[i].end = S_k[i].end;
		change_S[i].next = NULL;
		core_run* temp;
		if (S_k[i].next != NULL)
		{
			temp = S_k[i].next;
			core_run* v = new core_run;
			v->task_num = temp->task_num;
			v->end = temp->end;
			v->start = temp->start;
			v->next = NULL;
			change_S[i].next = v;
			tt = change_S[i].next;
			temp = temp->next;
			while (temp != NULL)
			{
				core_run* v = new core_run;
				v->task_num = temp->task_num;
				v->end = temp->end;
				v->start = temp->start;
				v->next = NULL;
				tt->next = v;
				tt = tt->next;
				temp = temp->next;
			}
		}
	}
	return change_S;
}

int in_cloud(int task_num, std::vector<core_run> change_S)
{
	int i;
	int j;
	core_run* temp;
	temp = change_S[0].next;
	while (temp != NULL)
	{
		if (temp->task_num == task_num)
		{
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

std::vector<core_run> prepare_time_energy(std::vector<core_run> change_S, std::vector<std::vector<int> > time_array, std::vector<use_sort> select_result, int row_ready, int T_cr, int T_s, int T_c)
{
	int i;
	int j;
	int k;
	int max;
	std::vector<core_run> ready(time_array.size());
	core_run* temp;

	for (i = 0; i < change_S.size(); i++)
	{
		temp = change_S[i].next;
		while (temp != NULL) // initial the array
		{
			core_run* v = new core_run;
			v->task_num = temp->task_num;
			v->start = temp->start;
			v->end = temp->end;
			v->next = NULL;
			ready[temp->task_num - 1].next = v;
			temp = temp->next;
		}
	}
	for (i = select_result.size() - 1; i >= 0; i--)
	{
		core_run* sud = new core_run;
		core_run* pre_sud = new core_run;
		sud = NULL;
		pre_sud = NULL;
		
		for (j = 0; j < change_S.size(); j++)
		{
			core_run* sud1;
			sud1 = change_S[j].next;
			core_run* pre1 = new core_run;
			pre1 = NULL;
			while (sud1 != NULL)
			{
				if (select_result[i].num_task == sud1->task_num)
				{
					sud = sud1;
					if (pre1 == NULL)
					{
						pre_sud = NULL;
					}
					else
					{
						pre_sud = pre1;
					}
					break;
				}
				pre1 = sud1;
				sud1 = sud1->next;
			}
			if (sud1 != NULL)
			{
				break;
			}			//to be sure that the change time step can process in order
		}
		if (1) 
		{
			graph* temp2 = new graph;
			int compare;
			temp2 = vertex_from[sud->task_num - 1].next; //check the end time of the task process before this task
			max = 0;
			while (temp2 != NULL)
			{
				if (in_cloud(temp2->name, change_S)) //temp2->name at cloud
				{
					if ((in_cloud(sud->task_num, change_S) == 0))
					{
						compare = ready[temp2->name - 1].next->end + T_cr;
					}
					else
					{
						if (in_cloud(sud->task_num, change_S) && (ready[temp2->name - 1].next->end + T_c <= sud->start + T_s))
						{
							compare = ready[temp2->name - 1].next->end;
						}
						else
						{
							compare = ready[temp2->name - 1].next->end + T_c - T_s;
						}
					}
				}
				else
				{
					compare = ready[temp2->name - 1].next->end;
				}
				if (max < compare)
				{
					max = compare;
				}
				temp2 = temp2->next;
			}
			if (pre_sud == NULL)
			{
				
				{
					sud->end = sud->end - sud->start;
					sud->start = max;
					sud->end = sud->end + max;
					ready[sud->task_num - 1].next->start = sud->start;
					ready[sud->task_num - 1].next->end = sud->end;
				}
			}
			else
			{
				if (max < pre_sud->end)
				{
					
					{
						sud->end = sud->end - sud->start;
						sud->start = pre_sud->end;
						sud->end = sud->end + pre_sud->end;
						ready[sud->task_num - 1].next->start = sud->start;
						ready[sud->task_num - 1].next->end = sud->end;
					}
				}
				else
				{
					
					{
						sud->end = sud->end - sud->start;
						sud->start = max;
						sud->end = sud->end + max;
						ready[sud->task_num - 1].next->start = sud->start;
						ready[sud->task_num - 1].next->end = sud->end;
					}
				}
			}
			
		}
	}
	return change_S;
}

double calculate_energy(std::vector<core_run> change_S, std::vector<std::vector<int> > time_array, int T_s)
{
	int i;
	int j;
	double energy;
	energy = 0;
	for (i = 0; i < change_S.size(); i++)
	{
		core_run* temp;
		temp = change_S[i].next;
		while (temp != NULL)
		{
			if (i == 0)
			{
				energy = energy + a_energy[0] * T_s;
			}
			else
			{
				energy = energy + a_energy[i] * time_array[temp->task_num - 1][i - 1]; //*(100/time_array[temp->task_num - 1][i - 1])*(100/time_array[temp->task_num - 1][i - 1]); //what's the frequency
			}
			temp = temp->next;
		}

	}
	return energy;
}

int calculate_time(std::vector<core_run> change_S, int T_cr)
{
	int i;
	int j;
	int time;
	int max;
	int middle;
	max = 0;
	for (i = 0; i < change_S.size(); i++)
	{
		core_run* temp = new core_run;
		if (change_S[i].next != NULL)
		{
			temp = change_S[i].next;
			while (temp->next != NULL)
			{
				temp = temp->next;
			}
			middle = temp->end;
			if (i == 0)
			{
				middle = middle + T_cr;
			}
			if (max < middle)
			{
				max = middle;
			}
		}
	}
	return max;
}

std::vector<core_run> chose_best_result(std::vector<final_result> kernel_result, std::vector<core_run> S_k, double ori_energy, int ori_time, std::vector<std::vector<int> > time_array, int T_s, int T_cloud)
{
	int i;
	int j;
	int index;
	double rate;
	index = -1;
	rate = 0;
	for (i = 0; i < kernel_result.size(); i++)
	{
		if (kernel_result[i].time <= ori_time && kernel_result[i].Energy < ori_energy) //ori_e1     kernel_result[i].time <= 18 && kernel_result[i].Energy < ori_e
		{
			ori_energy = kernel_result[i].Energy;
			index = i;
		}
	}
	if (index == -1)
	{
		for (i = 0; i < kernel_result.size(); i++)
		{
			if (ori_energy > kernel_result[i].Energy  && kernel_result[i].time <= (small_time + 8 * small_time / 18) && kernel_result[i].time > ori_time) //ori_e > kernel_result[i].Energy && kernel_result[i].time <= (small_time + 8) && kernel_result[i].time > ori_time     small_time/2, can change the largest running time
			{
				if (rate < (double(ori_energy - kernel_result[i].Energy) / (double(kernel_result[i].time - ori_time))))
				{
					rate = (double(ori_energy - kernel_result[i].Energy) / (double(kernel_result[i].time - ori_time)));
					index = i;
				}
			}
			
		}
	}
	if (index != -1)
	{
		std::cout << "Best choice is ";
		print_core(kernel_result[index].final, kernel_result[index].final.size() - 1, time_array, T_s, calculate_time(kernel_result[index].final, T_cloud - T_s), calculate_energy(kernel_result[index].final, time_array, T_s));
		
		return kernel_result[index].final;
	}
	else
	{
		std::cout << "we can stop here, can't find a better one" << std::endl;
		return S_k;
	}
}

std::vector<core_run> kernel(std::vector<core_run> S_k, std::vector<use_sort> select_result, std::vector<std::vector<int> > time_array, int T_cloud, int T_s, int T_c)
{
	int i;
	int j;
	int s;
	int f = 0;
	int one;
	int two;
	int ff;
	int change_time;
	std::vector<core_run> change_S;
	std::vector<final_result> kernel_result;
	//change_S = S_k;
	for (i = 0; i < S_k.size(); i++)
	{
		core_run* pre;
		core_run* temp;
		temp = S_k[i].next;
		pre = NULL;
		while (temp != NULL)
		{
			for (j = 0; j < S_k.size(); j++)
			{
				core_run* temp1;
				core_run* temp11; //
				core_run* pre1;
				core_run* pre11; //
				core_run* pre2;
				core_run* temp2;
				change_S = copy_vec(S_k, S_k.size());
				temp1 = change_S[i].next;
				temp11 = change_S[i].next;//
				pre1 = NULL;
				if (pre != NULL)
				{
					while (temp1 != NULL && temp->task_num != temp1->task_num)
					{
						pre1 = temp1;
						temp1 = temp1->next;   // copy pre and temp, because we don't want to change them
						temp11 = temp11->next;
					}
				}
				if (temp1 != NULL)
				{
					if (temp1->next != NULL)
					{
						temp1->next->end = temp1->next->end - temp1->next->start;
						if (pre1 != NULL)
						{
							temp1->next->start = pre1->end;
						}
						else
						{
							temp1->next->start = 0; 
						}
						temp1->next->end = temp1->next->start + temp1->next->end;
						pre11 = temp11->next;
						temp11 = temp11->next->next;
						while (temp11 != NULL)
						{
							temp11->end = temp11->end - temp11->start;
							temp11->start = pre11->end;// inorder to make them one by one
							temp11->end = temp11->start + temp11->end;
							pre11 = temp11;
							temp11 = temp11->next;
						}
					}
				}

				f = 0;
				ff = 0;
				if (j != i)   //because we don't need to move it to it's original sequence
				{
					temp2 = change_S[j].next;
					pre2 = NULL;
					if (temp2 == NULL)
					{
						if (pre1 == NULL)
						{
							change_S[i].next = temp1->next;  //delete task i in original core
						}
						else
						{
							pre1->next = temp1->next;   //delete task i in original core
						}
						temp1->next = change_S[j].next;
						change_S[j].next = temp1;
						temp1->end = temp1->end - temp1->start;
						temp1->start = 0;
						if (j != 0)
						{
							temp1->end = time_array[temp1->task_num - 1][j - 1] + temp1->start;
						}
						else
						{
							temp1->end = temp1->start + T_s;
						}
					}
					else
					{
						while (temp2 != NULL)//move the task to core j
						{
							one = 0;
							two = 0;
							if ((temp2->start >= temp1->start) && f == 0) //original process before   
							{
								if (pre2 == NULL)
								{
									if (pre1 == NULL)
									{
										change_S[i].next = temp1->next;  //delete task i in original core
									}
									else
									{
										pre1->next = temp1->next;   //delete task i in original core
									}
									//if two task start at same time, then order them
									if (temp2->start == temp1->start)
									{
										for (s = 0; s < select_result.size(); s++)
										{
											if (select_result[s].num_task == temp1->task_num)
												one = s; //select_result[s].time
											if (select_result[s].num_task == temp2->task_num)
												two = s; //select_result[s].time
										}
									}
									if (temp2->start > temp1->start)	
									{

										temp1->next = temp2;
										
										change_S[j].next = temp1;
										temp1->end = temp1->end - temp1->start;
										temp1->start = 0;
										if (j != 0)
										{
											temp1->end = temp1->start + time_array[temp1->task_num - 1][j - 1];
										}
										else
										{
											temp1->end = temp1->start + T_s;
										}
										
									}
									else if (temp2->start <= temp1->start)   
									{
										temp1->next = temp2->next;
										temp2->next = temp1;
										temp1->start = temp2->end;
									}
									if (j != 0)
									{
										temp1->end = temp1->start + time_array[temp1->task_num - 1][j - 1];
									}
									else
									{
										temp1->end = temp1->start + T_s;
									}
								}
								else
								{
									if (pre1 == NULL)
									{
										change_S[i].next = temp1->next;
									}
									else
									{
										pre1->next = temp1->next;
									}
									//if two task start at same time, then order them
									if (temp2->start == temp1->start)
									{
										for (s = 0; s < select_result.size(); s++)
										{
											if (select_result[s].num_task == temp1->task_num)
												one = s; //select_result[s].time
											if (select_result[s].num_task == temp2->task_num)
												two = s; //select_result[s].time
										}
									}
									if (temp2->start > temp1->start) 
									{
										temp1->next = temp2;
										pre2->next = temp1;
										temp1->start = pre2->end;
									}
									else if (temp2->start <= temp1->start)  
									{
										temp1->next = temp2->next;
										temp2->next = temp1;
										temp1->start = temp2->end;
										 // if temp1 and temp2 start at same time
									}

									if (j != 0)
									{
										temp1->end = temp1->start + time_array[temp1->task_num - 1][j - 1];
									}
									else
									{
										temp1->end = temp1->start + T_s;
									}

								}
								f = 1;
							}
							else if (temp2->next == NULL && f == 0)
							{

								if (pre1 == NULL)
								{
									change_S[i].next = temp1->next;  //delete task i in original core
								}
								else
								{
									pre1->next = temp1->next;   //delete task i in original core
								}
								temp1->next = temp2->next;
								temp2->next = temp1;
								temp1->start = temp2->end;
								if (j != 0)
								{
									temp1->end = temp1->start + time_array[temp1->task_num - 1][j - 1];
								}
								else
								{
									temp1->end = temp1->start + T_s;
								}

								ff = 1;
								f = 1;
							}
							if (f > 0)
							{
								if (f == 1)
								{
									if (ff != 1 && (temp2->start > temp1->start))
									{
										temp2->start = temp1->end;
									}
									if (j != 0)
									{
										temp2->end = temp2->start + time_array[temp2->task_num - 1][j - 1];
									}
									else
									{
										temp2->end = temp2->start + T_s;
									}
									f = f + 1;
								}
								else
								{
									if (temp2->start < pre2->end)
									{
										temp2->start = pre2->end;
									}
									if (j != 0)
									{
										temp2->end = temp2->start + time_array[temp2->task_num - 1][j - 1];
									}
									else
									{
										temp2->end = temp2->start + T_s;
									}
								}
							}
							pre2 = temp2;
							temp2 = temp2->next;
						}
					}
					change_S = prepare_time_energy(change_S, time_array, select_result, j, T_cloud - T_s, T_s, T_c);
					change_S = prepare_time_energy(change_S, time_array, select_result, j, T_cloud - T_s, T_s, T_c);
					int time;
					time = calculate_time(change_S, T_cloud - T_s);
					change_S = prepare_time_energy(change_S, time_array, select_result, j, T_cloud - T_s, T_s, T_c);
					while (time != calculate_time(change_S, T_cloud - T_s))
					{
						time = calculate_time(change_S, T_cloud - T_s);
						change_S = prepare_time_energy(change_S, time_array, select_result, j, T_cloud - T_s, T_s, T_c);
					}
					final_result p;
					p.final = change_S;
					p.Energy = calculate_energy(change_S, time_array, T_s);
					p.time = calculate_time(change_S, T_cloud - T_s);
					kernel_result.push_back(p);

				}

			}
			pre = temp;
			temp = temp->next;
		}
	}
	return chose_best_result(kernel_result, S_k, calculate_energy(S_k, time_array, T_s), calculate_time(S_k, T_cloud - T_s), time_array, T_s, T_cloud);
}

int main()
{
	int n;
	int m;
	int e;
	int i;
	int j;
	int T_s;
	int T_c;
	int T_r;
	graph v;

	std::cout << "Tell me how many Task you have?: ";
	std::cin >> m;
	std::cout << "Tell me how many core in your computer?: ";
	std::cin >> n;
	std::vector<std::vector<int> > time_array(m);
	for (i = 0; i < m; i++)
	{
		time_array[i].resize(n);
		priority.push_back(0);
	}
	std::cout << "Now you can copy your Task-Core time consumption matrix in:" << std::endl;
	for (i = 0; i < m; i++)
	{
		v.name = i + 1;
		v.end_time = 0;
		v.finish = 'f';
		v.next = NULL;
		vertex_from.push_back(v);
		vertex_come.push_back(v);
		for (j = 0; j < n; j++)
		{
			std::cin >> time_array[i][j];
		}
	}

	std::cout << "If send to cloud, the time is(Ts):";
	std::cin >> T_s;
	std::cout << "How long of cloud processing?(Tc): ";
	std::cin >> T_c;
	std::cout << "How many time can cloud send data back?(Tr):";
	std::cin >> T_r;

	//Input the graph of the order of task
	std::cout << "How many edge you have in your task order graph?: ";
	std::cin >> e;
	std::vector<std::vector<int> > row_graph(e);
	std::cout << "Please input an matrix have 2 columns, each row means an edge, edge is from column 1 to column 2" << std::endl;
	for (i = 0; i < e; i++)
	{
		row_graph[i].resize(2);
		for (j = 0; j < 2; j++)
		{
			std::cin >> row_graph[i][j];
		}
		graph *temp = new graph;
		graph *nw = new graph;
		graph *nw2 = new graph;
		graph *temp2 = new graph;
		nw->name = row_graph[i][0];
		nw->next = NULL;
		nw2->name = row_graph[i][1];
		nw2->next = NULL;
		if (vertex_from[row_graph[i][1] - 1].next == NULL)
		{
			vertex_from[row_graph[i][1] - 1].next = nw;
		}
		else
		{
			temp = vertex_from[row_graph[i][1] - 1].next;

			while (temp->next != NULL)
			{
				temp = temp->next;
			}
			temp->next = nw;
		}
		if (vertex_come[row_graph[i][0] - 1].next == NULL)
		{
			vertex_come[row_graph[i][0] - 1].next = nw2;
		}
		else
		{
			temp2 = vertex_come[row_graph[i][0] - 1].next;
			while (temp2->next != NULL)
			{
				temp2 = temp2->next;
			}
			temp2->next = nw2;
		}
	}
	//input the energy consumption rate a_k of each core
	double y;
	std::cout << "Plsease input the energy consumption power when sending data: ";
	std::cin >> y;
	a_energy.push_back(y);
	for (i = 0; i < n; i++)
	{
		std::cout << "Plseae input the energy consumption power rate (a_k) of core " << i + 1 << " :";
		std::cin >> y;
		a_energy.push_back(y);
	}

	//primary_assignment
	int T_cloud;
	T_cloud = T_s + T_c + T_r;
	primary_assignment(time_array, m, n, T_cloud);
	//Prioriting
	prioriting(time_array, m, n, T_cloud);
	//SELECT
	std::vector<use_sort> select_result(m); //use to save the result of selected
	std::vector<core_run> core_result;
	core_result = unit_selection(select_result, m, n, time_array, T_cloud, T_s); //the result of schedule
	//print the first time_min schedule
	std::vector<save_min> min_v;
	save_min temp_min;
	temp_min = print_core(core_result, n, time_array, T_s, calculate_time(core_result, T_cloud - T_s), calculate_energy(core_result, time_array, T_s));
	min_v.push_back(temp_min);
	//run the kernel function
	std::vector<core_run> S_k;
	S_k = copy_vec(core_result, core_result.size());

	/////if want to use the initial scheduling shown in the paper, use the code below//////////////////////////
	/*
	core_run* tt = new core_run;
	tt->start = 5;
	tt->end = 8;
	tt->next = NULL;
	tt->task_num = 2;
	S_k[0].next = tt;
	core_run* tt1 = new core_run;
	tt1->start = 5;
	tt1->end = 12;
	tt1->next = NULL;
	tt1->task_num = 4;
	S_k[1].next = tt1;
	core_run* tt2 = new core_run;
	tt2->start = 5;
	tt2->end = 11;
	tt2->next = NULL;
	tt2->task_num = 6;
	S_k[2].next = tt2;
	core_run* tt3 = new core_run;
	tt3->start = 12;
	tt3->end = 16;
	tt3->next = NULL;
	tt3->task_num = 8;
	S_k[2].next->next = tt3;
	core_run* tt4 = new core_run;
	tt4->start = 0;
	tt4->end = 5;
	tt4->next = NULL;
	tt4->task_num = 1;
	S_k[3].next = tt4;
	core_run* tt5 = new core_run;
	tt5->start = 5;
	tt5->end = 9;
	tt5->next = NULL;
	tt5->task_num = 3;
	S_k[3].next->next = tt5;
	core_run* tt6 = new core_run;
	tt6->start = 9;
	tt6->end = 11;
	tt6->next = NULL;
	tt6->task_num = 5;
	S_k[3].next->next->next = tt6;
	core_run* tt7 = new core_run;
	tt7->start = 11;
	tt7->end = 14;
	tt7->next = NULL;
	tt7->task_num = 7;
	S_k[3].next->next->next->next = tt7;
	core_run* tt8 = new core_run;
	tt8->start = 14;
	tt8->end = 16;
	tt8->next = NULL;
	tt8->task_num = 9;
	S_k[3].next->next->next->next->next = tt8;
	core_run* tt9 = new core_run;
	tt9->start = 16;
	tt9->end = 18;
	tt9->next = NULL;
	tt9->task_num = 10;
	S_k[3].next->next->next->next->next->next = tt9;
	std::cout<<"initial ";
	print_core(S_k, n, time_array, T_s, calculate_time(S_k, T_cloud - T_s), calculate_energy(S_k, time_array, T_s));
	*/
	////////if want to use the initial scheduling in paper, use the code above///////////////////
	small_time = calculate_time(core_result, T_cloud - T_s);
	ori_e = calculate_energy(core_result, time_array, T_s);
	ori_e1 = ori_e;
	for (i = 0; i < 10; i++) //repeat for ten times
	{
		S_k = kernel(S_k, order_priority(select_result), time_array, T_cloud, T_s, T_c);
	}
}
