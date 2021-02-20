#include "matrix.h"
#include "polynomial.h"

struct RelationGenerator {
   vector<string> names;
   vector<Fraction<Univariate>> rational_fractions;

   vector<Univariate> polynomial_basis;
   
   void addPolynomial(Univariate poly);
   vector<pair<int, int>> decompose(Univariate poly);
   void addFraction(string name, Fraction<Univariate> frac);
   
   void printRelations();
};

void RelationGenerator::addFraction(string name, Fraction<Univariate> frac) {
   names.push_back(name);
   rational_fractions.push_back(frac);
   
   addPolynomial(frac.getNumerator());
   addPolynomial(frac.getDenominator());
}

void RelationGenerator::addPolynomial(Univariate poly) {
   for(int iElement = 0;iElement < (int)polynomial_basis.size();iElement++) {
   	if(poly.size() <= 1) {
   		return;
   	}
   	
      Univariate element = polynomial_basis[iElement];
      
      Univariate pgcd = univariate_gcd(poly, element);
      
      if(pgcd.size() <= 1) continue;
      
      polynomial_basis[iElement] = pgcd;
      
      while(element % pgcd == Univariate(0)) {
      	Univariate save = element;
      	element = element / pgcd;
      }
      
      addPolynomial(element);
      
      while(poly % pgcd == Univariate(0))
      	poly = poly / pgcd;
      iElement--;
   }
   
   if(poly.size() > 1) {
   	polynomial_basis.push_back(poly);
   }
}

vector<pair<int, int>> RelationGenerator::decompose(Univariate poly) {
   vector<pair<int, int>> decomposition;
   for(int iFactor = 0;iFactor < (int)polynomial_basis.size();iFactor++) {
      int nb = 0;
      while(poly.size() > 1 && poly % polynomial_basis[iFactor] == Univariate(0)) {
         poly = poly / polynomial_basis[iFactor];
         nb++;
      }
      
      if(nb != 0) {
         decomposition.push_back({iFactor, nb});
      }
   }
   return decomposition;
}

void RelationGenerator::printRelations() {
   Matrix<Rational> decompositions(0, 0);
   for(auto& fraction: rational_fractions) {
      vector<pair<int, int>> numerator = decompose(fraction.getNumerator());
      vector<pair<int, int>> denominator = decompose(fraction.getDenominator());
      
      vector<Rational> decomposition(polynomial_basis.size(), Rational(0));
      
      for(pair<int, int> poly : numerator) {
         decomposition[poly.first] = decomposition[poly.first] + Rational(poly.second);
      }
      
      for(pair<int, int> poly : denominator) {
         decomposition[poly.first] = decomposition[poly.first] - Rational(poly.second);
      }
      
      decompositions.coeffs.push_back(decomposition);
   }
   
   //Matrix<Rational> relations = row_echelon_form(kernel_basis(decompositions));
   Matrix<Rational> rows = kernel_basis(decompositions);
   
   cerr << "Relations computed. Size : " << endl;
   cerr << rows.nbRows() << " " << rows.nbCols() << endl;
   cerr << "Simplifying.." << endl;
   
   Matrix<Rational> relations = LLL(rows, Rational(3) / Rational(4));
   
   for(const vector<Rational>& relation : relations.coeffs) {
      for(size_t iCol = 0;iCol < relation.size();iCol++) {
         if(!(relation[iCol] == Rational(0))) {
            cout << names[iCol] << "^" << toString(relation[iCol]) << " ";
         }
      }
      cout << "= 1" << endl;
   }
}

