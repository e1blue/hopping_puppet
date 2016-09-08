#!/bin/bash

# ----------------------------------------------------------------

export TESTMODE=0
export RELEASEMODE=1
export OPTIMMODE=0

echo
pushd system  >/dev/null ; sh create.sh ; popd >/dev/null ; echo
pushd puppet  >/dev/null ; sh create.sh ; popd >/dev/null ; echo
pushd meister >/dev/null ; sh create.sh ; popd >/dev/null ; echo
pushd gacha   >/dev/null ; sh create.sh ; popd >/dev/null ; echo
pushd field   >/dev/null ; sh create.sh ; popd >/dev/null ; echo
pushd enemy   >/dev/null ; sh create.sh ; popd >/dev/null ; echo
pushd manual  >/dev/null ; sh create.sh ; popd >/dev/null ; echo

# ----------------------------------------------------------------

