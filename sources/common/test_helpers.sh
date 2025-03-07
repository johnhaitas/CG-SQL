#!/bin/bash
# Copyright (c) Facebook, Inc. and its affiliates.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

colordiff() {
  diff \
          --old-line-format=$'\e[0;31m< %l\e[0m
' \
          --new-line-format=$'\e[0;32m> %l\e[0m
' \
          --old-group-format='-------- %dn line%(n=1?:s) deleted at %df:
%<' \
          --new-group-format='-------- %dN line%(N=1?:s) added after %de:
%>' \
          --changed-group-format='-------- %dn line%(n=1?:s) changed at %df:
%<-------- to:
%>' \
          --unchanged-group-format='' \
          "$@"
  return $?
}

# Note that we need the line numbers in the main output so that we can use the test
# tools to see which output came from what input.  However this causes silly diffs
# so the reference output has the line numbers stripped.  When comparing against
# the reference output we replace the line numbers with XXXX
normalize_lines() {
  sed -e "s/The statement ending at line .*/The statement ending at line XXXX/" \
      -e "/(c) Facebook, Inc. and its affiliates/d" \
      -e "/g.nerated S.gnedSource<<.*>>/d" \
      -e "s/MBQ/CGB/g" \
      -e "s/MCQ/CGC/g" \
      -e "s/com.facebook.msys.mci/com.facebook.cgsql/g" \
      -e "/autogenerated by CQL/d" \
      -e "s/\.sql:[0-9]* :/.sql:XXXX :/" <"$1" >"$1.tmp"
  cp "$1.tmp" "$1"
  rm "$1.tmp"
}


__on_diff_exit() {
  normalize_lines "$1"
  normalize_lines "$2"
  if ! colordiff "$1" "$2"
  then
    echo "When running: diff" "$@"
    echo "The above differences were detected. If these are expected then run ok.sh to proceed."
    echo "Don't just run ok.sh to make the error go away; you have to really understand the diff first!"
    echo " "
    failed
  fi
}

on_diff_exit() {
   __on_diff_exit "${TEST_DIR}/$1.ref" "${OUT_DIR}/$1" 
}
