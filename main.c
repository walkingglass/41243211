#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define checkerboard_x 16   //棋盤X軸
#define checkerboard_y 16   //棋盤Y軸

void n16(void);
void input(int);

int checkerboard[checkerboard_x][checkerboard_y] = { 2 }, checkerboard_history[checkerboard_x][checkerboard_y][512] = { 2 };//棋盤 歷史棋盤(空2 黑0 白1 )
int player = 0;                                                                     //玩家狀態(黑0 白1 )
char letter, cletter[128] = { ' ' }, location[] = { ' ' }, history[2048][4] = { "" };     //輸入字元 指令字串 下子定位 輸入下子歷史列表
int number, cnumber, Win = 0, history_flag = 0, history_num = 0, language = 0;              //輸入數字 指令數字 獲勝狀態 歷史列表旗標 歷史棋盤計數 語言
int flag_0 = 0, flag_45 = 0, flag_90 = 0, flag_135 = 0;                             //當顆棋子的8方角度(共4個)
int first_o16 = 0;

void show()//顯示棋盤
{
    system("cls");//清除終端
    printf("╔══════════════════════════════════╗\n");
    printf("║    ");
    for (char alpha = 'A'; alpha <= 'O'; alpha++)//放置定位字母
    {
        checkerboard[alpha - 'A'][0] = alpha;
        printf("%c ", checkerboard[alpha - 'A'][0]);
    }
    printf("║\n");
    for (short int y = 1; y <= checkerboard_y - 1; y++)
    {
        checkerboard[0][y] = y;
        printf("║ %2i ", checkerboard[0][y]);//放置定位數字

        for (short int x = 1; x <= checkerboard_x - 1; x++)//放置旗子與空格
        {
            if (checkerboard[x][y] == 0)printf("○");
            else if (checkerboard[x][y] == 1)printf("●");
            else printf("+ ");
        }
        printf("║\n");
    }
    printf("╚══════════════════════════════════╝\n");

    for (int i = 0; i < checkerboard_x; ++i) {//備份棋盤
        for (int j = 0; j < checkerboard_y; ++j) {
            checkerboard_history[i][j][history_num + 1] = checkerboard[i][j];
        }
    }
    history_num++;
    if (language == 0)printf("Time location:%d\n", history_num);
    else if (language == 1)printf("時間位置:%d\n", history_num);
    if (language == 0)printf("Number of entries:%d\n", history_flag);
    else if (language == 1)printf("輸入次數:%d\n", history_flag);
}

