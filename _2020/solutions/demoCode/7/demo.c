#include <stdio.h>
#define SIZE 4
struct Stru{
    char a[SIZE];
    int num;
}stru;

void initialize(char *a, int size){
    while (size>0)
		a[size--] = 0; 
}

void multiply(char *a, int size, int num)
{
	int i;
	for (i=0; i<size; i++)
		a[i] = i * num;
}

void print_array(char *a, int size)
{
	int i;
	for (i=0; i<size; i++)
		printf("f(%d)=%d\n", i, a[i]);
}

int main(int argc, char **argv)
{
    stru.num = 2;
	initialize(stru.a, SIZE);
	multiply(stru.a, SIZE, stru.num);
	print_array(stru.a, SIZE);
	return 0;
}
