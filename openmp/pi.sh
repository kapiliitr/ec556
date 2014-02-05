#!/bin/bash

make pi

i=1
n=10
t=2
while [ $i -lt 10 ]
do
  echo "Num-samples: $n, Num-threads: $t"
  ./pi $n $t
  n=$[n * 10]
  t=$[t * 2]
  i=$[i + 1]
done

rm -rf pi.o pi
