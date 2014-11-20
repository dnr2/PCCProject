#include "Bitset.h"
#include <iostream>

int main()
{


	Bitset x(200);
	Bitset y(20);

	x.reset();
	y.reset();

	x.set(110);
	y.set(19);

	cout << "Bitset x:" << endl;
	cout << "Tamanho - " << x.size << endl;
	cout << "Bits - " << x.n_bits << endl;
	cout << "Valor - " << x.toString() << endl << endl;

	cout << "Bitset y:" << endl;
	cout << "Tamanho - " << y.size << endl;
	cout << "Bits - " << y.n_bits << endl;
	cout << "Valor - " << y.toString() << endl << endl;

	if (x < y)
		cout << "x < y" << endl << endl;
	else
		cout << "x >= y" << endl << endl;


	return 0;
}