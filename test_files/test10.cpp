bool foo()
{
	return true;
}

int main()
{
	int _a = 1;
	int _var = 0;
	switch (_a)
	{
	case 1:
		_var = _var + foo();
	case 2:
		_var = _var + 10;
	case 3:
		_var = _var + 100;
	default:
		_var = _var + 1000;
	}
	
	_a = 2;
	_var = 0;
	
	switch (_a)
	{
	case 1:
		_var = _var + 1;
	case 2:
		_var = _var + 10;
	case 3:
		_var = _var + foo() * 100;
		break;
	default:
		_var = _var + 1000;
	}
	
	_a = 0;
	_var = 0;
	
	switch (_a)
	{
	case 1:
		_var = _var + 1;
	case 2:
		_var = _var + 10;
	case 3:
		_var = _var + 100;
	default:
		_var = _var + 1000;
	}

}