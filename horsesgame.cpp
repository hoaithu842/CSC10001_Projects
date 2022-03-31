#include <iostream>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <iomanip>
#include <unistd.h>
using namespace std;

//Bien in bang
const char rightArrow = 26, leftArrow = '<', upArrow = 24, downArrow = 25, startSpace = '`', finSpace = '~';
char block[100][100], defaultBlock[100][100];
int blockHorseNum[100][100], blockPlayerNum[100][100];
//Bien thong so
int mapNum, nRow, nCol, numOfHorses, numOfPlayers, minDice, maxDice;
//Bien nguoi choi
char name[100];
int horsesWin[100], savedMoves[100][100], posRow[100][100], posCol[100][100];
bool finishedHorse[100][100];

void aboutMap(){
    system("CLS");
    cout << "These are maps that are available: " << endl;
    cout << "\t1. Line map \n\t2. Spiral map \n\t3. Zigzag map \n\t4. U map " << endl;
    recin:
        cout << "Type map number: "; 
        cin >> mapNum;
        if (mapNum>4 || mapNum <1) goto recin;
    cout << "------------------------------------------\n";
}
void aboutBoard(){
    int temp;
    cout << "These are options for board size: " << endl;
    if (mapNum==1){
        cout << "\t0. User Input (1xn), Input n \n\t1. 1x5 \n\t2. 1x10" << endl;
        cout << "Your option: "; cin >> temp;
        switch (temp){
            case 1: nCol = 5; break;
            case 2: nCol = 10; break;
            default: 
                recin1: 
                    cout << "\tType n (n<25): "; 
                    cin >> nCol; 
                    if (nCol>25 || nCol<3) goto recin1;
                break;
        }
        nRow = 1; 
    } else if (mapNum==3){
        cout << "\t0. User Input (nxm) \n\t1. 3x3 \n\t2. 5x5" << endl;
        cout << "Your option: "; cin >> temp;
        switch (temp){
            case 1: nCol = 3; nRow=3; break;
            case 2: nCol = 5; nRow=5; break;
            default: 
                recin2:
                    cout << "\tType n (n<25): "; 
                    cin >> nRow;
                    cout << "\tType m (m<25): "; 
                    cin >> nCol;
                    if (nRow>25 || nRow<3 || nCol>25 || nRow<3) goto recin2;
                break;
        }     
    } else if (mapNum==2){
        cout << "\t0. User Input (nxn) \n\t1. 3x3 \n\t2. 5x5" << endl;
        cout << "Your option: "; cin >> temp;
        switch (temp){
            case 1: nCol = 3; nRow=3; break;
            case 2: nCol = 5; nRow=5; break;
            default: 
                recin3: 
                    cout << "\tType n (n<25), n odd: "; 
                    cin >> nCol; 
                    if (nCol>25 || nCol<3 || nCol%2==0) goto recin3;
                break;
        }
        nRow = nCol;        
    } else if (mapNum==4){
        cout << "\t0. User Input (nxn) \n\t1. 4x4 \n\t2. 6x6" << endl;
        cout << "Your option: "; cin >> temp;
        switch (temp){
            case 1: nCol = 4; nRow=4; break;
            case 2: nCol = 6; nRow=6; break;
            default: 
                recin4: 
                    cout << "\tType n (n<25), n even: "; 
                    cin >> nCol; 
                    if (nCol>25 || nCol<3 || nCol%2==1) goto recin4;
                break;
        }
        nRow = nCol; 
    }
    cout << "------------------------------------------\n";
}
void aboutPlayer(){
    cout << "Please type the number of players: "; 
    cin >> numOfPlayers;
    cout << "Type the number of horses each player has: "; 
    cin >> numOfHorses;
        
    for (int i=0; i<numOfPlayers; i++){
        cout << "Player " << i+1 << " please type first alphabet of your name: ";
        cin >> name[i];
        for (int j=0; j<numOfHorses; j++){
            savedMoves[i][j] = 1;
            posCol[i][j] = 0; posRow[i][j] = 0;
        }
    }
    cout << "------------------------------------------\n";
}
void aboutDiceRange(){
    recinn:
    cout << "Max range of your dice: "; 
    cin >> maxDice;
    cout << "Min range of your dice: "; 
    cin >> minDice;
    if (minDice>maxDice) goto recinn;
    cout << "------------------------------------------\n";
}

