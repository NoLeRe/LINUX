#include<iostream>
#include<cstdio>

class single_instance{
	
public:

	int* get_instance()
	{
		return &_data;	
	}


private:
	static int _data;
};
int single_instance::_data=0;


int main()
{
	single_instance a;
	single_instance b;

	printf("a:%d-----%p\n",*(a.get_instance()),a.get_instance());
	printf("b:%d-----%p\n",*(b.get_instance()),b.get_instance());

	return 0;
	
}
