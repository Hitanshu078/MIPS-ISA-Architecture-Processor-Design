#include <iostream>
#include <unordered_map>
using namespace std;
//defining functions 
void checkRFormatFields(uint32_t instruction);
void checkIFormatFields(uint32_t instruction);
uint32_t extractJumpTarget(uint32_t instruction);
void executeRFormatInstruction(uint32_t instruction);
void executeBranchInstruction(uint32_t instruction);
void executeIFormatInstruction(uint32_t instruction);
void executeJumpInstruction(uint32_t instruction);
void executeAddiInstruction(uint32_t instruction);
// Define HI and LO registers as global variables
int HI = 0;
int LO = 0;

// array of registers
int registerFile[32];

// Placeholder memory array and program counter (PC)
int memory[1024]; // Assuming a memory size of 1024

uint32_t PC = 0; // Program counter
//for printing register values if neede
void printRegisters() {
    cout << "Registers:" << endl;
    for (int i = 0; i < 32; ++i) {
        cout << "$" << i << ": " << registerFile[i] << endl;
    }
    cout << "HI: " << HI << endl;
    cout << "LO: " << LO << endl;
    cout << endl;
}
// Define control signals struct
struct ControlSignals {
    bool RegDst;    // Selects destination register number
    bool RegWrite;  // Enable write to register
    bool MemRead;   // Read data from memory
    bool MemWrite;  // Write data to memory
    bool MemToReg;  // Selects data source for register write
    bool ALUSrc;    // Selects second ALU operand
    int ALUOp;      // ALU operation code
};

// Function to get control signals based on opcode
ControlSignals getControlSignals(int opcode) {
    // Map opcode to control signals
    std::unordered_map<int, ControlSignals> controlSignalsMap = {
        {0b000000, {true, true, false, false, false, false, 2}}, // R-type instructions
        {0b100011, {true, true, true,  false, true,  true,  0}}, // LW
        {0b101011, {false, false, false, true,  false, true,  0}}, // SW
        {0b000100, {false, false, false, false, false, false, 2}}, // BEQ
        {0b000011, {true, true, false, false, false, true, 2}}, // JAL
        {0b001000, {true, true, false, false, false, true, 0}}, // ADDI
    };

    // Lookup control signals for the given opcode
    auto it = controlSignalsMap.find(opcode);
    if (it != controlSignalsMap.end()) {
        return it->second;
    } else {
        // Default control signals if opcode not found
        return {false, false, false, false, false, false, 0};
    }
}

// Function to check the instruction type and execute accordingly
void checkInstructionType(uint32_t binary_input) {
    uint32_t opcode = (binary_input >> 26) & 0x3F;

    // R-format opcode
    if (opcode == 0) {
        cout << "Instruction type: R-format" << endl;
        checkRFormatFields(binary_input);
    }
    // I-format opcode for load (lw) instruction
    else if (opcode == 0b100011) {
        cout << "Instruction type: I-format load (lw)" << endl;
        checkIFormatFields(binary_input);
    }
    // I-format opcode for store (sw) instruction
    else if (opcode == 0b101011) {
        cout << "Instruction type: I-format store (sw)" << endl;
        checkIFormatFields(binary_input);
    }
    // BEQ opcode
    else if (opcode == 0b000100) {
        cout << "Instruction type: I-format branch equal (beq)" << endl;
        checkIFormatFields(binary_input);
    }
    // Jump format opcodes
    else if (opcode == 0b000010 || opcode == 0b000011) {
        cout << "Instruction type: J-format jump" << endl;
        extractJumpTarget(binary_input);
    }
    // ADDI opcode
    else if (opcode == 0b001000) {
        cout << "Instruction type: I-format add immediate (addi)" << endl;
        checkIFormatFields(binary_input);
    } 
}

