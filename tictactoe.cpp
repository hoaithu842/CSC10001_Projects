#include <iostream>
#include <windows.h>
#include <conio.h> //cursor
#include <fstream> //file
#include <string> //getline
#include <stdlib.h> //atoi
#include <cstring>
#include <iomanip> //setprecision

using namespace std;

const char UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77, ENTER = 13;
const int YELLOW = 238, PURPLE = 221, RED = 204, GREEN = 170, BLUE = 23;
const int yellow_ = 6, purple_ = 5, red_ = 4, green_ = 10, blue_ = 1;
int amount = 0, mode = 0, theme =0;

struct boardStruct{
    int mark[100][100];
    char symbol[100][100];
    int nCol, nRow;
    int MAX[100][100][5];
};
struct playerStruct{
    char icon;
    int color;
    int isRegisted = -1;
    char username[100];
};
struct accountRecordStruct{
   char username[100], password[100];
   int gamePlayed, gameWin;
};

boardStruct board;
struct playerStruct player[3];
struct accountRecordStruct account[100];

void gotoxy(int x, int y){ 
    COORD coord;
    coord.X = x; coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void setColor(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void stringToChar(string st, char arr[]){
    for (int i=0; i<st.length(); i++) arr[i] = st[i];
    arr[st.length()] = '\0';
}
void openFileToRead(){
    fstream file;
    file.open("data.txt", fstream::in); 
    string temp;
    getline(file, temp, '\n');
    amount = atoi(temp.c_str());
    
    for (int i=0; i<amount; i++){
        getline(file, temp, ';'); stringToChar(temp, account[i].username);
        getline(file, temp, ';'); stringToChar(temp, account[i].password);
        getline(file, temp, ';'); account[i].gamePlayed = atoi(temp.c_str());
        getline(file, temp, '\n'); account[i].gameWin = atoi(temp.c_str());
    }
    file.close();
}
void openFileToWrite(){
    fstream file;
    file.open("data.txt", fstream::out);
    file << amount << endl;
    
    for (int i=0; i<amount; i++){
        file << account[i].username << ";";
        file << account[i].password << ";";
        file << account[i].gamePlayed << ";";
        file << account[i].gameWin << endl;
    }
    file.close();
}
void openFileToSaveGame(bool saved, int playerNum, int ticked){
    fstream file;
    file.open("lastgame.txt", fstream::out);
    file << saved << endl;
    
    if (saved!=0){
        file << board.nRow << ";" << board.nCol << ";" << mode << ";" << theme << ";" << playerNum << ";" << ticked << endl;

        file << player[1].isRegisted << ";" << player[1].color << ";" << player[1].icon << endl;
        file << player[2].isRegisted << ";" << player[2].color << ";" << player[2].icon << endl;

        for (int i=0; i<board.nRow; i++){
            for (int j=0; j<board.nCol; j++) 
                if (j!=board.nCol-1) file << board.mark[i][j] << ";";
                else file << board.mark[i][j] << endl;
        }

        for (int k = 1; k < 5; k++){
            for (int i=0; i<board.nRow; i++){
                for (int j=0; j<board.nCol; j++) 
                    if (j!=board.nCol-1) file << board.MAX[i][j][k] << ";";
                    else file << board.MAX[i][j][k] << endl;
            }
        }

        for (int i=0; i<board.nRow; i++){
            for (int j=0; j<board.nCol; j++) 
                if (j!=board.nCol-1) file << board.symbol[i][j] << ";";
                else file << board.symbol[i][j] << endl;
        }
    }
    file.close();
}
void stringToCharArray(int row, string st){
    int count = 0;
    for (int i = 0; i<st.length(); i++){
        if (st[i]==';'){
            board.symbol[row][count] = st[i-1];
            count++;
        }
    }
    board.symbol[row][count] = st[st.length()-1];
}
void openFileToReadSavedGame(bool &saved, int &playerNum, int &ticked){
    fstream file;
    file.open("lastgame.txt", fstream::in);
    string temp;
    getline(file, temp, '\n');
    saved = atoi(temp.c_str());
    
    if (saved==0) return;
    getline(file, temp, ';'); board.nRow = atoi(temp.c_str());
    getline(file, temp, ';'); board.nCol = atoi(temp.c_str());
    getline(file, temp, ';'); mode = atoi(temp.c_str());
    getline(file, temp, ';'); theme = atoi(temp.c_str());
    getline(file, temp, ';'); playerNum = atoi(temp.c_str());
    getline(file, temp, '\n'); ticked = atoi(temp.c_str());

    getline(file, temp, ';'); player[1].isRegisted = atoi(temp.c_str());
    getline(file, temp, ';'); player[1].color = atoi(temp.c_str());
    file >> player[1].icon;
    
    getline(file, temp, ';'); player[2].isRegisted = atoi(temp.c_str());
    getline(file, temp, ';'); player[2].color = atoi(temp.c_str());
    file >> player[2].icon;
    

    for (int i=0; i<board.nRow; i++){
        for (int j=0; j<board.nCol; j++){
            if (j!=board.nCol-1) getline(file, temp, ';');
            else getline(file, temp, '\n');
            
            board.mark[i][j] = atoi(temp.c_str());         
        }
    }
    for (int k=1; k<5; k++){
        for (int i=0; i<board.nRow; i++){
            for (int j=0; j<board.nCol; j++){
                if (j!=board.nCol-1) getline(file, temp, ';');
                else getline(file, temp, '\n');
            
                board.MAX[i][j][k] = atoi(temp.c_str());         
            }
        }
    }
    for (int i=0; i<board.nRow; i++){
        getline(file, temp, '\n');
        stringToCharArray(i, temp);
    }
    file.close();
}

void printWelcomeMessage(int color, int goto_r, int goto_c){
    int i = 0;
    while (i!=5){
        setColor(color); 
        gotoxy(goto_c, goto_r+ i);    cout << " ______   __     ______     ______   ______     ______     ______   ______     ______    \n";
        gotoxy(goto_c, goto_r+ i+ 1); cout << "/\\__  _\\ /\\ \\   /\\  ___\\   /\\__  _\\ /\\  __ \\   /\\  ___\\   /\\__  _\\ /\\  __ \\   /\\  ___\\   \n"; 
        gotoxy(goto_c, goto_r+ i+ 2); cout << "\\/_/\\ \\/ \\ \\ \\  \\ \\ \\____  \\/_/\\ \\/ \\ \\  __ \\  \\ \\ \\____  \\/_/\\ \\/ \\ \\ \\/\\ \\  \\ \\  __\\   \n";
        gotoxy(goto_c, goto_r+ i+ 3); cout << "   \\ \\_\\  \\ \\_\\  \\ \\_____\\    \\ \\_\\  \\ \\_\\ \\_\\  \\ \\_____\\    \\ \\_\\  \\ \\_____\\  \\ \\_____\\ \n";
        gotoxy(goto_c, goto_r+ i+ 4); cout << "    \\/_/   \\/_/   \\/_____/     \\/_/   \\/_/\\/_/   \\/_____/     \\/_/   \\/_____/   \\/_____/ \n";
        gotoxy(goto_c, goto_r+ i+ 5); cout << "                                                                                         \n";
        setColor(7);
        Sleep(0.99999999999999999999*1000); 
        system("CLS");
        color = color + 26; i++;
    }
    setColor(7);
}
void printHorizontalLine(int nCol, int size){
    if (size == 7) for (int i = 0; i < nCol; i++) cout << " _______";
        else for (int i = 0; i < nCol; i++) cout << " ____________";
    cout << "\n";
}
void printVerticalLine(char c, int size){
    if (size == 7) cout << "|";
    for (int i = 0; i < board.nCol; i++){
        for (int j = 0; j < size; j++) cout << c;
        cout << "|";
    }
    cout << "\n";
}
void printStatus(int playerNum){
    cout << "Player 1: ";   
    cout << player[1].icon << "\n"; 
    if (mode==2) cout << "Player 2: "; else cout << "Bot: "; 
    cout << player[2].icon << "\n"; 
    if (mode==1 && playerNum==2) cout << "\t\tIt's bot's turn!\n";
    else cout << "\t\tIt's player " << playerNum << "'s turn!\n";
    cout << "[S]: Move suggestion...             [Q]: Quit game...\n";
}
void printBoard(int theme, int x, int y, int goto_r, int goto_c, bool isSuggest){
    if (theme==1){
        printHorizontalLine(board.nCol, 7);
        for (int i=0; i<board.nRow; i++){
            printVerticalLine(' ', 7);
            cout << "|";
            for (int j=0; j<board.nCol; j++){
                if (i==x && j==y){
                    if (isSuggest){
                        setColor(80); cout << "SUGGEST";
                        setColor(7);
                    }else if (board.mark[i][j]==0) cout << "<     >";
                    else{
                        setColor(55); cout << "<";
                        setColor(7);  cout << "     ";
                        setColor(55); cout << ">";
                    }
                }else if (board.mark[i][j]==1 || board.mark[i][j] ==2){
                    cout << "   ";
                    setColor(player[board.mark[i][j]].color);   cout << board.symbol[i][j];
                    setColor(7);                                cout << "   ";
                } else cout << "       ";
                setColor(7);    cout << "|";
            }
            cout << "\n";
            printVerticalLine('_', 7);
        }
    } else if (theme==2){
        gotoxy(goto_c, goto_r);
        printHorizontalLine(board.nCol, 13);
        for (int i=0; i<board.nRow; i++){
    	    for (int j=0; j<board.nCol; j++){
                int color = player[board.mark[i][j]].color;

                if (j==0) for (int k=1; k<6; k++){
                    gotoxy(goto_c+ 0, goto_r+ i*5+ k);    cout << "|";
                }

                if (i==x && j==y){
                    gotoxy(goto_c+ j*13+ 1, goto_r+ i*5+ 1);   cout << "            |\n";
                    gotoxy(goto_c+ j*13+ 1, goto_r+ i*5+ 2);   cout << "            |\n";
                    gotoxy(goto_c+ j*13+ 1, goto_r+ i*5+ 3);
                    if (isSuggest){
                        setColor(80);   cout << "   SUGGEST  ";
                        setColor(7);    cout << "|\n";
                    }else if (board.mark[i][j] != 0){
                        setColor(55);   cout << "<<";
                        setColor(7);    cout << "        ";
                        setColor(55);   cout << ">>";
                        setColor(7);    cout << "|\n";
                    } else cout << "<<        >>|\n";
                    gotoxy(goto_c+ j*13+ 1, goto_r+ i*5+ 4);    cout << "            |\n";

    		    } else if (board.symbol[i][j] == 'X'){
    		    	gotoxy(goto_c+ j*13+ 1, goto_r+ i*5+ 1);  cout << "            |\n";
    		    	gotoxy(goto_c+ j*13+ 1, goto_r+ i*5+ 2);
    		    	    cout << "  |"; setColor(color); cout << "  "; setColor(7); cout << "  |"; setColor(color); cout << "  "; setColor(7); cout << "  |\n";
    		    	gotoxy(goto_c+ j*13+ 1, goto_r+ i*5+ 3);
    		    	    cout << "    |"; setColor(color); cout << "   "; setColor(7); cout << "    |\n";
    		        gotoxy(goto_c+ j*13+ 1, goto_r+ i*5+ 4);
                        cout << "  |"; setColor(color); cout << "  "; setColor(7); cout << "  |"; setColor(color); cout << "  "; setColor(7); cout << "  |\n";
                } else if (board.symbol[i][j] == 'O'){
    		    	gotoxy(goto_c+ j*13+ 1, goto_r+ i*5+ 1);  cout << "            |\n";
    		    	gotoxy(goto_c+ j*13+ 1, goto_r+ i*5+ 2);
    		    	    cout << "  |"; setColor(color); cout << "       "; setColor(7); cout << "  |\n";
    		    	gotoxy(goto_c+ j*13+ 1, goto_r+ i*5+ 3);
    		    	    cout << "  |"; setColor(color); cout << "  "; setColor(7); cout << "  |"; setColor(color); cout << "  "; setColor(7); cout << "  |\n";
    		    	gotoxy(goto_c+ j*13+ 1, goto_r+ i*5+ 4);
                        cout << "  |"; setColor(color); cout << "       "; setColor(7); cout << "  |\n";
                } else{
    			    for (int k=1; k<5; k++){
                        gotoxy(goto_c+ j*13+ 1, goto_r+ i*5+ k);
                        cout << "            |\n";
    			    }
    		    }
                gotoxy(goto_c+ j*13+ 1, goto_r+ i*5+ 5);
                cout << "____________|";
    	    }
        }
        cout << "\n";
    }
}
void printAchievements(int stt){
    system("CLS");
    setColor(112); cout << "\t   ACHIEVEMENTS   \n";
    setColor(7); 
    cout << "Match played: \t\t\t" << account[stt].gamePlayed << "\n";
    cout << "Match won: \t\t\t" << account[stt].gameWin << "\n";
    if (account[stt].gamePlayed==0) cout << "Winning percentage: \t\t0%\n";
    else cout << "Winning percentage: \t\t" << fixed << setprecision(2) << (float)((account[stt].gameWin*100.0)/account[stt].gamePlayed) << "%\n";
    cout << "Press any key to quit viewing...";
    getch();
}
void printWinningMessage(int playerNum){
    if (playerNum==1 && mode ==2){
        cout << "\n\tCongratulation! Player 1 WIN!";
    } else if (playerNum==1 && mode ==1){
        cout << "\n\tCongratulation! You WIN!";
    } else if (playerNum==2 && mode ==2){
        cout << "\n\tCongratulation! Player 2 WIN!";
    } else{
        cout << "\n\tCongratulation! Bot WIN!";
    }
}
void printLeaderboard(){
    setColor(112); cout << "\t    LEADERBOARD     \n"; setColor(7);
    if (amount<1) cout << "There is no record/ account at the moment!\n";
    if (amount>0 && amount<10) 
        for (int i = 0; i<amount; i++) 
        cout << i+1 << ". " << account[i].username << ": \t\t" << fixed << setprecision(2) << (float)((account[i].gameWin*100.0)/account[i].gamePlayed) << "%\n";
    if (amount>0 && amount>=10)
        for (int i=0; i<10; i++)
        cout << i+1 << ". " << account[i].username << ": \t\t" << fixed << setprecision(2) << (float)((account[i].gameWin*100.0)/account[i].gamePlayed) << "%\n";
    cout << "\tThanks for playing!";
    getch();
}

int chooseMode(int goto_r, int goto_c){
    int temp = 2; char c = ' ';
    
    while (c!=ENTER){
        system("CLS");
        setColor(24); 
        gotoxy(goto_c, goto_r+ 0); cout << "|               |\n";
        gotoxy(goto_c, goto_r+ 1); cout << "|   GAME MODE   |\n";
        gotoxy(goto_c, goto_r+ 2); cout << "|               |\n";
        setColor(7); 
        gotoxy(goto_c+ 1, goto_r+ 3); cout << "Enter to choose!\n";

        gotoxy(goto_c+ 3, goto_r+ 5); if (temp==2) setColor(112);
            cout << "   P v P   \n"; setColor(7);
        gotoxy(goto_c+ 3, goto_r+ 6); if (temp==1) setColor(112);
            cout << "   P v E   \n"; setColor(7);

        goto1:
            c = getch();
            if (c!=UP && c!=DOWN && c!=ENTER) goto goto1;
        if (c==UP) temp--;
        else if (c==DOWN) temp++;

        if (temp==0) temp = 2;
        if (temp==3) temp = 1;
    }
    system("CLS");
    return temp;
}
int chooseTheme(){
    player[1].icon = 'O'; player[2].icon = 'X';
    board.mark[1][1] = 1; board.mark[3][1] = 2;
    board.symbol[1][1] = player[1].icon; board.symbol[3][1] = player[2].icon;

    int temp = 1; char c = ' ';
    while (c!=ENTER){
        system("CLS");
        gotoxy(2,1);
        if (c==LEFT) setColor(112); cout << " < ";
        setColor(24); cout << "     THEME " << temp << "    "; setColor(7);
        if (c==RIGHT) setColor(112); cout << " > \n";
        setColor(7); 

        if (temp==1){
            cout << "\nYou can input your icon from keyboard in this theme.\n";
            player[1].color = red_; player[2].color = yellow_;
            printBoard(1, -1, -1, 4, 0, 0);
        } else if (temp==2){
            cout << "\nThis theme is default and you can't change the icon.\n";
            player[1].color = RED; player[2].color = YELLOW;
            printBoard(2, -1, -1, 4, 0, 0);
        }
        goto2:
            c = getch();
            if (c!=LEFT && c!=RIGHT && c!=ENTER) goto goto2;
        if (c==LEFT) temp--;
        else if (c==RIGHT) temp++;

        if (temp==0) temp = 2;
        if (temp==3) temp = 1;
    }
    player[1].color = 0; player[1].icon = ' ';
    player[2].color = 0; player[2].icon = ' ';
    board.mark[1][1] = 0; board.mark[3][1] = 0;
    board.symbol[1][1] = ' '; board.symbol[3][1] = ' ';

    system("CLS");
    return temp;
}
void chooseSize(int goto_r, int goto_c){
    int temp = 1; char c = ' ';
    while (c!=ENTER){
        system("CLS");
        setColor(24); 
        gotoxy(goto_c, goto_r+ 0); cout << "|               |\n";
        gotoxy(goto_c, goto_r+ 1); cout << "|   BOARD SIZE  |\n";
        gotoxy(goto_c, goto_r+ 2); cout << "|               |\n";
        setColor(7); 
        gotoxy(goto_c+ 1, goto_r+ 3); cout << "Enter to choose!\n";

        if (temp==1) setColor(112);
        gotoxy(goto_c+ 2, goto_r+ 5); cout << "     3x3     \n"; setColor(7);
        if (temp==2) setColor(112);
        gotoxy(goto_c+ 2, goto_r+ 6); cout << "     5x5     \n"; setColor(7);
        if (temp==3) setColor(112);
        gotoxy(goto_c+ 2, goto_r+ 7); cout << "     7x7     \n"; setColor(7);
        if (temp==4) setColor(112);
        gotoxy(goto_c+ 2, goto_r+ 8); cout << " PlayerInput \n"; setColor(7);

        goto3:
            c = getch();
            if (c!=UP && c!=DOWN && c!=ENTER) goto goto3;
        if (c==UP) temp--;
        else if (c==DOWN) temp++;

        if (temp==0) temp = 4;
        if (temp==5) temp = 1;

        if (c==ENTER){
            if (temp==1){ board.nCol = 3; board.nRow = 3;}
            if (temp==2){ board.nCol = 5; board.nRow = 5;}
            if (temp==3){ board.nCol = 7; board.nRow = 7;}
            if (temp==4){ 
                board.nRow = 0; board.nCol = 0;
                while (board.nRow<3 || board.nRow>50){ 
                    gotoxy(goto_c+ 3, goto_r+ 10);
                    cout << "Input n: "; cin >> board.nRow; }
                while (board.nCol<3 || board.nCol>50){ 
                    gotoxy(goto_c+ 3, goto_r+ 11);
                    cout << "Input m: "; cin >> board.nCol; }
            }
        }
    }
    system("CLS");
}
void chooseColorAndIcon(int playerNum){
    system("CLS");
    int temp = 1; char c = ' ';
    while (c!=ENTER && playerNum!=0){
        system("CLS");
        gotoxy(8, 0);
        
        setColor(112); 
                        if (playerNum==1) cout << "|  PLAYER 1's COLOR  |\n";
        else if (playerNum==2 && mode==1) cout << "|     BOT's COLOR    |\n";
                   else if (playerNum==2) cout << "|  PLAYER 2's COLOR  |\n";
        setColor(7);

        gotoxy(6, 3);
        setColor(YELLOW); cout << "     ";
        setColor(PURPLE); cout << "     ";
        setColor(RED); cout << "     ";
        setColor(GREEN); cout << "     ";
        setColor(BLUE); cout << "     ";
        
        if (temp==1) gotoxy(8,4);
        if (temp==2) gotoxy(13,4);
        if (temp==3) gotoxy(18,4);
        if (temp==4) gotoxy(23,4);
        if (temp==5) gotoxy(28,4);
        setColor(7); cout << "^\n";

        goto4:
            c = getch();
            if (c!=LEFT && c!=RIGHT && c!=ENTER) goto goto4;
        if (c==LEFT) temp--;
        else if (c==RIGHT) temp++;

        if (temp==0) temp = 5;
        if (temp==6) temp = 1;
    }

if (playerNum!=0){
    if (temp==1) if (theme==2) player[playerNum].color = YELLOW; else player[playerNum].color = yellow_;
    if (temp==2) if (theme==2) player[playerNum].color = PURPLE; else player[playerNum].color = purple_;
    if (temp==3) if (theme==2) player[playerNum].color = RED; else player[playerNum].color = red_;
    if (temp==4) if (theme==2) player[playerNum].color = GREEN; else player[playerNum].color = green_;
    if (temp==5) if(theme==2) player[playerNum].color = BLUE; else player[playerNum].color = blue_;
} else{
    system("CLS");
    gotoxy(8,0);
        
    setColor(112); cout << "|     BOT's COLOR    |\n"; setColor(7);
    cout << "\nBot is set to this color : ";
    setColor(112); cout << "     "; setColor(7); cout << " by default!";
    if (theme==2) player[2].color = 112; else player[2].color = 7;
}


    setColor(112); 
    gotoxy(8, 7); 
         if (playerNum==1) cout << "|  PLAYER 1's ICON   |\n";
    else if (playerNum==0) cout << "|     BOT's ICON     |\n";
    else if (playerNum==2) cout << "|  PLAYER 2's ICON   |\n";
    setColor(7);

    if (playerNum == 2){
        if (theme!=2){
            cout << "\nType in your icon, 1 char only: "; player[2].icon = getch();
            while (player[2].icon < 33 || player[2].icon > 126 ) player[2].icon = getch();
        }else{
            if (player[1].icon == 'X') player[2].icon = 'O';
            else player[2].icon = 'X';
            if (mode == 2) cout << "\nPlayer 2 is set to " << player[2].icon << " by default!";
        }
    }else if (playerNum == 1){
        if (theme!=2){
            cout << "\nType in your icon, 1 char only: "; player[1].icon = getch();
            while (player[1].icon < 33 || player[1].icon > 126 ) player[1].icon = getch();
        } else{
            while (player[1].icon!='X' && player[1].icon!='O'){
                cout << "\nType in your icon, X/O only: "; 
                while (player[1].icon!='X' && player[1].icon!='O') player[1].icon = getch();
            }
        }
    } else if (playerNum ==0){
        if (player[1].icon=='O') player[2].icon = 'X'; else player[2].icon = 'O';
        cout << "\nYour bot is set to " << player[2].icon << " by default!";
    }
}
bool chooseBeforeGameOption(){
    system("CLS");
    int temp = 1; char c = ' ';
    while (c!=ENTER){
        system("CLS");
        cout << "Continue?\n\n";
        if (temp==1) setColor(112);
        cout << "  YES "; setColor(7);
        if (temp==2) setColor(112);
        cout << "  NO  "; setColor(7);

        goto12:
            c = getch();
            if (c!=ENTER && c!=LEFT && c!=RIGHT) goto goto12;
        if (c==LEFT) temp--;
        if (c==RIGHT) temp++;

        if (temp==3) temp=1;
        if (temp==0) temp=2;
    }

    if (temp==1) return 1; else return 0;
}
bool chooseAfterGameOption(int goto_r, int goto_c){
    int temp = 1; char c = ' ';
    while (c!=ENTER){
        system("CLS");
        setColor(24); 
        gotoxy(goto_c,goto_r+0); cout << "|                |\n";
        gotoxy(goto_c,goto_r+1); cout << "|     OPTION     |\n";
        gotoxy(goto_c,goto_r+2); cout << "|                |\n";
        setColor(7); 

        gotoxy(goto_c+3,goto_r+5); if (temp==1) setColor(112);
        cout << "   Replay   \n"; setColor(7);
        gotoxy(goto_c+3,goto_r+6); if (temp==2) setColor(112);
        cout << "    Quit    \n"; setColor(7);

        recin:
            c = getch();
            if (c!=UP && c!=DOWN && c!=ENTER) goto recin;
        if (c==UP) temp--;
        else if (c==DOWN) temp++;

        if (temp==0) temp = 2;
        if (temp==3) temp = 1;
    }
    system("CLS");
    if (temp==1) return 1; else return 0;
}
void chooseQuitOption(int playerNum, int ticked){
    system("CLS");
    int temp = 1; char c = ' ';
    while (c!=ENTER){
        system("CLS");
        cout << "Save game?\n\n";
        if (temp==1) setColor(112);
        cout << " SAVE "; setColor(7);
        if (temp==2) setColor(112);
        cout << " QUIT "; setColor(7);
        if (temp==3) setColor(112);
        cout << " BACK "; setColor(7);
        cout << "\n";

        goto11:
            c = getch();
            if (c!=ENTER && c!=LEFT && c!=RIGHT) goto goto11;
        if (c==LEFT) temp--;
        if (c==RIGHT) temp++;

        if (temp==4) temp=1;
        if (temp==0) temp=3;
    }

    if (temp==2){
        openFileToWrite();
        openFileToSaveGame(0, 0, 0);
        exit(0);
    }else if (temp==1){
        openFileToWrite();
        openFileToSaveGame(1, playerNum, ticked);
        exit(0);
    }else if (temp==3) return;
}

bool login(int playerNum){
    char username[100], password[100];
    tryAgain:
        int error = 0; 
        system("CLS");
        cout << "username: "; cin >> username;
        cout << "password: "; cin >> password;
        if (playerNum==2 && strcmp(player[1].username,username) == 0){
            cout << "that account logged in as player 1, please choose another!";
            error = 2;
        }else{
            for (int i=0; i<amount; i++)
                if (strcmp(account[i].username, username) == 0){
                    if (strcmp(account[i].password, password) == 0){
                        cout << "succesfully logged in!\n";
                        Sleep(1000);
                        player[playerNum].isRegisted = i;
                        player[playerNum].username[0] = '\0';
                        strncpy(player[playerNum].username, username, strlen(username));
                        return 1;
                    }else{
                        cout << "wrong password! sign in again? "; 
                        error = 1;
                    }
                }
        }
        if (error!=1 && error!=2) cout << "invalid username! sign in again? ";
        int temp = 1; char c = ' '; 
        while (c!=ENTER){
            system("CLS");
            if (error==1) cout << "wrong password! sign in again? ";
            else if (error ==2) cout << "that account logged in as player 1, please choose another!";
            else cout << "invalid username! sign in again? "; 
            
            if (temp==1) setColor(112); 
            gotoxy(3,5); cout << " BACK "; setColor(7);
            if (temp==2) setColor(112);
            gotoxy(9,5); cout << " AGAIN \n"; setColor(7);
                        
            retry:
                c = getch();
                if (c!=LEFT && c!=RIGHT && c!=ENTER) goto retry;
            
            if (c==LEFT) temp--;
            else if (c==RIGHT) temp++;

            if (temp==0) temp = 2;
            if (temp==3) temp = 1;
        }
        if (temp==1) return 0;
        else if (temp==2) goto tryAgain;
    return 0;
}
bool createAcc(int playerNum){
    char username[100], password[100];
    system("CLS");
    if (amount == 100){
        cout << "Full accounts, please sign in or play as Guest!";
        return 0;
    }
    recin1:
        system("CLS");
        cout << "username: "; cin >> username;
        for (int i=0; i<amount; i++) if (strcmp(account[i].username, username) == 0){
            cout << "this username has been used. please try another!\n";
            int temp = 1; char c = ' '; 
            while (c!=ENTER){
                system("CLS");
                cout << "this username has been used. please try another!\n";
            
                if (temp==1) setColor(112); 
                gotoxy(3,5); cout << " BACK "; setColor(7);
                if (temp==2) setColor(112);
                gotoxy(9,5); cout << " AGAIN \n"; setColor(7);
                        
                gotoo:
                    c = getch();
                    if (c!=LEFT && c!=RIGHT && c!=ENTER) goto gotoo;
            
                if (c==LEFT) temp--;
                else if (c==RIGHT) temp++;

                if (temp==0) temp = 2;
                if (temp==3) temp = 1;
            }
            
            if (temp==1) return 0;
            else if (temp==2) goto recin1;
    } 

    account[amount].username[0] = '\0';
    strncpy(account[amount].username, username, strlen(username));

    cout << "password: "; cin >> password;
    account[amount].password[0] = '\0';
    strncpy(account[amount].password, password, strlen(password));

    account[amount].gamePlayed = 0;
    account[amount].gameWin = 0;
    amount++;
    cout << "registed successfully! log in again to play!\n";
    Sleep(1000);
    if (login(playerNum)) return 1; else return 0;
}
bool viewAcc(int playerNum, int col, int row){
    tryy:
    int temp = 1; char c = ' ';
        while (c!=ENTER){
            system("CLS");
            gotoxy(col+0,row+0); cout << "Player " << playerNum << "'s account:\n";
            gotoxy(col+1,row+1); cout << "Enter to choose!\n";
        
            if (temp==1) setColor(112);   
            gotoxy(col+3,row+5); cout << " Achievements \n"; setColor(7);
            if (temp==2) setColor(112);
            gotoxy(col+3,row+6); cout << "  Delete Acc  \n"; setColor(7);
            if (temp==3) setColor(112);
            gotoxy(col+3,row+8); cout << "   Continue   \n"; setColor(7);
       
            recinn:
                c = getch();
                if (c!=UP && c!=DOWN && c!=ENTER) goto recinn;
            if (c==UP) temp--;
            else if (c==DOWN) temp++;

            if (temp==0) temp = 3;
            if (temp==4) temp = 1;
        }
    if (temp==1){
        printAchievements(player[playerNum].isRegisted);
        goto tryy;
    } else if (temp==2){
        for (int i=player[playerNum].isRegisted; i<amount-1; i++){
            if (i+1==player[1].isRegisted) player[1].isRegisted = i;
            else if (i+1==player[2].isRegisted) player[2].isRegisted = i;
            account[i] = account[i+1];
        }
        cout << "Account Deleted!"; Sleep(2000);
        amount--;
        return 0;
    } else{
        cout << "Please choose icon and color!";
        Sleep(2000);
        return 1;
    } 
}
void chooseAccountOption(int playerNum, int row, int col){
    bool inSuccessful = 0;
    while (!inSuccessful){
        int temp = 1; char c = ' ';
        while (c!=ENTER){
            system("CLS");
            gotoxy(col,row+0); cout << "Player " << playerNum << " please select!\n";
            gotoxy(col+1,row+1); cout << "Enter to choose mode!\n";
        
            if (temp==1) setColor(112);
            gotoxy(col+4,row+5); cout << "   Sign Up   \n"; setColor(7);
            if (temp==2) setColor(112);
            gotoxy(col+4,row+6); cout << "   Sign In   \n"; setColor(7);
            if (temp==3) setColor(112);
            gotoxy(col+4,row+7); cout << "   as Guest  \n"; setColor(7);
       
            recinn:
                c = getch();
                if (c!=UP && c!=DOWN && c!=ENTER) goto recinn;
            if (c==UP) temp--;
            else if (c==DOWN) temp++;

            if (temp==0) temp = 3;
            if (temp==4) temp = 1;
        }
        system("CLS");
        if (temp==1) {
            inSuccessful = createAcc(playerNum);
            if (inSuccessful) inSuccessful = viewAcc(playerNum, 2, 1);
        } else if (temp==2) {
            inSuccessful = login(playerNum);
            if (inSuccessful) inSuccessful = viewAcc(playerNum, 2, 1);
        } else{
            inSuccessful = 1;
            player[playerNum].isRegisted = -1;
        }
    }
    chooseColorAndIcon(playerNum);
}

void bot(int &tickedX, int &tickedY){
    int player1morethan4x = -1, player1morethan4y = -1;
    int randomx = -1, randomy = -1;
    int player1equal3x = -1, player1equal3y = -1;
    int botequal3x = -1, botequal3y = -1;
    int trickx = -1, tricky = -1;

    for (int i = 0; i < board.nRow; i++){
        for (int j = 0; j < board.nCol; j++){
            int temp1= 0, temp2= 0, temp3= 0, temp4= 0;
            int temp5= 0, temp6= 0, temp7= 0, temp8= 0;
            if (board.mark[i][j] == 0){
                randomx = i; randomy = j;

                if (board.mark[i-1][j+1] == 1) temp1 = board.MAX[i-1][j+1][1]; else temp5 = board.MAX[i-1][j+1][1];
                if (board.mark[i+1][j-1] == 1) temp1 = temp1 + board.MAX[i+1][j-1][1]; else temp5 = temp5 + board.MAX[i+1][j-1][1];
                
                if (board.mark[i][j+1] == 1) temp2 = board.MAX[i][j+1][2]; else temp6 = board.MAX[i][j+1][2];
                if (board.mark[i][j-1] == 1) temp2 = temp2 + board.MAX[i][j-1][2]; else temp6 = temp6 + board.MAX[i][j-1][2];
                
                if (board.mark[i-1][j-1] == 1) temp3 = board.MAX[i-1][j-1][3]; else temp7 = board.MAX[i-1][j-1][3];
                if (board.mark[i+1][j+1] == 1) temp3 = temp3 + board.MAX[i+1][j+1][3]; else temp7 = temp7 + board.MAX[i+1][j+1][3];

                if (board.mark[i-1][j] == 1) temp4 = board.MAX[i-1][j][4]; else temp8 = board.MAX[i-1][j][4];
                if (board.mark[i+1][j] == 1) temp4 = temp4 + board.MAX[i+1][j][4]; else temp8 = temp8 + board.MAX[i+1][j][4];

                    if ((temp5==2 && temp6==2) || (temp5==2 && temp7==2) || (temp5==2 && temp8==2) || (temp6==2 && temp7==2) || (temp6==2 && temp8==2) || (temp7==2 && temp8==2)){
                        trickx = i; tricky = j;
                    }else if ((temp1==2 && temp2==2) || (temp1==2 && temp3==2) || (temp1==2 && temp4==2) || (temp2==2 && temp3==2) || (temp2==2 && temp4==2) || (temp3==2 && temp4==2)){
                        trickx = i; tricky = j;
                    }
            }
            if (temp5 >= 4 || temp6 >=4 || temp7 >=4 || temp8 >=4){ tickedX = i; tickedY = j; return;}
            if (temp1 >= 4 || temp2 >=4 || temp3 >=4 || temp4 >=4){ player1morethan4x = i; player1morethan4y = j;}
            if (temp1 == 3 || temp2 ==3 || temp3 ==3 || temp4 ==3){ player1equal3x = i; player1equal3y = j;}
            if (temp5 == 3 || temp6 ==3 || temp7 ==3 || temp8 ==3){ botequal3x = i; botequal3y = j;}
        }
    }
    if (player1morethan4x != -1){ tickedX = player1morethan4x; tickedY = player1morethan4y; return;}
    if (botequal3x!= -1){ tickedX = botequal3x; tickedY = botequal3y; return;}
    if (player1equal3x!= -1){ tickedX = player1equal3x; tickedY = player1equal3y; return;}
    if (trickx!=-1){ tickedX = trickx; tickedY = tricky; return; }
    if (randomx!= -1){ tickedX = randomx; tickedY = randomy; return;}
}
void tick(int playerNum, int &x, int &y, int ticked){
    x = 0, y = 0;
    char c = ' ';
    while (c!=ENTER){
        system("CLS");
        printStatus(playerNum);

        printBoard(theme, x, y, 10, 0, 0);

        rec:
            c = getch();
            if (c!=LEFT && c!=RIGHT && c!=UP && c!=DOWN && c!=ENTER && c!='S' && c!='Q' && c!= 's' && c!='q') goto rec; //kh enter vuot, kh
            if (c==ENTER && board.mark[x][y]!=0) goto rec;
        if (c==UP) x--; if (c==DOWN) x++;
        if (c==LEFT) y--; if (c==RIGHT) y++;
        
        if (x==-1) x = board.nRow-1; if (x==board.nRow) x = 0;
        if (y==-1) y = board.nCol-1; if (y==board.nCol) y = 0;
        
        if (c=='S' || c=='s'){
            int tempx = 0, tempy = 0;
            bot(tempx, tempy);
            system("CLS");
            printBoard(theme, tempx, tempy, 10, 0, 1);
            Sleep(1000);
        }
        if (c=='Q' || c=='q') chooseQuitOption(playerNum, ticked);
    }
    board.symbol[x][y] = player[playerNum].icon;
    board.mark[x][y] = playerNum;
}
void update(int x, int y, int max, int direction, int nInaRow){
    int count = 0, lefti = 0, leftj = 0, righti = 0, rightj = 0;
    bool lock1 = 0, lock2 = 0;
    while (count!=nInaRow-1){
    	count++;
        if (direction == 1){ lefti++; leftj--; righti--; rightj++; }
        else if (direction == 2){ leftj--; rightj++; }
        else if (direction == 3){ lefti--; leftj--; righti++; rightj++; }
        else if (direction ==4){ lefti--; righti++; }

    	if (board.mark[x][y] == board.mark[x+lefti][y+leftj] && !lock1) board.MAX[x+lefti][y+leftj][direction] = max; else lock1 = 1;
    	if (board.mark[x][y] == board.mark[x+righti][y+rightj] && !lock2) board.MAX[x+righti][y+rightj][direction] = max; else lock2 = 1;
    }
}
bool winnerCheck(INT X, INT Y, INT nInARow){
    for (int i = -1; i < 2; i++){
        for (int j = -1; j < 2; j++){
        if (i!=0 || j!=0){
            if (board.mark[X][Y] == board.mark[X+i][Y+j]){
                if (i+j!=1 && i+j!=-1) board.MAX[X][Y][abs(i+j)+1] = board.MAX[X][Y][abs(i+j)+1] + board.MAX[X+i][Y+j][abs(i+j)+1];
                else if (i==0) board.MAX[X][Y][2] = board.MAX[X][Y][2] + board.MAX[X+i][Y+j][2];
                else if (j==0) board.MAX[X][Y][4] = board.MAX[X][Y][4] + board.MAX[X+i][Y+j][4];
            } 
        }
        }
    }
    for (int i = 1; i < 5; i++) if (board.MAX[X][Y][i] +1 >1) update(X, Y, board.MAX[X][Y][i]+1, i, nInARow);
    board.MAX[X][Y][1]++; board.MAX[X][Y][2]++; board.MAX[X][Y][3]++; board.MAX[X][Y][4]++;
    if (board.MAX[X][Y][1] >= nInARow) return 1; 
    if (board.MAX[X][Y][2] >= nInARow) return 1;
    if (board.MAX[X][Y][3] >= nInARow) return 1;
    if (board.MAX[X][Y][4] >= nInARow) return 1;
    return 0;        
}
int controlGame(int playerNum, int ticked){
    bool stop = 0;
    int tickedX, tickedY;
    while (!stop && ticked!=board.nCol*board.nRow){
        if (playerNum==2) playerNum = 1;
        else playerNum++;

        if (mode==2) tick(playerNum, tickedX, tickedY, ticked);
        else if (playerNum == 1) tick(playerNum, tickedX, tickedY, ticked);
        else {
            bot(tickedX, tickedY);
            board.mark[tickedX][tickedY] = 2;
            board.symbol[tickedX][tickedY] = player[2].icon;
            system("CLS");
            printBoard(theme, tickedX, tickedY, 0, 0, 0);
        }
        ticked++;
        if (board.nRow>5 && board.nCol>5) stop = winnerCheck(tickedX, tickedY, 5);
        else stop = winnerCheck(tickedX, tickedY, 3);
    }
    if (!stop) return 0; else return playerNum;
}

void initialize(){
    for (int i = 0; i < board.nRow; i++){
        for (int j = 0; j < board.nCol; j++){
            board.mark[i][j] = 0;
            board.symbol[i][j] = ' ';
            board.MAX[i][j][1] = 0; board.MAX[i][j][2] = 0; board.MAX[i][j][3] = 0; board.MAX[i][j][4] = 0;
        }
    }
}
void sort(){
    for (int i=0; i<amount-1; i++){
        for (int j=i+1; j<amount; j++){
            if (account[i].gamePlayed!=0)
            if (((account[i].gameWin*1.0)/(account[i].gamePlayed*1.0)) < ((account[j].gameWin*1.0)/(account[j].gamePlayed*1.0)) || account[j].gamePlayed==0){
                accountRecordStruct temp;
                temp = account[i];
                account[i] = account[j];
                account[j] = temp;
            }
        }
    }
}
void updateWinning(int playerWin){
    if (player[1].isRegisted!=-1) account[player[1].isRegisted].gamePlayed++;
    if (player[2].isRegisted!=-1) account[player[2].isRegisted].gamePlayed++;
    if (playerWin==2 && mode==1) return;
    else account[player[playerWin].isRegisted].gameWin++;
}

int main(){
    system("CLS");
    openFileToRead();
    bool saved; bool choice;
    int nowPlayer, ticked;
    openFileToReadSavedGame(saved, nowPlayer, ticked);
    printWelcomeMessage(37, 15, 45);
    if (saved==1) choice = chooseBeforeGameOption();

    if (saved==1 && choice == 1){
        if (nowPlayer==1) nowPlayer = 2;
        else nowPlayer = 1; 
    }else{
        mode = chooseMode(15, 75);
        chooseSize(15, 75);  
        theme = chooseTheme();

        if (mode == 1){
            chooseAccountOption(1, 15, 75);
            chooseColorAndIcon(0);
            cout << "\nPlayer 2: Your bot is ready!";
            Sleep(2000);
        }
        else{
            chooseAccountOption(1, 15, 75);
            chooseAccountOption(2, 15, 75);
            cout << "\nGame is ready!";
            Sleep(2000);
        }
    }

    bool playAgain = 1;
    while (playAgain){
        if (saved!=1 || choice != 1){
            initialize();
            nowPlayer=2; ticked=0;
        } else saved = 0;
        
        int temp = controlGame(nowPlayer, ticked);
        system("CLS");
        printBoard(theme, -1, -1, 10, 0, 0);
        if (temp==0) cout << "\nTie!"; else printWinningMessage(temp);
        Sleep(2000);
        updateWinning(temp);
        playAgain = chooseAfterGameOption(15, 75);
    }
    system("CLS");
    sort();
    printLeaderboard();
    openFileToWrite();
    openFileToSaveGame(0, 0, 0);
    return 0;
}