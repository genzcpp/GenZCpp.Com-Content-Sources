PREFIX="test"
../gcc_build.sh custom_allocator_test.cpp --big_stack --defines -DMAX_STACK_SIZE=100000000 -DEACH_VECTOR_SIZE=2 > ${PREFIX}_stacksize_100000000_vecsize_2.txt
../gcc_build.sh custom_allocator_test.cpp --big_stack --defines -DMAX_STACK_SIZE=100000000 -DEACH_VECTOR_SIZE=5 > ${PREFIX}_stacksize_100000000_vecsize_5.txt
../gcc_build.sh custom_allocator_test.cpp --big_stack --defines -DMAX_STACK_SIZE=100000000 -DEACH_VECTOR_SIZE=10 > ${PREFIX}_stacksize_100000000_vecsize_10.txt