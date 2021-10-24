###
 # @Author: Haitian Li
 # @Date: 2021-05-17 22:38:27
 # @LastEditors: Haitian Li
 # @LastEditTime: 2021-05-18 05:01:31
 # @Description: Assignment4
### 

#!bin/bash

echo "##########################"
echo "### Testing"
echo "##########################"
count=0


for test in tests/*.sh; do
#if [ $test != run_bash.sh ];then
bash $test
#fi
# name=$(basename $test .x2017)
# Output=tests/$name.out
# #echo $test
# #valgrind ./vm_x2017 $test
# ./vm_x2017 $test | diff - $Output

# if [ $? -eq "1" ]; then
#     echo -e "\033[;31m [Failed] $name\033[0m"
# else
# 	echo -e "\033[;32m [Passed] $name\033[0m"
# fi

count=$((count+1))
done

echo "Finished running $pass_count/$count tests!"