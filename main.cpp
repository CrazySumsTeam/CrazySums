#include <fstream>
#include <iostream>
#include "arith_f.h"
#include "print.h"
#include "relations.h"
using namespace std;

static HFormula name_append_component(const HFormula& name, FormulaNode::LeafType component, int power)
{
    return HFormulaProduct(name, HFormulaPower(HFormulaLeaf(component), power));
}

static HFormula name_append_component(const HFormula& name, FormulaNode::LeafType component,
                                      int extra, int power)
{
    return HFormulaProduct(name, HFormulaPower(
               HFormulaLeaf(component, (FormulaNode::LeafExtraArg){.k = extra, .l = 0}), power));
}

static HFormula name_make_lfunc(const HFormula& name, int exponent)
{
    return HFormulaLFunction(name, exponent);
}

static void add_relation(RelationGenerator &manager,
                         int i_phi, int i_J_2,
                         int i_sigma_1, int i_sigma_2, int i_sigma_3,
                         int i_mu, int i_theta,
                         int s)
{
   auto t1 = std::chrono::high_resolution_clock::now();

   assert(i_mu <= 2);

   Fraction<Univariate> frac =
      (pow(phi(), i_phi)
    * pow(jordan_totient(2), i_J_2)
    * pow(theta(), i_theta)
    * pow(sigma_k(1), i_sigma_1)
    * pow(sigma_k(2), i_sigma_2)
    * pow(sigma_k(3), i_sigma_3)
    * pow(mobius(), i_mu)
    * pow(inv_id(), s)).get_fraction();

   auto t2 = std::chrono::high_resolution_clock::now();

   HFormula name = HFormulaOne();
   name = name_append_component(name, FormulaNode::LEAF_JORDAN_T, 1, i_phi);
   name = name_append_component(name, FormulaNode::LEAF_JORDAN_T, 2, i_J_2);
   name = name_append_component(name, FormulaNode::LEAF_THETA, i_theta);
   name = name_append_component(name, FormulaNode::LEAF_SIGMA, 1, i_sigma_1);
   name = name_append_component(name, FormulaNode::LEAF_SIGMA, 2, i_sigma_2);
   name = name_append_component(name, FormulaNode::LEAF_SIGMA, 3, i_sigma_3);
   name = name_append_component(name, FormulaNode::LEAF_MU, i_mu);

   name = name_make_lfunc(name, s);

   manager.addFraction(name, frac);

   std::chrono::duration<float> e21 = t2 - t1;
   cout << KBLD << name << KRST
        << KGRY "   (" << e21.count() << "s)" KRST << endl;
#if 0
   cout << toString(frac.getNumerator(), "x") << KGRN "/" KRST
        << toString(frac.getDenominator(), "x") << endl;
#endif
   //name->print_full(latex, 1);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
   precomputeInverses(211);
   X.setCoeff(1, 1);
   U.setCoeff(0, 1);
   latex_init();

   int maxi_phi = 2;
   int maxi_J_2 = 0;
   int maxi_theta = 0;
   int maxi_sigma_1 = 2;
   int maxi_sigma_2 = 1;
   int maxi_sigma_3 = 1;
   int maxi_mu = 2;
   int maxi_sum = 8;

   auto t1 = std::chrono::high_resolution_clock::now();

   RelationGenerator manager;

   for(int s = 2;s <= 2+maxi_sum*3;s++) {
      add_relation(manager, 0, 0, 0, 0, 0, 0, 0, s);
   }

   for(int i_phi = 0;i_phi <= maxi_phi;i_phi++) {
   for(int i_J_2 = 0;i_J_2 <= maxi_J_2;i_J_2++) {
   for(int i_theta = 0;i_theta <= maxi_theta;i_theta++) {
   for(int i_sigma_1 = 0;i_sigma_1 <= maxi_sigma_1;i_sigma_1++) {
   for(int i_sigma_2 = 0;i_sigma_2 <= maxi_sigma_2;i_sigma_2++) {
   for(int i_sigma_3 = 0;i_sigma_3 <= maxi_sigma_3;i_sigma_3++) {
   for(int i_mu = 0;i_mu <= maxi_mu;i_mu++) {
      int sum = i_phi + 2*i_J_2 + 0*i_theta + i_sigma_1 + 2*i_sigma_2 + 3*i_sigma_3 + 0*i_mu;
      if ((i_J_2 > 0) && (i_mu > 0)) {
         continue; /* Avoid generating C-8: J_2µ == φσµ */
      }
      for(int s = sum + 2;s <= sum + 2 + max(0, maxi_sum);s++) {
         if(i_phi+i_J_2+i_theta+i_sigma_1+i_sigma_2+i_sigma_3+i_mu > 0) {
            add_relation(manager, i_phi, i_J_2, i_sigma_1, i_sigma_2, i_sigma_3, i_mu, i_theta , s);
         }
      }
   }}}}}}}

   auto t2 = std::chrono::high_resolution_clock::now();
   std::chrono::duration<float> e21 = t2 - t1;
   cerr << "Data generated" << KGRY << " (" << e21.count() << "s)" KRST << endl;

   auto t3 = std::chrono::high_resolution_clock::now();
   manager.prepareBasis();
   auto t4 = std::chrono::high_resolution_clock::now();
   std::chrono::duration<float> e43 = t4 - t3;
   cerr << "Basis prepared" << KGRY << " (" << e43.count() << "s)" KRST << endl;
   manager.printRelations();

   latex_end();
   return 0;
}
