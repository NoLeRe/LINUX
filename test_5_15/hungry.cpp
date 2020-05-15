#include<cstdio>
#include<iostream>
using namespace std;

class single{
	
public:
	int* getserouce()
	{
		return &a;	
	}

private:
	static int a;
};
int single::a =10;

int main()
{
	
	single a;
	single b;
	printf("a:%d----%p\n",*(a.getserouce()),a.getserouce());
	printf("b:%d----%p\n",*(b.getserouce()),b.getserouce());

	
	return 0;	
}
