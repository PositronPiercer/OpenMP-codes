#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int isSubsetSum(int set[], int n, int sum)
{
	// Base Cases 
	if (sum == 0)
		return 1;
	if (n == 0 && sum != 0)
		return 0;

	// If last element is greater than sum, then ignore it 
	if (set[n - 1] > sum)
		return isSubsetSum(set, n - 1, sum);

	/* else, check if sum can be obtained by any of the following
		(a) including the last element
		(b) excluding the last element */
#pragma omp task
	int a = isSubsetSum(set, n - 1, sum);
#pragma omp task
	int b = isSubsetSum(set, n - 1, sum - set[n - 1]);
#pragma omp taskwait

	return (a + b);

}

// Driver program to test above function 
int main()
{
	int set[] = { 3, 34, 4, 12, 5, 2 };
	int sum = 9;
	int n = sizeof(set) / sizeof(set[0]);

#pragma omp parallel
#pragma omp single
	int result = isSubsetSum(set, n, sum);

	if (result)
		printf("Found a subset with given sum");
	else
		printf("No subset with given sum");
	return 0;
}
