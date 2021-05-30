/*
 * Copyright 2020-2021, Hadrien Barral, Arthur Leonard, Samuel Vivien and the CrazySums contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include "generation.h"

static constexpr GenerationConstraintLine default_large_config_lines[] = {
    /* leaf_type                  , min_exp, max_exp, [extra min/max k, [min/max l]] */
    {FormulaNode::LEAF_LIOUVILLE  ,       0, 1,       {}},
    {FormulaNode::LEAF_TAUK       ,       0, 3,       {2, 3}},
    {FormulaNode::LEAF_THETA      ,       0, 3,       {}},
    {FormulaNode::LEAF_JORDAN_T   ,       0, 2,       {1, 4}},
    {FormulaNode::LEAF_SIGMA      ,       0, 3,       {1, 5}},
    {FormulaNode::LEAF_SIGMA_PRIME,       0, 3,       {1, 5}},
    {FormulaNode::LEAF_XI         ,       0, 1,       {2, 3}},
    {FormulaNode::LEAF_NU         ,       0, 1,       {2, 3}},
};

static constexpr GenerationConstraint default_large_config = {
    .lines = default_large_config_lines,
    .lines_count = sizeof(default_large_config_lines)/sizeof(default_large_config_lines[0]),
    .min_sum = 0,
    .max_sum = 8,
    .max_score = 10,
};


static constexpr GenerationConstraintLine default_small_config_lines[] = {
    /* leaf_type                  , min_exp, max_exp, [extra min/max k, [min/max l]] */
    {FormulaNode::LEAF_LIOUVILLE  ,       0, 1,       {}},
    {FormulaNode::LEAF_TAUK       ,       0, 2,       {2, 2}},
    {FormulaNode::LEAF_THETA      ,       0, 1,       {}},
    {FormulaNode::LEAF_JORDAN_T   ,       0, 1,       {1, 2}},
    {FormulaNode::LEAF_SIGMA      ,       0, 2,       {1, 3}},
    {FormulaNode::LEAF_SIGMA_PRIME,       0, 2,       {1, 3}},
    {FormulaNode::LEAF_XI         ,       0, 1,       {2, 3}},
    {FormulaNode::LEAF_NU         ,       0, 1,       {2, 2}},
};

static constexpr GenerationConstraint default_small_config = {
    .lines = default_small_config_lines,
    .lines_count = sizeof(default_small_config_lines)/sizeof(default_small_config_lines[0]),
    .min_sum = 0,
    .max_sum = 8,
    .max_score = 6,
};