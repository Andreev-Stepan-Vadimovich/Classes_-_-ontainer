#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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

//--------------------------------------------------------Big Rational---------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------//

class BigRational {
private: //Fields
	BigInt Chisl;
	BigInt Znam = 1;
	bool sign;
public:
	//Constructors
	BigRational() = default;
	BigRational(int num) {
		if (num > 0) sign = true;
		else sign = false;
		if (num < 0) num *= -1;
		Chisl = num;
		Znam = 1;

	}
	BigRational(BigInt num) {
		Chisl = num;
		if (Chisl <= 0) Chisl *= -1;
		Znam = 1;
		sign = num.Get_Sign();
	}
	BigRational(std::string num) {
		std::string s1, s2;
		int i = 0;
		if (num[0] == '-') {
			sign = false;
			++i;
		}
		else sign = true;
		while (num[i] != '.') {
			s1.push_back(num[i]);
			++i;
		}
		++i;
		BigInt C1 = s1;
		while (num[i] != '\0' && i < num.size()) {
			s2.push_back(num[i]);
			++i;
		}
		for (int j = 0; j < s2.size(); ++j) Znam *= 10;
		Chisl = s2;
		Chisl += s1 * Znam;
	}

	//Methods
	std::string To_String() const {
		std::string ans;
		BigRational ThisCopy = *this;
		if (*this < 0) ans += '-';
		ans += ThisCopy.Chisl.To_String();
		if (ThisCopy.Znam == 1) return ans;
		ans += '/';
		ans += ThisCopy.Znam.To_String();
		return ans;
	}
	bool Get_Sign() {
		return sign;
	}
	void Clear() {
		Chisl = 0;
		Znam = 1;
		sign = false;
	}
	void Change_Znam(int x) {
		if (x == 0) throw std::overflow_error("Divide by zero exception");
		Znam = x;
	}
	std::string asDesimal(BigInt Precision) {
		std::string wr;
		wr = To_a_Decimal_Fraction(*this, Precision);
		return wr;
	}

