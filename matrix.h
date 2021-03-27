#pragma once
#include <vector>
using namespace std;

// We work with row vectors
template<typename T>
class MatrixRow{
public:
   vector<pair<size_t, T>> coeffs;
   MatrixRow(vector<T> _coeffs);
   MatrixRow(vector<pair<size_t, T>> _coeffs);
   MatrixRow(size_t nbCols, size_t value = 0);

   void setCoeff(size_t num_col, T value);

   size_t size() const;
   size_t max_index() const;
   T getCoeff(size_t num_col) const;

};

template<typename T>
MatrixRow<T>::MatrixRow(vector<T> _coeffs) {
   for (size_t i = 0; i < _coeffs.size(); i++) {
      if (!(_coeffs[i] == T(0))) {
         coeffs.push_back(make_pair(i, _coeffs[i]));
      }
   }
}

template<typename T>
MatrixRow<T>::MatrixRow(vector<pair<size_t, T>> _coeffs) {
   coeffs = _coeffs;
}

template<typename T>
MatrixRow<T>::MatrixRow(size_t nbCols, size_t value) {
   if (value != 0) {
      for (size_t i = 0; i < nbCols; i++) {
         coeffs.push_back(make_pair(i, value));
      }
   }
}

template<typename T>
void MatrixRow<T>::setCoeff(size_t num_col, T value) {
   size_t ind = 0;
   while (ind < coeffs.size() && coeffs[ind].first < num_col) {
      ind++;
   }
   if (ind == coeffs.size()) {
      if (!(value == T(0))) {
         coeffs.push_back(make_pair(num_col, value));
      }
   } else {
      if (coeffs[ind].first == num_col) {
         if (value == T(0)) {
            for (size_t i = ind; i + 1 < coeffs.size(); i++) {
               coeffs[i] = coeffs[i + 1];
            }
            coeffs.pop_back();
         } else {
            coeffs[ind].second = value;
         }
      } else {
         if (!(value == T(0))) {
            coeffs.push_back(make_pair(num_col, value)); // bullshit value
            for (size_t i = coeffs.size() - 1; i > ind; i--) {
               coeffs[i] = coeffs[i - 1];
            }
            coeffs[ind] = make_pair(num_col, value);
         }
      }
   }
}

template<typename T>
T MatrixRow<T>::getCoeff(size_t num_col) const {
   for (size_t i = 0; i < coeffs.size(); i++) {
      if (coeffs[i].first == num_col) {
         return coeffs[i].second;
      } else if (coeffs[i].first > num_col) {
         return T(0);
      }
   }
   return T(0);
}

template<typename T>
size_t MatrixRow<T>::size() const {
   return coeffs.size();
}

template<typename T>
size_t MatrixRow<T>::max_index() const {
   if (coeffs.size() == 0) {
      return 0;
   }
   return 1 + coeffs[coeffs.size() - 1].first;
}

template<typename T>
T operator * (const MatrixRow<T>& a, const MatrixRow<T>& b) {
   T result;
   size_t c_a = 0;
   size_t c_b = 0;
   while (c_a < a.coeffs.size() && c_b < b.coeffs.size()) {
      if (a.coeffs[c_a].first < b.coeffs[c_b].first) {
         c_a++;
      } else if (a.coeffs[c_a].first > b.coeffs[c_b].first) {
         c_b++;
      } else {
         result += a.coeffs[c_a++].second * b.coeffs[c_b++].second;
      }
   }
   return result;
}

template<typename T>
MatrixRow<T> operator * (const T& a, const MatrixRow<T>& b) {
   vector<pair<size_t, T>> result;
   for (auto& coord : b.coeffs) {
      result.push_back(make_pair(coord.first, a * coord.second));
   }
   return MatrixRow<T>(result);
}

template<typename T>
MatrixRow<T> operator + (const MatrixRow<T>& a, const MatrixRow<T>& b) {
   vector<pair<size_t, T>> result;
   size_t c_a = 0;
   size_t c_b = 0;
   while (c_a < a.size() && c_b < b.size()) {
      if (a.coeffs[c_a].first < b.coeffs[c_b].first) {
         result.push_back(a.coeffs[c_a++]);
      } else if (a.coeffs[c_a].first > b.coeffs[c_b].first) {
         result.push_back(b.coeffs[c_b++]);
      } else {
         size_t id = a.coeffs[c_a].first;
         T result = a.coeff[c_a++].second + b.coeff[c_b++].second;
         if (!(result == T(0))) {
            result.push_back(make_pair(id, result));
         }
      }
   }
   for (size_t i = c_a; i < a.size(); i++) {
      result.push_back(a.coeffs[i]);
   }

   for (size_t i = c_b; i < b.size(); i++) {
      result.push_back(b.coeffs[i]);
   }

   return MatrixRow<T>(result);
}

