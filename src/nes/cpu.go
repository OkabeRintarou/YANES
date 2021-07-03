package nes

import (
	"errors"
	"fmt"
)

type CPU struct {
	registerA uint8
	status    uint8
	pc        uint16

	tables []func(*CPU, []byte) ()
}

func NewCPU() *CPU {
	c := &CPU{
		registerA: 0,
		status:    0,
		pc:        0,
		tables:    make([]func(*CPU, []byte) (), 256, 256),
	}
	c.tables[0xa9] = (*CPU).ldaA9

	return c
}

func (c *CPU) ldaA9(p []byte) {
	param := p[c.pc]
	c.pc += 1
	c.registerA = param

	if c.registerA == 0 {
		c.status |= 0b0000_0010
	} else {
		c.status &= 0b1111_1101
	}

	if (c.registerA & 0b1000_0000) != 0 {
		c.status |= 0b1000_0000
	} else {
		c.status &= 0b0111_1111
	}
}

func (c *CPU) Run(program []byte) error {
	if program == nil || len(program) == 0 {
		return errors.New("invalid empty program")
	}

	var opcode uint8
	c.pc = 0

	for int(c.pc) < len(program) {
		opcode = program[c.pc]
		c.pc += 1

		if opcode == 0x00 {
			break
		}
		if fun := c.tables[opcode]; fun != nil {
			fun(c, program)
		} else {
			return errors.New(fmt.Sprintf("invalid opcode (%d)", opcode))
		}
	}
	return nil
}
