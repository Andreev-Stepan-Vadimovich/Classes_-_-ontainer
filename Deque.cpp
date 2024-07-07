#include <iostream>

using std::cout;
using std::endl;

template<bool B, typename U, typename V>
struct my_conditional {
	using type = V;
};

template<typename U, typename V>
struct my_conditional<true, U, V> {
	using type = U;
};

template <bool B, typename U, typename V>
using my_conditional_t = typename my_conditional<B, U, V>::type;

template<typename iterator>
class my_reverse_Iterator {
private:
	iterator bs;

public:
	using reference = typename std::iterator_traits<iterator>::reference;
	
	reference operator *() const {
		iterator tmp = bs;
		return *bs;
	}
	
	my_reverse_Iterator<iterator>& operator ++() {
		--bs;
		return *this;
	}

	my_reverse_Iterator<iterator>& operator --() {
		++bs;
		return *this;
	}

	my_reverse_Iterator<iterator>& operator +=(int number) {
		bs -= number;
		return *this;
	}

	my_reverse_Iterator<iterator>& operator -=(int number) {
		bs += number;
		return *this;
	}

	my_reverse_Iterator<iterator>& operator + (int number) const {
		iterator tmp = bs;
		tmp -= number;
		return tmp;
	}

	my_reverse_Iterator<iterator>& operator - (int number) const {
		iterator tmp = bs;
		tmp += number;
		return tmp;
	}

	bool operator==(const my_reverse_Iterator<iterator>& iter) const {
		return this->bs == iter.base();
	}

	bool operator!=(const my_reverse_Iterator<iterator>& iter) const {
		return !(this->bs == iter.base());
	}

	my_reverse_Iterator(const iterator& iter) : bs(iter) {};

	iterator base() const {
		return bs;
	}
};

template<typename T>
class deque {
private:
	template <bool isConst>
	class common_Iterator {
	private:
		my_conditional_t<isConst, const T**, T**> basePtrsElem;
		my_conditional_t<isConst, const int, int> backetElementPosition;
	public:	
		using iterator_category = std::random_access_iterator_tag;
		using pointer = my_conditional_t<isConst, const T**, T**>;
		using difference_type = std::ptrdiff_t;
		using reference = my_conditional_t<isConst, const T&, T&>;

		common_Iterator(T** basePtrsElem, size_t backetElementPosition) : basePtrsElem(basePtrsElem), backetElementPosition(backetElementPosition) {};
		common_Iterator() {
			basePtrsElem = nullptr;
			backetElementPosition = 0;
		}

		my_conditional_t<isConst, const T&, T&> operator*() {
			return (*basePtrsElem)[backetElementPosition];
		}

		common_Iterator operator ++() {
			basePtrsElem = (backetElementPosition + 1) > 9 ? ++basePtrsElem : basePtrsElem;
			backetElementPosition = (backetElementPosition + 1) % 10;
			return *this;
		}
		common_Iterator operator += (const size_t number) {
			basePtrsElem += (number + backetElementPosition) / 10;
			backetElementPosition = (backetElementPosition + number) % 10;
			return *this;
		}
		common_Iterator operator + (const size_t number) const{
			common_Iterator result = *this;
			return result += number;
		}
		common_Iterator operator --() {
			basePtrsElem = (backetElementPosition - 1) < 0 ? --basePtrsElem : basePtrsElem;
			backetElementPosition = (backetElementPosition - 1) < 0 ? backetElementPosition - 1 + 10 : --backetElementPosition;
			return *this;
		}
		common_Iterator operator -=(size_t number) {
			for (int i = 0; i < number; ++i) {
				--*this;
			}
			return *this;
		}
		common_Iterator operator - (const size_t number) const{
			common_Iterator result = *this;
			return result -= number;
		}
		bool operator == (common_Iterator a) {
			return this->backetElementPosition == a.backetElementPosition && this->basePtrsElem == a.basePtrsElem;
		}
		bool operator != (common_Iterator a) {
			return !(*this == a);
		}
		bool operator < (common_Iterator a) {
			if (this->basePtrsElem < a.basePtrsElem) {
				return true;
			}
			if (this->basePtrsElem == a.basePtrsElem) {
				return this->backetElementPosition < a.backetElementPosition;
			}
		}
		bool operator > (common_Iterator a) {
			return !(*this < a) && (*this != a);
		}
		bool operator >= (common_Iterator a) {
			return !(*this < a);
		}
		bool operator <= (common_Iterator a) {
			return (*this < a) || (*this == a);
		}
		int getBucketPos() const {
			return backetElementPosition;
		}

