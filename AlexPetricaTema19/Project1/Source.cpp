#include<iostream>
#include<vector>
#include<string>
#include<cctype>

using namespace std;

class BigInt {
public:
	vector<char> number;
	bool isNegative;

	BigInt() {
		//this->number.push_back('0');
		this->isNegative = false;
	}

	BigInt(int number) {
		this->isNegative = false;
		char value_char = char('0' + number);
		this->number.push_back(value_char);
	}

	BigInt(const BigInt& obj) {
		this->isNegative = obj.isNegative;
		this->number = obj.number;
	}

	BigInt & operator=(const BigInt& o) {
		this->number.clear();
		this->isNegative = o.isNegative;
		for (int i = 0; i < o.number.size(); i++) {
			this->number.push_back(o.number.at(i));
		}

		return *this;

		/*BigInt result = BigInt(o);

		return result;*/
	}

	BigInt& operator++() {
		bool increment = true;
		for (int i = 0; i < this->number.size(); i++) {
			int value = ((int)this->number.at(i) - 48);
			if (increment) {
				if (value == 9) {
					value = 0;
				}
				else {
					value++;
					increment = false;
				}
			}

			char value_char = char('0' + value);

			this->number.at(i) = value_char;
		}

		if (increment) this->number.push_back('1');
		//BigInt result(*this);
		return *this;
	}

	friend class BigRat;

	friend BigInt cmmdc(const BigInt a, const BigInt b) {
		BigInt *x = new BigInt(a);
		BigInt *y = new BigInt(b);

		x->isNegative = false;
		y->isNegative = false;

		BigInt *temp;
		BigInt zero(0);

		while (*y > zero) {
			temp = new BigInt(*x % *y);
			x = y;
			y = temp;
			//cout << "x: " << *x << " y: " << *y << " temp: " << *temp << endl;
		}

		return *x;
	}

	friend bool operator>(const BigInt& a, const BigInt& b) {

		bool equal = false;
		if (a.number.size() == b.number.size()) {
			for (int i = a.number.size() - 1; i >= 0; i--)
			{
				if (a.number.at(i) < b.number.at(i)) return false;
				else if (a.number.at(i) > b.number.at(i)) return true;
			}
			equal = true;
		}
		else if (a.number.size() > b.number.size()) return true;
		else return false;

		if (equal) return false;
		return true;
	}

	friend bool operator>=(const BigInt& a, const BigInt& b) {

		if (a.number.size() == b.number.size()) {
			for (int i = a.number.size() - 1; i >= 0; i--)
			{
				if (a.number.at(i) < b.number.at(i)) return false;
				else if (a.number.at(i) > b.number.at(i)) return true;
			}
		}
		else if (a.number.size() > b.number.size()) return true;
		else return false;

		return true;
	}

	friend BigInt operator-(const BigInt& a, const BigInt& b) {
		BigInt *result = new BigInt();

		BigInt *temp1 = new BigInt(a);
		BigInt *temp2 = new BigInt(b);

		if (a.isNegative && !b.isNegative) {
			temp2->isNegative = true;
			result = new BigInt(*temp1 + *temp2);
			return *result;
		}
		else if (!a.isNegative && b.isNegative) {
			temp2->isNegative = false;
			result = new BigInt(*temp1 + *temp2);
			return *result;
		}

		if (a >= b) {
			temp1 = new BigInt(a);
			temp2 = new BigInt(b);
			if (a.isNegative && b.isNegative) {
				result->isNegative = true;
			}
			else result->isNegative = false;
		}
		else {
			temp1 = new BigInt(b);
			temp2 = new BigInt(a);
			if (a.isNegative && b.isNegative) {
				result->isNegative = false;
			}
			else result->isNegative = true;
		}
		bool over = false;
		for (int i = 0; i < temp1->number.size(); i++) {

			int a = (int)temp1->number.at(i) - 48;
			int b = (i<temp2->number.size()) ? ((int)temp2->number.at(i) - 48) : (0);

			if (over) a--;
			if (a < b) {
				a += 10;
				over = true;
			}
			else over = false;

			int sum = a - b;

			char sum_char = char('0' + sum);

			result->number.push_back(sum_char);

		}

		while (result->number.back() == '0' && result->number.size()>1) {
			result->number.pop_back();
		}

		if (result->number.size() == 1 && result->number.at(0) == '0') result->isNegative = false;

		return *result;
	}

