.data


.text
main:

    #"Enter the value of n: "
    lw $t3,0($zero)

    #"Enter the value of r: "
    lw $t4,1($zero)

    addi $t1,$zero,1

    # Checking condition : (n < 0)
    slt $s0,$t3,$zero
    beq $s0,$t1,not_applicable

    # Checking condition : (n = r)
    beq $t3, $t4, calculate_permutation

    # Checking condition : (n < r)
    slt $t0, $t3, $t4 
    beq $t0, $zero, calculate_permutation
    j not_applicable

calculate_permutation:
    # Calculating factorial(n)

    factorial1:  #For n!
        addi $t8,$zero,1
        addi $t6,$zero,1
        add $t7,$t3,$zero   # Load Input in $t3 into $t7 , substitute of move.
        loop1:
            beq $t7, $zero, loop_end1
            mult $t6, $t7
            mflo $t6
            sub $t7,$t7,$t8
            j loop1

        loop_end1:
            add $s2,$t6,$zero   # Return the result , substitute for move

    # Calculating factorial(n-r)

    sub $s5,$t3,$t4

    factorial2: #For (n-r)!
        addi $t5,$zero,1
        addi $t9,$zero,1
        add $s7,$s5,$zero   # Load Input in $t4 into $s7 , substitute of move.
        loop2:
            beq $s7, $zero, loop_end2
            mult $t9, $s7
            mflo $t9
            sub $s7,$s7,$t5
            j loop2

        loop_end2:
            add $s3,$t9,$zero   # Return the result , substitute for move


    div $s2,$s3 
    mflo $s6      #FINAL RESULT of nPr is in $s6 
    sw $s6,2($zero)
    j end
    

not_applicable:
    addi $s6,$zero,-1   #FINAL RESULT of nPr is in $s6 
    sw $s6,2($zero)
    j end

end:
    addi $v0,$zero,10
    syscall
