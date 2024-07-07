#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
//-----------------------------------------------------Big Integer-------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------//

class BigInt {
private:
	std::vector<int> arr;
	int sz;
	bool sign;
public:
	//Constructors
	BigInt() = default;
	BigInt(const char num[]) {
		int i = 0;

		if (num[i] == '-') {
			sign = false;
			++i;
		}
		else sign = true;

		while (num[i] != '\0') {
			arr.push_back(int(num[i] - 48));
			++i;
		}
		sz = i;
		if (sign == false) --sz;
	}
	BigInt(std::string num) {
		int i = 0;

		if (num[i] == '-') {
			sign = false;
			++i;
		}
		else sign = true;

		while (i < num.size()) {
			arr.push_back(int(num[i] - 48));
			++i;
		}
		sz = i;
		if (sign == false) --sz;
	}
	BigInt(int num) {
		if (num < 0) {
			sign = false;
			num *= -1;
		}
		else sign = true;

		while (num / 10) {
			arr.push_back(num % 10);
			num /= 10;
		}
		arr.push_back(num);
		std::reverse(arr.begin(), arr.end());
		sz = arr.size();
	}
	BigInt(long long num) {
		if (num < 0) {
			sign = false;
			num *= -1;
		}
		else sign = true;

		while (num / 10) {
			arr.push_back(num % 10);
			num /= 10;
		}
		arr.push_back(num);
		std::reverse(arr.begin(), arr.end());
		sz = arr.size();
	}
	BigInt(bool num) {
		if (num == true) {
			arr.push_back(1);
			sign = true;
			sz = 1;
		}
		else {
			arr.push_back(0);
			sign = false;
			sz = 1;
		}
	}

	//Methods
	int Get_Quantity_Of_Numerals() {
		return sz;
	}
	bool Get_Sign() {
		return sign;
	}
	int getInt() {
		if (*this > 1000000000) throw std::overflow_error("too big number");
		int res = 0;
		BigInt copy = *this;
		for (int i = sz - 1, j = 1; i >= 0; --i) {
			res += arr[i] * j;
			j *= 10;
		}
		return res;
	}
	void Swap(BigInt& num) {
		std::swap(arr, num.arr);
		std::swap(sz, num.sz);
		std::swap(sign, num.sign);
	}
	void Print() {
		if (sign == false) std::cout << '-';
		for (int i = 0; i < sz; ++i) {
			std::cout << arr[i];
		}
	}
	void Clear() {
		arr.clear();
		sz = 0;
		sign = false;
	}
	std::string To_String() {
		std::string stroka;
		for (int i = 0; i < sz; ++i) {
			stroka.push_back(char(arr[i] + 48));
		}
		return stroka;
	}
	BigInt gcd(BigInt num) {
		BigInt wr = *this;
		while (wr > 0 && num > 0) {
			if (wr > num) wr %= num;
			else num %= wr;
		}
		wr += num;
		return wr;
	}

	//Assignment operator
	BigInt operator =(BigInt num) {
		Swap(num);
		return *this;
	}
	BigInt operator =(int num) {
		arr.clear();

		if (num < 0) {
			sign = false;
			num *= -1;
		}
		else sign = true;

		while (num / 10) {
			arr.push_back(num % 10);
			num /= 10;
		}
		arr.push_back(num);
		std::reverse(arr.begin(), arr.end());
		sz = arr.size();
		return *this;
	}

