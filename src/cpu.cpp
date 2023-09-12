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
	case 0x69:
	case 0x65:
	case 0x75:
	case 0x6D:
	case 0x7D:
	case 0x79:
	case 0x61:
	case 0x71:
		return make_unique<ADC>(op);
    case 0xA9:
    case 0xA5:
    case 0xB5:
    case 0xAD:
    case 0xBD:
    case 0xB9:
    case 0xA1:
    case 0xB1:
        return make_unique<LDA>(op);

	case 0xAA:
		return make_unique<TAX>(op);
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

void Processor::load(u8 *insts, size_t n) {
	status_.pc = 0x8000;
	memcpy(status_.memory + status_.pc, insts, n);
}

u16 Processor::get_operand_address(AddressingMode mode) const {
	switch (mode) {
	case AddressingMode::Immediate:
		return status_.pc;
	case AddressingMode::ZeroPage:
		return mem_read(status_.pc);
	case AddressingMode::ZeroPage_X:
		return 0;
	case AddressingMode::ZeroPage_Y:
		return 0;
	case AddressingMode::Absolute_X:
		return 0;
	case AddressingMode::Absolute_Y:
		return 0;
	case AddressingMode::Indirect_X:
		return 0;
	case AddressingMode::Indirect_Y:
		return 0;
	default:
		assert(0);
		break;
	}
}

void Instruction::update_zero_and_negative_flags(Status &status, const u8 v) {
	status.set_zero_flag(v == 0);
	status.set_negative_flag(is_negative(v));
}

void ADC::execute(Status &status) {
	
}

void LDA::execute(Status &status) {
	AddressingMode mode{};

    switch (op_code_) {
    case 0xA9:
		mode = AddressingMode::Immediate;
        break;
	case 0xA5:
		mode = AddressingMode::ZeroPage;
		break;
	case 0xB5:
		mode = AddressingMode::ZeroPage_X;
		break;
	case 0xAD:
		mode = AddressingMode::Absolute;
		break;
	case 0xBD:
		mode = AddressingMode::Absolute_X;
		break;
	case 0xB9:
		mode = AddressingMode::Absolute_Y;
		break;
	case 0xA1:
		mode = AddressingMode::Indirect_X;
		break;
	case 0xB1:
		mode = AddressingMode::Indirect_Y;
		break;
	default:
		assert(0);
		break;
    }
    u16 addr = get_operand_address(mode);
    status.acc = p;
		update_zero_and_negative_flags(status, p);
}

void BRK::execute(Status &status) { status.set_break_command(true); }

void TAX::execute(Status &status) {
	status.x = status.acc;
	update_zero_and_negative_flags(status, status.x);
}
