#!/bin/bash

i=1
m=1000
n=10
t=2
while [ $i -lt 5 ]
do
  ./matvec $m $n $t
  n=$[n * 10]
  t=$[t * 2]
  i=$[i + 1]
done
