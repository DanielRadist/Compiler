const int a = 1;
const bool boop = true;			// объявление глобальной константы

bool kek (int a, bool b_) {		// объявление функции
	bool tmp = false;			// объявление и инициализация переменной
	b_ = 7;						// присвоение переменной
	bool noInit;				// объявление переменной
	int __t = (a + b_);			// объявление и инициализация переменной и выражение в скобках
	return ((__t + 1));			// возврат ин функции	
}

int main()						// функция main
{
	const int qwe = 15;			// объявление константы
	int a = 5;					// объявление и инициализация переменной
	kek(2, a);					// вызов функции с передачей параметров
	
	int i_ = -10;				// объявление и инициализация целых переменных
	
	a = i_;						// присвоение целым переменным
	
	bool q = a && true || kek(2, i_);			// объявление и инициализация переменной результата выражения
	
	q = ((7 || 1 + 88) != (a && 14) && false);	// "сложное" выражение
	
	a >> 4;						// сдвиг
	
	5;							// целое значение
	switch (a)					// объявление switch
	{
	case 1:						// case c выходом
		s1 = true;
		break;
	case 2:
	case 3:						// case без выхода
		s1 = 14;
		s3 = 400;	
		return 0;
	default:					//default c выходом
		s5 = 0;
		s6 = false;
		s7 = s5 + (s6 * a);
		break;
	}
	
	a--;						// декримент
	b++;
	++a;						// инкремент
	--b;
	y = b + (a++) - y;
	
	return 0;
}