template<typename T>
MatrixRow<T> operator - (const MatrixRow<T>& a, const MatrixRow<T>& b) {
   vector<pair<size_t, T>> result;
   size_t c_a = 0;
   size_t c_b = 0;
   while (c_a < a.size() && c_b < b.size()) {
      if (a.coeffs[c_a].first < b.coeffs[c_b].first) {
         result.push_back(a.coeffs[c_a++]);
      } else if (a.coeffs[c_a].first > b.coeffs[c_b].first) {
         T val = - b.coeffs[c_b].second;
         result.push_back(make_pair(b.coeffs[c_b++].first, val));
      } else {
         size_t id = a.coeffs[c_a].first;
         T v_a = a.coeffs[c_a++].second;
         T v_b = b.coeffs[c_b++].second;
         if (!(v_a == v_b)) {
            result.push_back(make_pair(id, v_a - v_b));
         }
      }
   }
   for (size_t i = c_a; i < a.size(); i++) {
      result.push_back(a.coeffs[i]);
   }

   for (size_t i = c_b; i < b.size(); i++) {
      T val = - b.coeffs[i].second;
      result.push_back(make_pair(b.coeffs[i].first, val));
   }

   return MatrixRow<T>(result);
}

#if 0
template<typename T>
vector<T> operator + (const vector<T>& a, const vector<T>& b) {
   vector<T> res(b.size(), T(0));
   for(size_t iCoeff = 0;iCoeff < b.size();iCoeff++) {
      res[iCoeff] = a[iCoeff] + b[iCoeff];
   }
   return res;
}

template<typename T>
vector<T> operator - (const vector<T>& a, const vector<T>& b) {
   vector<T> res(b.size(), T(0));
   for(size_t iCoeff = 0;iCoeff < b.size();iCoeff++) {
      res[iCoeff] = a[iCoeff] - b[iCoeff];
   }
   return res;
}

template<typename T>
vector<T> operator * (T a, const vector<T>& b) {
   vector<T> res(b.size(), T(0));
   for(size_t iCoeff = 0;iCoeff < b.size();iCoeff++) {
      res[iCoeff] = a * b[iCoeff];
   }
   return res;
}

template<typename T>
T operator * (const vector<T>& a, const vector<T>& b) {
   T res(0);
   
   for(size_t iCoeff = 0;iCoeff < b.size();iCoeff++) {
      res = res + a[iCoeff] * b[iCoeff];
   }
   
   return res;
}

template<typename T>
T squared_norm(const vector<T>& a) {
   return a * a;
}
#endif

template<typename T>
class Matrix {
public:
   size_t nbRows() const;
   size_t nbCols() const;
   void actualizeNCols();
   Matrix(size_t nbRows, size_t nbCols, size_t value = 0);
   Matrix(vector<vector<T>> _coeffs);
   Matrix(vector<MatrixRow<T>> _coeffs);
   size_t nCol;
   vector<MatrixRow<T>> coeffs;
};

template<typename T>
size_t Matrix<T>::nbRows() const {
   return coeffs.size();
}

template<typename T>
void Matrix<T>::actualizeNCols() {
   size_t new_nCol = 0;
   for (auto& row : coeffs) {
      size_t value = row.max_index();
      if (value > new_nCol) {
         new_nCol = value;
      }
   }
   nCol = new_nCol;
}

template<typename T>
size_t Matrix<T>::nbCols() const {
   return nCol;
}

template<typename T>
Matrix<T>::Matrix(size_t nbRows, size_t nbCols, size_t value) {
   nCol = nbCols;
   coeffs = vector<MatrixRow<T>>(nbRows, MatrixRow<T>(nbCols, value));
}

template<typename T>
Matrix<T>::Matrix(vector<vector<T>> _coeffs) {
   for (auto& row : _coeffs) {
      coeffs.push_back(MatrixRow<T>(_coeffs));
   }
   actualizeNCols();
}