		T** getBucket() const {
			return basePtrsElem;
		}
	};

public:
	using Iterator = common_Iterator<false>;
	using const_Iterator = common_Iterator<true>;
	using reverse_Iterator = my_reverse_Iterator<Iterator>;
	using const_reverse_Iterator = my_reverse_Iterator<const_Iterator>;

private:
	T** basePtrs = nullptr;
	Iterator beg; //начало дека
	Iterator en; //конец дека
	Iterator dBegin; //начало выделенной памяти
	Iterator dEnd; //конец выделенной памяти
	
	size_t sz = 0;

public:
	size_t basecap;	
	deque() {
		basePtrs = new T*[10];
		
		for (int i = 0; i < 10; ++i) {
			basePtrs[i] = reinterpret_cast<T*>(new int8_t[10 * sizeof(T)]);
		}
		
		basecap = 10;
		beg = Iterator(basePtrs + 5, 5);
		en = beg;
		dBegin = Iterator(basePtrs, 0);
		dEnd = Iterator(basePtrs + 9, 9);
	}
	
	deque(const deque& another) {
		basecap = another.basecap;
		basePtrs = new T * [basecap];
		for (int i = 0; i < basecap; ++i) {
			basePtrs[i] = reinterpret_cast<T*>(new int8_t[10 * sizeof(T)]);

		}
		int b = (another.beg.getBucket() - another.dBegin.getBucket());
		int e = (another.en.getBucket() - another.dBegin.getBucket());
		for (int i = b; i < basecap; ++i) {
			for (int j = 0; j < 10; ++j) {
				if (i == 0) {
					j = another.beg.getBucketPos();
				}
				basePtrs[i][j] = another.basePtrs[i][j];
			}
		}
		dBegin = Iterator(basePtrs, 0);
		dEnd = Iterator(basePtrs + basecap, 9);
		beg = Iterator(dBegin.getBucket() + (another.beg.getBucket() - another.dBegin.getBucket()), another.beg.getBucketPos());
		en = Iterator(dBegin.getBucket() + (another.en.getBucket() - another.dBegin.getBucket()), another.en.getBucketPos());
	}
	
	
	~deque() {
		for (int i = 0; i < basecap; ++i) {
			delete[] reinterpret_cast<int8_t*>(basePtrs[i]);
		}
		delete[] basePtrs;
	}
	
	
	deque operator = (deque& another) {
		basecap = another.basecap;
		basePtrs = new T * [basecap];
		for (int i = 0; i < basecap; ++i) {
			basePtrs[i] = reinterpret_cast<T*>(new int8_t[10 * sizeof(T)]);
		}
		int b = (another.beg.getBucket() - another.dBegin.getBucket());
		int e = (another.en.getBucket() - another.dBegin.getBucket());
		for (int i = b; i < basecap; ++i) {
			for (int j = 0; j < 10; ++j) {
				if (i == 0) {
					j = another.beg.getBucketPos();
				}
				basePtrs[i][j] = another.basePtrs[i][j];
			}
		}
		dBegin = Iterator(basePtrs, 0);
		dEnd = Iterator(basePtrs + basecap, 9);
		beg = Iterator(dBegin.getBucket() + (another.beg.getBucket() - another.dBegin.getBucket()), another.beg.getBucketPos());
		en = Iterator(dBegin.getBucket() + (another.en.getBucket() - another.dBegin.getBucket()), another.en.getBucketPos());
		sz = another.sz;
		return *this;
	}
	