// Function to execute R-format instructions
void executeRFormatInstruction(uint32_t instruction) {
    // Extracting fields from the instruction
    uint32_t funct = instruction & 0x3F;         // Function code
    uint32_t rs = (instruction >> 21) & 0x1F;    // Source register
    uint32_t rt = (instruction >> 16) & 0x1F;    // Target register
    uint32_t rd = (instruction >> 11) & 0x1F;    // Destination register
    uint32_t shamt = (instruction >> 6) & 0x1F;  // Shift amount

    // Check the function field to determine the instruction type
    ControlSignals control = getControlSignals(0);
    switch (funct) {
        case 0x20: // Add
            if (control.RegWrite) {
                if (control.RegDst)
                    registerFile[rd] = registerFile[rs] + registerFile[rt];
                else
                    registerFile[rt] = registerFile[rs] + registerFile[rt];
            }
            cout << "Add: $" << rd << " = $" << rs << " + $" << rt <<"" << endl;
            break;
        case 0x22: // Sub
            if (control.RegWrite) {
                if (control.RegDst)
                    registerFile[rd] = registerFile[rs] - registerFile[rt];
                else
                    registerFile[rt] = registerFile[rs] - registerFile[rt];
            }
            cout << "Sub: $" << rd << " = $" << rs << " - $" << rt <<"" << endl;
            break;
        case 0x1A: // Divide (div)
            if (control.RegWrite) {
                LO = registerFile[rs] / registerFile[rt];
                HI = registerFile[rs] % registerFile[rt];
            }
            cout << "Divide: LO = $" << rs << " / $" << rt << ", HI = " << HI <<"" << endl;
            break;
        case 0x24: // And
            if (control.RegWrite) {
                if (control.RegDst)
                    registerFile[rd] = registerFile[rs] & registerFile[rt];
                else
                    registerFile[rt] = registerFile[rs] & registerFile[rt];
            }
            cout << "And: $" << rd << " = $" << rs << " & $" << rt <<"" << endl;
            break;
        case 0x25: // Or
            if (control.RegWrite) {
                if (control.RegDst)
                    registerFile[rd] = registerFile[rs] | registerFile[rt];
                else
                    registerFile[rt] = registerFile[rs] | registerFile[rt];
            }
            cout << "Or: $" << rd << " = $" << rs << " | $" << rt <<"" << endl;
            break;
        case 0x2A: // Set on less than (slt)
            if (control.RegWrite) {
                if (control.RegDst)
                    registerFile[rd] = (registerFile[rs] < registerFile[rt]) ? 1 : 0;
                else
                    registerFile[rt] = (registerFile[rs] < registerFile[rt]) ? 1 : 0;
            }
            cout << "Set on Less Than: $" << rd << " = ($" << rs << " < $" << rt << ")" << endl;
            break;
        case 0x18: // Mult
            if (control.RegWrite) {
                int64_t result = static_cast<int64_t>(registerFile[rs]) * static_cast<int64_t>(registerFile[rt]);
                HI = static_cast<int32_t>((result >> 32) & 0xFFFFFFFF);
                LO = static_cast<int32_t>(result & 0xFFFFFFFF);
                cout << "Mult: HI = " << HI << ", LO = " << LO <<"" << endl;
            }
            break;

        case 0x12: // Mflo
            if (control.RegWrite) {
                registerFile[rd] = LO;
            }
            cout << "Mflo: $" << rd << " = LO" <<"" << endl;
            break;
        case 0x10: // Mfhi
            // Move from HI register
            if (control.RegWrite) {
                registerFile[rd] = HI;
            }
            cout << "Mfhi: $" << rd << " = HI" <<"" << endl;
            break;
            
        case 0x08: // jr
            // Jump to the address stored in register $ra
            PC = registerFile[31];
            cout << "Jump to address stored in $ra: " << PC <<"" << endl;
            break;
        case 0x2F: // Move
            // Move operation
            if (control.RegWrite) {
                registerFile[rd] = registerFile[rs]; // Copy value from source register to destination register
            }
            cout << "Move: $" << rd << " = $" << rs <<"" << endl;
            break;
    }
}