void defaultLineMap(){
    for (int i=0; i<nCol; i++) defaultBlock[0][i] = rightArrow;
    defaultBlock[0][0] = startSpace;
    defaultBlock[0][nCol-1] = finSpace;
}
void defaultSpiralMap(){
    int temp=0, hang1=0, hang2=nRow-1, cot1=0, cot2=nCol-1;
    while (temp!= nRow*nCol){
        if (cot1!=0) defaultBlock[hang1][cot1-1] = rightArrow;

        for (int i=cot1; i<= cot2; i++){
            defaultBlock[hang1][i] = rightArrow;
            defaultBlock[hang2][i] = leftArrow;
            temp = temp +2;
        }

        for (int j=hang1; j<=hang2; j++){
            if (j!=hang1) defaultBlock[j][cot1] = upArrow;
            if (j!=hang2) defaultBlock[j][cot2] = downArrow;
            temp = temp +2;
        }
    
        hang1++; hang2--; cot1++; cot2--;
        temp = temp -4;
        if (hang2==hang1 && cot1==cot2){
            temp++;
            defaultBlock[hang1][cot1-1] = rightArrow;
        }
    }
    defaultBlock[0][0] = startSpace;
    defaultBlock[hang1][cot1] = finSpace;
}
void defaultZigzagMap(){
    int j;
    for (int i=0; i<nRow; i++){
        if (i%2 == 0){
            j=0;
            while (j<nCol){
                (j!=nCol-1) ? defaultBlock[i][j] = rightArrow : defaultBlock[i][j] = downArrow;
                j++;
            }
        } else{
            j=nCol-1;
            while (j>=0){
                (j!=0) ? defaultBlock[i][j] = leftArrow : defaultBlock[i][j] = downArrow;
                j--;
            }
        }
    }
    defaultBlock[0][0] = startSpace;
    ((nRow-1)%2==1) ? defaultBlock[nRow-1][0] = finSpace : defaultBlock[nRow-1][nCol-1] = finSpace;
}
void defaultUMap(){
    int temp=0;
    for (int i=0; i<nCol/2; i++){
        for (int j=0; j<nRow-i; j++){
            if (temp==0){
                defaultBlock[j][i] = downArrow;
                (j==0) ? defaultBlock[j][nCol-i-1] = leftArrow : defaultBlock[j][nCol-i-1] = upArrow;
            } else{
                (j==0) ? defaultBlock[j][i] = rightArrow : defaultBlock[j][i] = upArrow;
                defaultBlock[j][nCol-i-1] = downArrow;
            }
        }
        if (i!=nCol/2){
            for (int j=i; j<nCol-i; j++){
                if (temp==0 && j!=nCol-i-1) defaultBlock[nRow-i-1][j] = rightArrow;
                else if (temp!=0 && j!=i) defaultBlock[nRow-i-1][j] = leftArrow;
            }
        }
        (temp==0) ? temp++ : temp=0;
    }
    defaultBlock[0][0] = startSpace;
    ((nCol/2)%2==0) ? defaultBlock[0][nCol/2-1] = finSpace : defaultBlock[0][nCol/2] = finSpace;
}
void initializeBlock(){
    if (mapNum==1) defaultLineMap();
    else if (mapNum==2) defaultSpiralMap();
    else if (mapNum==3) defaultZigzagMap();
    else if (mapNum==4) defaultUMap();

    for (int i=0; i<nRow; i++){
        for (int j=0; j<nCol; j++){
            block[i][j]=defaultBlock[i][j];
            blockHorseNum[i][j] = 0;
            blockPlayerNum[i][j] = 0;
        }
    }
}
void initializePlayer(){
    for (int i=0; i<numOfPlayers; i++){
        for (int j=0; j<numOfHorses; j++){
            savedMoves[i][j] = 1;
            posCol[i][j] = 0; posRow[i][j] = 0;
            finishedHorse[i][j] = false;
        }
        horsesWin[i] = 0; 
    }
}