	//Operators
	BigRational operator +=(const BigRational num) {
		//Bringing to a common denominator
		BigRational ThisCopy = *this, NumCopy = num;
		BigInt Common_Denominator = ThisCopy.Find_a_Common_Denominator(NumCopy);
		ThisCopy.Chisl *= (Common_Denominator / ThisCopy.Znam);
		NumCopy.Chisl *= (Common_Denominator / NumCopy.Znam);
		ThisCopy.Znam = Common_Denominator;
		NumCopy.Znam = Common_Denominator;

		//Computations
		BigRational ans;
		ans.Znam = 1;
		if (ThisCopy.sign == true && NumCopy.sign == true) {
			ans.Znam = Common_Denominator;
			ans.Chisl = ThisCopy.Chisl + NumCopy.Chisl;
			ans.sign = true;
			ans.Simplifying_the_Number();
			*this = ans;
			return *this;
		}
		else if (ThisCopy.sign == false && NumCopy.sign == false) {
			ans.Znam = Common_Denominator;
			ans.Chisl = ThisCopy.Chisl + NumCopy.Chisl;
			ans.sign = false;
			ans.Simplifying_the_Number();
			*this = ans;
			return *this;
		}

		if (ThisCopy.Chisl == NumCopy.Chisl) {
			ans.Chisl = 0;
			ans.Znam = Common_Denominator;
			ans.sign = false;
			ans.Simplifying_the_Number();
			*this = ans;
			return *this;
		}

		else if (ThisCopy.sign == true && NumCopy.sign == false) {
			ans.Znam = Common_Denominator;
			if (ThisCopy.Chisl > NumCopy.Chisl) {
				ans.Chisl = ThisCopy.Chisl - NumCopy.Chisl;
				ans.sign = true;
				ans.Simplifying_the_Number();
				*this = ans;
				return *this;
			}
			if (ThisCopy.Chisl < NumCopy.Chisl) {
				ans.Chisl = NumCopy.Chisl - ThisCopy.Chisl;
				ans.sign = false;
				ans.Simplifying_the_Number();
				*this = ans;
				return *this;
			}
		}
		else if (ThisCopy.sign == false && NumCopy.sign == true) {
			ans.Znam = Common_Denominator;
			if (ThisCopy.Chisl > NumCopy.Chisl) {
				ans.Chisl = ThisCopy.Chisl - NumCopy.Chisl;
				ans.sign = false;
				ans.Simplifying_the_Number();
				*this = ans;
				return *this;
			}
			if (ThisCopy.Chisl < NumCopy.Chisl) {
				ans.Chisl = NumCopy.Chisl - ThisCopy.Chisl;
				ans.sign = true;
				ans.Simplifying_the_Number();
				*this = ans;
				return *this;
			}
		}

	}
	BigRational operator -=(const BigRational num) {
		BigRational wr = num;
		if (num.sign == true) {
			wr.sign = false;
			return *this += wr;
		}
		else {
			wr.sign = true;
			return *this += wr;
		}
	}
	BigRational operator *=(const BigRational num) {
		BigRational ans;
		if (sign != num.sign) ans.sign = false;
		else ans.sign = true;

		ans.Chisl = Chisl * num.Chisl;
		ans.Znam = Znam * num.Znam;
		ans.Simplifying_the_Number();
		*this = ans;
		return *this;
	}
	BigRational operator /=(const BigRational num) {
		if (num.Chisl == 0) throw std::overflow_error("Divide by zero exception");
		BigRational ans;

		if (sign != num.sign) ans.sign = false;
		else ans.sign = true;

		ans.Chisl = Chisl * num.Znam;
		ans.Znam = Znam * num.Chisl;
		ans.Simplifying_the_Number();
		*this = ans;
		return *this;
	}

	operator double() {
		std::string s = To_a_Decimal_Fraction(*this, 16);
		double ans = std::stod(s);
		return ans;
	}

	//Bool operators
	bool operator <(const BigRational& num) const {
		//Bringing to a common denominator
		BigRational ThisCopy = *this, NumCopy = num;

		if (sign == false && num.sign == true) return true;
		else if (sign == true && num.sign == false) return false;

		BigInt Common_Denominator = ThisCopy.Find_a_Common_Denominator(NumCopy);
		ThisCopy.Chisl *= (Common_Denominator / ThisCopy.Znam);
		NumCopy.Chisl *= (Common_Denominator / NumCopy.Znam);
		ThisCopy.Znam = Common_Denominator;
		NumCopy.Znam = Common_Denominator;

		if (sign == true)
			if (ThisCopy.Chisl < NumCopy.Chisl) return true;
			else return false;
		else
			if (ThisCopy.Chisl > NumCopy.Chisl) return true;
			else return false;
	}
	bool operator ==(const BigRational& num) const {
		return !(*this < num) && !(num < *this);
	}
	bool operator !=(const BigRational& num) const {
		return (*this < num || num < *this);
	}
	bool operator <=(const BigRational& num) const {
		return (*this < num || *this == num);
	}
	bool operator >(const BigRational& num) const {
		return !(*this <= num);
	}
	bool operator >=(const BigRational& num) const {
		return !(*this < num);
	}

