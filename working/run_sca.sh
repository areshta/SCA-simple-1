#!/bin/bash

rm *.log


echo -e "\n============ g++ version >>  ======================\n"
echo -e "\n g++ version \n"
g++ -v 2>  0_gcc.log

echo -e "\n====== test c++ file compilation ============\n"
g++ -Wall -Wextra -Weffc++ sca_cpp.cpp 2>>1_gpp.log >> 2_gpp.log

echo -e "\n============ clang ======================\n"
echo -e "\n sca using clang \n" >> 2_clang.log
scan-build  -o clang_html \
   -enable-checker security.FloatLoopCounter \
   -enable-checker security.insecureAPI.rand \
   -enable-checker security.insecureAPI.strcpy \
	g++ sca_cpp.cpp  \
	2>>2_clang.log >> 2_clang.log

echo -e "\n============ cpp check ======================\n"
echo -e "\n sca using cpp check \n" >> 3_cppcheck.log
cppcheck --enable=all --project=compile_commands.json 2>>3_cppcheck.log >> 3_cppcheck.log
