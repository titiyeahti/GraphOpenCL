#define TEMP (1<<31) 
#define MAX_INT TEMP-1

__kernel
void djikstra1(__global int* vertices,
				__global int* edges,
				__global int* weights, 
				__global int* costs,
				__global int* u_costs,
				__global int* tovisit)
{
	int id = get_global_id(0);
	int dest;
	int i;
	int cost;
	int n_cost;
	int start;
	int end;

	if (tovisit[id])
	{
		tovisit[id] = 0;
		start = vertices[id];
		end = vertices[id+1];
		for(i = start; i<end; i++)
		{
			dest = edges[i];
			
			cost = u_costs[dest];
			n_cost = costs[id] + weights[i];
			
			if (n_cost < cost)
			{
				u_costs[dest] = n_cost;
			}
		}
	}
};

__kernel
void djikstra2(__global int* u_costs,
				__global int* tovisit,
				__global int* costs)
{
	int id = get_global_id(0);
	int c = costs[id];
	int nc = u_costs[id];

	if (c > nc)
	{
		costs[id] = nc;
		tovisit[id] = 1;
	}
};

__kernel
void init(__global int* u_costs,
				__global int* tovisit,
				__global int* costs,
				const int start)
{
	int id = get_global_id(0);
	if (id == start)
	{
		costs[id] = 0;
		u_costs[id] = 0;
		tovisit[id] = 1;
	}
	else 
	{
		costs[id] = MAX_INT;
		u_costs[id] = MAX_INT;
		tovisit[id] = 0;
	}
};


