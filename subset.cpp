#include <bits/stdc++.h>
#include <omp.h>
 
using namespace std;



// A hash function used to hash a pair of any kind 
struct hash_pair { 
    template <class T1, class T2> 
    size_t operator()(const pair<T1, T2>& p) const
    { 
        auto hash1 = hash<T1>{}(p.first); 
        auto hash2 = hash<T2>{}(p.second); 
        return hash1 ^ hash2; 
    } 
}; 

unordered_map<pair<int, int>, bool, hash_pair> um;

bool isSubsetSum(int set[], int n, int sum)
{

	if (sum == 0)
		return 1;
	if (n == 0 && sum != 0)
		return 0;

	if (set[n - 1] > sum)
		return isSubsetSum(set, n - 1, sum);

	bool a = false, b = false;
	unordered_map<pair<int, int>, bool, hash_pair>::iterator it;
	#pragma omp task default(shared)
	{
		if((it = um.find(make_pair(n - 1, sum))) != um.end()){
			a = it->second;
		}
		else{
			a = isSubsetSum(set, n - 1, sum);
			um.insert(make_pair(make_pair(n - 1, sum), a));
		}
		
	}
	

	#pragma omp task default(shared)
	{
		if((it = um.find(make_pair(n - 1, sum - set[n - 1]))) != um.end()){
			a = it->second;
		}
		else{
			b = isSubsetSum(set, n - 1, sum - set[n - 1]);
			um.insert(make_pair(make_pair(n - 1, sum - set[n - 1]), a));
		}
		
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
	

	#pragma omp parallel default(shared)
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
