#pragma once
#include "BigInt.h"
#include "BigRational.h"
#include "Finite.h"

template<unsigned N, unsigned M, typename Field = BigRational>
class Matrix {
private:
	Field mat[N][M];
public:
	//Constructors
	Matrix() {
		if (N == M) {
			for (int i = 0; i < N; ++i) {
				for (int j = 0; j < M; ++j) {
					if (i == j) mat[i][j] = 1;
					else mat[i][j] = 0;
				}
			}
		}
		else {
			for (int i = 0; i < N; ++i) {
				for (int j = 0; j < M; ++j) {
					mat[i][j] = 0;
				}
			}
		}
	}
	Matrix(const std::vector<std::vector<Field>>& arr) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < M; ++j) {
				mat[i][j] = arr[i][j];
			}
		}
	}
	Matrix(const Field** arr) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < M; ++j) {
				mat[i][j] = arr[i][j];
			}
		}
	}

	//Methods
	void Show() {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < M; ++j) {
				std::cout << mat[i][j] << ' ';
			}
			std::cout << '\n';
		}
	}
	std::vector<Field> Get_Row(int num) {
		std::vector<Field> res;
		--num;
		for (int i = 0; i < M; ++i) {
			res.push_back(mat[num][i]);
		}
		return res;
	}
	std::vector<Field> Get_Col(int num) {
		std::vector<Field> res;
		--num;
		for (int i = 0; i < N; ++i) res.push_back(mat[i][num]);
		return res;
	}
	Field trace() const {
		Field sum = Field(0);
		for (unsigned i = 0; i < std::min(N, M); ++i)
			sum = sum + mat[i][i];
		return sum;
	}
	int rank() {
		Matrix copy = *this;
		int rank = 0;
		for (int i = 0; i < N; ++i) {
			bool swap_Flag = false;
			for (int j = rank; j < M; ++j) {
				if (copy.mat[i][j] != Field(0)) {
					for (int k = 0; k < N; ++k)
						std::swap(copy.mat[k][rank], copy.mat[k][j]);
					swap_Flag = true;
					break;
				}
			}
			if (!swap_Flag) continue;
			for (int j = i + 1; j < N; ++j) {
				Field Ratio = copy.mat[j][rank] / copy.mat[i][rank];
				for (int k = rank; k < M; ++k) {
					copy.mat[j][k] = copy.mat[j][k] - Ratio * copy.mat[i][k];
				}
			}
			++rank;
		}
		return rank;
	}

	//Getters
	unsigned Get_N() { return N; }
	unsigned Get_M() { return M; }
	Field Get_mat() { return mat; }
	Matrix<M, N, Field> transposed() {
		Matrix<M, N, Field> res;
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < M; ++j) {
				res.mat[j][i] = mat[i][j];
			}
		}
		return res;
	}

	//Operators
	void operator = (const Matrix& other) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < M; ++j) {
				mat[i][j] = other.mat[i][j];
			}
		}
	}

	Matrix& operator += (const Matrix& other) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < M; ++j) {
				mat[i][j] += other.mat[i][j];
			}
		}
		return *this;
	}
	Matrix& operator + (const Matrix& other) {
		Matrix res = *this;
		res += other;
		return res;
	}
	Matrix& operator -= (const Matrix& other) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < M; ++j) {
				mat[i][j] -= other.mat[i][j];
			}
		}
		return *this;
	}
	Matrix& operator - (const Matrix& other) {
		Matrix res = *this;
		res -= other;
		return res;
	}
	Matrix& operator *= (const Field& x) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < M; ++j) {
				mat[i][j] *= x;
			}
		}
		return *this;
	}
	Matrix& operator * (const Field& x) {
		Matrix res = *this;
		res *= x;
		return res;
	}

	template<unsigned K>
	Matrix<N, K, Field> operator * (const Matrix<M, K, Field>& other) const {
		Matrix<N, K, Field> Res;
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < K; ++j) {
				Res[i][j] = 0;
				for (int y = 0; y < M; ++y) {
					Res[i][j] += mat[i][y] * other[y][j];
				}
			}
		}
		return Res;
	}

	bool operator == (const Matrix& other) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < M; ++j) {
				if (mat[i][j] != other.mat[i][j]) return false;
			}
		}
		return true;
	}
	bool operator != (const Matrix& other) {
		return !(*this == other);
	}

	//Operator []
	Field* operator [](const int& row) {
		return mat[row];
	}
	const Field* operator [] (const int& row) const {
		return mat[row];
	}

