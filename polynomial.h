#pragma once
#include "fraction.h"
#include "print.h"
#include "matrix.h"

using namespace std;

template<typename T>
class Polynomial {
public:
	Polynomial(vector<T> _coeffs = vector<T>());
	Polynomial(vector<pair<int, T>> sparse_vect);
	Polynomial(MatrixRow<T> sparse_vect);
	Polynomial(int64_t _constant);
	size_t size() const;
	T getCoeff_unsafe(size_t pos) const;
	T getCoeff(size_t pos) const;
	void setCoeff(size_t pos, T coeff);
	void setCoeff_unsafe(size_t pos, T coeff);
	void toMonic();
	void reduce();
	void operator *= (const T& a);
	void operator -= (const Polynomial<T>& a);
	void operator %= (const Polynomial<T>& a);
	void substractShiftedForReduction(const Polynomial<T>& a, size_t shift);
private:
	vector<T> coeffs;
};

template<typename T>
bool operator < (const Polynomial<T>& a, const Polynomial<T>& b) {
	if(a.size() != b.size())
		return a.size() < b.size();

	for(ssize_t iCoeff = a.size()-1;iCoeff >= 0;iCoeff--) {
		if(a.getCoeff(iCoeff) == b.getCoeff(iCoeff))
			continue;
		return a.getCoeff(iCoeff) < b.getCoeff(iCoeff);
	}

	return false;
}

template<typename T>
Polynomial<T>::Polynomial(vector<T> _coeffs) {
	coeffs = _coeffs;
	coeffs.reserve(64);
	reduce();
}

template<typename T>
Polynomial<T>::Polynomial(vector<pair<int, T>> sparse_vect) {
	coeffs = vector(sparse_vect[sparse_vect.size()-1][0]+1, 0);
	for (auto& val : sparse_vect) {
		coeffs[val[0]] = val[1];
	}
	reduce();
}

template<typename T>
Polynomial<T>::Polynomial(MatrixRow<T> sparse_vect) {
	coeffs = vector<T>(sparse_vect.coeffs[sparse_vect.size()-1][0]+1, 0);
	for (auto& val : sparse_vect.coeffs) {
		coeffs[val[0]] = val[1];
	}
	reduce();
}

template<typename T>
Polynomial<T>::Polynomial(int64_t _constant) {
	coeffs.clear();
	coeffs.push_back(T(_constant));
	reduce();
}

template<typename T>
inline size_t Polynomial<T>::size() const {
	return coeffs.size();
}

template<typename T>
inline T Polynomial<T>::getCoeff_unsafe(size_t pos) const {
	/* Unsafe: no size check */
	return coeffs[pos];
}


template<typename T>
inline T Polynomial<T>::getCoeff(size_t pos) const {
	if(pos >= coeffs.size())
		return T(0);
	return getCoeff_unsafe(pos);
}

template<typename T>
inline void Polynomial<T>::setCoeff_unsafe(size_t pos, T coeff) {
	while(pos >= coeffs.size()) {
		coeffs.push_back(T(0));
	}
	coeffs[pos] = coeff;
	/* Unsafe: caller should perform reduce() once it has batched operations. */
}

template<typename T>
inline void Polynomial<T>::setCoeff(size_t pos, T coeff) {
	setCoeff_unsafe(pos, coeff);
	reduce();
}

template<typename T>
bool operator == (const Polynomial<T>& a, const Polynomial<T>& b) {
	if(a.size() != b.size())
		return false;

	for(size_t iCoeff = 0;iCoeff < a.size();iCoeff++) {
		if(!(a.getCoeff(iCoeff) == b.getCoeff(iCoeff)))
			return false;
	}
	return true;
}


template<typename T>
void Polynomial<T>::reduce() {
	while(!coeffs.empty() && coeffs.back() == T(0)) {
		coeffs.pop_back();
	}
}

template<typename T>
Polynomial<T> operator << (const Polynomial<T>& a, size_t shift) {
	Polynomial<T> sum;

	for(int iCoeff = (int)a.size() - 1;iCoeff >= 0;iCoeff--) {
		sum.setCoeff_unsafe(iCoeff + shift, a.getCoeff(iCoeff));
	}

	sum.reduce();
	return sum;
}

template<typename T>
Polynomial<T> operator + (const Polynomial<T>& a, const Polynomial<T>& b) {
	Polynomial<T> sum;
	for(int iCoeff = max<int>(a.size(), b.size()) - 1;iCoeff >= 0;iCoeff--) {
		sum.setCoeff_unsafe(iCoeff, a.getCoeff(iCoeff) + b.getCoeff(iCoeff));
	}
	sum.reduce();
	return sum;
}

template<typename T>
void Polynomial<T>::operator *= (const T& a) {
	for(size_t iCoeff = 0;iCoeff < size();iCoeff++) {
		setCoeff_unsafe(iCoeff, a * getCoeff(iCoeff));
	}
	reduce();
}

template<typename V, typename T>
Polynomial<T> operator * (const V& a, const Polynomial<T>& b) {
	Polynomial<T> sum = b;
	sum *= a;
	return sum;
}

