#include "ass4.h"
#include <stdio.h>
#include <string.h>

/***********
* Ido Aharon
* 01
* ass4
***********/

int isValidString(char str[]);
int isSpace(char letter);
int isNumber(char letter);
int charNumValue(char num);
char toChar(int num);

/*******************************************************************************************************************
*Function name: IsSameBoard
*Input: char board[][SIZE*SIZE], char board2[][SIZE*SIZE]
*Output: int(1 or 0)
*Function Operation: the function gets two boards and compares between the two.
*                    if the board found different, the function prints the indexes where
*                    the boards are different and returns 0, but if the boards are the same the function returns 1.
********************************************************************************************************************/
int isSameBoard(char board1[][SIZE * SIZE], char board2[][SIZE * SIZE])
{
    int boardLength = SIZE * SIZE;
    for (int i = 0; i < boardLength; i++)
    {
        for (int j = 0; j < boardLength; j++)
        {
            if (board1[i][j]!=board2[i][j])
            {
                printf("Found inequality on row %d col %d.\n", i, j);
                return 0;
            }
        }
    }
    return 1;
}

/*****************************************************************************************
*Function name: charNumValue
*Input: char num(values between 0-9)
*Output: int(value between 0-9)
*Function Operation: the function gets a number in his char version and coverts it to int
******************************************************************************************/
int charNumValue(char num)
{
    return (num - '0');
}

/*******************************************************************************************************
*Function name: testBoard
*Input: board[][SIZE*SIZE]
*Output: int (1 or 0)
*Function Operation: the function gets a sudokou board and check if its passing the sudoku rules.
*                    the function checks the sudoku 3 times: 1 for rows, 1 for columns, and 1 for boxes.
*                    if there is any invalid char or the row/col/box contains the same number more than
*                    one time, the function returns 0.
*                    if the sudoku are ok, the function returns 1
********************************************************************************************************/

int testBoard(char board[][SIZE * SIZE]) {
    //in order to avoid magic numbers
    const char space = ' ';
    const int sudokuLength = SIZE * SIZE, boxLength = SIZE, boxSum = SIZE * SIZE;
    int timesShown[SIZE * SIZE] = {0};
    int currentNumber;
    //columns counting
    for (int i = 0; i < sudokuLength; i++)
    {
        for (int j = 0; j < sudokuLength; j++)
        {
            //if char is number
            if (isNumber(board[i][j]))
            {
                //calculate the numeric value of the current char, and add the counting for him in the int array
                currentNumber = charNumValue(board[i][j]);
                timesShown[currentNumber - 1]++;
                //if the counting for this number crossed 1, its a false sudoku
                if (timesShown[currentNumber - 1] > 1)
                    return 0;
            }
            //if the current char is space continue
            else if (board[i][j] == space)
                continue;
            //if the current char is not a space or a number, its an invalid char
            else
                return 0;
            
        }
        //reset the counter to 0
        memset(timesShown, 0, sizeof(timesShown));
    }
    
    //rows counting
    for (int j = 0; j < sudokuLength; j++)
    {
        for (int i = 0; i < sudokuLength; i++)
        {
            //if char is number
            if (isNumber(board[i][j]))
            {
                //calculate the numeric value of the current char, and add the counting for him in the int array
                currentNumber = charNumValue(board[i][j]);
                timesShown[currentNumber - 1]++;
                //if the counting for this number crossed 1, its a false sudoku
                if (timesShown[currentNumber - 1] > 1)
                    return 0;
            }
            //if the current char is space continue
            else
                continue;
        }
        //reset the counter to 0
        memset(timesShown, 0, sizeof(timesShown));
    }

    //box counting-split the board to SIZExSIZE boxes and do the same checking on them
    for (int boxRow = 1; boxRow < boxLength; boxRow++)
    {
        for (int boxColumn = 1; boxColumn <= boxLength; boxColumn++)
        {
            //start the internal box calculation
            for (int i = 0; i <= boxLength; i++)
            {
                for (int j = 0; j < boxLength; j++)
                {
                    //if char is number
                    if (isNumber(board[i+i*boxRow][j+j*boxColumn]))
                    {
                        //calculate the numeric value of the current char,add the counting for him in the int array
                        currentNumber = charNumValue(board[i+i*boxRow][j+j*boxColumn]);
                        timesShown[currentNumber - 1]++;
                        //if the counting for this number crossed 1, its a false sudoku
                        if (timesShown[currentNumber - 1] > 1)
                            return 0;
                    }
                    //if the current char is space continue
                    else
                        continue;
                }
                //reset the counter to 0
                memset(timesShown, 0, sizeof(timesShown));
            }
        }
    }

    //after finishing checking all possibilities, return 1
    return 1;
}

