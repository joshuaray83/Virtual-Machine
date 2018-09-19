/*
    Name:   Josh Heyer
    Email:  joshuaray83@gmail.com


    Function: This program emulates a virtual machine. It operates by taking
    in commands one line at a time. Each command comes in three parts. The
    first section of the command is the memory location to save the command,
    the second section of the command is the operation to perform, the third
    section of the command is the value saved into the memory location.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void HALT();
long convertToBinary(long);
int convertToNum(char);
char convertToPrint(int);

int Accumulator = 0;
long yy, xx, zz;
int mem[400];
int InstructionCounter = 0;
int InstructionRegister = 0;
int OperationCode = 0;
int Operand = 0;
int halt = 0;
int x = 0;
int location, y, z, w, ex;
char temp, temp1, temp2, temp3;

/*
    Retrieves a value from the user and places it in the given address.
*/
void READ (int oper) {

    ex = 0;
    printf("Please enter a number(press enter twice if entering only one number): ");
    scanf("%c", &temp);
    scanf("%c", &temp1);
    if (!isspace(temp1)) {
        scanf("%c", &temp2);
    }
    if (!isspace(temp2)) {
        scanf("%c", &temp3);
    }
    if (temp1 == '\n') {
        ex = convertToNum(temp); mem[4*oper+3] = ex;
    }
    else if (temp2 == '\n') {
        ex = convertToNum(temp); mem[4*oper+2] = ex;
        ex = convertToNum(temp1); mem[4*oper+3] = ex;
    }
    else if (temp3 == '\n') {
        ex = convertToNum(temp); mem[4*oper+1] = ex;
        ex = convertToNum(temp1); mem[4*oper+2] = ex;
        ex = convertToNum(temp2); mem[4*oper+3] = ex;
    }
    else {
        ex = convertToNum(temp); mem[4*oper] = ex;
        ex = convertToNum(temp1); mem[4*oper+1] = ex;
        ex = convertToNum(temp2); mem[4*oper+2] = ex;
        ex = convertToNum(temp3); mem[4*oper+3] = ex;
    }
    ex = 0;
}

/*
    Outputs a word from the given address to the terminal.
*/
void WRIT (int oper) {

    printf("The word at memory location %d is: ", oper);
    printf("%d", mem[oper*4]); printf("%d", mem[(oper*4)+1]);
    printf("%d", mem[(oper*4)+2]); printf("%d", mem[(oper*4)+3]);
    printf("\n");
}

/*
    Outputs a string starting at the given address, will continue outputting consecutive
    words as strings until NULL is reached.
*/
void PRNT (int oper) {

    ex = 0;
    do {
        ex = (mem[oper*4] * 10) + mem[(oper*4)+1];
        temp = convertToPrint(ex);
        if (ex == 0) {
            break;
        }
        printf ("%c", temp);
        ex = (mem[(oper*4)+2] *10) + mem[(oper*4)+3];
        if (ex == 0) {
            break;
        }
        temp = convertToPrint(ex);
        printf ("%c", temp);
        oper+=1;
    } while (mem[oper] != 0);
}

/*
    Load a word from the given memory address into the accumulator. The word is decoded
    into a binary value.
*/
void LOAD (int oper) {

    ex = 0;
    ex = (mem[oper*4]*1000) + (mem[(oper*4)+1]*100) + (mem[(oper*4)+2]*10) 
        + (mem[(oper*4)+3]);
    if (ex < 10000) {
        Accumulator = ex;
    }
    else {
        printf("Runtime Error: Word Overflow\n");
    }
}

/*
    Store the word in the accumulator into the given address. The value is encoded into
    ASCII format.
*/
void STOR (int oper) {

}

/*
    Stores the given word into the preceeding address (Note: The operation code value of
    22 will never appear in a compiled program. It is only included for completeness).
*/
void SET (int oper) {

}

