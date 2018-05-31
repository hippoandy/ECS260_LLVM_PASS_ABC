# ECS260_LLVM_PASS_ABC

This homework is to modified a pre-implemented LLVM Pass code to perform the array out-of-bound error check for a given program.

The material is provided by the instructor.

This homework is required to use the LLVM Docker container, which is available here:
[LLVM Docker](https://github.com/HGuo15/docker-llvm-3.8)

The homework is guided by the hint inside the code. We have to modify the file Instrument.hpp, Instrument.cpp, and check.c.
1. **Instrument.cpp**: the main logic to check the instructions of a given program.
2. **Instrument.hpp**: the header file of the LLVM Pass, we have to add additional lib headers to achieve the task.
3. **check.c**: the array boundary checking program.

Step 1: Find the correct instruction that is doing the array access

Step 2: Get the size of the array

Step 3: Get the pointer (index) of the array access

Step 4: Get the line number of a certain instruction in the code

Step 5: Prepare the arguments obtained previously for passing to the check.c logic

Step 6: Implement the check.c logic
