#pragma once
#include <cassert>
#include "matrix.h"
#include "polynomial.h"
using namespace std;

Univariate X, U, Z;
Fraction<Univariate> x, u, z;

typedef Matrix<Fraction<Univariate>> FArithMatrix;

struct FArith {
    FArithMatrix A;
    FArithMatrix u;

    Fraction<Univariate> get_fraction() {
        return (inverse(identity<Fraction<Univariate>>(A.nbRows()) - A) * u).coeffs[0].getCoeff(0);
    }
    
    void remove_row(size_t row) {
    	FArithMatrix nA(A.coeffs.size() - 1, A.coeffs.size() - 1);
		FArithMatrix nu(A.coeffs.size() - 1, 1);
		
		size_t nRow = 0;
		for(size_t iRow = 0;iRow < A.coeffs.size();iRow++) {
			if(iRow == row)
				continue;
				
			nu.coeffs[nRow].setCoeff(0, u.coeffs[iRow].getCoeff(0));
			
			size_t nCol = 0;
			for(size_t iCol = 0;iCol < A.coeffs.size();iCol++) {
				if(iCol == row)
					continue;
				
				nA.coeffs[nRow].setCoeff(nCol, A.coeffs[iRow].getCoeff(iCol));
				
				nCol++;
			}
			nRow++;
		}
		
		A = nA;
		u = nu;
    }
    
    void simplify() {
    	for(size_t iCol = 1;iCol < A.nbCols();iCol++) {
    		bool is_zero = true;
    		
    		for(size_t iRow = 0;is_zero && iRow < A.nbRows();iRow++) {
    			if(iRow != iCol && A.coeffs[iRow].getCoeff(iCol).getNumerator().size() != 0) {
    				is_zero = false;
    			}
    		}
    		
    		if(is_zero) {
    			remove_row(iCol);
    			simplify();
    			return;
    		}
    	}
    
    	FArithMatrix new_mat = transpose(A);
    	new_mat.coeffs.push_back(transpose(u).coeffs[0]);
    	new_mat = transpose(new_mat);
    	
    	FArithMatrix basis = kernel_basis(new_mat);
    	
    	if(basis.nbRows() == 0)
			return;
    	
		size_t row = basis.coeffs[0].coeffs[0].first;
		
		Fraction<Univariate> bCoeff = basis.coeffs[0].getCoeff(A.nbCols());
		basis.coeffs[0].setCoeff(A.nbCols(), 0);
		
		for(size_t iRow = 0;iRow < A.nbRows();iRow++) {
			if(iRow != row) {
				u.coeffs[iRow].setCoeff(0, u.coeffs[iRow].getCoeff(0) - A.coeffs[iRow].getCoeff(row) / basis.coeffs[0].getCoeff(row) * bCoeff);
				
				A.coeffs[iRow] = A.coeffs[iRow] - A.coeffs[iRow].getCoeff(row) / basis.coeffs[0].getCoeff(row) * basis.coeffs[0];
			}
		}
	
    	simplify();
    }
};

FArith operator * (const FArith &a, const FArith &b) {
	FArith res = {.A = tensor(a.A, b.A), .u = tensor(a.u, b.u)};
	res.simplify();
    return res;
}

FArith operator ^ (const FArith &a, const FArith &b) {
    FArithMatrix tensAId = tensor(a.A, identity<Fraction<Univariate>>(b.A.nbRows()));
    FArithMatrix tensIdB = tensor(identity<Fraction<Univariate>>(a.A.nbRows()), b.A);
    FArithMatrix cross_mat = tensAId - tensIdB;

    FArithMatrix v = tensor(a.u, b.u);
    v = inverse(cross_mat) * v;

    FArithMatrix va = tensAId * v;
    FArithMatrix vb = -u * tensIdB * v;

    FArithMatrix matrix = magic_op(tensAId, tensIdB);

    FArithMatrix vfinal = va;

    for(auto& row : vb.coeffs) {
        vfinal.coeffs.push_back(row);
    }

    vfinal.coeffs[0].setCoeff(0, vfinal.coeffs[0].getCoeff(0) + vb.coeffs[0].getCoeff(0));
	
	FArith res = {.A = matrix, .u = vfinal};
	res.simplify();
    return res;
}

/* Generate some usual function */

FArith id() {
    return {
        .A = FArithMatrix({
            {x}
        }),
        .u = FArithMatrix({
            {u}
        })
    };
}

FArith inv_id() {
    return {
        .A = FArithMatrix({
            {u / x}
        }),
        .u = FArithMatrix({
            {u}
        })
    };
}

FArith mobius() {
    return {
        .A = FArithMatrix({
            {z, u},
            {z, z}
        }),
        .u = FArithMatrix({
            {u},
            {-u}
        })
    };
}

FArith one() {
    return {
        .A = FArithMatrix({
            {u}
        }),
        .u = FArithMatrix({
            {u}
        })
    };
}

FArith pow(const FArith &a, size_t exp) {
    /* Exponentiation by squaring */
    if (exp <= 0) {
        return one();
    }
    FArith res = pow(a, exp/2);
    res = res * res;
    if (exp%2 != 0) {
        res = res * a;
    }
    return res;
}

FArith jordan_totient(size_t k) {
    assert(k > 0);
    return pow(id(), k) ^ mobius();
}

FArith phi() {
    return jordan_totient(1);
}

FArith sigma_k(size_t k) {
    assert(k > 0);
    return one() ^ pow(id(), k);
}

FArith theta() {
    return pow(mobius(), 2) ^ one();
}

FArith nb_divisors() {
    return {
        .A = FArithMatrix({
            {u, u},
            {z, u},
        }),
        .u = FArithMatrix({
            {u},
            {u},
        })
    };
}

FArith liouville() {
    return {
        .A = FArithMatrix({
            {-u}
        }),
        .u = FArithMatrix({
            {-u}
        })
    };
}
