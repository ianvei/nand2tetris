//R0 has main number
//R1 has second number (i.e 4*3, r0 is 4 r1 is 3)
//R2 stores result
// implement multiplication (R2 = R0 * R1)

//for(i=0; i<R1; i++){
//  R2 += R0 
//}

//step 1: init i 
@0
D=A
@i 
M=D 

//init R2, each run should be reset to 0 (dont want to accumulate result of mult between runs)
@0 
D=A 
@R2 
M=D 

//step 2: 'save' end (R1) value as 'end' register
@R1 
D=M 
@end 
M=D 

//step 3: loop condition
(loop_cond)
@i 
D=M 
@end 
D=D-M 
@loop_end 
D;JGE // jump to end if i-R1 >= 0

// step 4: loop body, do multiplication 
@R0 
D=M
@R2 
M=D+M 

// loop end, increment and check condition 
@i 
M=M+1

@loop_cond
0;JMP 

(loop_end)