	//Friends
	friend std::ostream& operator <<(std::ostream& out, const BigRational& num);
	friend std::istream& operator >>(std::istream& in, BigRational& num);

private: //Methods
	BigInt Find_a_Common_Denominator(BigRational num) {
		BigInt nod = Znam.gcd(num.Znam);
		BigInt nok = (Znam * num.Znam) / nod;
		return nok;
	}
	void Simplifying_the_Number() {
		while (Chisl.gcd(Znam) > 1) {
			BigInt Divdier = Chisl.gcd(Znam);
			Chisl /= Divdier;
			Znam /= Divdier;
		}
	}
	std::string To_a_Decimal_Fraction(BigRational num, BigInt Precision) {
		BigInt Zel_Part, Drob_Part;
		Zel_Part = num.Chisl / num.Znam;
		num.Chisl -= Zel_Part * num.Znam;

		BigInt x, i = 0;
		std::string Drob;
		while (i <= Precision) {
			if (num.Chisl < num.Znam) {
				num.Chisl *= 10;
			}
			x = num.Chisl / num.Znam;
			num.Chisl -= x * num.Znam;
			Drob += x.To_String();
			++i;
		}
		Drob_Part = Drob.substr(0, Drob.size() - 1);
		if (Drob[Drob.size() - 1] - 48 >= 5) {
			Drob_Part += 1;
			if (Drob_Part.Get_Quantity_Of_Numerals() == Drob.size()) {
				Zel_Part += 1;
				Drob_Part = Drob_Part.To_String().substr(1, Drob_Part.To_String().size());
			}
		}
		std::string ans;
		if (num.sign == false) ans.push_back('-');
		ans += Zel_Part.To_String() + "." + Drob_Part.To_String();
		return ans;
	}
};

//Operators <<, >>
std::ostream& operator <<(std::ostream& out, const BigRational& num) {
	if (num.Chisl == 0) {
		std::cout << 0 << '\n';
		return out;
	}
	if (num.sign == false) std::cout << '-';
	std::cout << num.Chisl;
	BigRational num_Copy = num;
	if (num.Znam != 1 && num_Copy.Chisl.Get_Quantity_Of_Numerals() < 120) std::cout << '/' << num.Znam;
	else if (num.Znam != 1) {
		std::cout << '\n';
		for (int i = 0; i < 120; ++i) std::cout << '-';
		std::cout << '\n' << num.Znam;
	}
	return out;
}
std::istream& operator >>(std::istream& in, BigRational& num) {
	std::cout << "Insert BigRational-Number like this: [minus]numerator/denominator  OR [minus]XXXX.XXXXX... :\n";
	std::string s;
	std::cin >> s;
	bool Flad_minus = false;
	if (s[0] == '-') {
		Flad_minus = true;
		num.sign = false;
	}
	else num.sign = true;

	std::string Chisl, Znam;
	int i = 0;
	int flag_period = 0;
	if (Flad_minus) ++i;
	for (; i < s.size(); ++i) {
		if (s[i] == '/') {
			flag_period = 1;
			++i;
			break;
		}
		if (s[i] == '.') {
			flag_period = 2;
			++i;
			break;
		}
		Chisl.push_back(s[i]);
	}
	if (flag_period == 1 || flag_period == 0) {
		num.Chisl = Chisl;
		if (i == s.size()) {
			num.Znam = 1;
			return in;
		}
		for (; i < s.size(); ++i) {
			Znam.push_back(s[i]);
		}
		if (Znam[0] == '0') throw std::overflow_error("Divide by zero exception");
		num.Znam = Znam;
		num.Simplifying_the_Number();
	}
	if (flag_period == 2) {
		Chisl.push_back('.');
		for (; i < s.size(); ++i) {
			Chisl.push_back(s[i]);
		}
		num = Chisl;
		if (Flad_minus == true) num.sign = false;
	}

	return in;
}

//Operators +, -, *, /, %
BigRational operator +(BigRational x, BigRational num) {
	return x += num;
}
BigRational operator -(BigRational x, BigRational num) {
	return x -= num;
}
BigRational operator *(BigRational x, BigRational num) {
	return x *= num;
}
BigRational operator /(BigRational x, BigRational num) {
	return x /= num;
}

//--------------------------------------------------------Permutation----------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------//

