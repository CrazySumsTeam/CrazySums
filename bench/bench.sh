#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")"

rm -f logs_*

# $1: configuration name
# $2: log file name suffix
bench_with_config () {
    echo "Bench with config '$1'"
    echo "======================"
    LOG="logs_$2"
    MAKE="make -C .."
    ${MAKE} clean >/dev/null
    EXTRA="-DCZ_CONFIG=$1 -UHAS_COLOR" ${MAKE} build >/dev/null
    time ${MAKE} run > "${LOG}"
    D_NB="$(grep -c "\[D-..\]" "${LOG}")"
    C_NB="$(grep -c "\[C-..\]" "${LOG}")"
    other_NB=$(($(grep -c "\[\!\!\!\!\]" "${LOG}") +
                $(grep -c "\[\*\*\*\*\]" "${LOG}")))
    echo "D-XX: ${D_NB} C-XX: ${C_NB} other: ${other_NB}"
}

bench_with_config default_small_config small
bench_with_config default_large_config large