/************************************************************************************
*Function name: printBoard
*Input: char board[][SIZE*SIZE]
*Output: none
*Function Operation: the function gets the sudoku board from the user, and prints it.
*                    when the function gets to the end of the box/ end of line/ etc,
*                    it prints | or \n.
*************************************************************************************/

void printBoard(char board[][SIZE * SIZE]) {
    //sudokuLength calculation
    const int sudokuLength = SIZE * SIZE;

    //sum of lines
    for (int i = 0; i < sudokuLength; i++)
    {
        //sum of columns
        for (int j = 0; j < sudokuLength; j++)
        {
            //if you got to the end of the box
            if (j % SIZE == 0 && j != 0)
            {
                printf(" | ");
            }
            printf("%c", board[i][j]);

        }
        //every last line print "enter"
        printf("\n");
        //i+1 in order to get real the real line
        if ((i + 1) % SIZE == 0 && (i + 1) != sudokuLength)
        {
            printf("\n");
        }

    }
    printf("\n");
}


/**************************************************************************************************************
*Function name: createBoard
*Input: char board[][SIZE*SIZE], char str[]
*Output: none
*Function Operation: the function gets an uninitialized board and an encrypted string.
*                    the function checks if the string is valid due the rules of the sudoku.
*                    if its not, the function prints an error message and doesn't change the board.
*                    if the string is valid, the function sets the board as the user's encrypted string orders.
***************************************************************************************************************/
void createBoard(char board[][SIZE * SIZE], char str[]) {
    //check if the string is invalid. if it does, print error and don't continue
    if (!isValidString(str))
    {
        printf("Error\n");
        return;
    }

    //in order not to use magic numbers
    const char newRow = '/', space = ' ';
    const int lettersNeeded = SIZE * SIZE;
    int boardRow = 0, boardColumn = 0,spacesAmount,lastColumn;
    //"for" loop that runs until the end of the string
    for (int i = 0; i < strlen(str); i++)
    {
        //check if the current char is a new row.
        if (str[i] == newRow)
        {
            //if it is, check if there are missing spaces
            if (boardColumn < lettersNeeded)
            {
                //if there are missing spaces, add them to the sudoku matrix
                for (int j = boardColumn; boardColumn < lettersNeeded; j++)
                {
                    board[boardRow][boardColumn] = space;
                    boardColumn++;
                }
            }
            //reset the board column and continue to the next row
            boardRow++;
            boardColumn = 0;
        }
        //check if the current letter is a number. if it is, set the sudoku with that number and continue
        else if (isNumber(str[i]))
        {
            board[boardRow][boardColumn] = str[i];
            boardColumn++;
        }
        //enter if the current letter is a space
        else if (isSpace(str[i]) != 0)
        {
            //I don't want to call the function in the "for" loop everytime
            spacesAmount = isSpace(str[i]);
            //"for" loop that adds the board the amount of spaces the user asked
            for (int currentSpaces = 1; currentSpaces <= spacesAmount; currentSpaces++)
            {
                board[boardRow][boardColumn] = space;
                boardColumn++;
            }
        }
        //if the letter is a variable
        else
        {
            board[boardRow][boardColumn] = str[i];
            boardColumn++;
        }
    }

    //for last row completion
    for (int j = boardColumn; j < lettersNeeded; j++)
    {
        board[(SIZE * SIZE) - 1][j] = space;
    }
}