	friend BigInt operator*(const BigInt& a, const BigInt& b) {
		//BigInt *result=new BigInt();

		BigInt *result = new BigInt(0);

		int state = 1;
		for (int i = 0; i < b.number.size(); i++) {
			int limit = ((int)b.number.at(i) - 48);
			if (limit != 0) {
				int length = state * limit;
				for (int j = 0; j < length; j++) {
					result = new BigInt(*result + a);
				}
			}


			state *= 10;
		}

		if ((a.isNegative == b.isNegative) || (result->number.size() == 1 && result->number.at(0) == '0')) result->isNegative = false;
		else result->isNegative = true;

		return *result;
	}

	friend BigInt operator%(const BigInt& a, const BigInt& b) {
		if (b.number.size() == 1 && b.number.at(0) == '0') throw exception("Cannot divide by 0");

		BigInt *result = new BigInt(a);
		BigInt *temp = new BigInt(b);
		temp->isNegative = false;
		result->isNegative = false;

		while (*result >= b) {
			result = new BigInt(*result - *temp);
		}

		//if ((a.isNegative == b.isNegative) || (result->number.size() == 1 && result->number.at(0) == '0')) result->isNegative = false;
		//else result->isNegative = true;

		return *result;
	}

	friend BigInt operator/(const BigInt& a, const BigInt& b) {
		//BigInt *result=new BigInt();
		if (b.number.size() == 1 && b.number.at(0) == '0') throw exception("Cannot divide by 0");

		BigInt *result = new BigInt(a);
		BigInt *temp = new BigInt(b);
		temp->isNegative = false;
		result->isNegative = false;

		BigInt count(0);
		while (*result >= b) {
			result = new BigInt(*result - *temp);
			++count;
		}

		if ((a.isNegative == b.isNegative) || (count.number.size() == 1 && count.number.at(0) == '0')) count.isNegative = false;
		else count.isNegative = true;

		return count;
	}

	friend BigInt operator+(const BigInt& a, const BigInt& b) {
		BigInt *result = new BigInt();

		BigInt *temp1 = new BigInt(a);
		BigInt *temp2 = new BigInt(b);

		if (a.isNegative && b.isNegative) result->isNegative = true;
		else if (a.isNegative && !b.isNegative) {
			temp2->isNegative = true;
			result = new BigInt(*temp1 - *temp2);
			return *result;
		}
		else if (!a.isNegative && b.isNegative) {
			temp2->isNegative = false;
			result = new BigInt(*temp1 - *temp2);
			return *result;
		}


		int length = 0;
		if (a.number.size() > b.number.size()) length = a.number.size();
		else length = b.number.size();

		bool over = false;
		for (int i = 0; i < length || over; i++) {

			int sum = 0;

			int x = (i < a.number.size()) ? ((int)a.number.at(i) - 48) : (0);
			int y = (i < b.number.size()) ? ((int)b.number.at(i) - 48) : (0);

			sum = x + y;

			if (over) sum++;
			if (sum >= 10) {
				over = true;
				sum = sum % 10;
			}
			else over = false;

			char sum_char = char('0' + sum);

			result->number.push_back(sum_char);

		}

		if (result->number.size() == 1 && result->number.at(0) == '0') result->isNegative = false;

		return *result;
	}

	friend ostream &operator<<(ostream & out, BigInt& o)
	{
		if (o.isNegative) out << '-';
		for (int i = o.number.size() - 1; i >= 0; i--) {
			out << o.number.at(i);
		}
		return out;
	}

	friend istream &operator>>(istream & in, BigInt& o)
	{
		string raw;
		in >> raw;

		for (int i = raw.length() - 1; i >= 0; i--) {
			char c = raw[i];
			if (i == 0 && (c == '+' || c == '-')) {
				if (c == '+') o.isNegative = false;
				else if (c == '-') o.isNegative = true;
			}
			else if (isdigit(c)) {
				o.number.push_back(c);
			}
			else {
				o.number.clear();
				throw exception("Invalid number!");
				break;
			};

		}

		return in;
	}
};

class BigRat {
public:
	BigInt numarator;
	BigInt numitor;

	BigRat() {
		//constructor
	}

