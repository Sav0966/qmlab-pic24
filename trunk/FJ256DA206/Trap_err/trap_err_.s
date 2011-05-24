.global _illegal_opcode_func ; Export

.text
_illegal_opcode_func:
nop
.pbyte 0x00, 0xE4, 0x0B ; Illegal instruction opcode
nop
return
