#!/bin/bash

OS=$(uname)
if [[ "$OS" == "Linux" ]]; then
	flags=( -b -e -E -n -s -t -T )
elif [[ "$OS" == "Darwin" ]]; then
	flags=( -b -e -n -s -t )
else
	flags=( -b -e -E -n -s -t -T )
fi

long_flags=( --number-nonblank --number --squeeze-blank )

DATASETS_DIR=./datasets
test_files=($( find ${DATASETS_DIR} -type f -name "*.txt" ))

BINARY_DIR=../../../src/cat
BINARY=${BINARY_DIR}/s21_cat

succeed=0
failed=0

make -C ${BINARY_DIR} all

echo " "
echo "Response to missing file:"
./"${BINARY}" -n

echo " "
echo "Response to wrong file:"
./"${BINARY}" -n nofile.txt

echo " "
echo "Response to wrong flag:"
./"${BINARY}" -q ${test_files[0]}

function run_test() {
    local test_name="$1"
    local flag="$2"
    local files="${@:3}"

	echo -n "$test_name"

	./"${BINARY}" $flag $files > 1.txt
	cat $flag $files > 2.txt
	if cmp -s 1.txt 2.txt ; then
		echo "Success"
		((succeed++))
	else
		echo "Fail"
		((failed++))
	fi
    if ! valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --tool=memcheck --error-exitcode=1 ./"${BINARY}" $flag $files > /dev/null ; then
        echo "MEMORY LEAK DETECTED!!!"
        exit
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

for long_flag in ${long_flags[@]}
do
    echo ""
    echo "Tests with flag $long_flag:"
    for test_file in ${test_files[@]}
    do
		run_test "Test $test_number: " "$flag" "$test_file"
		((test_number++))
    done
done

echo ""
echo "Additional tests"
echo "Without flags"
run_test "Test $test_number: " "${test_files[0]}" "${test_files[1]}"
((test_number++))

echo ""
echo "Succeed: $succeed"
echo "Failed: $failed"