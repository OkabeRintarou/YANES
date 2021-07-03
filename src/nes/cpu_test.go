package nes

import "testing"

func Test_0xa9_lda_immediate_load_data(t *testing.T) {
	cpu := NewCPU()
	if err := cpu.Run([]byte{0xa9, 0x05, 0x00}); err != nil {
		t.Fatal(err)
	}

	if cpu.registerA != 0x05 {
		t.Fatalf("")
	}
}