void n16(void)
{
    if (language == 0)printf("\ncommand(letter+' '+number)(NULL->0):");
    else if (language == 1)printf("\n命令(英文+' '+數字)(空->0):");
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);// 清除輸入緩衝區中的多餘字符
    scanf("%s %d", &cletter, &cnumber);
    if (strcmp(cletter, "help") == 0 || strcmp(cletter, "H") == 0)//指令列表
    {
        printf("-------------------------\nhelp\n");
        printf("{e+0}exit+0\n");
        printf("{H+0}help+0\n");
        printf("{h+0}history+0\n");
        printf("{h+number}history+number\n");
        printf("{t+0}time+0\n");
        printf("{t+number}time+number[number>1]\n");
        printf("{en+0}=english\n");
        printf("{ch+0}=chinese\n");
        system("pause");
        n16();
    }
    else if ((strcmp(cletter, "history") == 0 && cnumber == 0) || (strcmp(cletter, "h") == 0 && cnumber == 0))//列出全部歷史輸入紀錄
    {
        printf("-------------------------\nHistory\n ");
        for (int i = 0; i < history_flag; i++)
        {
            printf("%d.%s\n ", i + 1, history[i]);
        }
    }
    else if ((strcmp(cletter, "time") == 0 && cnumber == 0) || (strcmp(cletter, "t") == 0 && cnumber == 0))//列出全部歷史棋盤紀錄
    {
        int i = 1;
        while (i < history_num)  // 使用歷史數量控制迴圈
        {
            if (language == 0)printf("time location:%d\n", i);
            else if (language == 1)printf("時間位置:%d\n", i);
            printf("╔══════════════════════════════════╗\n");
            printf("║    ");
            for (char alpha = 'A'; alpha <= 'O'; alpha++)//放置定位字母
            {
                checkerboard[alpha - 'A'][0] = alpha;
                printf("%c ", checkerboard[alpha - 'A'][0]);
            }
            printf("║\n");

            printf("║ %2i ", i + 1);  // 修正顯示的歷史棋盤數字

            for (short int x = 1; x <= checkerboard_x - 1; x++)
            {
                for (short int y = 1; y <= checkerboard_y - 1; y++)
                {
                    if (checkerboard_history[y][x][i] == 0) printf("○");
                    else if (checkerboard_history[y][x][i] == 1) printf("●");
                    else printf("+ ");
                }
                printf("║\n");
                if (x < checkerboard_x - 1) printf("║ %2i ", x + 1);  // 修正顯示的歷史棋盤數字
            }
            printf("╚══════════════════════════════════╝\n");
            i++;
        }
    }
    else if ((strcmp(cletter, "time") == 0 && cnumber > 1) || (strcmp(cletter, "t") == 0 && cnumber > 1))//傳送至歷史有紀錄的指定棋盤
    {
        if (cnumber <= history_num + 1)
        {
            for (int i = 0; i < checkerboard_x; ++i) {
                for (int j = 0; j < checkerboard_y; ++j) {
                    checkerboard[i][j] = checkerboard_history[i][j][cnumber];
                }
            }
            show();
            input(1);
        }
        else {//錯誤報錯
            show();
            if (language == 0) {
                printf("\033[31mERROR7(non-existent future)\033[0m\n");
                printf("\033[31mAGAIN\033[0m\n");
            }
            else if (language == 1)
            {
                printf("\033[31m錯誤7(不存在的未來)\033[0m\n");
                printf("\033[31m再次輸入\033[0m\n");
            }
            printf("\n");
            history_num--;
            input(1);
        }
    }
    else if ((strcmp(cletter, "history") == 0 && cnumber > 0) || (strcmp(cletter, "h") == 0 && cnumber > 0))//列出指定歷史輸入紀錄
    {
        printf("-------------------------\nHistory->%d\n ", cnumber);
        printf("%d.%s\n ", cnumber, history[cnumber - 1]);
    }
    else if (strcmp(cletter, "en"))//切換英文
    {
        language = 1;
    }
    else if (strcmp(cletter, "ch"))//切換中文
    {
        language = 0;
    }
    if (strcmp(cletter, "exit") || strcmp(cletter, "e") == 0)//離開
    {
        system("pause");
        show();
        input(1);
    }

    n16();
}
void input(int in)//輸入系統
{
    if (Win == 0) {//沒勝負時
        if (player == 0)//顯示上次棋子與其位置和現在棋子
        {
            printf("●%s\n", location);
            printf("○");
        }
        else if (player == 1)
        {
            printf("○%s\n", location);
            printf("●");
        }
        if (language == 0)printf("\nLOCATION(letter+number):");
        else if (language == 1)printf("\n棋子位置(字母+數字):");
        if (in == 1) //不是初次輸入
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);// 清除輸入緩衝區中的多餘字符
        }

        if (scanf("%c%d", &letter, &number) == 2)
        {
            sprintf(location, "%c%d", letter, number);//以資料輸入
            strcpy(history[history_flag], location);//複製至歷史輸入
            history_flag++;
            if (((letter >= 'a' && letter <= 'o') || (letter >= 'A' && letter <= 'O')))//限制大小寫A到O
            {
                if (letter >= 'a' && letter <= 'o')letter = toupper((unsigned char)letter);//小寫轉大寫

                int letter_num = 0;
                while (1)//對應A到O,將A到O存成棋盤的1到15
                {
                    if (letter == ('A' + letter_num))break;
                    letter_num++;
                }
                if (number > 0 && number <= 16)//限制數字
                {
                    if ((letter_num + 1) == 14 && number == 16)//n16,指令
                    {
                        history_num--;
                        n16();
                        system("pause");
                        show();
                        input(1);
                    }
                    else if ((letter_num + 1) == 15 && number == 16)//o16,悔棋
                    {
                        if (history_flag > 2)
                        {
                            if (history_num > 1)
                            {
                                for (int i = 0; i < checkerboard_x; ++i) {//回傳棋盤
                                    for (int j = 0; j < checkerboard_y; ++j) {
                                        checkerboard[i][j] = checkerboard_history[i][j][history_num - 2];
                                    }
                                }
                                history_num -= 2;
                                if (first_o16 == 0)
                                {
                                    history_num++;
                                    first_o16++;
                                }
                                if (history_num == 0)history_num = 1;//修正

                                if (player == 1)
                                {
                                    player = 0;
                                }
                                else if (player == 0)
                                {
                                    player = 1;
                                }
                                show();
                                input(1);
                            }
                            else {//錯誤報錯
                                show();
                                if (language == 0) {
                                    printf("\033[31mERROR1(NO history checkerboard)\033[0m\n");
                                    printf("\033[31mAGAIN\033[0m\n");
                                }
                                else if (language == 1)
                                {
                                    printf("\033[31m錯誤1(無棋盤歷史)\033[0m\n");
                                    printf("\033[31m再次輸入\033[0m\n");
                                }
                                printf("\n");
                                history_num--;
                                input(1);
                            }
                        }
                        else {//錯誤報錯
                            show();
                            if (language == 0) {
                                printf("\033[31mERROR2(NO history data)\033[0m\n");
                                printf("\033[31mAGAIN\033[0m\n");
                            }
                            else if (language == 1)
                            {
                                printf("\033[31m錯誤2(無歷史資料)\033[0m\n");
                                printf("\033[31m再次輸入\033[0m\n");
                            }
                            printf("\n");
                            history_num--;
                            input(1);
                        }
                    }
                    else if (checkerboard[letter_num + 1][number] == 2)//如果當前輸入位置為空可下棋
                    {
                        if (player == 0)checkerboard[letter_num + 1][number] = 0;
                        else if (player == 1)checkerboard[letter_num + 1][number] = 1;

                        show();
                        judge(letter_num + 1, number);//判斷函式
                    }
                    else {//錯誤報錯
                        show();
                        if (language == 0) {
                            printf("\033[31mERROR3(Enter a duplicate location)\033[0m\n");
                            printf("\033[31mAGAIN\033[0m\n");
                        }
                        else if (language == 1)
                        {
                            printf("\033[31m錯誤3(輸入重複位置)\033[0m\n");
                            printf("\033[31m再次輸入\033[0m\n");
                        }
                        printf("\n");
                        history_num--;
                        input(1);
                    }
                }
                else {//錯誤報錯
                    show();
                    if (language == 0) {
                        printf("\033[31mERROR4(Number is too big)\033[0m\n");
                        printf("\033[31mAGAIN\033[0m\n");
                    }
                    else if (language == 1)
                    {
                        printf("\033[31m錯誤4(數字太大)\033[0m\n");
                        printf("\033[31m再次輸入\033[0m\n");
                    }
                    printf("\n");
                    history_num--;
                    input(1);
                }
            }
            else {//錯誤報錯
                show();
                if (language == 0) {
                    printf("\033[31mERROR5(Does not meet input requirements)\033[0m\n");
                    printf("\033[31mAGAIN\033[0m\n");
                }
                else if (language == 1)
                {
                    printf("\033[31m錯誤5(不符合輸入要求)\033[0m\n");
                    printf("\033[31m再次輸入\033[0m\n");
                }
                printf("\n");
                history_num--;
                input(1);
            }
        }
        else {//錯誤報錯
            show();
            if (language == 0) {
                printf("\033[31mERROR6(Enter wrong)\033[0m\n");
                printf("\033[31mAGAIN\033[0m\n");
            }
            else if (language == 1)
            {
                printf("\033[31m錯誤6(輸入錯誤)\033[0m\n");
                printf("\033[31m再次輸入\033[0m\n");
            }
            printf("\n");
            history_num--;
            input(1);
        }
    }
}