template<typename V, typename T>
Polynomial<T> operator / (const Polynomial<T>& a, const V& b) {
	Polynomial<T> sum;
	for(int iCoeff = (int)a.size() - 1;iCoeff >= 0;iCoeff--) {
		sum.setCoeff_unsafe(iCoeff, a.getCoeff(iCoeff) / b);
	}
	sum.reduce();
	return sum;
}


template<typename T>
Polynomial<T> operator - (const Polynomial<T>& a) {
	return T(-1) * a;
}

template<typename T>
inline void Polynomial<T>::operator -= (const Polynomial<T>& a) {
	for(int iCoeff = max<int>(size(), a.size()) - 1;iCoeff >= 0;iCoeff--) {
		setCoeff_unsafe(iCoeff, getCoeff(iCoeff) - a.getCoeff(iCoeff));
	}
	reduce();
}

template<typename T>
Polynomial<T> operator - (const Polynomial<T>& a, const Polynomial<T>& b) {
	Polynomial<T> sum = a;
	sum -= b;
	return sum;
}

template<typename T>
Polynomial<T> operator * (const Polynomial<T>& a, const Polynomial<T>& b) {
	Polynomial<T> sum;
	for(int iCoeffA = (int)a.size() - 1;iCoeffA >= 0;iCoeffA--) {
		for(int iCoeffB = (int)b.size() - 1;iCoeffB >= 0;iCoeffB--) {
			sum.setCoeff_unsafe(iCoeffA + iCoeffB, sum.getCoeff(iCoeffA + iCoeffB) + a.getCoeff(iCoeffA) * b.getCoeff(iCoeffB));
		}
	}

	sum.reduce();
	return sum;
}

template<typename T>
Polynomial<T> derive(Polynomial<T> a) {
	Polynomial<T> sum;
	for (size_t iCoeff = 1;iCoeff < a.size();iCoeff++) {
		sum.setCoeff_unsafe(iCoeff - 1, T(iCoeff) * a.getCoeff(iCoeff));
	}
	sum.reduce();
	return sum;
}

template<typename T>
T leading(const Polynomial<T>& a) {
	return a.getCoeff(a.size() - 1);
}

template<typename T>
void Polynomial<T>::toMonic(void) {
	if (!(leading(*this) == T(1))) {
		*this *= inverse(leading(*this));
	}
}

template<typename T, typename... Args>
string toString(const Polynomial<T>& poly, string variable, Args... args) {
	string result;

	bool first = true;
	for(size_t iCoeff = 0;iCoeff < poly.size();iCoeff++) {
		if(!(poly.getCoeff(iCoeff) == T(0))) {
			if(!first) {
				result += " + ";
			}
			first = false;

			result += toString(poly.getCoeff(iCoeff), args...);
			switch(iCoeff) {
				case 0:
					break;
				case 1:
					result += KGRY + variable + KRST;
					break;
				default:
					result += KGRY + variable + "^" + KRST + to_string(iCoeff);
		}

		}
	}

	return "(" + result + ")";
}

typedef Polynomial<Mod> Univariate;

template<typename T>
inline void Polynomial<T>::operator %= (const Polynomial<T>& a) {
	while(size() >= a.size()) {
		size_t shift = size() - a.size();
		substractShiftedForReduction(a, shift);
	}
}


template<typename T>
Polynomial<T> operator % (Polynomial<T> a, Polynomial<T> b) {
	a %= b;
	return a;
}

/*
 * This avoid comparing to T(0), which might be slow to generate
 */
template<typename T>
bool isMultipleOf(Polynomial<T> a, Polynomial<T> b) {
	a %= b;
	return a.size() == 0;
}

template<typename T>
Polynomial<T> operator / (Polynomial<T> a, Polynomial<T> b) {
	Polynomial<T> quotient;

	while(a.size() >= b.size()) {
		size_t shift = a.size() - b.size();

		quotient.setCoeff_unsafe(shift, leading(a) / leading(b));
		a.substractShiftedForReduction(b, shift);
	}

	quotient.reduce();
	return quotient;
}

/*
 * Input condition: a.size() + shift <= this.size()
 */
template<typename T>
inline void Polynomial<T>::substractShiftedForReduction(const Polynomial<T>& a, size_t shift) {
	T mult = leading(*this) * inverse(leading(a)); /* FIXME: leading(*this) / leading(a)` is slower */
	for(size_t iCoeff = shift;iCoeff < size();iCoeff++) {
		setCoeff_unsafe(iCoeff, getCoeff_unsafe(iCoeff) - mult*a.getCoeff_unsafe(iCoeff-shift));
	}
	reduce();
}

template<typename T>
Polynomial<T> gcd(Polynomial<T> a, Polynomial<T> b) {
	while(a.size() != 0) {
		if(a.size() > b.size())
			swap(a, b);

		b.substractShiftedForReduction(a, b.size() - a.size());
		swap(a, b);
	}

	return b;
}

template<typename T>
Polynomial<T> normalFactor(const Polynomial<T>& a, const Polynomial<T>& b) {
	return gcd(a, b);
}