	BigRat & operator=(const BigRat& o) {
		this->numarator = o.numarator;
		this->numitor = o.numitor;

		return *this;

	}

	BigRat(const BigRat& obj) {
		this->numarator = obj.numarator;
		this->numitor = obj.numitor;
	}

	friend ostream &operator<<(ostream & out, BigRat& o)
	{
		out << o.numarator << "/" << o.numitor ;

		return out;
	}

	friend istream &operator>>(istream & in, BigRat& o)
	{
		cout << "numarator: ";
		in >> o.numarator;
		cout << "numitor: ";
		in >> o.numitor;

		return in;
	}

	friend BigRat operator+(const BigRat& a, const BigRat& b) {
		BigRat *result = new BigRat();

		BigRat *temp1 = new BigRat(a);
		BigRat *temp2 = new BigRat(b);

		temp1->numarator = temp1->numarator * b.numitor;
		temp1->numitor = temp1->numitor * b.numitor;
		temp2->numarator = temp2->numarator * a.numitor;
		temp2->numitor = temp2->numitor * a.numitor;

		result->numarator = temp1->numarator + temp2->numarator;
		result->numitor = temp1->numitor;

		BigInt cmd = cmmdc(result->numarator, result->numitor);

		result->numarator = result->numarator / cmd;
		result->numitor = result->numitor / cmd;

		return *result;
	}

	friend BigRat operator-(const BigRat& a, const BigRat& b) {
		BigRat *result = new BigRat();

		BigRat *temp1 = new BigRat(a);
		BigRat *temp2 = new BigRat(b);

		temp1->numarator = temp1->numarator * b.numitor;
		temp1->numitor = temp1->numitor * b.numitor;
		temp2->numarator = temp2->numarator * a.numitor;
		temp2->numitor = temp2->numitor * a.numitor;

		result->numarator = temp1->numarator - temp2->numarator;
		result->numitor = temp1->numitor;

		BigInt cmd = cmmdc(result->numarator, result->numitor);

		result->numarator = result->numarator / cmd;
		result->numitor = result->numitor / cmd;

		return *result;
	}

	friend BigRat operator*(const BigRat& a, const BigRat& b) {
		BigRat *result = new BigRat();



		result->numarator = a.numarator * b.numarator;
		result->numitor = a.numitor * b.numitor;

		BigInt cmd = cmmdc(result->numarator, result->numitor);

		result->numarator = result->numarator / cmd;
		result->numitor = result->numitor / cmd;

		return *result;
	}
	friend BigRat operator/(const BigRat& a, const BigRat& b) {
		BigRat *result = new BigRat();

		result->numarator = a.numarator * b.numitor;
		result->numitor = a.numitor * b.numarator;

		BigInt cmd = cmmdc(result->numarator, result->numitor);

		result->numarator = result->numarator / cmd;
		result->numitor = result->numitor / cmd;

		return *result;
	}
};



