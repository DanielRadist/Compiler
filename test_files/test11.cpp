int fib(int n)
{
	switch (n)
	{
	case 1:
	case 2:
		return 1;
	default: 
		return fib(n - 1) + fib(n - 2);
	}
}

int main()
{
	int _VAL = fib(1);	// 1ое число
	_VAL = fib(2);		// 2ое
	_VAL = fib(3);		// 3ое
	_VAL = fib(4);
	_VAL = fib(5);
	_VAL = fib(6);
	_VAL = fib(7);
	_VAL = fib(8);
	_VAL = fib(9);
	_VAL = fib(10);
}