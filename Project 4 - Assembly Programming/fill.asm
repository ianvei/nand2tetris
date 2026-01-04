// Program runs forever
// Continuously poll the keyboard memory-mapped register (KBD = RAM[24576])
//
// while (true):
//   if (RAM[24576] == 0):        // no key pressed
//     for (i = 16384; i < 24576; i++):   // iterate over screen memory
//       RAM[i] = 0               // clear screen (white)
//   else:                        // some key pressed
//     for (i = 16384; i < 24576; i++):   // iterate over screen memory
//       RAM[i] = -1              // fill screen (black)
//
// Notes:
// - Screen memory spans RAM[16384] to RAM[24575] (8192 words)
// - Each RAM word controls 16 horizontal pixels
// - Writing 0 clears pixels; writing -1 (all bits = 1) fills pixels
// - Program never halts; behavior is implemented via infinite looping

// main loop - always repeating
(main)

    //set up loop (same for draw and clear)
    @SCREEN
    D=A
    @index
    M=D 

    @KBD
    D=A 
    @endIndex
    M=D 
    
    //check keyboard register
    @KBD
    D=M
    @clear
    D;JEQ //jump to clear instruction if kbd value is 0  


    (draw)
        @index
        D=M 
        @endIndex 
        D=D-M 
        @main 
        D;JGE 

        //loop body 
        @index 
        A=M //A = value at RAM[index]
        M=-1 // M = value at RAM[A]
        

        //increment index and loop back to draw
        @index
        M=M+1
        @draw
        0;JMP

    (clear)
        @index
        D=M 
        @endIndex 
        D=D-M 
        @main 
        D;JGE 

        //loop body 
        @index 
        A=M
        M=0

        //increment index and loop back to clear
        @index
        M=M+1
        @clear
        0;JMP
   

@main
0;JMP 