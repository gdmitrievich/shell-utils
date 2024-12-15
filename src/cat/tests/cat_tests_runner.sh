#!/bin/bash

OS=$(uname)
if [[ "$OS" == "Linux" ]]; then
	flags=( -b -e -E -n -s -t -T )
elif [[ "$OS" == "Darwin" ]]; then
	flags=( -b -e -n -s -t )
else
	flags=( -b -e -E -n -s -t -T )
fi

DATASETS_DIR=./datasets
test_files=($( find ${DATASETS_DIR} -type f -name "*.txt" ))

BINARY_DIR=../../../src/cat
BINARY=${BINARY_DIR}/s21_cat

make -C ${BINARY_DIR} all

echo " "
echo "Response to wrong flag:"
./"${BINARY}" -q $test_file

echo " "
echo "Response to wrong file:"
./"${BINARY}" -n nofile.txt

function run_test() {
    local test_name="$1"
    local flag="$2"
    local files="${@:3}"

	echo -n "$test_name"

	./"${BINARY}" $flag $files > 1.txt
	cat $flag $files > 2.txt
	if cmp -s 1.txt 2.txt ; then
		echo "Success"
	else
		echo "Fail"
	fi
	rm 1.txt 2.txt
}

for flag in ${flags[@]}
do
    test_number=1
    echo ""
    echo "Tests with flag $flag:"
	echo "With one file:"
    for test_file in ${test_files[@]}
    do
		run_test "Test $test_number: " "$flag" "$test_file"
		((test_number++))
    done

    echo ""
	echo "With multiple files:"
	len=${#test_files[@]}
	for (( n=0; n<len-1; n++ )); do
		next=$((n + 1))
		run_test "Test $test_number: " "$flag" "${test_files[$n]}" "${test_files[$next]}"
		((test_number++))
    done
done