	//Addition and subtraction
	BigInt operator +=(const BigInt num) {
		BigInt Num_No_Const = num;
		BigInt res;
		BigInt work1, work2;

		//С разным знаком
		if (sign == true && Num_No_Const.sign == false) {
			Num_No_Const.sign = true;
			if (*this == Num_No_Const) {
				Num_No_Const.sign = false;
				*this = 0;
				return *this;
			}
			else {
				if (*this < Num_No_Const) {
					BigInt trasmit = *this;
					*this = Num_No_Const;
					Num_No_Const = trasmit;
					res.sign = false;
				}
				else res.sign = true;
				Num_No_Const.sign = false;
				work2 = *this;
				work1 = Num_No_Const;

				std::reverse(work1.arr.begin(), work1.arr.end());
				std::reverse(work2.arr.begin(), work2.arr.end());
				while (work1.sz != work2.sz) {
					work1.arr.push_back(0);
					++work1.sz;
				}

				int Raz_Mine = 0;
				for (int i = 0; i < sz; ++i) {
					int Work_In_For = work2.arr[i] - work1.arr[i] - Raz_Mine;
					if (Work_In_For < 0) {
						Raz_Mine = 1;
						Work_In_For += 10;
						res.arr.push_back(Work_In_For);
						++res.sz;
					}
					else {
						res.arr.push_back(Work_In_For);
						++res.sz;
						Raz_Mine = 0;
					}
				}
				for (int i = res.sz - 1; i >= 0 && res.sz != 1; --i) {
					if (res.arr[i] == 0) {
						res.arr.pop_back();
						--res.sz;
					}
					else break;
				}
				std::reverse(res.arr.begin(), res.arr.end());
				*this = res;
				return *this;
			}
		}
		if (sign == false && Num_No_Const.sign == true) {
			BigInt wr1 = Num_No_Const, wr2 = *this;
			wr1 += wr2;
			*this = wr1;
			return *this;
		}


		//С одинаковым знаком
		if (sz > Num_No_Const.sz) {
			work1 = Num_No_Const;
			work2 = *this;
		}
		else if (sz < Num_No_Const.sz) {
			work1 = *this;
			work2 = Num_No_Const;
		}
		else {
			work1 = Num_No_Const;
			work2 = *this;
		}

		std::reverse(work1.arr.begin(), work1.arr.end());
		std::reverse(work2.arr.begin(), work2.arr.end());
		while (work1.sz != work2.sz) {
			work1.arr.push_back(0);
			++work1.sz;
		}

		if (sign == true && Num_No_Const.sign == true) {
			res.sign = true;
			int Raz_More = 0;
			for (int i = 0; i < work1.sz; ++i) {
				int Work_in_For = work1.arr[i] + work2.arr[i] + Raz_More;
				if (Work_in_For > 9) {
					res.arr.push_back(Work_in_For % 10);
					++res.sz;
					Raz_More = Work_in_For / 10;
				}
				else {
					res.arr.push_back(Work_in_For);
					++res.sz;
					Raz_More = 0;
				}
			}
			if (Raz_More) {
				res.arr.push_back(Raz_More);
				++res.sz;
			}
			std::reverse(res.arr.begin(), res.arr.end());
			*this = res;
			return *this;
		}
		if (sign == false && Num_No_Const.sign == false) {
			res.sign = false;
			int Raz_More = 0;
			for (int i = 0; i < work1.sz; ++i) {
				int Work_in_For = work1.arr[i] + work2.arr[i] + Raz_More;
				if (Work_in_For > 9) {
					res.arr.push_back(Work_in_For % 10);
					++res.sz;
					Raz_More = Work_in_For / 10;
				}
				else {
					res.arr.push_back(Work_in_For);
					++res.sz;
					Raz_More = 0;
				}
			}
			if (Raz_More) {
				res.arr.push_back(Raz_More);
				++res.sz;
			}
			std::reverse(res.arr.begin(), res.arr.end());
			*this = res;
			return *this;
		}
	}
	BigInt operator -=(const BigInt num) {
		BigInt Num_No_Const = num;
		if (num.sign == false) {
			Num_No_Const.sign = true;
			return *this += Num_No_Const;
		}
		if (Num_No_Const.sign == true) {
			Num_No_Const.sign = false;
			return *this += Num_No_Const;
		}
	}

	//Increment
	BigInt operator ++() {
		return *this += 1;
	}
	BigInt operator ++(int) {
		BigInt Copy = *this;
		*this += 1;
		return Copy;
	}

	//Decrement
	BigInt operator --() {
		return *this += -1;
	}
	BigInt operator --(int) {
		BigInt Copy = *this;
		*this -= 1;
		return Copy;
	}

	//Multiplication
	BigInt operator *=(const BigInt num) {
		BigInt Num_No_Const = num;
		if (Num_No_Const == 0) {
			*this = 0;
			return *this;
		}

		BigInt res, work1, work2;

		if ((sz == 1 && arr[0] == 0) || (Num_No_Const.sz == 1 && Num_No_Const.arr[0] == 0)) {
			return res;
		}

		std::reverse(Num_No_Const.arr.begin(), Num_No_Const.arr.end());
		std::reverse(arr.begin(), arr.end());

		while (arr.size() > Num_No_Const.arr.size()) Num_No_Const.arr.push_back(0);
		while (arr.size() < Num_No_Const.arr.size()) arr.push_back(0);
		sz = arr.size();
		Num_No_Const.sz = Num_No_Const.arr.size();

		int Raz_More = 0;
		for (int i = 0; i < Num_No_Const.sz; ++i) { //Идём по разрядам первого множителя
			BigInt work;
			bool f = 0;
			for (int j = 0; j < sz; ++j) { //Идём по разрядам второго множителя

				if (!f)
					for (int t = 0; t < i; ++t) { //Добавляем необходимое количество нулей
						work.arr.push_back(0);
						++work.sz;
						f = 1;
					}
				int Work_in_For = Num_No_Const.arr[i] * arr[j] + Raz_More;
				if (Work_in_For > 9) {
					work.arr.push_back(Work_in_For % 10);
					++work.sz;
					Raz_More = Work_in_For / 10;
				}
				else {
					work.arr.push_back(Work_in_For);
					++work.sz;
					Raz_More = 0;
				}
			}
			if (Raz_More) {
				work.arr.push_back(Raz_More);
				++work.sz;
			}
			Raz_More = 0;
			std::reverse(work.arr.begin(), work.arr.end());
			res += work;

		}

		if (sign != Num_No_Const.sign) res.sign = false;
		else res.sign = true;

		res.Delete_First_Zeros();
		*this = res;
		return *this;
	}