// Function to execute I-format instructions
void executeIFormatInstruction(uint32_t instruction) {
    // Extracting fields from the instruction
    uint32_t opcode = (instruction >> 26) & 0x3F; // Opcode
    uint32_t rs = (instruction >> 21) & 0x1F;     // Source register
    uint32_t rt = (instruction >> 16) & 0x1F;     // Target register
    int32_t imm = static_cast<int16_t>(instruction & 0xFFFF);           // Immediate value (signed)

    // Perform operation based on the opcode
    ControlSignals control = getControlSignals(opcode);

    switch (opcode) {
        case 0b100011: // lw (load word)
            // Perform load operation
            if (control.RegWrite) {
                registerFile[rt] = memory[registerFile[rs] + imm];
            }
            cout << "lw: $" << rt << " = mem[$" << rs << " + " << imm << "]" <<"" << endl;
            cout << "mem[$" << rs << " + " << imm << "] ="<<memory[rs+imm]<<endl;
            break;
        case 0b101011: // sw (store word)
            // Perform store operation
            if (control.MemWrite) {
                memory[registerFile[rs] + imm] = registerFile[rt];
            }
            cout << "sw: mem[$" << rs << " + " << imm << "] = $" << rt << endl;
            break;
        case 0b000100: // beq (branch equal)
            // Perform branch equal operation
            if (registerFile[rs] == registerFile[rt]) {
                // Calculate the branch target address
                int32_t branch_target = (imm << 2)+PC+4; // PC + 4 + offset * 4

                // Update the program counter (PC) to the branch target address
                PC = branch_target-4;

                cout << "Branching to address: " << (branch_target+4)/4 << " (registers $" << rs << " and $" << rt << " are equal)" << endl;
            } else {
                // Branch not taken, continue to the next instruction
                cout << "Branch not taken (registers $" << rs << " and $" << rt << " are not equal)" << endl;
                
            }
            break;
        case 0b001000: // addi (add immediate)
            // Perform add immediate operation
            registerFile[rt] = registerFile[rs] + imm;
            cout << "ADDI: $" << rt << " = $" << rs << " + " << imm << "" <<endl;
            break;
    }
}

// Function to execute jump instructions
void executeJumpInstruction(uint32_t instruction) {
    // Extracting fields from the instruction
    uint32_t opcode = (instruction >> 26) & 0x3F; // Opcode
    uint32_t address = instruction & 0x03FFFFFF;  // Jump target address

    // Perform operation based on the opcode
    ControlSignals control = getControlSignals(opcode);

    switch (opcode) {
        case 0b000010: // j (jump)
            // Perform jump operation
            PC = address*4-4; // Set the program counter (PC) to the target address
            cout << "Jump to address: " << address+1 <<"" << endl;
            break;
        case 0b000011: // jal (jump and link)
            // Perform jump and link operation
            registerFile[31] = PC + 4; // Store the return address in register $ra
            PC = address; // Set the program counter (PC) to the target address
            cout << "Jump to address: " << address+1 << " and store return address in $ra" << endl;
            break;
    }
}

// Function to check R-format instruction fields
void checkRFormatFields(uint32_t instruction) {
    // Extracting fields from the instruction
    uint32_t rs = (instruction >> 21) & 0x1F;    // Source register
    uint32_t rt = (instruction >> 16) & 0x1F;    // Target register
    uint32_t rd = (instruction >> 11) & 0x1F;    // Destination register
    uint32_t shamt = (instruction >> 6) & 0x1F;  // Shift amount
    uint32_t funct = instruction & 0x3F;         // Function code

    // Outputting the fields
    cout << "Source Register (RS): " << rs << endl;
    cout << "Target Register (RT): " << rt << endl;
    cout << "Destination Register (RD): " << rd << endl;
    cout << "Shift Amount (SHAMT): " << shamt << endl;
    cout << "Function Code (Funct): " << funct << endl;

    // Executing the R-format instruction
    executeRFormatInstruction(instruction);
}

// Function to check I-format instruction fields
void checkIFormatFields(uint32_t instruction) {
    // Extracting fields from the instruction
    uint32_t rs = (instruction >> 21) & 0x1F;    // Source register
    uint32_t rt = (instruction >> 16) & 0x1F;    // Target register
    int32_t imm = instruction & 0xFFFF;          // Immediate field (signed)

    // Outputting the fields
    cout << "Source Register (RS): " << rs << endl;
    cout << "Target Register (RT): " << rt << endl;
    cout << "Immediate (signed): " << imm << endl;

    // Executing the I-format instruction
    executeIFormatInstruction(instruction);
}

// Function to extract jump target address
uint32_t extractJumpTarget(uint32_t instruction) {
    // Extracting the jump target address (26 bits)
    uint32_t jump_target = instruction & 0x03FFFFFF;

    // Left shift the jump target address by 2 bits (as jump targets are word addresses)
    jump_target <<= 2;

    // Executing the jump instruction
    executeJumpInstruction(instruction);

    return jump_target;
}