private:
	void Make_NULL() {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < M; ++j) {
				mat[i][j] = 0;
			}
		}
	}
};

template<unsigned N, unsigned M, typename Field = BigRational>
std::ostream& operator <<(std::ostream& cout, Matrix<N, M, int>& other) {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			std::cout << other[i][j];
		}
		std::cout << '\n';
	}
	return cout;
}

template<unsigned N, unsigned M, typename Field = double>
std::ostream& operator <<(std::ostream& cout, Matrix<N, M, double>& other) {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			std::cout << other[i][j] << ' ';
		}
		std::cout << '\n';
	}
	return cout;
}

template<unsigned N, unsigned M, typename Field = BigRational>
std::ostream& operator <<(std::ostream& cout, Matrix<N, M, Field>& other) {
	for (int i = 0; i < N; ++i) {
		std::string s;
		for (int j = 0; j < M; ++j) {
			s += other[i][j].To_String();
			s += ' ';
		}
		std::cout << s << '\n';
	}
	return cout;
}


template<unsigned N, typename Field>
class Matrix<N, N, Field> {
private:
	Field mat[N][N];
public:
	//Constructors
	Matrix() {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				if (i == j) mat[i][j] = Field(1);
				else mat[i][j] = Field(0);
			}
		}
	}
	Matrix(const std::vector<std::vector<Field>>& arr) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				mat[i][j] = arr[i][j];
			}
		}
	}
	Matrix(const Field** arr) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				mat[i][j] = arr[i][j];
			}
		}
	}

	//Methods
	void Show() {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				std::cout << mat[i][j] << ' ';
			}
			std::cout << '\n';
		}
	}
	std::vector<Field> Get_Row(int num) {
		std::vector<Field> res;
		--num;
		for (int i = 0; i < N; ++i) {
			res.push_back(mat[num][i]);
		}
		return res;
	}
	std::vector<Field> Get_Col(int num) {
		std::vector<Field> res;
		--num;
		for (int i = 0; i < N; ++i) res.push_back(mat[i][num]);
		return res;
	}
	Field trace() const {
		Field sum = Field(0);
		for (unsigned i = 0; i < N; ++i)
			sum = sum + mat[i][i];
		return sum;
	}
	int Det() {
		int det = 0;
		Field submat[N][N];

		if (N == 1) {
			return mat[0][0];
		}
		else if (N == 2) {
			return ((mat[0][0] * mat[1][1]) - (mat[1][0] * mat[0][1]));
		}
		else {
			for (int x = 0; x < N; x++) {
				int subi = 0;
				for (int i = 1; i < N; i++) {
					int subj = 0;
					for (int j = 0; j < N; j++) {
						if (j == x) {
							continue;
						}
						submat[subi][subj] = mat[i][j];
						subj++;
					}
					subi++;
				}
				det = det + (pow(-1, x) * mat[0][x] * Sub_Det(submat, N - 1));
			}
		}
		return det;
	}
	void invert() {
		int detThis = this->Det();
		if (!detThis) throw std::overflow_error("There is no inverse matrix");
		Matrix res;
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				res[i][j] = this->Del_Row_and_Col__Det(i, j) * pow(-1, i + j);
			}
		}
		res = res.transposed();
		res *= 1. / double(detThis);
		*this = res;
	}
	Matrix inverted() {
		int detThis = this->Det();
		if (!detThis) throw std::overflow_error("There is no inverse matrix");
		Matrix res;
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				res[i][j] = this->Del_Row_and_Col__Det(i, j) * pow(-1, i + j);
			}
		}
		res = res.transposed();
		res *= 1. / double(detThis);
		return res;
	}
	int rank() {
		Matrix copy = *this;
		int rank = 0;
		for (int i = 0; i < N; ++i) {
			bool swap_Flag = false;
			for (int j = rank; j < N; ++j) {
				if (copy.mat[i][j] != Field(0)) {
					for (int k = 0; k < N; ++k)
						std::swap(copy.mat[k][rank], copy.mat[k][j]);
					swap_Flag = true;
					break;
				}
			}
			if (!swap_Flag) continue;
			for (int j = i + 1; j < N; ++j) {
				Field Ratio = copy.mat[j][rank] / copy.mat[i][rank];
				for (int k = rank; k < N; ++k) {
					copy.mat[j][k] = copy.mat[j][k] - Ratio * copy.mat[i][k];
				}
			}
			++rank;
		}
		return rank;
	}

	//Getters
	unsigned Get_N() { return N; }
	Field Get_mat() { return mat; }
	Matrix<N, N, Field> transposed() {
		Matrix<N, N, Field> res;
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				res.mat[j][i] = mat[i][j];
			}
		}
		return res;
	}

	//Operators
	void operator = (const Matrix& other) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				mat[i][j] = other.mat[i][j];
			}
		}
	}

	Matrix& operator += (const Matrix& other) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				mat[i][j] += other.mat[i][j];
			}
		}
		return *this;
	}
	Matrix& operator + (const Matrix& other) {
		Matrix res = *this;
		res += other;
		return res;
	}
	Matrix& operator -= (const Matrix& other) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				mat[i][j] -= other.mat[i][j];
			}
		}
		return *this;
	}
	Matrix& operator - (const Matrix& other) {
		Matrix res = *this;
		res -= other;
		return res;
	}
	Matrix& operator *= (const Field& x) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				mat[i][j] *= x;
			}
		}
		return *this;
	}
	Matrix& operator * (const Field& x) {
		Matrix res = *this;
		res *= x;
		return res;
	}

	template<unsigned N>
	Matrix<N, N, Field> operator * (const Matrix<N, N, Field>& other) const {
		Matrix<N, N, Field> Res;
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				Res[i][j] = 0;
				for (int y = 0; y < N; ++y) {
					Res[i][j] += mat[i][y] * other[y][j];
				}
			}
		}
		return Res;
	}

	template<unsigned N>
	Matrix<N, N, Field> operator *= (const Matrix<N, N, Field>& other) {
		Matrix<N, N, Field> res = *this * other;
		*this = res;
		return *this;
	}

	bool operator == (const Matrix& other) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				if (mat[i][j] != other.mat[i][j]) return false;
			}
		}
		return true;
	}
	bool operator != (const Matrix& other) {
		return !(*this == other);
	}

	//Operator []
	Field* operator [](const int& row) {
		return mat[row];
	}
	const Field* operator [] (const int& row) const {
		return mat[row];
	}

