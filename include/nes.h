#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <memory>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;

class Processor;

struct Status {
    explicit Status(Processor *p) {
		memset(memory, 0, sizeof(memory));	
		p_ = p;
	}
	Processor *p_ = nullptr;
    u8 *insts = nullptr;
    std::size_t size = 0;
    u16 pc = 0;
    u8 acc = 0;
    u8 flag = 0;
	u8 x = 0;
	u8 memory[0xffff];

    static constexpr u8 CARRY_FLAG = 0b00000001;
    static constexpr u8 ZERO_FLAG = 0b00000010;
    static constexpr u8 INTERRUPT_DISABLE = 0b00000100;
    static constexpr u8 DECIMAL_MODE = 0b00001000;
    static constexpr u8 BREAK_COMMAND = 0b00010000;
    static constexpr u8 OVERFLOW_FLAG = 0b00100000;
    static constexpr u8 NEGATIVE_FLAG = 0b01000000;

    void set_zero_flag(bool t) { t ? flag |= ZERO_FLAG : flag &= ~ZERO_FLAG; }
    bool is_zero() const { return flag & ZERO_FLAG; }

    void set_carry_flag(bool t) {
        t ? flag |= CARRY_FLAG : flag &= ~CARRY_FLAG;
    }
    bool is_carry() const { return flag & CARRY_FLAG; }

    void set_interrupt_disable(bool t) {
        t ? flag |= INTERRUPT_DISABLE : flag &= ~INTERRUPT_DISABLE;
    }
    bool is_interrupt_disable() const { return flag & INTERRUPT_DISABLE; }

    void set_decimal_mode(bool t) {
        t ? flag |= DECIMAL_MODE : flag &= ~DECIMAL_MODE;
    }
    bool is_decimal_mode() const { return flag & DECIMAL_MODE; }

    void set_break_command(bool t) {
        t ? flag |= BREAK_COMMAND : flag &= ~BREAK_COMMAND;
    }
    bool is_break_command() const { return flag & BREAK_COMMAND; }

    void set_overflow_flag(bool t) {
        t ? flag |= OVERFLOW_FLAG : flag &= ~OVERFLOW_FLAG;
    }
    bool is_overflow() const { return flag & OVERFLOW_FLAG; }

    void set_negative_flag(bool t) {
        t ? flag |= NEGATIVE_FLAG : flag &= ~NEGATIVE_FLAG;
    }
    bool is_negative() const { return flag & NEGATIVE_FLAG; }
};

inline bool is_negative(u8 v) { return v & Status::NEGATIVE_FLAG; }

struct Instruction {
    Instruction(u8 op) : op_code_(op) {}
    virtual void execute(Status &staus) = 0;
	void update_zero_and_negative_flags(Status &status, const u8 v);

  protected:
    u8 op_code_;
};

#define DEFINE_INST(INST)                                                      \
    struct INST : public Instruction {                                         \
        INST(u8 op) : Instruction(op) {}                                       \
        void execute(Status &status) override;                                 \
    };

DEFINE_INST(ADC)
DEFINE_INST(AND)
DEFINE_INST(ASL)
DEFINE_INST(BCC)
DEFINE_INST(BCS)
DEFINE_INST(BEQ)
DEFINE_INST(BIT)
DEFINE_INST(BMI)
DEFINE_INST(BNE)
DEFINE_INST(BPL)
DEFINE_INST(BRK)
DEFINE_INST(BVC)
DEFINE_INST(BVS)
DEFINE_INST(CLC)
DEFINE_INST(CLD)
DEFINE_INST(CLI)
DEFINE_INST(CLV)
DEFINE_INST(CMP)
DEFINE_INST(CPX)
DEFINE_INST(CPY)
DEFINE_INST(DEC)
DEFINE_INST(DEX)
DEFINE_INST(DEY)
DEFINE_INST(EOR)
DEFINE_INST(INC)
DEFINE_INST(INX)
DEFINE_INST(INY)
DEFINE_INST(JMP)
DEFINE_INST(JSR)
DEFINE_INST(LDA)
DEFINE_INST(LDX)
DEFINE_INST(LDY)
DEFINE_INST(LSR)
DEFINE_INST(NOP)
DEFINE_INST(ORA)
DEFINE_INST(PHA)
DEFINE_INST(PHP)
DEFINE_INST(PLA)
DEFINE_INST(PLP)
DEFINE_INST(ROL)
DEFINE_INST(ROR)
DEFINE_INST(RTI)
DEFINE_INST(RTS)
DEFINE_INST(SBC)
DEFINE_INST(SEC)
DEFINE_INST(SED)
DEFINE_INST(SEI)
DEFINE_INST(STA)
DEFINE_INST(STX)
DEFINE_INST(STY)
DEFINE_INST(TAX)
DEFINE_INST(TAY)
DEFINE_INST(TSX)
DEFINE_INST(TXA)
DEFINE_INST(TXS)
DEFINE_INST(TYA)

#undef DEFINE_INST

enum class AddressingMode {
	Immediate,
	ZeroPage,
	ZeroPage_X,
	ZeroPage_Y,
	Absolute,
	Absolute_X,
	Absolute_Y,
	Indirect_X,
	Indirect_Y,
};

class Processor {
  public:
    std::unique_ptr<Instruction> decode();
	void load_and_run(u8 *insts, std::size_t n) {
		load(insts, n);
		run(insts, n);
	}
    void run(u8 *insts, std::size_t n);
	void load(u8 *inst, std::size_t n);

    const Status &status() const { return status_; }
	Status &status() { return status_; }

  private:
	u8 mem_read(u16 pc) const;
	u16 mem_read_u16(u16 pc) const;

	u16 get_operand_address(AddressingMode mode) const;
  private:
    Status status_;
};