class Permutation {
private:
	int* First_Row;
	int* Second_Row;
	unsigned int sz = 0;
public:
	//Constructors
	Permutation() = default;
	Permutation(const unsigned int Number) {
		unsigned First_Copy = Number, Second_Copy = Number;
		sz = Number;
		First_Row = new int[sz];
		Second_Row = new int[sz];

		for (int i = sz - 1; i >= 0; --i) {
			First_Row[i] = i + 1;
			Second_Row[i] = i + 1;
		}
	}
	Permutation(const int* Array, unsigned int Size_Of_Array) {
		First_Row = new int[Size_Of_Array];
		Second_Row = new int[Size_Of_Array];
		sz = Size_Of_Array;

		for (int i = Size_Of_Array - 1; i >= 0; --i) {
			First_Row[i] = i + 1;
			Second_Row[i] = Array[i];
		}
	}
	Permutation(int N, BigInt LexNum) {
		BigInt wr = factorial(N);
		if (wr < LexNum) {
			BigInt wr2 = LexNum / wr;
			LexNum -= wr2 * wr;
		}
		int k = LexNum.getInt();

		std::vector<int> digits;
		for (int i = 1; i <= N; ++i) {
			digits.push_back(i);
		}

		std::vector<int> result;
		k -= 1; // уменьшаем k на 1 для удобства вычислений
		for (int i = 0; i < N; ++i) {
			int index = k / factorial(N - 1 - i);
			result.push_back(digits[index]);
			digits.erase(digits.begin() + index);
			k %= factorial(N - 1 - i);
		}
		
		First_Row = new int[N];
		Second_Row = new int[N];
		for (int i = 1; i <= N; ++i) {
			First_Row[i - 1] = i;
			Second_Row[i - 1] = result[i - 1];
		}
		sz = N;
	}
	
	//Copy consturctor
	Permutation(const Permutation& For_Copy) {
		First_Row = new int[For_Copy.sz];
		Second_Row = new int[For_Copy.sz];
		sz = For_Copy.sz;
		for (int i = 0; i < sz; ++i) {
			First_Row[i] = For_Copy.First_Row[i];
			Second_Row[i] = For_Copy.Second_Row[i];
		}
	}

	//Getters
	int Get_SZ() {
		return sz;
	}
	int* Get_First_Row() {
		return First_Row;
	}
	int* Get_Second_Row() {
		return Second_Row;
	}
	int Get_LexNumber() {
		std::string permutation;
		for (int i = 0; i < sz; ++i) permutation.push_back(Second_Row[i]);
		int rank = 1;
		int length = permutation.length();

		for (int i = 0; i < length; ++i) {
			int smaller = 0;
			for (int j = i + 1; j < length; ++j) {
				if (permutation[j] < permutation[i]) {
					smaller++;
				}
			}
			rank += smaller * factorial(length - i - 1);
		}

		return rank;
	}

	//Methods
	Permutation Next() {
		Permutation ans = *this;
		++ans;
		return ans;
	}
	Permutation Previous() {
		Permutation ans = *this;
		--ans;
		return ans;
	}

	//Operators
	Permutation operator =(const Permutation For_Copy){
		if (sz != 0) {
			delete[] First_Row;
			delete[] Second_Row;
			sz = 0;
		}
		First_Row = new int[For_Copy.sz];
		Second_Row = new int[For_Copy.sz];
		sz = For_Copy.sz;
		for (int i = 0; i < sz; ++i) {
			First_Row[i] = For_Copy.First_Row[i];
			Second_Row[i] = For_Copy.Second_Row[i];
		}
		return *this;
	}

