.data


.text
    #Input the first number n1 and store it in $t0.
    lw $t0,0($zero)
    
    #Input the second number n2 and store it in $t1.
    lw $t1,1($zero)
    
    check:
    	beq $t0,1,end1
    	beq $t1,1,end1
    check2:
    	slt $t4,$t0,$t1
    	beq $t4,1,swap
    	j continue
    swap:
    	add $t3,$zero,$t0
    	add $t0,$zero,$t1
    	add $t1,$zero,$t3
    continue:
    	sub $t6,$t0,$t1

    prime1:
        addi $t7,$zero,0 
        addi $t9,$zero,2 #value of 'i'
        add $s6,$t0,$zero 
        loop1:
            beq $t9,$s6,loop_end1
            div $s6,$t9
            mfhi $t2 #Storing remainder in $t2
            beq $t2,$zero,increment_c1
            addi $t9,$t9,1
            j loop1

        increment_c1:
            addi $t7,$t7,1
            addi $t9,$t9,1
            j loop2

        loop_end1:
            add $s2,$t7,$zero
        
    prime2:
        addi $t7,$zero,0 
        addi $t9,$zero,2 #value of 'i'
        add $s6,$t1,$zero 
        loop2:
            beq $t9,$s6,loop_end2
            div $s6,$t9
            mfhi $t2 #Storing remainder in $t2
            beq $t2,$zero,increment_c2
            addi $t9,$t9,1
            j loop2

        increment_c2:
            addi $t7,$t7,1
            addi $t9,$t9,1
            j loop2

        loop_end2:
            add $s3,$t7,$zero

    beq $s2,$zero,true1
    j false

true1:
    beq $s3,$zero,true2
    j false

true2:
    addi $s4,$zero,2
    beq $t6,$s4,result
    j false

result:
    addi $s0,$zero,1 #Final Result in $s0
    sw $s0,2($zero)
    j end

false:
    addi $s0,$zero,-1 #Final Result in $s0
    sw $s0,2($zero)
    j end

end1:
    addi $s7,$zero,1 
    j end

end:
    addi $v0,$zero,10
    syscall



