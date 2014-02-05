#!/bin/bash

make gauss

i=1
n=100
t=1

while [ $i -lt 5 ]
do
  echo "Num-variables: $n, Num-threads: $t"
  ./gauss $n $t
  t=$[t * 2]
  i=$[i + 1]
done

i=1
n=500
t=1

while [ $i -lt 5 ]
do
  echo "Num-variables: $n, Num-threads: $t"
  ./gauss $n $t
  t=$[t * 2]
  i=$[i + 1]
done

rm -rf gauss.o gauss
