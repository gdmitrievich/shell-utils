#!/bin/bash
DATASETS_DIR=./datasets

flags=( i v c l n h s o )
templates=( '[a-z]' '[0-9]' '^[#]' 'int' ';$' )
test_files="${DATASETS_DIR}/1.txt ${DATASETS_DIR}/2.txt ${DATASETS_DIR}/3.txt"
template_file="file_with_templates.txt"

BINARY_DIR=..
BINARY=${BINARY_DIR}/s21_grep

make -C ${BINARY_DIR} all

echo " "
echo "Response to wrong flag:"
./"${BINARY}" -w $test_file

echo " "
echo "Response to wrong file:"
./"${BINARY}" -c nofile.txt

function run_test() {
    local test_name="$1"
    local flags="$2"
	local template_flag="$3"
	local template_arg="$4"
    local files="${@:5}"

	echo -n "$test_name"
	# echo "flags: $flags"
	# echo "template_flag: $template_flag"
	# echo "template_arg: $template_arg"
	# echo "files: $files"

	./"${BINARY}" "-$flags" "${template_flag}" "${template_arg}" "$files" > 1.txt
	grep "-$flags" "${template_flag}" "${template_arg}" "$files" > 2.txt

	if cmp -s 1.txt 2.txt ; then
		echo "Success"
	else
		echo "Fail"
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
		echo ""
		echo "Tests with -$flag and -e:"

		run_test "Test $test_number: " "$flag" "-e" "$template" "$test_files"
		((test_number++))
	done

	echo ""
	echo "Tests with -$flag and -f:"

	run_test "Test $test_number: " "$flag" "-f" "$template_file" "$test_files"
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
			echo ""
			echo "Tests with -${flag1}${flag2} and -e:"

			run_test "Test $test_number: " "${flag1}${flag2}" "-e" "$template" "$test_files"
			((test_number++))
		done

		echo ""
		echo "Tests with -${flag1}${flag2} and -f:"

		run_test "Test $test_number: " "${flag1}${flag2}" "-f" "$template_file" "$test_files"
		((test_number++))
	done
done