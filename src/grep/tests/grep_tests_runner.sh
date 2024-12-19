#!/bin/bash
DATASETS_DIR=./datasets

flags=( i v c l n h s o )
long_flags=( ignore-case invert-match count files-with-matches line-number no-filename no-messages only-matching )
templates=( '[a-z]' '[0-9]' '^[#]' 'int' ';$' )
test_files="${DATASETS_DIR}/1.txt ${DATASETS_DIR}/2.txt ${DATASETS_DIR}/3.txt"
template_file="file_with_templates.txt"

succeed=0
failed=0

BINARY_DIR=..
BINARY=${BINARY_DIR}/s21_grep

make -C ${BINARY_DIR} all

echo " "
echo "Response to the missing file and pattern:"
./"${BINARY}" -c

echo " "
echo "Response to the missing file:"
./"${BINARY}" -c 'int'

echo " "
echo "Response to the non existed file:"
./"${BINARY}" -c 'int' nofile.txt

echo " "
echo "Response to wrong flag:"
./"${BINARY}" -w $test_files

echo " "
echo "Response for an empty pattern given to -e flag:"
./"${BINARY}" -e "${test_files[0]}"

echo " "
echo "Response for an error pattern given to -e flag:"
./"${BINARY}" -e "[a-Z" nofile.txt

function run_test() {
    local test_name="$1"
    local flags="$2"
	local template_flag="$3"
	local template_arg="$4"
	IFS=' ' read -ra files <<< "${@:5}"

	echo -n "$test_name"

	./"${BINARY}" "-$flags" "${template_flag}" "${template_arg}" "${files[@]}" > 1.txt 2>&1
	grep "-$flags" "${template_flag}" "${template_arg}" "${files[@]}" > 2.txt 2>&1
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

test_number=1
echo ""
echo "Tests with one flag"
for flag in ${flags[@]}
do
	for template in ${templates[@]}
	do
		run_test "Test $test_number (-$flag and -e): " "$flag" "-e" "$template" "$test_files"
		((test_number++))
	done

	run_test "Test $test_number (-$flag and -f): " "$flag" "-f" "$template_file" "$test_files"
	((test_number++))
done

echo ""
echo "Tests with two flags"
for flag1 in ${flags[@]}
do
	for flag2 in ${flags[@]}
	do
		if [[ $flag1 == $flag2 ]]; then
			continue
		fi

		for template in ${templates[@]}
		do
			run_test "Test $test_number (-${flag1}${flag2} and -e): " "${flag1}${flag2}" "-e" "$template" "$test_files"
			((test_number++))
		done

		run_test "Test $test_number (-${flag1}${flag2} and -f): " "${flag1}${flag2}" "-f" "$template_file" "$test_files"
		((test_number++))
	done
done

echo ""
echo "Tests with long flags"
for long_flag in ${long_flags[@]}
do
	run_test "Test $test_number (--$long_flag and -e): " "-$long_flag" "-e" "${templates[0]}" "$test_files"
	((test_number++))
done

echo ""
echo "Additional tests"
echo "Tests with -s flag"
run_test "Test $test_number (-s and -e): " s -e "int" "g o o d j o b"
((test_number++))
run_test "Test $test_number (-s and -f): " s -f "nofile.txt" "file.txt"
((test_number++))
run_test "Test $test_number (-s and -f): " s -f "$template_file" "file.txt good job"
((test_number++))

echo ""
echo "Test with -n flag and files (both existed and nonexisted)"
run_test "Test $test_number (-n and -e): " n -e "int" "a b $test_files"
((test_number++))

echo ""
echo "Succeed: $succeed"
echo "Failed: $failed"