template<typename T>
Matrix<T> identity(size_t size) {
   Matrix<T> res = Matrix<T>(size, size, 0);
   for(size_t i = 0;i < size;i++) {
      res.coeffs[i].setCoeff(i, 1);
   }
   return res;
}

template<typename T>
Matrix<T> operator + (const Matrix<T>& a, const Matrix<T>& b) {
   Matrix<T> res(a.nbRows(), a.nbCols());
   
   for(size_t iRow = 0;iRow < a.nbRows();iRow++) {
      res.coeffs[iRow] = a.coeffs[iRow] + b.coeffs[iRow];
   }
   
   return res;
}

template<typename T>
Matrix<T> operator - (const Matrix<T>& a, const Matrix<T>& b) {
   Matrix<T> res(a.nbRows(), a.nbCols());
   
   for(size_t iRow = 0;iRow < a.nbRows();iRow++) {
      res.coeffs[iRow] = a.coeffs[iRow] + b.coeffs[iRow];
   }
   
   return res;
}

template<typename T>
Matrix<T> transpose(Matrix<T> mat) {
   Matrix<T> res(mat.nbCols(), mat.nbRows());
   for(size_t iRow = 0;iRow < mat.nbRows();iRow++) {
      for(size_t iCol = 0;iCol < mat.nbCols();iCol++) {
         res.coeffs[iCol].setCoeff(iRow, mat.coeffs[iRow].getCoeff(iCol));
      }
   }
   return res;
}

template<typename T>
void debug(const Matrix<T>& mat) {
   for(size_t iRow = 0;iRow < mat.nbRows();iRow++) {
      cout << ((iRow == 0) ? "[" : " ") << "[ ";
      for(size_t iCol = 0;iCol < mat.nbCols();iCol++) {
         T value = mat.coeffs[iRow].getCoeff(iCol);
         cout << ((value == T(0)) ? KGRY : "") << std::setw(2) << value
              << ((value == T(0)) ? KRST : "") << " ";
      }
      cout << "]" << ((iRow+1 == mat.nbRows()) ? "]" : "") << endl;
   }
}

template<typename T>
Matrix<T> kernel_basis(Matrix<T> mat) {
   /* We use Gaussian Elimination */

   Matrix<T> id = identity<T>(mat.nbRows());
   
   size_t rowStart = 0;
   for(size_t iCol = 0;iCol < mat.nbCols();iCol++) {
   	if(rowStart >= mat.nbRows()) break;
   	
      size_t non_zero = rowStart;
      for(size_t iRow = rowStart;iRow < mat.nbRows();iRow++) {
         if(!(mat.coeffs[iRow].getCoeff(iCol) == T(0))) {
            non_zero = iRow;
         }
      }
      
      swap(mat.coeffs[rowStart], mat.coeffs[non_zero]);
      swap(id.coeffs[rowStart], id.coeffs[non_zero]);
      
      if(mat.coeffs[rowStart].getCoeff(iCol) == T(0)) {
      	continue;
      }
      
      id.coeffs[rowStart] = (T(1) / mat.coeffs[rowStart].getCoeff(iCol)) * id.coeffs[rowStart];
      mat.coeffs[rowStart] = (T(1) / mat.coeffs[rowStart].getCoeff(iCol)) * mat.coeffs[rowStart];

      for(size_t iRow = 0;iRow < mat.nbRows();iRow++) {
         if(iRow == rowStart) continue;
         if(!(mat.coeffs[iRow].getCoeff(iCol) == T(0))) {
            id.coeffs[iRow] = id.coeffs[iRow] - mat.coeffs[iRow].getCoeff(iCol) * id.coeffs[rowStart];
            mat.coeffs[iRow] = mat.coeffs[iRow] - mat.coeffs[iRow].getCoeff(iCol) * mat.coeffs[rowStart];
         }
      }
      
      rowStart++;
   }

   Matrix<T> basis(0, 0);
   
   for(size_t iRow = 0;iRow < mat.nbRows();iRow++) {
      bool is_zero = true;
      
      for(size_t iCol = 0;iCol < mat.nbCols();iCol++) {
         is_zero &= mat.coeffs[iRow].getCoeff(iCol) == T(0);
      }
      
      if(is_zero) {
         basis.coeffs.push_back(id.coeffs[iRow]);
      }
   }
   basis.actualizeNCols();
   
   return basis;
}

