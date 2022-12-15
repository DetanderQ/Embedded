#!/bin/bash


assert (){
if [[ "$1" == "$2" ]];
then
	echo "Test passed ($1)"
else
	echo "Test falled ($1)"  
fi
}

RESULT=$(echo "3 2" | ./main)
assert "$RESULT" "0.105946 0.105946"

RESULT=$(echo "5 5" | ./main)
assert "$RESULT" "0.000000 0.000000"

RESULT=$(echo "10 6" | ./main)
assert "$RESULT" "0.071279 0.071279"

RESULT=$(echo "3 5" | ./main)
assert "$RESULT" "-0.168006 -0.168006"

RESULT=$(echo "7 4" | ./main)
assert "$RESULT" "0.092250 0.092250"
