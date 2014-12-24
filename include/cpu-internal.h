#ifndef CPU_INTERNAL_H
#define CPU_INTERNAL_H

typedef enum {
    carry_flag     = 0x01,
    zero_flag      = 0x02,
    interrupt_flag = 0x04,
    decimal_flag   = 0x08,
    break_flag     = 0x10,
    unused_flag    = 0x20,
    overflow_flag  = 0x40,
    negative_flag  = 0x80
} cpu_p_flag;

typedef enum {
    carry_bp      = 0,
    zero_bp       = 1,
    interrupt_bp  = 2,
    decimal_bp    = 3,
    break_bp      = 4,
    unused_bp     = 5,
    overflow_bp   = 6,
    negative_bp   = 7
} cpu_p_bp;

typedef struct {
    word PC; // Program Counter,
    byte SP; // Stack Pointer,
    byte A, X, Y; // Registers
    byte P; // Flag Register
} CPU_STATE;

CPU_STATE cpu;

byte CPU_RAM[0x8000];

byte op_code;             // Current instruction code
int op_value, op_address; // Arguments for current instruction
int op_cycles;            // Additional instruction cycles used (e.g. when paging occurs)

unsigned long long cpu_cycles;  // Total CPU Cycles Since Power Up (wraps)

void (*cpu_op_address_mode[256])();       // Array of address modes
void (*cpu_op_handler[256])();            // Array of instruction function pointers
bool cpu_op_in_base_instruction_set[256]; // true if instruction is in base 6502 instruction set
char *cpu_op_name[256];                   // Instruction names
int cpu_op_cycles[256];                   // CPU cycles used by instructions

byte cpu_ram_read(word address);
void cpu_ram_write(word address, byte data);

// Interrupt Addresses
word cpu_nmi_interrupt_address();
word cpu_reset_interrupt_address();
word cpu_irq_interrupt_address();

// Updates Zero and Negative flags in P
void cpu_update_zn_flags(byte value);

// If OP_TRACE, print current instruction with all registers into the console
void cpu_trace_instruction();

// CPU Adressing Modes
void cpu_address_implied();
void cpu_address_immediate();
void cpu_address_zero_page();
void cpu_address_zero_page_x();
void cpu_address_zero_page_y();
void cpu_address_absolute();
void cpu_address_absolute_x();
void cpu_address_absolute_y();
void cpu_address_relative();
void cpu_address_indirect();
void cpu_address_indirect_x();
void cpu_address_indirect_y();

static const byte cpu_zn_flag_table[256] =
{
  zero_flag,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,
  negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,
  negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,
  negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,
  negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,
  negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,
  negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,
  negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,
  negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,
  negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,
  negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,
  negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,
  negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,
  negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,
  negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,
  negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,negative_flag,
};



#endif