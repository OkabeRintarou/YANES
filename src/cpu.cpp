#include "nes.h"
#include <iostream>

using namespace std;

unique_ptr<Instruction> Processor::decode() {
    if (status_.size == 0)
        return nullptr;

    u8 op = status_.insts[status_.pc++];
    switch (op) {
    case 0x00:
        return make_unique<BRK>(op);
    case 0xA9:
    case 0xA5:
    case 0xB5:
    case 0xAD:
    case 0xBD:
    case 0xB9:
    case 0xA1:
    case 0xB1:
        return make_unique<LDA>(op);
    }

    return nullptr;
}

void Processor::run(u8 *insts, size_t n) {
    status_.insts = insts;
    status_.size = n;

    for (;;) {
        auto inst = decode();
        assert(inst != nullptr);
        inst->execute(status_);
        if (status_.is_break_command())
            break;
    }
}

void LDA::execute(Status &status) {
    switch (op_code_) {
    case 0xA9:
        u8 p = status.insts[status.pc++];
        status.acc = p;
        status.set_zero_flag(p == 0);
        status.set_negative_flag(is_negative(p));

        break;
    }
}

void BRK::execute(Status &status) { status.set_break_command(true); }
