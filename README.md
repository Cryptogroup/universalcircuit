# UniversalCircuit
## Requirements
- A Linux distribution of your choice (The UC compiler was tested with Ubuntu and Kali)
- The following packages:  
  - g++
  - make
  - CMake
## UniversalCircuit Compliler
- Clone a copy of the main UniversalCircuit git repository and its submodules by running:
  - git clone https://github.com/SJTU-CS-Lattice/UniversalCircuit.git    
- Enter the UniversalCircuit directory: 
  - cd UniversalCircuit
- Choose one of the compilers [here](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html) (e.g. Unix Makefiles) and run the following commands:
  - cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles"
  - cmake --build ./ --target all -- -j 2

## Examples
### Included Example Circuits
- Our UC compiler's file input format is the same as that of [Encryptogroup](https://github.com/encryptogroup/UC/blob/master/README.md).
- And our UC compiler is also compatible with the test circuits for basic function as encryptogroup from [here](https://homes.esat.kuleuven.be/~nsmart/MPC/old-circuits.html)
### Testing Circuits
- If you are using the format of the circuits from [here]((https://homes.esat.kuleuven.be/~nsmart/MPC/old-circuits.html)), you can add your circuit under /Test (e.g. adder_32bit.txt) and set the name of your circuit, e.g., circuit_name, within bristol_to_SHDL.cpp and it creates standardized circuits based on input files and also inputs the resulting standardized circuits into UC. Run:
  - ./UC -ufile adder_32bit.txt

- You can also only create the standardized circuits but not run UC, run:
  - ./bristol adder_32bit.txt

- If you generated your circuit file using FairplayPF or you are done with the previous step, next the fanout-2 gates are eliminated and the UC and its programming are generated. For generating and testing the UC, run:
  - ./UC -sfile adder_32bit.txt_SHDL.circuit

- the UC argument “-sfile” indicates that the input is a standardized circuit file and output the generated UC circuit format and the structure of each x-type node
- the UC argument “-ufile” indicates that the input is a unstandardized circuit file and output the generated UC circuit format and the structure of each x-type node
- the result file will be stored in folder Output/
- There are two kinds of output files, respectively _prog.txt suffix and _circ.txt suffix, _prog.txt suffix represents whether each x-type structure is crossed, _circ.txt suffix represents the specific line of x-type circuit
For example, the output of _circ.txt is: X 1, 2, 3, 4, indicating that the input line of this node is 1 and 2 from left to right, and the output line is 3 and 4 from left to right. The 0 or 1 corresponding to _prog.txt represents whether the current is crossed or not
