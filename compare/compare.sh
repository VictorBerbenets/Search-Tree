#!/bin/bash
#
#color atributes
red="\033[1;31m"
blue="\033[1;36m"
usual="\033[0m"


comp_file="../compare/compare.txt"
tests_files="../tests/end2end/resources/tests/"
comp_out="./compare/compare"


#---------------------------------------------#
function run_compare_program {
    echo -e "${blue}generating compare files...${usual}"
    pwd
    for file in ${tests_files}*
    do
        ${comp_out} ${file}
    done
    echo -e "${blue}done${usual}"
}
#---------------------------------------------#


#---------------------------------------------#
function clear_compareTXT {
    echo -n > ${comp_file}
}
#---------------------------------------------#

# main() #
error="false"

if [ `ls ${tests_files} | wc -l` -eq 0 ]
then
    error="true"
    echo -e "${red}error:${usual} tests directory is empty. Firstly need to generate end2end tests"
fi

if [ ${error} = "false" ]
then
    clear_compareTXT
    run_compare_program
fi
