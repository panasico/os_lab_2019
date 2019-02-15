#!/bin/bash
echo There was $# parameters passed
sum=0;
for param in "$@"
do
sum=$((sum+$param))
done
echo $((sum/$#))