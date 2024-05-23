#include <iostream>
#include <cmath>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 10000000

using namespace std;

bool is_prime(long int x)
{
    if (x == 1) return false;
    for (long int i = 2; i <= sqrt(x); i++)
    {
        if (x % i == 0) return false;
    }
    return true;
}

bool is_fibonnaci(long int N)
{
    if (N == 0 || N == 1)
        return true;
    long int a = 0, b = 1, c;
    while (true) {
        c = a + b;
        a = b;
        b = c;
        if (c == N)
            return true;
        else if (c >= N) {
            return false;
        }
    }
}

int watek(long int min, long int max)
{
    int counter = 0;

    for (long int i = min; i < max; i++)
    {
        if (is_prime(i) && is_fibonnaci(i))
        {
            counter++;
        }
    }

    return counter;
}

int main()
{
    clock_t start = clock();

    int child_1, child_2, leaf_1_1, leaf_1_2, leaf_2_1, leaf_2_2;
    int div = MAX / 7, min = 1, max = min + div;
    
    child_1 = fork();
    if (child_1 == 0)
    {
    	leaf_1_1 = fork();
    	if (leaf_1_1 != 0) 
    	{
    		leaf_1_2 = fork();
    		if (leaf_1_2 == 0) 
    		{
    			const int p = watek(min + (div * 0), max + (div * 0));
    			cout << getpid() << " leaf 1.2: "<< p << endl;
    			exit(p);
    		}
    		const int p = watek(min + (div * 1), max + (div * 1));
    		int wstatus1, wstatus2;
    		waitpid(leaf_1_1, &wstatus1, 0);
    		waitpid(leaf_1_2, &wstatus2, 0);
    		
    		const int p1 = WEXITSTATUS(wstatus1);
    		const int p2 = WEXITSTATUS(wstatus2);
    		
    		const int w = p + p1 + p2;
    		cout << getpid() << " child 1: " << w << endl;
    		exit(w);
        }
        const int p = watek(min + (div * 2), max + (div * 2));
    	cout << getpid() << " leaf 1.1: " << p << endl;
    	exit(p);
    }
    else
    {
        child_2 = fork();
        if (child_2 == 0)
        {
        	leaf_2_1 = fork();
    		if (leaf_2_1 != 0)
    		{
    			leaf_2_2 = fork();
    			if (leaf_2_2 == 0) 
    			{
    				const int p = watek(min + (div * 3), max + (div * 3));
    				cout << getpid() << " leaf 2.2: "<< p << endl;
    				exit(p);
    			}
    			const int p = watek(min + (div * 4), max + (div * 4));
    			int wstatus1, wstatus2;
    			waitpid(leaf_2_1, &wstatus1, 0);
    			waitpid(leaf_2_2, &wstatus2, 0);
    			
    			const int p1 = WEXITSTATUS(wstatus1);
    			const int p2 = WEXITSTATUS(wstatus2);
    			
    			const int w = p + p1 + p2;
    			cout << getpid() << " child 2: " << w << endl;
    			exit(w);
    		}
    		const int p = watek(min + (div * 5), max + (div * 5));
    		cout << getpid() << " leaf 2.1: "<< p << endl;
    		exit(p);
        }
    }
    
    const int p = watek(min + (div * 6), MAX);
    			
    int wstatus1, wstatus2;
    waitpid(child_1, &wstatus1, 0);
    waitpid(child_2, &wstatus2, 0);
    
    const int p1 = WEXITSTATUS(wstatus1);
    const int p2 = WEXITSTATUS(wstatus2);
    
    const int w = p + p1 + p2;
    cout << getpid() << " root: " << w << endl;
    
    clock_t end = clock();
    double time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    cout << "znalezionych liczb: " << w << ", czas: " << time << endl;
    
    return 0;
}
