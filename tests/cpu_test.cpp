#include "nes.h"
#include <gtest/gtest.h>

TEST(BasicInst, test_0xa9_lda_immediate_load_data) {
    Processor cpu;
    u8 insts[] = {0xa9, 0x05, 0x00};
    cpu.run(insts, 3);
    EXPECT_FALSE(cpu.status().is_zero());
    EXPECT_FALSE(cpu.status().is_negative());
}

TEST(BasicInst, test_0xa9_lda_zero_flag) {
    Processor cpu;
    u8 insts[] = {0xa9, 0x00, 0x00};
    cpu.run(insts, 3);
    EXPECT_TRUE(cpu.status().is_zero());
}

TEST(BasicInst, test_0xaa_tax_move_a_to_x) {
    Processor cpu;
	cpu.status().acc = 10;
    u8 insts[] = {0xaa, 0x00};
	cpu.run(insts, 2);
	ASSERT_EQ(cpu.status().x, 10);
}
