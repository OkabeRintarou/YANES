#pragma once

#include <cassert>
#include <cstdint>
#include <memory>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;

struct Status {
    Status() {}
    u8 *insts = nullptr;
    std::size_t size = 0;
    u16 pc = 0;
    u8 acc = 0;
    u8 flag = 0;

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

  protected:
    u8 op_code_;
};

#define DEFINE_INST(INST)                                                      \
    struct INST : public Instruction {                                         \
        INST(u8 op) : Instruction(op) {}                                       \
        void execute(Status &status) override;                                 \
    };

DEFINE_INST(LDA)
DEFINE_INST(BRK)

#undef DEFINE_INST

class Processor {
  public:
    std::unique_ptr<Instruction> decode();
    void run(u8 *insts, std::size_t n);

    const Status &status() const { return status_; }

  private:
    Status status_;
};