/*************************************************************************************************************
*Function name: makeMove
*Input: board[][SIZE*SIZE], char move[]
*Output: none
*Function Operation: the function gets a board that was set with createBoard function(which means it's valid)
*                    and an encrypted string that ask to make one of the following moves:
*                    1) replaceAll 2)change 3)add 4)delete
*                    first the function checks if the encrypted string is valid due the rules of the moves,
*                    if its not, the function won't change the board and prints an error message.
*                    if its valid, the function changes the board as the user's orders.
**************************************************************************************************************/
void makeMove(char board[][SIZE * SIZE], char move[])
{
    //in order not to cause stack overflow in tempStr, max string length is 14.
    const int maxStringLength = 14;
    if (strlen(move) > maxStringLength)
    {
        printf("Error\n");
        return;
    }

    int isCharExist = 0;
    const int sudokuLength = SIZE * SIZE;
    //our definitions of space and enter
    const char space = ' ', enter = '/';
    //our origin and destination chars
    char chosenChar, char1, char2;
    int lCol, lRow;
    //our temp str is the maximum-letters case
    char tempMove[15];
    //copy to tempMove our move string
    strcpy(tempMove, move);
    //make a pointer to the substring (command)
    char* pMove = strtok(tempMove, ",");
    if (strcmp(pMove, "replaceAll") == 0)
    {
        pMove = strtok(NULL, ",");
        //2 char and above origin is not allowed, also NULL char is not allowed
        if (pMove == NULL || strlen(pMove) > 1)
        {
            printf("Error\n");
            return;
        }

        char1 = pMove[0];
        //not allowed: space, enter, letter space
        if (char1 == space || char1 == enter || isSpace(char1) != 0)
        {
            printf("Error\n");
            return;
        }
        pMove = strtok(NULL, "");
        //2 chars and above replacement is not allowed, alsu NULL char is not allowed
        if (pMove == NULL || strlen(pMove) > 1)
        {
            printf("Error\n");
            return;
        }
        char2 = pMove[0];
        //not allowed: space, enter,letter space
        if (isSpace(char2) != 0 || char2 == enter || char2 == space)
        {
            printf("Error\n");
            return;
        }

        //check if the origin char exist in the sudoku board
        for (int i = 0; i < sudokuLength; i++)
        {
            for (int j = 0; j < sudokuLength; j++)
            {
                //if the origin char is found
                if (board[i][j] == char1)
                {
                    isCharExist = 1;
                    break;
                }
            }
            if (isCharExist)
                break;
        }
        //if the origin char is not exist in our board, print error
        if (!isCharExist)
        {
            printf("Error\n");
            return;
        }


        //if all test are passed, exucute command
        for (int i = 0; i < sudokuLength; i++)
        {
            for (int j = 0; j < sudokuLength; j++)
            {
                if (board[i][j] == char1)
                {
                    board[i][j] = char2;
                }
            }
        }
    }

    else if (strcmp(pMove, "change") == 0)
    {
        pMove = strtok(NULL, ",");
        //2 char and above row  is not allowed, also NULL row is not allowed
        if (pMove == NULL || strlen(pMove) > 1)
        {
            printf("Error\n");
            return;
        }
        //convert the char row to number
        lRow = charNumValue(pMove[0]);
        //if the row does not exist in sudoku
        if (lRow >= sudokuLength || lRow < 0)
        {
            printf("Error\n");
            return;
        }
        pMove = strtok(NULL, ",");
        //2 char and above col is not allowed, also NULL row is not allowed.
        if (pMove == NULL || strlen(pMove) > 1)
        {
            printf("Error\n");
            return;
        }
        //convert the char col to number
        lCol = charNumValue(pMove[0]);
        //if the col does not exist in sudoku
        if (lCol >= sudokuLength || lCol < 0)
        {
            printf("Error\n");
            return;
        }
        pMove = strtok(NULL, "");
        //2 char and above replacement char is not allowed, also empty char is not allowed
        if (pMove == NULL || strlen(pMove) > 1)
        {
            printf("Error\n");
            return;
        }
        chosenChar = pMove[0];
        //not allowed: space, enter,letter space
        if (isSpace(chosenChar) != 0 || chosenChar == enter || chosenChar == space)
        {
            printf("Error\n");
            return;
        }
        //if the destination place is a space
        if (board[lRow][lCol] == space)
        {
            printf("Error\n");
            return;
        }

        //if all checks are O.K exucute command
        board[lRow][lCol] = chosenChar;
    }

    else if (strcmp(pMove, "add") == 0)
    {
        pMove = strtok(NULL, ",");
        //2 char and above row  is not allowed, also NULL row is not allowed
        if (pMove == NULL || strlen(pMove) > 1)
        {
            printf("Error\n");
            return;
        }
        //convert the char row to number
        lRow = charNumValue(pMove[0]);
        //if the row does not exist in sudoku
        if (lRow >= sudokuLength || lRow < 0)
        {
            printf("Error\n");
            return;
        }
        pMove = strtok(NULL, ",");
        //2 char and above col is not allowed, also NULL row is not allowed.
        if (pMove == NULL || strlen(pMove) > 1)
        {
            printf("Error\n");
            return;
        }
        //convert the char col to number
        lCol = charNumValue(pMove[0]);
        //if the col does not exist in sudoku
        if (lCol >= sudokuLength || lCol < 0)
        {
            printf("Error\n");
            return;
        }
        pMove = strtok(NULL, "");
        //2 char and above replacement char is not allowed, also empty char is not allowed
        if (pMove == NULL || strlen(pMove) > 1)
        {
            printf("Error\n");
            return;
        }
        chosenChar = pMove[0];
        //not allowed: space, enter,letter space
        if (isSpace(chosenChar) != 0 || chosenChar == enter || chosenChar == space)
        {
            printf("Error\n");
            return;
        }
        //if the destination is not empty, return false
        if (board[lRow][lCol] != space)
        {
            printf("Error\n");
            return;
        }

        //if all checks are O.K execute command
        board[lRow][lCol] = chosenChar;
    }

    else if (strcmp(pMove, "delete") == 0)
    {
        pMove = strtok(NULL, ",");
        //2 char and above row  is not allowed, also NULL row is not allowed
        if (pMove == NULL || strlen(pMove) > 1)
        {
            printf("Error\n");
            return;
        }
        //convert the char row to number
        lRow = charNumValue(pMove[0]);
        //if the row does not exist in sudoku
        if (lRow >= sudokuLength || lRow < 0)
        {
            printf("Error\n");
            return;
        }
        pMove = strtok(NULL, "");
        //2 char and above col is not allowed, also NULL row is not allowed.
        if (pMove == NULL || strlen(pMove) > 1)
        {
            printf("Error\n");
            return;
        }
        //convert the char col to number
        lCol = charNumValue(pMove[0]);
        //if the col does not exist in sudoku
        if (lCol >= sudokuLength || lCol < 0)
        {
            printf("Error\n");
            return;
        }
        //deleting a space from board is not allowed
        if (board[lRow][lCol] == space)
        {
            printf("Error\n");
            return;
        }
        //if all tests are passed, execute command
        board[lRow][lCol] = space;
    }
    //if the command is none of the above, print error
    else
    {
        printf("Error\n");
        return;
    }
}

