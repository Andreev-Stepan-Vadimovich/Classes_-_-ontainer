#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

template<int N>
class Finite {
private:
	int num;
public:
	//Constructors
	Finite() = default;
	Finite(int x) {
		num = x % N;
	}

	//Methods
	void Show() {
		std::cout << num;
	}
	int Get_num() { return num; }
	std::string To_String() {
		std::string s = std::to_string(num);
		return s;
	}

	//Operators
	void operator =(const Finite& other) {
		num = other.num;
	}

	Finite& operator += (const Finite& other) {
		num += other.num;
		num %= N;
		if (num < 0) num += N;
		return *this;
	}
	Finite& operator -= (const Finite& other) {
		num -= other.num;
		num %= N;
		if (num < 0) num += N;
		return *this;
	}
	Finite& operator *= (const Finite& other) {
		num *= other.num;
		num %= N;
		if (num < 0) num += N;
		return *this;
	}
	Finite operator + (const Finite& other) {
		Finite copy = *this;
		copy += other;
		return copy;
	}
	Finite operator - (const Finite& other) {
		Finite copy = *this;
		copy -= other;
		return copy;
	}
	Finite operator * (const Finite& other) {
		Finite copy = *this;
		copy *= other;
		return copy;
	}

	Finite& operator /= (const Finite& other) {
		if (!is_prime(N)) throw std::overflow_error("Division is not supported for non-simple N");
		if (other.num == 0) throw std::overflow_error("Division by zero!");
		num /= other.num;
		return *this;
	}
	Finite operator / (const Finite& other) {
		Finite copy = *this;
		copy /= other;
		return copy;
	}

	bool operator == (const Finite& other) {
		return num == other.num;
	}
	bool operator != (const Finite& other) {
		return num != other.num;
	}

private:
	bool is_prime(const int n) {
		if (n <= 1) return false;
		for (int i = 2; i * i <= n; ++i) {
			if (n % i == 0) return false;
		}
		return true;
	}
};

template<int N>
std::ostream& operator <<(std::ostream& cout, Finite<N>& other) {
	std::cout << other.Get_num();
	return cout;
}