/*
    Add the word at the given memory address to the accumulator: acc = acc + operand.
*/
void ADD (int oper) {

    ex = 0;
    ex = (mem[oper*4]*1000) + (mem[(oper*4)+1]*100) + (mem[(oper*4)+2]*10) 
        + (mem[(oper*4)+3]);
    if ((Accumulator + ex) < 10000) {
        Accumulator += ex;
    }
    else {
        printf("Runtime Error: Word Overflow\n");
        HALT();
    }
}

/*
    Subtract the word at the given memory address from the accumulator: acc = acc - operand.
*/
void SUB (int oper) {

    ex = 0;
    ex = (mem[oper*4]*1000) + (mem[(oper*4)+1]*100) + (mem[(oper*4)+2]*10) 
        + (mem[(oper*4)+3]);
    if ((Accumulator - ex) < 10000) {
        Accumulator -= ex;
    }
    else {
        printf("Runtime Error: Word Overflow\n");
        HALT();
    }
}

/*
    Divide the accumulator w/ the word at given memory location. acc = acc / operand.
*/
void DIV (int oper) {

    ex = 0;
    ex = (mem[oper*4]*1000) + (mem[(oper*4)+1]*100) + (mem[(oper*4)+2]*10) 
        + (mem[(oper*4)+3]);
    if (ex == 0) {
        printf("Runtime Error: Unable to divide by zero\n");
        HALT();
    }
    else if ((Accumulator / ex) < 10000) {
        Accumulator /= ex;
    }
    else {
        printf("Runtime Error: Word Overflow\n");
        HALT();
    }
}

/*
    Multiply the word at given memory address to the accumulator: acc = acc * operand.
*/
void MULT (int oper) {

    ex = 0;
    ex = (mem[oper*4]*1000) + (mem[(oper*4)+1]*100) + (mem[(oper*4)+2]*10) 
        + (mem[(oper*4)+3]);
    if ((Accumulator * ex) < 10000) {
        Accumulator *= ex;
    }
    else {
        printf("Runtime Error: Word Overflow\n");
        HALT();
    }
}

/*
    Mods the word at the given memory address to the accumulator: acc = acc % operand.
*/
void MOD (int oper) {

    ex = 0;
    ex = (mem[oper*4]*1000) + (mem[(oper*4)+1]*100) + (mem[(oper*4)+2]*10) 
        + (mem[(oper*4)+3]);
    if ((Accumulator % ex) < 10000) {
        Accumulator %= ex;
    }
    else {
        printf("Runtime Error: Word Overflow\n");
        HALT();
    }
}

/*
    Execution jumps to the given memory locaation.
*/
int BRAN (int oper) {

    ex = 0;
    ex = oper*4;
    return ex;
}

/*
    Execution jumps to the given memory location if the accumulator is negative.
*/
int BRNG (int oper) {

    ex = 0;
    ex = oper*4;
    return ex;
}

/*
    Execution jumps to the given memory location if the accumulator is zero.
*/
int BRZR (int oper) {

    ex = 0;
    ex = oper*4;
    return ex;
}

/*
    Terminates execution, no address given, value of 99 is standard. Also prints out the
    state of memory in a tabular format as displayed in Word document.
*/
void HALT () {

    printf("REGISTERS:\naccumulator\t\t+%04d\ninstructionCounter\t%02d\ninstructionRegister\t+%04d\noperationCode\t\t%02d\noperand\t\t\t%02d\n", Accumulator, InstructionCounter, InstructionRegister, OperationCode, Operand);
    printf("MEMORY:\n");
    printf("\t0\t1\t2\t3\t4\t5\t6\t7\t8\t9");
    for (x = 0; x < 400; x++) {
        if ((x%40) == 0) {
            printf("\n%d", x/4);
        }
        if ((x%4) == 0 && x != 0) {
            printf("\t+");
        }
        if (x == 0) {
            printf("\t+");
        }
        printf("%d", mem[x]);
    }
    printf("\n");
    exit(0);
}