/*******************************************************************************************************************
*Function name: isValidString
*Input: char str[]
*Output: int (yes-1 or no-0)
*Function Operation: the function gets the string from createBoard function, and checks if its a valid string.
*                    the function checks if: 1) there are enough rows 2) the row are not longer than the sudoku row
*                    3) if there are more rows than the sudoku board
*                    if one of the tests fail, the function return 0.
*                    if all tests are passed, the function returns 1.
********************************************************************************************************************/
int isValidString(char str[])
{
    const int maxRowLetters = SIZE * SIZE, realTimeRows = (SIZE * SIZE) - 1;
    int lettersSum = 0, rowsSum = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '/')
        {
            if (lettersSum > maxRowLetters)
            {
                return 0;
            }
            rowsSum++;
            lettersSum = 0;
        }
        else
        {
            //if the letter is space, add the spaces amount
            if (isSpace(str[i]) != 0)
                lettersSum += isSpace(str[i]);
            //else the letter is number or variable and it means one letter both ways
            else
                lettersSum++;
        }
    }
    if (rowsSum != realTimeRows)
        return 0;
    return 1;

}

/*********************************************************************************************************
*Function name: isSpace
*Input: char
*Output: int (yes- amount of spaces. no-0)
*Function Operation: the function gets a char and returns if this char is a space due to the sudoku rules.
*                    when SIZE=1: 'a' only. SIZE=2: between 'a' and 'd'. SIZE=3: between 'a' and 'i'.
*                    if its none of the following, its not a space and the function returns 0.
*                    if it is space, the function return the num of spaces the letter express.
**********************************************************************************************************/
int isSpace(char letter) {
    //the spaces real amount
    int spacesAmount = (letter - 'a') + 1;
    //if the size of matrix is 1X1, the maximum space option is 1(='a')
    if (SIZE == 1)
    {
        if (letter == 'a')
            return spacesAmount;
    }
    //if the size of matrix is 4x4, the maximum space option is 4(='d')
    else if (SIZE == 2)
    {
        if (letter >= 'a' && letter <= 'd')
            return spacesAmount;
    }
    //if the size of matrix is 9x9, the maximum space option is 9(='d')
    else if (SIZE == 3)
    {
        if (letter >= 'a' && letter <= 'i')
            return spacesAmount;
    }
    //if the letters are not between 'a'-'i'(depend on SIZE), its not space at all
    return 0;
}

