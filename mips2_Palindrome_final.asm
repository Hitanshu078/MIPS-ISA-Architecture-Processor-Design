.data

.text

main:
    #Enter the value of n:
    lw $t3,0($zero)
    #addi $t3,$zero,1441

    # Checking condition : (n < 0)
    addi $t6,$zero,1
    addi $t8,$zero,0
    slt $t0, $t3,$zero
    beq $t0,$t6,negative
    add $a0,$t3,$zero
    j Checking_Palindrome

Checking_Palindrome:
    add $s3,$a0,$zero #originalnum
    add $t9,$a0,$zero #copy_originalnum
    addi $s4,$zero,0 #reversenum
    addi $s5,$zero,10
    loop:
        beq $t9 ,$zero ,loop_end
        div $t9 , $s5
        mfhi $t7
        mult $s4,$s5
        mflo $s6
        add $s4,$s6,$t7
        div $t9,$s5
        mflo $t9 
        j loop

    loop_end:
        beq $s3,$s4,true
        addi $s6,$zero,0
        j end

negative:
    addi $s6,$zero,-1
    sw $s6,1($zero)
    j end

true:
    addi $s6,$zero,1
    #sw $s6,1($zero)
    j end
    
end:
    addi $v0,$zero,10
    syscall