	Iterator begin() {
		return beg;
	}

	const_Iterator begin() const {
		return beg;
	}

	const_Iterator cbegin() const {
		return beg;
	}

	Iterator end() {
		return en;
	}

	const_Iterator end() const {
		return en;
	}

	const_Iterator cend() const {
		return en;
	}

	reverse_Iterator rbegin() {
		return en - 1;
	}

	reverse_Iterator rbegin() const{
		return en - 1;
	}

	reverse_Iterator rend() {
		return beg - 1;
	}

	reverse_Iterator rend() const{
		return beg - 1;
	}
	
	T& operator[](deque<T>::Iterator a) {
		return *a;
	}

	const T& operator[](deque<T>::Iterator a) const {
		return *a;
	}

	T& operator[](size_t pos){
		return *(beg + pos);
	}

	const T& operator[](size_t pos) const {
		return *(beg + pos);
	}

	void push_back(T element) {
		if (en != dEnd) {
			this->operator[](en) = element;
			++en;
			++sz;
		}
		
		else {
			T** newBase = new T* [3 * basecap];
			
			for (int i = 0; i < 3 * basecap; ++i) {
				newBase[i] = reinterpret_cast<T*>(new int8_t[10 * sizeof(T)]);		
			}
			
			for (size_t i = basecap; i < 2 * basecap; ++i) {
				newBase[i] = basePtrs[i - basecap];
			}
			
			beg = Iterator(&newBase[basecap + (beg.getBucket() - basePtrs)], beg.getBucketPos());
			en = Iterator(&newBase[basecap + (en.getBucket() - basePtrs)], en.getBucketPos());
			basecap *= 3;
			dBegin = Iterator(newBase, 0);
			dEnd = Iterator(newBase + basecap - 1, 9);
			delete[] reinterpret_cast<int8_t*>(basePtrs);
			basePtrs = newBase;
			this->operator[](en) = element;
			++en;
			++sz;
		}
		
	}
	void pop_back(){
		~(this->operator[](en - 1));
		--en;
		--sz;
	}
	void push_front(T element) {	
		if (beg - 1 != dBegin) {
			--beg;
			this->operator[](beg) = element;
			++sz;
		}
		else {
			T** newBase = new T* [3 * basecap];
			
			for (int i = 0; i < 3 * basecap; ++i) {
				newBase[i] = reinterpret_cast<T*>(new int8_t[10 * sizeof(T)]);
			}
			
			for (size_t i = basecap; i < 2 * basecap; ++i) {
				newBase[i] = basePtrs[i - basecap];
			}
			
			beg = Iterator(&newBase[basecap + (beg.getBucket() - basePtrs)], beg.getBucketPos());
			en = Iterator(&newBase[basecap + (en.getBucket() - basePtrs)], en.getBucketPos());
			basecap *= 3;
			dBegin = Iterator(newBase, 0);
			dEnd = Iterator(newBase + basecap - 1, 9);
			delete[] reinterpret_cast<int8_t*>(basePtrs);
			basePtrs = newBase;
			--beg;
			this->operator[](beg) = element;
			++sz;
		}

	}
	void pop_front() {
		~(this->operator[](beg));
		++beg;
		--sz;
	}	

	T& back() {
		return this->operator[](en - 1);
	}

	const T& back() const {
		return this->operator[](en - 1);
	}

	T& front() {
		return this->operator[](beg);
	}

	const T& front() const{
		return this->operator[](beg);
	}

	bool emply() const{
		return sz == 0;
	}

	size_t size() const {
		return sz;
	}


};

int main() {
	deque<int> a;
	
	for (int i = 0; i < 15; ++i) {
		a.push_back(i);
	}
	
	deque<int> b;
	b = a;
	cout << b[5];

	//const deque<int> c;
	//deque<int>::const_Iterator it = c.begin();

	return 0;
}