	Permutation operator *=(const Permutation Other) {
		if (sz != Other.sz) throw std::overflow_error("this->sz must be equal to the other.sz");
		Permutation ans = *this;
		for (int i = 0; i < sz; ++i) {
			ans.Second_Row[i] = Second_Row[Other.Second_Row[i] - 1];
		}
		*this = ans;
		return *this;
	}
	Permutation operator +=(const int count) {
		Permutation Copy = *this;
		int x = Copy.Get_LexNumber();
		x += count;
		Permutation ans(sz, x);
		*this = ans;
		return *this;
	}
	Permutation operator -=(const int count)  {
		Permutation Copy = *this;
		int x = Copy.Get_LexNumber();
		x -= count;
		if (x <= 0) {
			int wr = factorial(sz);
			while (x <= 0)
				x += factorial(sz);
		}

		Permutation ans(sz, x);
		*this = ans;
		return *this;
	}

	Permutation operator ++() {
		Permutation ans = *this;
		int indx = -1; //target for last item of suffix
		for (int i = sz - 1; i > 0 && indx == -1; --i) {
			if (Second_Row[i] <= Second_Row[i - 1]) continue;
			if (Second_Row[i] > Second_Row[i - 1]) indx = i;
		}
		if (indx == -1) {
			Permutation New = *this;
			for (int i = 0, t = sz - 1; i < sz; ++i, --t) {
				Second_Row[i] = New.Second_Row[t];
			}
			return *this;
		}
		int Item_Before_Suffix = Second_Row[indx - 1];
		for (int i = sz - 1; i >= indx; --i) {
			if (Second_Row[i] > Item_Before_Suffix) {
				int For_Swap = Second_Row[i];
				Second_Row[i] = Item_Before_Suffix;
				Second_Row[indx - 1] = For_Swap;
				break;
			}
		}

		for (int i = 0; i < indx; ++i) {
			ans.Second_Row[i] = Second_Row[i];
		}
		
		for (int i = sz - 1, t = indx; i >= indx; --i, ++t) {
			ans.Second_Row[i] = Second_Row[t];
		}
		*this = ans;
		return *this;
	}
	Permutation operator ++(int) {
		Permutation ans = *this;
		int indx = -1; //target for last item of suffix
		for (int i = sz - 1; i > 0 && indx == -1; --i) {
			if (Second_Row[i] <= Second_Row[i - 1]) continue;
			if (Second_Row[i] > Second_Row[i - 1]) indx = i;
		}
		if (indx == -1) {
			Permutation New = *this;
			for (int i = 0, t = sz - 1; i < sz; ++i, --t) {
				New.Second_Row[i] = Second_Row[t];
			}
			return New;
		}
		int Item_Before_Suffix = Second_Row[indx - 1];
		for (int i = sz - 1; i >= indx; --i) {
			if (Second_Row[i] > Item_Before_Suffix) {
				int For_Swap = Second_Row[i];
				Second_Row[i] = Item_Before_Suffix;
				Second_Row[indx - 1] = For_Swap;
				break;
			}
		}

		for (int i = 0; i < indx; ++i) {
			ans.Second_Row[i] = Second_Row[i];
		}

		for (int i = sz - 1, t = indx; i >= indx; --i, ++t) {
			ans.Second_Row[i] = Second_Row[t];
		}
		return ans;
	}

