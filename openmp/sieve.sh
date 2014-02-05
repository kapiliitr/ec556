#!/bin/bash

make sieve

i=1
n=10000
t=4
while [ $i -lt 5 ]
do
  echo "Max Num: $n, Num-threads: $t"
  ./sieve $n $t
  n=$[n * 10]
  t=$[t * 2]
  i=$[i + 1]
done

rm -rf sieve.o sieve
