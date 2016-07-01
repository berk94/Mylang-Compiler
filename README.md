# Mylang-Compiler
CMPE230_project1_Bogazici_University
README

According to the homework description pdf, my project does not have any shortcomings and is complete. 

I assumed in this project that each token in the input file is separated by at least one space character. Since the conditions of being a valid ID are not specified, I assumed every string other than the ones starting with integers 0 through 9 and the terminals in the parse tree can be declared without problem. 

I did not use any special libraries in this program – nothing that STL does not include. I only included <iostream>, <fstream> and <set>.

To compile the program, just write “make” as I also added a makefile. 

To run the program, enter “./prog.out input.txt output.asm” to the commandline, assuming that input.txt is in the same directory as the executable prog.out. The a86 assembly code will be in the output.asm file, which will also be created under the same directory with the executable.
