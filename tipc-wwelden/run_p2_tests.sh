#!/bin/bash

echo "Running p2 tests..."
echo "===================="

for testfile in testing-compilers/blocks/block*.i; do
  echo "Testing $(basename $testfile):"
  ./tipc -k 3 $testfile
  echo "===================="
done

echo "All tests completed."