	//Integer division
	BigInt operator /=(const BigInt num) {
		BigInt buffer;
		buffer.sign = true;
		BigInt result;
		BigInt Num_Modul = num;
		Num_Modul.sign = true;

		result.arr.clear();
		buffer.arr.clear();

		if (num.arr[0] == 0) throw std::overflow_error("Divide by zero exception");

		for (int i = 0; i < arr.size(); ++i) {
			buffer.arr.push_back(arr[i]);
			++buffer.sz;
			buffer.Delete_First_Zeros();

			if (buffer < Num_Modul) {
				result.arr.push_back(0);
				++result.sz;
				continue;
			}

			int count;

			for (count = 0; buffer >= Num_Modul; ++count) {
				buffer -= Num_Modul;
			}

			result.arr.push_back(count);
			++result.sz;
		}

		result.Delete_First_Zeros();
		result.sign = true;

		if (sign != num.sign && result.arr[0] != 0) {
			result.sign = false;
			--result;
		}
		*this = result;
		return *this;
	}

	//The remainder is modulo
	BigInt operator %=(const BigInt num) {
		BigInt x = *this;
		x /= num;
		x *= num;
		*this -= x;
		return *this;
	}

	//Bool operators
	bool operator<(const BigInt& num) const {
		if (!sign && num.sign) return true;
		if (sign && !num.sign) return false;

		if (sign && num.sign) {
			if (sz < num.sz) return true;
			if (sz > num.sz) return false;

			for (int i = 0; i < sz; ++i) {
				if (arr[i] < num.arr[i]) return true;
				if (arr[i] > num.arr[i]) return false;
			}
			return false;
		}

		if (!sign && !num.sign) {
			if (sz < num.sz) return false;
			if (sz > num.sz) return true;

			for (int i = 0; i < sz; ++i) {
				if (arr[i] < num.arr[i]) return false;
				if (arr[i] > num.arr[i]) return true;
			}
			return true;
		}

	}
	bool operator==(const BigInt& num) const {
		return !(*this < num) && !(num < *this);
	}
	bool operator!=(const BigInt& num) const {
		return (*this < num) || (num < *this);
	}
	bool operator<=(const BigInt& num) const {
		return (*this < num || *this == num);
	}
	bool operator>(const BigInt& num) const {
		return !(*this <= num);
	}
	bool operator>=(const BigInt& num) const {
		return !(*this < num);
	}

	//Friends
	friend std::istream& operator >>(std::istream& in, BigInt& num);
	friend std::ostream& operator <<(std::ostream& out, const BigInt& num);

	//Distructors
	~BigInt() {};

	//Private methods
private:
	void Delete_First_Zeros() {
		while (arr[0] == 0 && arr.size() > 1) {
			arr.erase(arr.begin(), arr.begin() + 1);
			--sz;
		}
	}
};

//Operators >> and <<
std::istream& operator >>(std::istream& in, BigInt& num) {
	num.Clear();
	num.sign = true;

	while (in) {
		char x;
		x = in.get();

		if (x == '-') {
			num.sign = false;
			continue;
		}
		if (x == ' ' || x == '\n') {
			break;
		}
		else {
			num.arr.push_back(x - 48);
			++num.sz;
		}
	}

	return in;
}
std::ostream& operator <<(std::ostream& out, const BigInt& num) {
	if (num.sign == false) std::cout << '-';
	for (int i = 0; i < num.sz; ++i) {
		std::cout << num.arr[i];
	}

	return out;
}

//Operators +, -, *, /, %
BigInt operator +(BigInt x, BigInt num) {
	return num += x;
}
BigInt operator -(BigInt x, BigInt num) {
	return x -= num;
}
BigInt operator *(BigInt x, BigInt num) {
	return x *= num;
}
BigInt operator /(BigInt x, BigInt num) {
	return x /= num;
}
BigInt operator %(BigInt x, BigInt num) {
	return x %= num;
}