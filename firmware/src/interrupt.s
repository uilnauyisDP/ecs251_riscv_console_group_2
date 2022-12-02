.section .text, "ax"
.global _interrupt_handler,InitContext,SwitchContext
_interrupt_handler:
    csrw    mscratch,ra
    csrr    ra,mcause
    addi    ra,ra,-11
    beqz    ra,_system_call
    csrr    ra,mscratch
    addi	sp,sp,-44
    sw	    gp,40(sp)
    sw	    ra,36(sp)
    sw	    t0,32(sp)
    sw	    t1,28(sp)
    sw	    t2,24(sp)
    sw	    a0,20(sp)
    sw	    a1,16(sp)
    sw	    a2,12(sp)
    sw	    a3,8(sp)
    sw	    a4,4(sp)
    sw	    a5,0(sp)
    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop
    csrr    a0,mcause
    call    c_interrupt_handler
    lw	    gp,40(sp)
    lw	    ra,36(sp)
    lw	    t0,32(sp)
    lw	    t1,28(sp)
    lw	    t2,24(sp)
    lw	    a0,20(sp)
    lw	    a1,16(sp)
    lw	    a2,12(sp)
    lw	    a3,8(sp)
    lw	    a4,4(sp)
    lw	    a5,0(sp)
    addi    sp,sp,40
    mret
_system_call:
    csrr    ra,mscratch
    csrw    mepc,ra
    csrw    mscratch,gp
    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop
    call    c_system_call
    csrr    gp,mscratch
    mret
#
# multithreads
#
# the thread got the entry function address
InitContext:
    # stacktop(stack pointer) in main.c
    addi    a0,a0,-56
    sw      a1,52(a0)
    sw      gp,48(a0)
    sw      zero,44(a0)
    sw      zero,40(a0)
    sw      zero,36(a0)
    sw      zero,32(a0)
    sw      zero,28(a0)
    sw      zero,24(a0)
    sw      a2,20(a0)
    sw      zero,16(a0)
    sw      zero,12(a0)
    sw      zero,8(a0)
    sw      zero,4(a0)
    sw      zero,0(a0)
    ret
# 16 registers 4 bytes each
# we don't need sp and zero
SwitchContext:
    # store old stack
    addi	sp,sp,-56
    sw      ra,52(sp)
    sw      gp,48(sp)
    sw      tp,44(sp)
    sw      t0,40(sp)
    sw      t1,36(sp)
    sw      t2,32(sp)
    sw      s0,28(sp)
    sw      s1,24(sp)
    sw      a0,20(sp)
    sw      a1,16(sp)
    sw      a2,12(sp)
    sw      a3,8(sp)
    sw      a4,4(sp)
    sw      a5,0(sp)
    # Save old sp -> a0 + 0
    sw      sp,0(a0)
    # Restore new sp from a1
    mv      sp,a1
    lw      ra,52(sp)
    lw      gp,48(sp)
    lw      tp,44(sp)
    lw      t0,40(sp)
    lw      t1,36(sp)
    lw      t2,32(sp)
    lw      s0,28(sp)
    lw      s1,24(sp)
    lw      a0,20(sp)
    lw      a1,16(sp)
    lw      a2,12(sp)
    lw      a3,8(sp)
    lw      a4,4(sp)
    lw      a5,0(sp)
    addi	sp,sp,56
    ret