int judge_right(int x, int y, int num)//往右判斷
{
    while ((checkerboard[x + num][y] == player) && (x + num) <= 15)
    {
        flag_0++;
        num++;
        if (!((checkerboard[x + num][y] == player) && (x + num) <= 15))break;
    }
}

int judge_left(int x, int y, int num)//往左判斷
{
    while ((checkerboard[x - num][y] == player) && (x + num) > 0)
    {
        flag_0++;
        num++;
        if (!((checkerboard[x - num][y] == player) && (x + num) > 0))break;
    }
}

int judge_right_up(int x, int y, int num)//往右上判斷
{
    while ((checkerboard[x + num][y - num] == player) && (x + num) <= 15 && (y - num) > 0)
    {
        flag_45++;
        num++;
        if (!((checkerboard[x + num][y - num] == player) && (x + num) <= 15 && (y - num) > 0))break;
    }
}

int judge_left_down(int x, int y, int num)//往左下判斷
{
    while ((checkerboard[x - num][y + num] == player) && (x + num) > 0 && (y + num) <= 15)
    {
        flag_45++;
        num++;
        if (!((checkerboard[x - num][y + num] == player) && (x + num) > 0 && (y + num) <= 15))break;
    }
}

int judge_up(int x, int y, int num)//往上判斷
{
    while ((checkerboard[x][y - num] == player) && (y - num) > 0)
    {
        flag_90++;
        num++;
        if (!((checkerboard[x][y - num] == player) && (y - num) > 0))break;
    }
}