void welcomeMessage(){
    cout << " --------------------------------------------\n";
    cout << "|     Welcome to HORSES BOARDGAME!           |\n";
    cout << " --------------------------------------------\n";
    cout<< "\n\tEnter to choose map!";
    getch();
}
void printHorizontalLine(){
    for (int i = 0; i < nCol; i++) cout << " ---------";
    cout << "\n";
}
void printVerticalLine(){
    cout << "|";
    for (int i = 0; i < nCol; i++) cout << "         |";
    cout << "\n";
}
void setColor(char status){
    if (status==leftArrow) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),1);
    else if (status==rightArrow) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
    else if (status==upArrow) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),3);
    else if (status==downArrow) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4);
    else if (status=='D') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
}
void printBoard(){
    for (int i=0; i<nRow; i++){
        printHorizontalLine();
        printVerticalLine();

        cout << "|";
        for (int j=0; j<nCol; j++){
            if (block[i][j]=='`' || block[i][j]=='~'){
                if (block[i][j]=='`'){
                    if (mapNum==4) cout << " START " << downArrow <<" ";
                    else cout << "  START " << rightArrow;
                } else cout << " FINISH  ";
            } else{
                setColor(block[i][j]);
                if (block[i][j]!=defaultBlock[i][j]){
                    if (blockHorseNum[i][j]/10==0) cout << "   " << block[i][j] << " " << blockHorseNum[i][j]+1 <<"   ";
                    else cout << "   " << block[i][j] << " " << blockHorseNum[i][j]+1 <<"  ";
                } else cout << "    " << block[i][j] << "    ";
            }
            setColor('D');
            if (j==nCol-1) cout << "|\n"; else cout << "|";
        }

        printVerticalLine();
        printHorizontalLine();
    }
    setColor('D');
}
void printPlayersStatus(int playerNum){
    for (int i=0; i<numOfPlayers; i++){
        cout << "Player " <<i+1<< " - " << name[i]<<" got " <<horsesWin[i]<< " horse(s) finished!\n";
    }
}
void printStatistic(int matchCount, int isPlayerWin[]){
    cout << "Match(s) played: " << matchCount << endl;
    for (int i=0; i<numOfPlayers; i++){
        cout << "Player " << i+1 << ": " <<name[i]<< "          |       " << fixed << setprecision(2) << (float)((isPlayerWin[i]*100.0)/matchCount) << "%\n";
    }
}

int rollDice(char playerName){
    cout << "It's " << playerName << "'s turn!" << endl;
    cout << "Enter to roll dice! "; 
    getch();
    int temp = minDice + rand()% (maxDice+1 -minDice);
    cout << playerName << " got: " << temp << " moves!\a\n";
    return temp;
}
int chooseHorse(int playerNum){
    int horseNum;
    bool isValid = false;
    while (!isValid){
        cout << "Choose horse: "; cin >> horseNum; horseNum--;
        if (finishedHorse[playerNum][horseNum]==true){
            cout << "Horse choosen finished, please choose another!\n";
            isValid = false;
        } else isValid = true;
        if (horseNum>numOfHorses-1){
            cout << "Exceed number of horses, please choose again! \n";
            isValid = false;
        }
    }
    return horseNum;
}
void update(int row, int col, char status){
    block[row][col] = status;
}
void updateMap(int step, int &posRow, int &posCol){ 
    while (step!=0){
        if (posCol==0&& posRow==0){
            if (mapNum!=4){
                step--;
                posCol++;
            } else{
                step--;
                posRow++;
            }
        } else {
            if (defaultBlock[posRow][posCol]==rightArrow){
                posCol++; step--;
            } else if (defaultBlock[posRow][posCol]==leftArrow){
                posCol--; step--;
            } else if (defaultBlock[posRow][posCol]==upArrow){
                posRow--; step--;
            } else if (defaultBlock[posRow][posCol]==downArrow){
                posRow++; step--;
            }
        }
    }
}