/*
    This function takes in a number and converts it to its binary equivilent.
*/
long convertToBinary (long n) {

    w = 0; yy = 0; zz = 1;
    printf("n = %ld\n", n);
    while (n != 0) {
        w = n%2;
        n = n/2;
        yy = yy + (w*zz);
        zz = zz * 10;
    }
    printf("yy = %ld\n", yy);
    return yy;
}

/*
    This function simply takes in a two-digit number and converts it to its ASCII letter.

*/
char convertToPrint (int a) {

    char b;
    switch (a) {
        case 0:
            b = '0';
            break;
        case 10:
            b = '\n';
            break;
        case 65:
            b = 'A';
            break;
        case 66:
            b = 'B';
            break;
        case 67:
            b = 'C';
            break;
        case 68:
            b = 'D';
            break;
        case 69:
            b = 'E';
            break;
        case 70:
            b = 'F';
            break;
        case 71:
            b = 'G';
            break;
        case 72:
            b = 'H';
            break;
        case 73:
            b = 'I';
            break;
        case 74:
            b = 'J';
            break;
        case 75:
            b = 'K';
            break;
        case 76:
            b = 'L';
            break;
        case 77:
            b = 'M';
            break;
        case 78:
            b = 'N';
            break;
        case 79:
            b = 'O';
            break;
        case 80:
            b = 'P';
            break;
        case 81:
            b = 'Q';
            break;
        case 82:
            b = 'R';
            break;
        case 83:
            b = 'S';
            break;
        case 84:
            b = 'T';
            break;
        case 85:
            b = 'U';
            break;
        case 86:
            b = 'V';
            break;
        case 87:
            b = 'W';
            break;
        case 88:
            b = 'X';
            break;
        case 89:
            b = 'Y';
            break;
        case 90:
            b = 'Z';
            break;
        default:
            printf ("Runtime Error: Unknown Character.\n");
            HALT();
    }
    return b;
}
/*
    This function simply takes in a temporary character that is a number and converts
    it to an integer, returning the int.
*/
int convertToNum (char Temp) {

    switch (Temp) {
            case '0':
                y = 0;
                break;
            case '1':
                y = 1;
                break;
            case '2':
                y = 2;
                break;
            case '3':
                y = 3;
                break;
            case '4':
                y = 4;
                break;
            case '5':
                y = 5;
                break;
            case '6':
                y = 6;
                break;
            case '7':
                y = 7;
                break;
            case '8':
                y = 8;
                break;
            case '9':
                y = 9;
                break;
            default:
                y = 200;
                break;
        }
    return y;
}

