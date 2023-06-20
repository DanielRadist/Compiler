bool _a = 1;
bool _b = 1 - 1;

int _y0 = true != false;
int _y1 = true == false;

int _y2 = true != true;
int _y3 = true == true;

int _z1 = 3 > 2;
int _z2 = 3 < 2;
int _z3 = 3 <= 2;
int _z4 = 3 >= 2;

int _z5 = 3 > 3;
int _z6 = 3 < 3;
int _z7 = 3 <= 3;
int _z8 = 3 >= 3;

int main()
{
	int _var = 10;
	_a = _var;
	_var = _b;
	_var = _b + true;
}