int judge_down(int x, int y, int num)//往下判斷
{
    while ((checkerboard[x][y + num] == player) && (y + num) <= 15)
    {
        flag_90++;
        num++;
        if (!((checkerboard[x][y + num] == player) && (y + num) <= 15))break;
    }
}

int judge_right_down(int x, int y, int num)//往右下判斷
{
    while ((checkerboard[x + num][y + num] == player) && (x + num) <= 15 && (y + num) <= 15)
    {
        flag_135++;
        num++;
        if (!((checkerboard[x + num][y + num] == player) && (x + num) <= 15 && (y + num) <= 15))break;
    }
}

int judge_left_up(int x, int y, int num)//往左判斷
{
    while ((checkerboard[x - num][y - num] == player) && (x - num) > 0 && (y - num) > 0)
    {
        flag_135++;
        num++;
        if (!((checkerboard[x - num][y - num] == player) && (x - num) > 0 && (y - num) > 0))break;
    }
}

int judge(int x, int y)//判斷轉接站
{
    flag_0 = 0;
    flag_45 = 0;
    flag_90 = 0;
    flag_135 = 0;

    judge_right(x, y, 1);
    judge_left(x, y, 1);
    judge_right_up(x, y, 1);
    judge_left_down(x, y, 1);
    judge_up(x, y, 1);
    judge_down(x, y, 1);
    judge_right_down(x, y, 1);
    judge_left_up(x, y, 1);

    if (flag_0 >= 4)win();
    if (flag_45 >= 4)win();
    if (flag_90 >= 4)win();
    if (flag_135 >= 4)win();
    //printf("\n%d %d %d %d\n", flag_0, flag_45, flag_90, flag_135);//檢查旗標條件
    //printf("%d %d\n", x, y);//檢查位置

    if (player == 0)player = 1;
    else if (player == 1)player = 0;

    input(1);
}
int win(void)
{
    if (language == 0) {
        if (player == 0)printf("BLACK");
        else if (player == 1)printf("WHITE");
        printf(" WIN!!!\n");
    }
    else if (language == 1) {
        if (player == 0)printf("黑棋");
        else if (player == 1)printf("白棋");
        printf(" 獲勝!!!\n");
    }
    Win = 1;
    system("pause");
    return 0;
}
void setConsoleFont(int width, int height, int fontWeight) {//設定字體

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // 取得標準輸出裝置的終端處理碼
    CONSOLE_FONT_INFOEX fontInfo;                       // 定義 CONSOLE_FONT_INFOEX 結構體，用於儲存終端字型資訊
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);      // 設定結構體的大小
    GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);// 取得當前終端字型資訊

    // 設定字型的寬度和高度
    fontInfo.dwFontSize.X = width;
    fontInfo.dwFontSize.Y = height;

    fontInfo.FontWeight = fontWeight;                   // 設定字型的粗細
    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);// 將新的字型資訊應用到終端
}


int main(void)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);// 取得標準輸出裝置的終端處理碼


    CONSOLE_SCREEN_BUFFER_INFO csbi;// 獲取終端窗口信息
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    SetConsoleDisplayMode(hConsole, CONSOLE_FULLSCREEN_MODE, 0);// 切换到全螢幕模式

    system("cls");//清除終端


    setConsoleFont(15, 30, FW_BOLD); // 設置粗字體大小 15x30

    printf("<Gobang>\n\n\n");
    printf("Play chess on a 15*15 chessboard.\n\n");
    printf("The black chesses are placed first, and they are placed at empty spots on the board in turn.\n\n");
    printf("The first one to connect five or more chess pieces in any horizontal,\n vertical and diagonal direction wins.\n\n");
    printf("Enter o16 or O16 to regret the move\n");

    printf("<五子棋>\n\n\n");
    printf("在15*15的棋盤上進行對弈。\n\n");
    printf("黑子先放，輪流下在棋盤空點處。\n\n");
    printf("先把五枚或以上棋相連成任何橫縱斜方向為勝。\n");
    printf("輸入o16或O16可悔棋\n");
    system("pause");

    for (short int y = 1; y <= checkerboard_y; y++) {// 初始設置全為空
        for (short int x = 1; x <= checkerboard_x; x++) {
            checkerboard[x][y] = 2;
        }
    }

    setConsoleFont(10, 20, FW_NORMAL); // 恢復字體與大小 10x20
    show();
    input(0);//初次進入輸入系統
}