/*
    This function takes in the commands given by the file and loads all the commands into
    memory in the memory locations given. It also checks to ensure a HALT command was
    entered, ensures all commands are valid, and ensures all words in memory are 4 digits.
*/
void compile() {

    do {

        temp = getchar();
        
        if (isspace(temp)) {
            temp = getchar();
        }

        location = convertToNum(temp)*10;
        temp = getchar();
        location += convertToNum(temp);
        temp = getchar(); temp = getchar();

        if (temp == 'A' || temp == 'a') {
            temp = getchar();
            if (temp == 'D' || temp == 'd') {
                temp = getchar();
                if (temp == 'D' || temp == 'd') {
                    InstructionCounter++;
                    mem[4*location] = 3; mem[4*location+1] = 0;
                    temp = getchar(); temp = getchar();
                    mem[4*location+2] = convertToNum(temp);
                    temp = getchar();
                    if (isspace(temp)) {
                        mem[4*location+3] = mem[4*location+2];
                        mem[4*location+2] = 0;
                        continue;
                    }
                    else {
                        mem[4*location+3] = convertToNum(temp);
                    }
                }
                else {
                    printf("Error: Invalid Command Found During Compiling.\n");
                    HALT();
                }
            }
            else {
                printf("Error: Invalid Command Found During Compiling.\n");
                HALT();
            }
        }

        else if (temp == 'B' || temp == 'b') {
            temp = getchar();
            if (temp == 'R' || temp == 'r') {
                temp = getchar();
                if (temp == 'A' || temp == 'a') {
                    temp = getchar();
                    if (temp == 'N' || temp == 'n') {
                        InstructionCounter++;
                        mem[4*location] = 4; mem[4*location+1] = 0;
                        temp = getchar(); temp = getchar();
                        mem[4*location+2] = convertToNum(temp);
                        temp = getchar();
                        if (isspace(temp)) {
                            mem[4*location+3] = mem[4*location+2];
                            mem[4*location+2] = 0;
                            continue;
                        }
                        else {
                            mem[4*location+3] = convertToNum(temp);
                        }
                    }
                    else {
                        printf("Error: Invalid Command Found During Compiling.\n");
                        HALT();
                    }
                }
                else if (temp == 'N' || temp == 'n') {
                    temp = getchar();
                    if (temp == 'G' || temp == 'g') {
                        InstructionCounter++;
                        mem[4*location] = 4; mem[4*location+1] = 1;
                        temp = getchar(); temp = getchar();
                        mem[4*location+2] = convertToNum(temp);
                        temp = getchar();
                        if (isspace(temp)) {
                            mem[4*location+3] = mem[4*location+2];
                            mem[4*location+2] = 0;
                            continue;
                        }
                        else {
                            mem[4*location+3] = convertToNum(temp);
                        }
                    }
                    else {
                        printf("Error: Invalid Command Found During Compiling.\n");
                        HALT();
                    }
                }
                else if (temp == 'Z' || temp == 'z') {
                    temp = getchar();
                    if (temp == 'R' || temp == 'r') {
                        InstructionCounter++;
                        mem[4*location] = 4; mem[4*location+1] = 2;
                        temp = getchar(); temp = getchar();
                        mem[4*location+2] = convertToNum(temp);
                        temp = getchar();
                        if (isspace(temp)) {
                            mem[4*location+3] = mem[4*location+2];
                            mem[4*location+2] = 0;
                            continue;
                        }
                        else {
                            mem[4*location+3] = convertToNum(temp);
                        }
                    }
                    else {
                        printf("Error: Invalid Command Found During Compiling.\n");
                        HALT();
                    }
                }
                else {
                    printf("Error: Invalid Command Found During Compiling.\n");
                    HALT();
                }
            }
            else {
                printf("Error: Invalid Command Found During Compiling.\n");
                HALT();
            }
        }

        else if (temp == 'D' || temp == 'd') {
            temp = getchar();
            if (temp == 'I' || temp == 'i') {
                temp = getchar();
                if (temp == 'V' || temp == 'v') {
                    InstructionCounter++;
                    mem[4*location] = 3; mem[4*location+1] = 2;
                    temp = getchar(); temp = getchar();
                    mem[4*location+2] = convertToNum(temp);
                    temp = getchar();
                    if (isspace(temp)) {
                        mem[4*location+3] = mem[4*location+2];
                        mem[4*location+2] = 0;
                        continue;
                    }
                    else {
                        mem[4*location+3] = convertToNum(temp);
                    }
                }
                else {
                    printf("Error: Invalid Command Found During Compiling.\n");
                    HALT();
                }
            }
            else {
                printf("Error: Invalid Command Found During Compiling.\n");
                HALT();
            }
        }

        else if (temp == 'H' || temp == 'h') {
            temp = getchar();
            if (temp == 'A' || temp == 'a') {
                temp = getchar();
                if (temp == 'L' || temp == 'l') {
                    temp = getchar();
                    if (temp == 'T' || temp == 't') {
                        mem[4*location] = 9; mem[4*location+1] = 9;
                        mem[4*location+2] = 9; mem[4*location+3] = 9;
                        temp = getchar(); temp = getchar();
                        temp = getchar(); temp = getchar();
                        halt++;
                        InstructionCounter++;
                    }
                    else {
                        printf("Error: Invalid Command Found During Compiling.\n");
                        HALT();
                    }
                }
                else {
                    printf("Error: Invalid Command Found During Compiling.\n");
                    HALT();
                }
            }
            else {
                printf("Error: Invalid Command Found During Compiling.\n");
                HALT();
            }
        }
       
        else if (temp == 'L' || temp == 'l') {
            temp = getchar();
            if (temp == 'O' || temp == 'o') {
                temp = getchar();
                if (temp == 'A' || temp == 'a') {
                    temp = getchar();
                    if (temp == 'D' || temp == 'd') {
                        InstructionCounter++;
                        mem[4*location] = 2; mem[4*location+1] = 0;
                        temp = getchar(); temp = getchar();
                        mem[4*location+2] = convertToNum(temp);
                        temp = getchar();
                        if (isspace(temp)) {
                            mem[4*location+3] = mem[4*location+2];
                            mem[4*location+2] = 0;
                            continue;
                        }
                        else {
                            mem[4*location+3] = convertToNum(temp);
                        }
                    }
                    else {
                        printf("Error: Invalid Command Found During Compiling.\n");
                        HALT();
                    }
                }
                else {
                    printf("Error: Invalid Command Found During Compiling.\n");
                    HALT();
                }
            }
            else {
                printf("Error: Invalid Command Found During Compiling.\n");
                HALT();
            }
        }

        else if (temp == 'M' || temp == 'm') {
            temp = getchar();
            if (temp == 'U' || temp == 'u') {
                temp = getchar();
                if (temp == 'L' || temp == 'l') {
                    temp = getchar();
                    if (temp == 'T' || temp == 't') {
                        InstructionCounter++;
                        mem[4*location] = 3; mem[4*location+1] = 3;
                        temp = getchar(); temp = getchar();
                        mem[4*location+2] = convertToNum(temp);
                        temp = getchar();
                        if (isspace(temp)) {
                            mem[4*location+3] = mem[4*location+2];
                            mem[4*location+2] = 0;
                            continue;
                        }
                        else {
                            mem[4*location+3] = convertToNum(temp);
                        }
                    }
                    else {
                        printf("Error: Invalid Command Found During Compiling.\n");
                        HALT();
                    }
                }
                else {
                    printf("Error: Invalid Command Found During Compiling.\n");
                    HALT();
                }
            }
            else if (temp == 'O' || temp == 'o') {
                temp = getchar();
                if (temp == 'D' || temp == 'd') {
                    InstructionCounter++;
                    mem[4*location] = 3; mem[4*location+1] = 4;
                    temp = getchar(); temp = getchar();
                    mem[4*location+2] = convertToNum(temp);
                    temp = getchar();
                    if (isspace(temp)) {
                        mem[4*location+3] = mem[4*location+2];
                        mem[4*location+2] = 0;
                        continue;
                    }
                    else {
                        mem[4*location+3] = convertToNum(temp);
                    }
                }
                else {
                    printf("Error: Invalid Command Found During Compiling.\n");
                    HALT();
                }
            }
            else {
                printf("Error: Invalid Command Found During Compiling.\n");
                HALT();
            }
        }

        else if (temp == 'P' || temp == 'p') {
            temp = getchar();
            if (temp == 'R' || temp == 'r') {
                temp = getchar();
                if (temp == 'N' || temp == 'n') {
                    temp = getchar();
                    if (temp == 'T' || temp == 't') {
                        InstructionCounter++;
                        mem[4*location] = 1; mem[4*location+1] = 2;
                        temp = getchar(); temp = getchar();
                        mem[4*location+2] = convertToNum(temp);
                        temp = getchar();
                        if (isspace(temp)) {
                            mem[4*location+3] = mem[4*location+2];
                            mem[4*location+2] = 0;
                            continue;
                        }
                        else {
                            mem[4*location+3] = convertToNum(temp);
                        }
                    }
                    else {
                        printf("Error: Invalid Command Found During Compiling.\n");
                        HALT();
                    }
                }
                else {
                    printf("Error: Invalid Command Found During Compiling.\n");
                    HALT();
                }
            }
            else {
                printf("Error: Invalid Command Found During Compiling.\n");
                HALT();
            }
        }

        else if (temp == 'R' || temp == 'r') {
            temp = getchar();
            if (temp == 'E' || temp == 'e') {
                temp = getchar();
                if (temp == 'A' || temp == 'a') {
                    temp = getchar();
                    if (temp == 'D' || temp == 'd') {
                        InstructionCounter++;
                        mem[4*location] = 1; mem[4*location+1] = 0;
                        temp = getchar(); temp = getchar();
                        mem[4*location+2] = convertToNum(temp);
                        temp = getchar();
                        if (isspace(temp)) {
                            mem[4*location+3] = mem[4*location+2];
                            mem[4*location+2] = 0;
                            continue;
                        }
                        else {
                            mem[4*location+3] = convertToNum(temp);
                        }
                    }
                    else {
                        printf("Error: Invalid Command Found During Compiling.\n");
                        HALT();
                    }
                }
                else {
                    printf("Error: Invalid Command Found During Compiling.\n");
                    HALT();
                }
            }
            else {
                printf("Error: Invalid Command Found During Compiling.\n");
                HALT();
            }
        }

        else if (temp == 'S' || temp == 's') {
            temp = getchar();
            if (temp == 'E' || temp == 'e') {
                temp = getchar();
                if (temp == 'T' || temp == 'v') {
                    temp = getchar(); temp = getchar();
                    mem[4*location+3] = convertToNum(temp);
                    temp = getchar();
                    InstructionCounter++;
                    if (isdigit(temp)) {
                        mem[4*location+2] = mem[4*location+3];
                        mem[4*location+3] = convertToNum(temp);
                        temp = getchar();
                        if (isdigit(temp)) {
                            mem[4*location+1] = mem[4*location+2];
                            mem[4*location+2] = mem[4*location+3];
                            mem[4*location+3] = convertToNum(temp);
                            temp = getchar();
                            if (isdigit(temp)) {
                                mem[4*location] = mem[4*location+1];
                                mem[4*location+1] = mem[4*location+2];
                                mem[4*location+2] = mem[4*location+3];
                                mem[4*location+3] = convertToNum(temp);
                                temp = getchar();
                                if (isdigit(temp)) {
                                    printf ("Too many digits to assign.\n");
                                    HALT();
                                }
                            }
                        }
                    }
                }
                else {
                    printf("Error: Invalid Command Found During Compiling.\n");
                    HALT();
                }
            }
            else if (temp == 'U' || temp == 'u') {
                temp = getchar();
                if (temp == 'B' || temp == 'b') {
                    InstructionCounter++;
                    mem[4*location] = 3; mem[4*location+1] = 1;
                    temp = getchar(); temp = getchar();
                    mem[4*location+2] = convertToNum(temp);
                    temp = getchar();
                    if (isspace(temp)) {
                        mem[4*location+3] = mem[4*location+2];
                        mem[4*location+2] = 0;
                        continue;
                    }
                    else {
                        mem[4*location+3] = convertToNum(temp);
                    }
                }
                else {
                    printf("Error: Invalid Command Found During Compiling.\n");
                    HALT();
                }
            }
            else if (temp == 'T' || temp == 't') {
                temp = getchar();
                if (temp == 'O' || temp == 'o') {
                    temp = getchar();
                    if (temp == 'R' || temp == 'r') {
                        InstructionCounter++;
                        mem[4*location] = 2; mem[4*location+1] = 1;
                        temp = getchar(); temp = getchar();
                        mem[4*location+2] = convertToNum(temp);
                        temp = getchar();
                        if (isspace(temp)) {
                            mem[4*location+3] = mem[4*location+2];
                            mem[4*location+2] = 0;
                            continue;
                        }
                        else {
                            mem[4*location+3] = convertToNum(temp);
                        }
                    }
                    else {
                        printf("Error: Invalid Command Found During Compiling.\n");
                        HALT();
                    }
                }
                else {
                    printf("Error: Invalid Command Found During Compiling.\n");
                    HALT();
                }
            }
            else {
                printf("Error: Invalid Command Found During Compiling.\n");
                HALT();
            }
        }
        
        else if (temp == 'W' || temp == 'w') {
            temp = getchar();
            if (temp == 'R' || temp == 'r') {
                temp = getchar();
                if (temp == 'I' || temp == 'i') {
                    temp = getchar();
                    if (temp == 'T' || temp == 't') {
                        InstructionCounter++;
                        mem[4*location] = 1; mem[4*location+1] = 1;
                        temp = getchar(); temp = getchar();
                        mem[4*location+2] = convertToNum(temp);
                        temp = getchar();
                        if (isspace(temp)) {
                            mem[4*location+3] = mem[4*location+2];
                            mem[4*location+2] = 0;
                            continue;
                        }
                        else {
                            mem[4*location+3] = convertToNum(temp);
                        }
                    }
                    else {
                        printf("Error: Invalid Command Found During Compiling.\n");
                        HALT();
                    }
                }
                else {
                    printf("Error: Invalid Command Found During Compiling.\n");
                    HALT();
                }
            }
            else {
                printf("Error: Invalid Command Found During Compiling.\n");
                HALT();
            }
        }
        else {
            if ((int) location > 99) {
                x = 200;
            }
            else {
                printf("Error: Invalid Command Found During Compiling.\n");
                HALT();
            }
        }
    } while (x != 200);

    if (halt == 0) {
        printf("Error: No HALT Command.\n");
        HALT();
    }
}


