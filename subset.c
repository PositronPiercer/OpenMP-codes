#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <stdbool.h> 

bool isSubsetSum(int set[], int n, int sum)
{

	if (sum == 0)
		return 1;
	if (n == 0 && sum != 0)
		return 0;

	if (set[n - 1] > sum)
		return isSubsetSum(set, n - 1, sum);

	bool a = false, b = false;
	#pragma omp task default(shared)
	{
		a = isSubsetSum(set, n - 1, sum);
	}
	

	#pragma omp task default(shared)
	{
		b = isSubsetSum(set, n - 1, sum - set[n - 1]);
	}
	

	#pragma omp taskwait

	return a||b;

}


int main()
{
	int set[] = { 3, 34, 4, 12, 5, 2 };
	int sum = 9;
	int n = sizeof(set) / sizeof(set[0]);
	bool result = false;

	#pragma omp parallel 
	{
		#pragma omp single
		{
			result = isSubsetSum(set, n, sum);
		}
		
	}

	if (result)
		printf("Found a subset with given sum\n");
	else
		printf("No subset with given sum\n");
	return 0;
}
