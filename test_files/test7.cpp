int foo(int c)
{
	c = c + 1;
	return c;
	c = 20;
}

int fib(int n)
{
	int q = foo(n) + 1;
	return q;
}

int main()
{	
	int c = fib(10);	
}