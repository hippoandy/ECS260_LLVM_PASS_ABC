#!/bin/bash

test_dir="test_programs"

### Compiling and linking source code to apply instrument pass

clang -emit-llvm -c check.c -o check.bc

echo -e "\nInstrument------------------------------------------"
for test in $test_dir/test*.c; do
  test=`echo $test | cut -d . -f 1`
  clang -g -c -emit-llvm $test.c -o $test.bc
  llvm-link -o ${test}_temp.bc $test.bc check.bc

  echo -e "Test: $test.c\n"
  opt -load ./Passes.so -instrument ${test}_temp.bc > ${test}_i.bc
done
echo "Done Instrument-------------------------------------"

### Running instrumented tests

echo -e "\nRun-------------------------------------------------"
for test in $test_dir/test*.c; do
  test=`echo $test | cut -d . -f 1`
  echo -e "Test: $test.c\n"
  lli ${test}_i.bc
done
echo -e "Done Run--------------------------------------------"