#if 0 /* UNUSED BUT SHOULD KEEP */

template<typename T>
Matrix<T> row_echelon_form(Matrix<T> mat) {
   for(size_t iCol = 0;iCol < mat.nbCols();iCol++) {
      size_t non_zero = iCol;
      for(size_t iRow = iCol;iRow < mat.nbRows();iRow++) {
         if(!(mat.coeffs[iRow].getCoeff(iCol) == T(0))) {
            non_zero = iRow;
         }
      }
      
      if(iCol >= mat.nbRows()) break;
      swap(mat.coeffs[iCol], mat.coeffs[non_zero]);
      if(mat.coeffs[iCol].getCoeff(iCol) == T(0)) continue;
      
      mat.coeffs[iCol] = (T(1) / mat.coeffs[iCol].getCoeff(iCol)) * mat.coeffs[iCol];
      
      for(size_t iRow = 0;iRow < mat.nbRows();iRow++) {
         if(iRow == iCol) continue;
         if(!(mat.coeffs[iRow][iCol] == T(0))) {
            mat.coeffs[iRow] = mat.coeffs[iRow] - mat.coeffs[iRow].getCoeff(iCol) * mat.coeffs[iCol];
         }
      }
   }
   return mat;
}

template<typename T>
void debug(const Matrix<T>& mat) {
   for(size_t iRow = 0;iRow < mat.nbRows();iRow++) {
      for(size_t iCol = 0;iCol < mat.nbCols();iCol++) {
         cout << mat.coeffs[iRow].getCoeff(iCol) << " ";
      }
      cout << endl;
   }
}

template<typename T>
void debug(const vector<T>& v) {
   for(size_t iCol = 0;iCol < v.size();iCol++) {
      cout << v[iCol] << " ";
   }
   cout << endl;
}

template<typename T>
vector<T> projection(size_t i, const Matrix<T>& mat, vector<T> v) {
   vector<T> proj(v.size(), T(0));
   
   for(size_t iRow = i;iRow < mat.nbRows();iRow++) {
      proj = proj + ((v * mat.coeffs[iRow]) / squared_norm(mat.coeffs[iRow])) 
       * mat.coeffs[iRow];
   }
   
   return proj;
}

template<typename T>
Matrix<T> gram_schmidt(Matrix<T> mat) {
   for(size_t iRow = 0;iRow < mat.nbRows();iRow++) {
      for(size_t iProj = 0;iProj < iRow;iProj++) {
         mat.coeffs[iRow] = mat.coeffs[iRow]
          - ((mat.coeffs[iRow] * mat.coeffs[iProj]) / squared_norm(mat.coeffs[iProj])) 
          * mat.coeffs[iProj];
      }
   }
   
   return mat;
}

template<typename T>
vector<T> nearest_plane(Matrix<T> mat, vector<T> v) {
   if(mat.nbRows() == 0) return vector<T>(v.size(), T(0));
   
   Matrix<T> ortho = gram_schmidt(mat);
   T coeff = round((v * ortho.coeffs.back()) / squared_norm(ortho.coeffs.back()));
   
   vector<T> bn = mat.coeffs.back();
   mat.coeffs.pop_back();
   
   return coeff * bn + nearest_plane(mat, v - coeff * bn);
}

template<typename T>
Matrix<T> LLL(Matrix<T> mat, T delta) {
   Matrix<T> ortho = gram_schmidt(mat);
   
   for(size_t iRow = 1;iRow < mat.nbRows();iRow++) {
      mat.coeffs[iRow] = mat.coeffs[iRow] + nearest_plane(mat, ortho.coeffs[iRow] - mat.coeffs[iRow]);
   }
   
   ortho = gram_schmidt(mat);
   for(size_t iRow = 1;iRow < mat.nbRows();iRow++) {
      vector<T> projA = ortho.coeffs[iRow - 1];
      vector<T> projB = projection(iRow - 1, ortho, mat.coeffs[iRow]);
      
      T diff = delta * squared_norm(projA) - squared_norm(projB);
      if(diff.getNumerator() > 0) {
         swap(mat.coeffs[iRow], mat.coeffs[iRow - 1]);
         return LLL(mat, delta);
      }
   }
   
   return mat;
}

#endif /* UNUSED BUT SHOULD KEEP */