/*
    This is the function that does the execution of the commands starting with the 
    command at memory location 00.
*/
void execute() {

    location = 0;
    for (z = 0; z < 400; z+=4) {
        OperationCode = (mem[z]*10) + mem[z+1];
        Operand = (mem[z+2]*10) + mem[z+3];

        switch (OperationCode) {
            case 10:
                InstructionRegister = 4948;
                READ(Operand);
                break;
            case 11:
                InstructionRegister = 4949;
                WRIT(Operand);
                break;
            case 12:
                InstructionRegister = 4950;
                PRNT(Operand);
                break;
            case 20:
                InstructionRegister = 5048;
                LOAD(Operand);
                break;
            case 21:
                InstructionRegister = 5049;
                STOR(Operand);
                break;
            case 22:
                InstructionRegister = 5050;
                SET(Operand);
                break;
            case 30:
                InstructionRegister = 5148;
                ADD(Operand);
                break;
            case 31:
                InstructionRegister = 5149;
                SUB(Operand);
                break;
            case 32:
                InstructionRegister = 5150;
                DIV(Operand);
                break;
            case 33:
                InstructionRegister = 5151;
                MULT(Operand);
                break;
            case 34:
                InstructionRegister = 5152;
                MOD(Operand);
                break;
            case 40:
                InstructionRegister = 5248;
                z = (BRAN(Operand)-4);
                break;
            case 41:
                InstructionRegister = 5249;
                if (Accumulator < 0) {
                    z = (BRNG(Operand)-4);
                }
                break;
            case 42:
                InstructionRegister = 5250;
                if (Accumulator == 0) {
                    z = (BRZR(Operand)-4);
                }
                break;
            case 99:
                InstructionRegister = 5757;
                HALT();
                break;
            default:
                printf("Invalid Command.\n");
                break;
        }
        location++;
        if (location == InstructionCounter) {
            break;
        }
    }
}

int main () {

    for (x = 0; x < 400; x++) {
        mem[x] = 0;
    }
    compile();
    stdin = fopen("/dev/tty", "r");
    execute();
    HALT();

    return 0;
}