	Permutation operator --() {
		Permutation ans = *this;
		int indx = -1; //target for last item of suffix
		for (int i = sz - 1; i > 0 && indx == -1; --i) {
			if (Second_Row[i] >= Second_Row[i - 1]) continue;
			if (Second_Row[i] < Second_Row[i - 1]) indx = i;
		}
		if (indx == -1) {
			Permutation New = *this;
			for (int i = 0, t = sz - 1; i < sz; ++i, --t) {
				Second_Row[i] = New.Second_Row[t];
			}
			return *this;
		}
		int Item_Before_Suffix = Second_Row[indx - 1];
		for (int i = sz - 1; i >= indx; --i) {
			if (Second_Row[i] < Item_Before_Suffix) {
				int For_Swap = Second_Row[i];
				Second_Row[i] = Item_Before_Suffix;
				Second_Row[indx - 1] = For_Swap;
				break;
			}
		}

		for (int i = 0; i < indx; ++i) {
			ans.Second_Row[i] = Second_Row[i];
		}

		for (int i = sz - 1, t = indx; i >= indx; --i, ++t) {
			ans.Second_Row[i] = Second_Row[t];
		}
		*this = ans;
		return *this;
	}
	Permutation operator --(int) {
		Permutation ans = *this;
		int indx = -1; //target for last item of suffix
		for (int i = sz - 1; i > 0 && indx == -1; --i) {
			if (Second_Row[i] >= Second_Row[i - 1]) continue;
			if (Second_Row[i] < Second_Row[i - 1]) indx = i;
		}
		if (indx == -1) {
			Permutation New = *this;
			for (int i = 0, t = sz - 1; i < sz; ++i, --t) {
				New.Second_Row[i] = Second_Row[t];
			}
			return New;
		}
		int Item_Before_Suffix = Second_Row[indx - 1];
		for (int i = sz - 1; i >= indx; --i) {
			if (Second_Row[i] < Item_Before_Suffix) {
				int For_Swap = Second_Row[i];
				Second_Row[i] = Item_Before_Suffix;
				Second_Row[indx - 1] = For_Swap;
				break;
			}
		}

		for (int i = 0; i < indx; ++i) {
			ans.Second_Row[i] = Second_Row[i];
		}

		for (int i = sz - 1, t = indx; i >= indx; --i, ++t) {
			ans.Second_Row[i] = Second_Row[t];
		}
		return ans;
	}

	//Bool operators
	bool operator <(const Permutation& Other) const {
		if (sz != Other.sz) throw std::overflow_error("this->sz must be equal to the other.sz");
		for (int i = 0; i < sz; ++i) {
			if (Second_Row[i] < Other.Second_Row[i]) return true;
			if (Second_Row[i] > Other.Second_Row[i]) return false;
		}

		return false;
	}
	bool operator ==(const Permutation& Other) const {
		return (!(*this < Other) && !(Other < *this));
	}
	bool operator !=(const Permutation& Other) const {
		return ((*this < Other) || (Other < *this));
	}
	bool operator <=(const Permutation& Other) const {
		return ((*this < Other) || (*this == Other));
	}
	bool operator >(const Permutation& Other) const {
		return !(*this <= Other);
	}
	bool operator >=(const Permutation& Other) const {
		return !(*this < Other);
	}

	~Permutation() {
		if (sz != 0) {
			delete[] First_Row;
			delete[] Second_Row;
		}
	}

private:
	int factorial(int n) {
		if (n == 0) return 1;
		return n * factorial(n - 1);
	}
};

std::ostream& operator <<(std::ostream& out, const Permutation& num) {
	Permutation Copy = num;
	std::cout << "| ";
	for (int i = 0; i < Copy.Get_SZ(); ++i) {
		std::cout << Copy.Get_First_Row()[i] << " | ";
	}
	std::cout << "\n| ";
	for (int i = 0; i < Copy.Get_SZ(); ++i) {
		std::cout << Copy.Get_Second_Row()[i] << " | ";
	}

	return out;
}
Permutation operator *(const Permutation Our_Per, const Permutation Other) {
	Permutation ans = Our_Per;
	ans *= Other;
	return ans;
}
Permutation operator +(const Permutation Our_Per, const int count) {
	Permutation Copy = Our_Per;
	Copy += count;
	return Copy;
}
Permutation operator +(const int count, const Permutation Our_Per) {
	Permutation Copy = Our_Per;
	Copy += count;
	return Copy;
}
Permutation operator -(const Permutation Our_Per, const int count) {
	Permutation Copy = Our_Per;
	Copy -= count;
	return Copy;
}
Permutation operator -(const int count, const Permutation Our_Per) {
	Permutation Copy = Our_Per;
	Copy -= count;
	return Copy;
}

int main() {
	Permutation x(3, "1904707309470279507092590");
	std::cout << x;
}
