#!/bin/bash

file=$1

cat ./$1 | ./mycc 2> ir.ll

llvm-as-13 -o ir.bc ir.ll

echo "---------------- Running Result: -------------------"

lli ir.bc
