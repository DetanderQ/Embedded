#!/bin/bash


assert (){
if [[ "$1" == "$2" ]];
then
	echo "Test passed ($1)"
else
	echo "Test falled ($1)"  
fi
}

RESULT=$(echo "2" | ./main)
assert "$RESULT" "0.954774 1.818595"

RESULT=$(echo "55" | ./main)
assert "$RESULT" "2.084080 -1.999510"

RESULT=$(echo "90" | ./main)
assert "$RESULT" "1.250213 1.787993"

RESULT=$(echo "120" | ./main)
assert "$RESULT" "0.026941 1.161222"

RESULT=$(echo "85" | ./main)
assert "$RESULT" "-1.706082 -0.352151"