int main()
{
	

		BigInt test;
		BigInt test2;





		try {
			char selection;
			cout << "  ----------Tema 19 --------------\n\n";
			do
			{


				cout << "  1.  Operatii cu numere intregi mari \n";
				cout << "  2.  Operatii cu numere rationale mari \n";
				cout << "  0.  ---Exit---\n";
				cout << endl;
				cout << "  Alegeti o optiune de calcul: ";
				cin >> selection;
				cout << endl;

				switch (selection)
				{
				case '1':
					try {
						char selection;
						cout << "  ----------Tema 19 --------------\n\n";
						do
						{
							cout << "Introduceti primul numar: ";
							cin >> test;
							cout << endl;
							cout << "Introduceti al 2-lea numar: ";
							cin >> test2;
							cout << endl;


							cout << "  1.  Adunare \n";
							cout << "  2.  Scadere \n";
							cout << "  3.  Inmultire \n";
							cout << "  4.  Restul impartirii \n";
							cout << "  5.  Impartirea \n";
							cout << "  6.  CMMDC \n";
							cout << "  7. Operatii cu numere rationale\n ";
							cout << "  0.  ---Exit---\n";
							cout << endl;
							cout << "  Alegeti o optiune de calcul: ";
							cin >> selection;
							cout << endl;

							switch (selection)
							{
							case '1':
								try {
									BigInt result1 = test + test2;
									BigInt result2 = test2 + test;

									cout << test << " + " << test2 << " = " << result1 << endl;
									cout << test2 << " + " << test << " = " << result2 << endl;

									cout << "\n";
								}
								catch (const exception& e) {
									cerr << "exception: " << e.what() << endl;
								}

								break;

							case '2':
								try {
									BigInt result3 = test - test2;
									BigInt result4 = test2 - test;

									cout << test << " - " << test2 << " = " << result3 << endl;
									cout << test2 << " - " << test << " = " << result4 << endl;

									cout << endl;
								}
								catch (const exception& e) {
									cerr << "exception: " << e.what() << endl;
								}
								break;
							case '3':
								try {

									BigInt result9 = test * test2;

									cout << test << " * " << test2 << " = " << result9 << endl;
									cout << endl;

								}
								catch (const exception& e) {
									cerr << "exception: " << e.what() << endl;
								}
								break;

							case '4':
								try {

									BigInt result6 = test % test2;
									BigInt result10 = test2 % test;
									cout << test << " % " << test2 << " = " << result6 << endl;
									cout << test2 << " % " << test << " = " << result10 << endl;
									cout << endl;
								}
								catch (const exception& e) {
									cerr << "exception: " << e.what() << endl;
								}
								break;
							case '5':
								try {




									BigInt result7 = test / test2;
									BigInt result11 = test2 / test;
									cout << test << " / " << test2 << " = " << result7 << " rest ";
									BigInt result6 = test % test2;
									cout << result6 << endl;
									cout << test2 << " / " << test << " = " << result11 << " rest ";
									BigInt result10 = test2 % test;
									cout << result10 << endl;
									cout << endl;
								}
								catch (const exception& e) {
									cerr << "exception: " << e.what() << endl;
								}
								break;

							case '6':
								try {


									BigInt result8 = cmmdc(test, test2);

									cout << "cmmdc(" << test << ", " << test2 << ") = " << result8 << endl;
									cout << endl;
								}
								catch (const exception& e) {
									cerr << "exception: " << e.what() << endl;
								}
								break;


							case '7':
								try {
									char selection;
									cout << " Introduceti numerele:\n\n";
									cout << "Introduceti primul numar:\n ";
									BigRat test3;
									cin >> test3;
									cout << endl;

									cout << "Introduceti al doilea  numar:\n ";
									BigRat test4;
									cin >> test4;
									cout << endl;

									do
									{



										cout << "  1.  -Adunare- \n";
										cout << "  2.  -Scadere- \n";
										cout << "  3.  -Inmultire- \n";
										cout << "  4.  -Impartirea- \n";
										cout << "  5.  -Fractie Ireductibila- \n";
										cout << "  0.  ---Exit---\n";
										cout << endl;
										cout << "  Alegeti o optiune de calcul: ";
										cin >> selection;
										cout << endl;

										switch (selection)
										{
										case '1':
											try {
												BigRat resultrat = test3 + test4;
												cout << test3 << " + " << test4 << " = " << resultrat << endl;


											}
											catch (const exception& e) {
												cerr << "exception: " << e.what() << endl;
											}

											break;

										case '2':
											try {
												BigRat resultrat2 = test3 - test4;
												cout << test3 << " - " << test4 << " = " << resultrat2 << endl;

											}
											catch (const exception& e) {
												cerr << "exception: " << e.what() << endl;
											}
											break;
										case '3':
											try {
												BigRat resultrat3 = test3 * test4;
												cout << test3 << " * " << test4 << " = " << resultrat3 << endl;

											}
											catch (const exception& e) {
												cerr << "exception: " << e.what() << endl;
											}
											break;

										case '4':
											try {
												BigRat resultrat4 = test3 / test4;
												cout << test3 << " / " << test4 << " = " << resultrat4 << endl;


											}
											catch (const exception& e) {
												cerr << "exception: " << e.what() << endl;
											}
											break;
										case '5':
											try {

												BigInt cmmdc1 = cmmdc(test3.numarator, test3.numitor);
												BigRat test5;
												test5.numarator = test3.numarator / cmmdc1;
												test5.numitor = test3.numitor / cmmdc1;
												cout << "fractie ireductibila pentru numarul 1 : ";
												cout << test5 << endl;

												BigInt cmmdc2 = cmmdc(test4.numarator, test4.numitor);
												BigRat test6;
												test6.numarator = test4.numarator / cmmdc1;
												test6.numitor = test4.numitor / cmmdc1;
												cout << "fractie ireductibila pentru numarul 2 : ";
												cout << test6 << endl;
												cout << endl;



											}
											catch (const exception& e) {
												cerr << "exception: " << e.what() << endl;
											}
											break;







										case '0':
											cout << "Goodbye.\n";
											break;

										default:
											
											cout <<"Optiunea " <<selection << " nu exista! \n";

											cout << endl;
										}

									} while (selection != 0);

								}
								catch (const exception& e) {
									cerr << "exception: " << e.what() << endl;
								}
								break;



							case '0':
								cout << "Goodbye.\n";
								break;

							default: cout <<"Optiunea " <<selection << " nu exista! .\n";

								cout << endl;
							}

						} while (selection != 0);
					}
					catch (const exception& e) {
						cerr << "exception: " << e.what() << endl;
					}

					break;

				case '2':
					try {

						try {

							char selection;
							cout << " Introduceti numerele:\n\n";
							cout << "Introduceti primul numar:\n ";
							BigRat test3;
							cin >> test3;
							cout << endl;

							cout << "Introduceti al doilea  numar:\n ";
							BigRat test4;
							cin >> test4;
							cout << endl;

							do
							{



								cout << "  1.  -Adunare- \n";
								cout << "  2.  -Scadere- \n";
								cout << "  3.  -Inmultire- \n";
								cout << "  4.  -Impartirea- \n";
								cout << "  5.  -Fractie Ireductibila- \n";
								cout << "  0.  ---Exit---\n";
								cout << endl;
								cout << "  Alegeti o optiune de calcul: ";
								cin >> selection;
								cout << endl;

								switch (selection)
								{
								case '1':
									try {
										BigRat resultrat = test3 + test4;
										cout << test3 << " + " << test4 << " = " << resultrat << endl;


									}
									catch (const exception& e) {
										cerr << "exception: " << e.what() << endl;
									}

									break;

								case '2':
									try {
										BigRat resultrat2 = test3 - test4;
										cout << test3 << " - " << test4 << " = " << resultrat2 << endl;

									}
									catch (const exception& e) {
										cerr << "exception: " << e.what() << endl;
									}
									break;
								case '3':
									try {
										BigRat resultrat3 = test3 * test4;
										cout << test3 << " * " << test4 << " = " << resultrat3 << endl;

									}
									catch (const exception& e) {
										cerr << "exception: " << e.what() << endl;
									}
									break;

								case '4':
									try {
										BigRat resultrat4 = test3 / test4;
										cout << test3 << " / " << test4 << " = " << resultrat4 << endl;


									}
									catch (const exception& e) {
										cerr << "exception: " << e.what() << endl;
									}
									break;
								case '5':
									try {

										BigInt cmmdc1 = cmmdc(test3.numarator, test3.numitor);
										BigRat test5;
										test5.numarator = test3.numarator / cmmdc1;
										test5.numitor = test3.numitor / cmmdc1;
										cout << "fractie ireductibila pentru numarul 1 : ";
										cout << test5 << endl;

										BigInt cmmdc2 = cmmdc(test4.numarator, test4.numitor);
										BigRat test6;
										test6.numarator = test4.numarator / cmmdc1;
										test6.numitor = test4.numitor / cmmdc1;
										cout << "fractie ireductibila pentru numarul 2 : ";
										cout << test6 << endl;
										cout << endl;



									}
									catch (const exception& e) {
										cerr << "exception: " << e.what() << endl;
									}
									break;







								case '0':
									cout << "Goodbye.\n";
									break;

								default: cout <<"Optiunea " <<selection << " nu exista! .\n";

									cout << endl;
								}

							} while (selection != 0);

						}
						catch (const exception& e) {
							cerr << "exception: " << e.what() << endl;
						}
						break;
						cout << endl;
					}
					catch (const exception& e) {
						cerr << "exception: " << e.what() << endl;
					}
					break;


				case '0':
					cout << "Goodbye.\n";
					break;

				default: cout <<"Optiunea " <<selection << " nu exista! .\n";

					cout << endl;
				}

			} while (selection != 0);
		}
		catch (const exception& e) {
			cerr << "exception: " << e.what() << endl;
		}


	cout << endl;
	system("pause");
	return 0;
}