/********************************************************************************************************************
*Function name: isNumber
*Input: char
*Output: int (yes-1 or no-0)
*Function Operation: the function gets a char and returns true if this char are number due to the size of the sudoku.
*                    every size has it's calculation, and if the char is indeed a number, the function returns 1.
*                    otherwise, the function returns 0.
*********************************************************************************************************************/
int isNumber(char letter) {
    //if the size of matrix is 1x1, the only possible number is 1
    if (SIZE == 1)
    {
        if (letter == '1')
            return 1;
        return 0;
    }

    //if the size of matrix is 4x4, the possibilties of numbers are 1-4
    else if (SIZE == 2)
    {
        if (letter >= '1' && letter <= '4')
            return 1;
        return 0;
    }

    //if the size of matrix is 9x9, the possibilites of numbers are 1-9
    else if (SIZE == 3)
    {
        if (letter >= '1' && letter <= '9')
            return 1;
        return 0;
    }

}

/***********************************************************************************************************
*Function name: completeBoard
*Input: char board[][SIZE*SIZE]
*Output: none
*Function Operation: the function gets an almost-resolved sudoku and completes him due to sudoku rules.
*                    almost-resolved sudoku is a sudoku with only numbers and one space in each row.
*                    if the sudoku is not almost-resolved, the function prints an error message and doesn't
*                    change the original board.
*                    otherwise, the function resolve the sudoku completely.
************************************************************************************************************/
void completeBoard(char board[][SIZE * SIZE])
{
    const int sudokuLength = SIZE * SIZE;
    const char space = ' ';
    //create a new char array that keeps the counter of numbers
    char rowNumbers[SIZE * SIZE] = { 0 };
    //the sudoku current num
    int sNum,numPlace,sumOfSpaces=0;
    //if the board is invalid print error and quit
    if (!testBoard(board))
    {
        printf("Error\n");
        return;
    }
    //another test that check if there is more than 1 space in a row
    for (int i = 0; i < sudokuLength; i++)
    {
        for (int j = 0; j < sudokuLength; j++)
        {
            if (board[i][j] == space)
                sumOfSpaces++;
            //if you counted more than 1 space in a row, print error and exit
            if (sumOfSpaces > 1)
            {
                printf("Error\n");
                return;
            }
        }
        sumOfSpaces = 0;
    }
    //"for" loop that runs all sudoku, when test is OK
    for (int i = 0; i < sudokuLength; i++)
    {
        for (int j = 0; j < sudokuLength; j++)
        {
            if (board[i][j] != space)
            {
                //calculate the num if it exists
                sNum = charNumValue(board[i][j]);
                //enlarge the counter in this position by 1.
                rowNumbers[sNum - 1]++;
            }
            //if its a space, keep the location
            else
                numPlace = j;
        }
        //check what is that missing num
        for (int k = 0; k < sudokuLength; k++)
        {
            if (rowNumbers[k] == 0)
                board[i][numPlace] = toChar(k + 1);
        }
        //reset the counter array
        memset(rowNumbers, 0, sizeof(rowNumbers));
    }
}

/***********************************************************************************
*Function name:  toChar
*Input: int num
*Output: char
*Function Operation: the function gets a number and returns the number as char type.
************************************************************************************/
char toChar(int num)
{
    switch (num)
    {
    case 0: return '0';
        break;
    case 1: return '1';
        break;
    case 2:return '2';
        break;
    case 3:return '3';
        break;
    case 4: return '4';
        break;
    case 5: return '5';
        break;
    case 6: return '6';
        break;
    case 7: return '7';
        break;
    case 8: return '8';
        break;
    case 9: return '9';
        break;
    }
}