int main() {
    for(int i=0;i<1024;i++){
        memory[i]=0;
    }
    uint32_t instructions1[31] = {0b10001100000010110000000000000000,
        0b00100000000011100000000000000001,
        0b00100000000110000000000000000000,
        0b00000001011000000100000000101010,
        0b00010001000011100000000000010010,
        0b00000001011000000010000000100000,
        0b00001000000000000000000000000111,
        0b00000000100000001001100000100000,
        0b00000000100000001100100000100000,
        0b00100000000101000000000000000000,
        0b00100000000101010000000000001010,
        0b00010011001000000000000000001000,
        0b00000011001101010000000000011010,
        0b00000000000000000111100000010000,
        0b00000010100101010000000000011000,
        0b00000000000000001011000000010010,
        0b00000010110011111010000000100000,
        0b00000011001101010000000000011010,
        0b00000000000000001100100000010010,
        0b00001000000000000000000000001011,
        0b00010010011101000000000000000101,
        0b00100000000101100000000000000000,
        0b00001000000000000000000000011101,
        0b00100000000101101111111111111111,
        0b10101100000101100000000000000001,
        0b00001000000000000000000000011101,
        0b00100000000101100000000000000001,
        0b10101100000101100000000000000001,
        0b00001000000000000000000000011101,
        0b00100000000000100000000000001010,
        0b00000000000000000000000000001100
                
    };
    
    uint32_t instructions2[37]={0b10001100000010110000000000000000,
        0b10001100000011000000000000000001,
        0b00100000000010010000000000000001,
        0b00000001011000001000000000101010,
        0b00010010000010010000000000011011,
        0b00010001011011000000000000000011,
        0b00000001011011000100000000101010,
        0b00010001000000000000000000000001,
        0b00001000000000000000000000100000,
        0b00100000000110000000000000000001,
        0b00100000000011100000000000000001,
        0b00000001011000000111100000100000,
        0b00010001111000000000000000000100,
        0b00000001110011110000000000011000,
        0b00000000000000000111000000010010,
        0b00000001111110000111100000100010,
        0b00001000000000000000000000001100,
        0b00000001110000001001000000100000,
        0b00000001011011001010100000100010,
        0b00100000000011010000000000000001,
        0b00100000000110010000000000000001,
        0b00000010101000001011100000100000,
        0b00010010111000000000000000000100,
        0b00000011001101110000000000011000,
        0b00000000000000001100100000010010,
        0b00000010111011011011100000100010,
        0b00001000000000000000000000010110,
        0b00000011001000001001100000100000,
        0b00000010010100110000000000011010,
        0b00000000000000001011000000010010,
        0b10101100000101100000000000000010,
        0b00001000000000000000000000100011,
        0b00100000000101101111111111111111,
        0b10101100000101100000000000000010,
        0b00001000000000000000000000100011,
        0b00100000000000100000000000001010,
        0b00000000000000000000000000001100
    };
    uint32_t instructions3[57]={0b10001100000010000000000000000000,
        0b10001100000010010000000000000001,
        0b00100000000000010000000000000001,
        0b00010000001010000000000000110001,
        0b00100000000000010000000000000001,
        0b00010000001010010000000000101111,
        0b00000001000010010110000000101010,
        0b00100000000000010000000000000001,
        0b00010000001011000000000000000001,
        0b00001000000000000000000000001101,
        0b00000000000010000101100000100000,
        0b00000000000010010100000000100000,
        0b00000000000010110100100000100000,
        0b00000001000010010111000000100010,
        0b00100000000011110000000000000000,
        0b00100000000110010000000000000010,
        0b00000001000000001011000000100000,
        0b00010011001101100000000000001000,
        0b00000010110110010000000000011010,
        0b00000000000000000101000000010000,
        0b00010001010000000000000000000010,
        0b00100011001110010000000000000001,
        0b00001000000000000000000000010001,
        0b00100001111011110000000000000001,
        0b00100011001110010000000000000001,
        0b00001000000000000000000000011110,
        0b00000001111000001001000000100000,
        0b00100000000011110000000000000000,
        0b00100000000110010000000000000010,
        0b00000001001000001011000000100000,
        0b00010011001101100000000000001000,
        0b00000010110110010000000000011010,
        0b00000000000000000101000000010000,
        0b00010001010000000000000000000010,
        0b00100011001110010000000000000001,
        0b00001000000000000000000000011110,
        0b00100001111011110000000000000001,
        0b00100011001110010000000000000001,
        0b00001000000000000000000000011110,
        0b00000001111000001001100000100000,
        0b00010010010000000000000000000001,
        0b00001000000000000000000000110010,
        0b00010010011000000000000000000001,
        0b00001000000000000000000000110010,
        0b00100000000101000000000000000010,
        0b00010001110101000000000000000001,
        0b00001000000000000000000000110010,
        0b00100000000100000000000000000001,
        0b10101100000100000000000000000010,
        0b00001000000000000000000000110111,
        0b00100000000100001111111111111111,
        0b10101100000100000000000000000010,
        0b00001000000000000000000000110111,
        0b00100000000101110000000000000001,
        0b00001000000000000000000000110111,
        0b00100000000000100000000000001010,
        0b00000000000000000000000000001100
    };
    
    // User choice
    int choice;

    // Prompt user for choice
    cout << "Select an option:" << endl;
    cout << "1. Palindrome Checker" << endl;
    cout << "2. Number of permutations" << endl;
    cout << "3. Checking Twin Primes" << endl;
    cout << "Enter your choice (1, 2, or 3): ";
    cin >> choice;
    
    int numInstructions;
    
    // Execute instructions based on user choice
    switch (choice) {
        case 1:
            
            // Total number of instructions in the instruction memory
            // Total number of instructions in the instruction memory
            cout << "Give a number: ";
            int x;
            cin >> x;
            memory[0] = x;

            numInstructions = sizeof(instructions1) / sizeof(instructions1[0]);
            cout << "Checking if a number is palindrome or not:\n" << endl;
            while(PC < numInstructions*4) {
                cout << "Instruction " << PC/4 + 1 << ":" << endl;
                checkInstructionType(instructions1[PC/4]);
                //printRegisters();
                cout<<"PC:"<<PC<<"\n"<<endl;
                PC=PC+4;
            }
            cout<<"\n"<<endl;
            if(memory[1]>0){
                cout<<"Given number "<< memory[0] <<" is a palindrome.";
            }
            else if(memory[1]<0){
                cout<<"Given number "<< memory[0] <<" is not positive.";
            }
            else{
                cout<<"Given number "<< memory[0] <<" is not a palindrome.";
            }
            break;
        case 2:
            cout << "Calculating number of permutations:\n" << endl;
            cout << "Give a positive number: " << endl;
            int z;
            cin >> z;
            memory[0] = z;
            cout << "Give an another positive number: " << endl;
            int y;
            cin >> y;
            memory[1] = y;
            numInstructions = sizeof(instructions2) / sizeof(instructions2[0]);
            while(PC < numInstructions*4) {
                cout << "Instruction " << PC/4 + 1 << ":" << endl;
                checkInstructionType(instructions2[PC/4]);
                cout<<"PC:"<<PC<<"\n"<<endl;
                //printRegisters();
                PC=PC+4;
            }
            cout<<"\n"<<endl;
            if(registerFile[22]>0){
                cout<<"The number of permutations of given numbers are: "<<memory[2]<<endl;
            }
            else{
                cout<<"Invalid Input(First number should be greater than second)"<<endl;
            }
            break;
        case 3: 
            cout << "Calculating if two numbers are twin prime or not:\n" << endl;
            cout << "Give a number: " << endl;
            int i;
            cin >> i;
            memory[0] = i;
            cout << "Give an another number: " << endl;
            int h;
            cin >> h;
            memory[1] = h;
            numInstructions = sizeof(instructions3) / sizeof(instructions3[0]);
            while(PC < numInstructions*4) {
                cout << "Instruction " << PC/4 + 1 << ":" << endl;
                checkInstructionType(instructions3[PC/4]);
                cout<<"PC:"<<PC<<"\n"<<endl;
                //printRegisters();
                PC=PC+4;
            }
            cout<<"\n"<<endl;
            if(registerFile[23]==1){
                cout << "1 is neither prime nor composite" <<endl;
            }
            else if(memory[2]>0){
                cout<<"Given numbers "<< memory[0]<<" and "<<memory[1]<<" are twin primes.";
            }
            else{
                cout<<"Given numbers "<< memory[0]<<" and "<<memory[1]<<" are not twin primes.";
            }
            break;
        default: // Invalid choice
            cout << "Invalid choice! Please select 1, 2, or 3." << endl;
            break;
    }


    return 0;
}
