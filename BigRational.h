#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include "BigInt.h"
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