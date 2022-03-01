# Sudoku
[Background](https://en.wikipedia.org/wiki/Sudoku)  
In this task, we will implement a Sudoku game visually.  
To do this we will implement the next functions:  

### Create board
```void createBoard(char board[][SIZE * SIZE], char str[]);``` receives an uninitialized 2D char array and a string.  
The function should use the board to store the data encoded in the string. The string can contain any ascii character. Some of the characters have meaning, the rest of the characters are variables.  
If an invalid string is received, we will print an ```error``` message.  

### Make move
```void makeMove(char board[][SIZE * SIZE], char move[]);``` receives a 2D array of characters that was initialized by the function createBoard, and a string describing a move.  
If the move is valid, the function will change the board accordingly. If the move is invalid, the board does not change and an error message is printed.  
The valid moves are:
1) "replaceAll, char1, char2"
2) "change, locationRow, locationCol, char"
3) "add, locationRow, locationCol, char"
4) "delete,locationRow, locationCol"

### Print board
```void printBoard(char board[][SIZE * SIZE]);``` receives a 2D array of characters that was initialized by the function createBoard, and prints the sudoku board on the screen.  

### Test board
```int testBoard(char board[][SIZE * SIZE]);``` receives a 2D array of characters that was initialized by the function createBoard, and checks whether the board is valid according to the sudoku rules (a number is not repeated twice in the same row, column or square).  
For a valid board the function will return 1, for an invalid board the function returns 0.  

### Is same board
```int isSameBoard(char board1[][SIZE * SIZE], char board2[][SIZE * SIZE]);``` receives two boards, and checks whether the sudoku boards they represent are equal to each other.  

### Complete board
```void completeBoard(char board[][SIZE * SIZE]);``` receives an almost solved board and solves it, i.e. completes the empty squares until the board is solved according to the sudoku rules.  If the board is not almost solved, an error message is printed.
