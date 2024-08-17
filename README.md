# MIPS ISA Processor Design

## Course: EG 212 - Computer Architecture - Processor Design

## Introduction

The MIPS (Microprocessor without Interlocked Pipeline Stages) architecture is a Reduced Instruction Set Computer (RISC) architecture developed by MIPS Computer Systems. Introduced in 1981, the MIPS architecture is renowned for its simplicity, high performance, and low power consumption, making it suitable for a wide range of applications from embedded systems to supercomputers.

Our project focuses on three main areas:

1. **Assembly Programming**: Translating C++ programs into MIPS assembly language.
2. **Simulation**: Assembling and running these programs using the MARS MIPS simulator.
3. **Processor Design**: Building and testing a single non-pipelined MIPS processor.

## Key Components

### 1. Instruction Set Architecture (ISA)

The MIPS ISA is characterized by:
- **Fixed-Length Instructions**: Each instruction is 32 bits long.
- **Instruction Types**:
  - **R-type**: Used for arithmetic and logical operations.
  - **I-type**: Used for data transfer and immediate operations.
  - **J-type**: Used for jump and branch operations.
- **Registers**: Includes 32 general-purpose registers ($0 to $31) and 32 floating-point registers ($f0 to $f31).
- **Endianness**: Supports both little-endian and big-endian byte orderings.

### 2. Memory Organization

MIPS architecture utilizes:
- A flat, linear memory model.
- Byte-addressable memory with a typical address space of 32 or 64 bits.
- Support for virtual memory, allowing programs to use more memory than physically available.

### 3. Pipeline Stages

The architecture employs a five-stage pipeline to enhance performance:
- **Instruction Fetch (IF)**
- **Instruction Decode (ID)**
- **Execute (EX)**
- **Memory Access (MEM)**
- **Write Back (WB)**

### 4. Privileged Instructions

MIPS includes special instructions for:
- Operating system and kernel-level operations.
- Controlling hardware resources like interrupt controllers and memory management units (MMUs).

### 5. Multi-threading and Multi-processing

MIPS supports:
- **Multi-threading**: Concurrent execution of multiple threads on a single core.
- **Multi-processing**: Concurrent execution across multiple processor cores.

### 6. Exception Handling

MIPS processors manage exceptions (interrupts, system calls, errors) by:
- Switching to kernel mode and executing predefined exception handlers.

## Project Tasks

### C++ Codes and Assembly Implementations

We have translated three C++ programs into MIPS assembly language. Here’s a brief overview of each:

#### 1. Number of Permutations

- **C++ Code**: Calculates permutations of two numbers using the formula \( \frac{n!}{(n - r)!} \).
- **MIPS Assembly**: Converts the factorial calculation into MIPS assembly instructions.

#### 2. Palindrome Checker

- **C++ Code**: Determines if a given number is a palindrome.
- **MIPS Assembly**: Implements the palindrome checking logic in MIPS assembly.

#### 3. Checking Twin Primes

- **C++ Code**: Checks if two given numbers are twin primes (primes that differ by 2).
- **MIPS Assembly**: Translates the twin primes logic into MIPS assembly.

### Memory Data

The memory data used for processor operations is provided in the file `MemoryData.txt`. This data is generated from MARS and is essential for running the simulations.

### Processor Design

Our processor design involves:
- Reading instructions from memory.
- Decoding instructions based on R, I, or J formats.
- Executing instructions through a five-stage pipeline.

The complete processor code can be found in `Non-Pipelined Processor.cpp`.

## Execution Results

The results of running the Palindrome Checker code are documented with screenshots in the `Results` folder. These results demonstrate the processor's ability to correctly execute the assembly code and validate its functionality.

## Getting Started

To explore our project, you can:

1. **View Code**: Check out the C++ and MIPS assembly code in the respective files.
2. **Run Simulations**: Use the MARS MIPS simulator to assemble and run the MIPS assembly code.
3. **Review Results**: Look at the `Results` folder for execution screenshots and results.

## Contact

For questions or additional information, feel free to reach out.

We hope you find this project insightful and informative as you explore the MIPS architecture and processor design. Happy coding!