bool checkValidMove(int step, int playerNum, int horseNum){
    if (savedMoves[playerNum][horseNum] + step <= nRow*nCol){ 
        savedMoves[playerNum][horseNum] = savedMoves[playerNum][horseNum] + step;
        return 1;
    } else return 0;
}
void checkBlankBlock(int playerNum, int horseNum, int row, int col){
    if (block[row][col] != defaultBlock[row][col]){
        cout << name[blockPlayerNum[row][col]] << "'s horse "<< blockHorseNum[row][col]+1 << " kicked to Start!\n";
        savedMoves[blockPlayerNum[row][col]][blockHorseNum[row][col]]=1;
        posRow[blockPlayerNum[row][col]][blockHorseNum[row][col]] = 0;
        posCol[blockPlayerNum[row][col]][blockHorseNum[row][col]] = 0;
        sleep(1);
    } 
    blockPlayerNum[row][col] = playerNum;
    blockHorseNum[row][col] = horseNum;
}
void checkFinish(int playerNum, int horseNum, int row, int col){
    if (savedMoves[playerNum][horseNum]==nRow*nCol){
        horsesWin[playerNum]++;
        finishedHorse[playerNum][horseNum] = true;
        block[row][col] = defaultBlock [row][col];
        cout << "Congratulation! Horse " <<horseNum+1<< " of " << name[playerNum] << " finished!";
        sleep(1);
    }
}
bool checkWinner(int playerNum, int &winner){
    if (horsesWin[playerNum]==numOfHorses){
        cout << "Player " << playerNum+1 << " - " << name[playerNum] << " win!\n";
        winner = playerNum;
        return true;
    } else return false;
}


void gamePlay(int playerNum){
    int step = rollDice(name[playerNum]);
    int horseNum = chooseHorse(playerNum);

    if (checkValidMove(step, playerNum, horseNum)){
        update(posRow[playerNum][horseNum], posCol[playerNum][horseNum], defaultBlock[posRow[playerNum][horseNum]][posCol[playerNum][horseNum]]);
        updateMap(step, posRow[playerNum][horseNum], posCol[playerNum][horseNum]);
        checkBlankBlock(playerNum, horseNum, posRow[playerNum][horseNum], posCol[playerNum][horseNum]);
        update(posRow[playerNum][horseNum], posCol[playerNum][horseNum], name[playerNum]);
        checkFinish(playerNum, horseNum, posRow[playerNum][horseNum], posCol[playerNum][horseNum]);
    } else{
        cout << "Invalid move!\n";
        sleep(1);
    }
}
int gameControl(bool &gameEnd){
    int playerNum = -1, winner;
    while (!gameEnd){
        (playerNum==numOfPlayers-1) ? playerNum=0 : playerNum++; //switchPlayer
        gamePlay(playerNum);

        sleep(1);
        system("CLS");
        
        printPlayersStatus(playerNum);
        printBoard();

        gameEnd = checkWinner(playerNum, winner); //Hinh nhu cho nay bi sai!
    }
    return winner;
}

int main(){
    welcomeMessage();
    aboutMap();
    aboutBoard();
    aboutPlayer();
    aboutDiceRange();

    system("CLS");
    initializeBlock();
    printBoard();
    srand(time(0));
    char gameReplay = 'Y';
    int matchCount = 0, isPlayerWin[100];
    for (int i=0; i< 51; i++) isPlayerWin[i]=0;
    while (gameReplay == 'Y' && matchCount < 100){
        matchCount++;
        bool gameEnd = 0;
        initializePlayer();
        initializeBlock();
        isPlayerWin[gameControl(gameEnd)]++;
        printStatistic(matchCount, isPlayerWin);
        cout << "Replay game? [Y/N] "; cin >> gameReplay;
        if (matchCount == 100) cout << "You played way too much! Please stop!";
    }
    
    return 0;
}