private:
	void Make_NULL() {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				mat[i][j] -= mat[i][j];
			}
		}
	}
	int Sub_Det(Field dop_mat[N][N], int n) {
		int det = 0;
		Field submat[N][N];

		if (n == 1) {
			return dop_mat[0][0];
		}
		else if (n == 2) {
			return ((dop_mat[0][0] * dop_mat[1][1]) - (dop_mat[1][0] * dop_mat[0][1]));
		}
		else {
			for (int x = 0; x < n; x++) {
				int subi = 0;
				for (int i = 1; i < n; i++) {
					int subj = 0;
					for (int j = 0; j < n; j++) {
						if (j == x) {
							continue;
						}
						submat[subi][subj] = dop_mat[i][j];
						subj++;
					}
					subi++;
				}
				det = det + (pow(-1, x) * dop_mat[0][x] * Sub_Det(submat, n - 1));
			}
		}
		return det;
	}
	int Del_Row_and_Col__Det(int k, int l) {
		std::vector<std::vector<Field>> wr(N - 1, std::vector<Field>(N - 1));
		for (int i = 0, ii = 0; i < N; ++i, ++ii) {
			for (int j = 0, jj = 0; j < N; ++j, ++jj) {
				if (i == k) {
					j = N;
					--ii;
					continue;
				}
				if (j == l) {
					--jj;
					continue;
				}
				if (i != k && j != l) {
					wr[ii][jj] = mat[i][j];
				}
			}
		}
		Matrix<N - 1, N - 1, Field> wr_mat(wr);
		return wr_mat.Det();
	}
};

template<unsigned N, typename Field = BigRational>
std::ostream& operator <<(std::ostream& cout, Matrix<N, N, int>& other) {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			std::cout << other[i][j];
		}
		std::cout << '\n';
	}
	return cout;
}

template<unsigned N, typename Field = double>
std::ostream& operator <<(std::ostream& cout, Matrix<N, N, double>& other) {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			std::cout << other[i][j] << ' ';
		}
		std::cout << '\n';
	}
	return cout;
}

template<unsigned N, typename Field = BigRational>
std::ostream& operator <<(std::ostream& cout, Matrix<N, N, Field>& other) {
	for (int i = 0; i < N; ++i) {
		std::string s;
		for (int j = 0; j < N; ++j) {
			s += other[i][j].To_String();
			s += ' ';
		}
		std::cout << s << '\n';
	}
	return cout;
}


template<unsigned N, typename Field = BigRational>
using SquareMatrix = Matrix<N, N, int>;