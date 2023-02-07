# Running my custom allocator tests

Just run `./test.sh`

For more details of the test commands:

`../gcc_build.sh custom_allocator_test.cpp --big_stack --defines -DMAX_STACK_SIZE=100000000 -DEACH_VECTOR_SIZE=2`

will generate a gcc command like this:

`g++12 -DMAX_STACK_SIZE=100000000 -DEACH_VECTOR_SIZE=2 -o main ../${FILE_NAME} -std=c++20 -O3 -ftrack-macro-expansion=0 -fpermissive -g -Wl,-stack_size,0x10000000,-stack_addr,0xc0000000`

- `--big_stack` adds `-Wl,-stack_size,0x10000000,-stack_addr,0xc0000000` to make the available stack bigger (1G in this case).
- `EACH_VECTOR_SIZE` is the size of each small vector we will create.
- `MAX_STACK_SIZE`is the max stack size in bytes hardcoded in the example to derive how may vectors (`MAX_STACK_SIZE/sizeof(max_align_t)/EACH_VECTOR_SIZE`) we can create with the available stack space.


## Running std_vector_example.cpp
`../gcc_build.sh std_vector_example.cpp`


### Compiler: `g++-12 (Homebrew GCC 12.2.0) 12.2.0`

`g++12` points to g++-12 installed by homebrew
- in my `~/.zprofile` ::::: `alias g++12="/opt/homebrew/Cellar/gcc/12.2.0/bin/g++-12"`


