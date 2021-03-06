/*
 * Copyright 2020-2021, Hadrien Barral, Arthur Leonard, Samuel Vivien and the CrazySums contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

constexpr int PRIME_MODULO = 997;

#include <iostream>
#include "configurations.h"
#include "hformula.h"
#include "generation.h"
#include "print.h"
#include "relations.h"
using namespace std;

/*
 * Edit me as you wish to change the generated L-functions
 */
static constexpr GenerationConstraintLine custom_generation_config_lines[] = {
    /* leaf_type                  , min_exp, max_exp, [extra min/max k, [min/max l]] */
    {FormulaNode::LEAF_LIOUVILLE  ,       0, 1,       {}},
    {FormulaNode::LEAF_TAUK       ,       0, 2,       {2, 2}},
    {FormulaNode::LEAF_THETA      ,       0, 1,       {}},
    {FormulaNode::LEAF_JORDAN_T   ,       0, 1,       {1, 2}},
    {FormulaNode::LEAF_SIGMA      ,       0, 2,       {1, 3}},
    {FormulaNode::LEAF_SIGMA_PRIME,       0, 2,       {1, 3}},
    {FormulaNode::LEAF_XI         ,       0, 1,       {2, 3}},
    {FormulaNode::LEAF_NU         ,       0, 1,       {2, 2}},
    //{FormulaNode::LEAF_MU         ,       0, 2,       {1, 2}},
    //{FormulaNode::LEAF_ZETAK      ,       0, 1,       {1, 3}},
};

/*
 * Edit me as you wish to change the generated L-functions
 */
static constexpr GenerationConstraint custom_generation_config = {
    .lines = custom_generation_config_lines,
    .lines_count = sizeof(custom_generation_config_lines)/sizeof(custom_generation_config_lines[0]),
    .min_sum = 0,
    .max_sum = 8,
    .max_score = 6,
};

#ifndef CZ_CONFIG
    #define CZ_CONFIG custom_generation_config
#endif

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    precomputeInverses();
    X.setCoeff(1, 1);
    U.setCoeff(0, 1);
    x = Fraction<Univariate>(X);
    u = Fraction<Univariate>(U);
    z = Fraction<Univariate>(Z);
    Latex latex;

    auto t1 = std::chrono::high_resolution_clock::now();
    RelationGenerator manager(&latex);
    add_relations(manager, latex, CZ_CONFIG);

    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> e21 = t2 - t1;
    cout.flush();
    cerr << "Data generated ("<< manager.rational_fractions.size() << " fractions)"
         << KGRY << " (" << e21.count() << "s)" KRST << endl;

    auto t3 = std::chrono::high_resolution_clock::now();
    manager.prepareBasis();
    auto t4 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> e43 = t4 - t3;
    cerr << "Basis prepared ("<< manager.polynomial_basis.size() << " polynomials)"
         << KGRY << " (" << e43.count() << "s)" KRST << endl;

    manager.printRelations();

    return 0;
}
