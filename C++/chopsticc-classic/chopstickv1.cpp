#include <cstdio>
#include <vector>
#include <map>
#include <algorithm>
typedef unsigned char uint8_t;
#define NEG_INF -2147483648;
#define INF 2147483647;
using std::vector;
using std::map;
using std::max;
using std::min;

//Global var
map<char, int>WIN_CONDITION = {{'C', 1000}, {'H', -1000}};
//FUNCTIONS
bool is_valid_confirm(char *c);
bool is_valid_hand(char input, uint8_t hand_valid);
bool is_valid_target(char input, uint8_t target_valid);
void lower_case_char(char* c);
void swap(uint8_t *a, uint8_t *b);
char get_yes_no();
void menu();
void intro();
void show_attack_menu(uint8_t game_pos[4], uint8_t* hand_valid, uint8_t* target_valid);
void attack_util(uint8_t game_pos[4], char attacker, char target);
bool attack(uint8_t game_pos[4]);
void divide_util(uint8_t game_pos[4], bool four_even);
bool can_divide(uint8_t game_pos[4]);
bool divide(uint8_t game_pos[4]);
void transfer_util(uint8_t game_pos[4]);
bool can_transfer(uint8_t game_pos[4]);
bool transfer(uint8_t game_pos[4]);
bool pencet_tombol_surrender(uint8_t game_pos[4]);
void get_available_move(vector<int>&vec, uint8_t game_pos[4]);
void do_move(int moveId, uint8_t game_pos[4]);
void computer_turn(uint8_t game_pos[4]);
void human_turn(uint8_t game_pos[4]);
void print_game_pos(uint8_t game_pos[4], int turns);
void sort_game_pos(uint8_t game_pos[4]);
char check_for_win(uint8_t game_pos[4], int turn);
int evalute_game_pos(uint8_t game_pos[4], int turn);
void reset_game_pos(uint8_t game_pos[4], uint8_t reset_pos[4]);
int minimax(uint8_t game_pos[4], int depth, bool maximizingPlayer, int turns);

int main(){
    uint8_t game_pos[4] = {1, 1, 1, 1};//A, B, C, D (A, B) are curr turn hand players, (C, D) are next turn hand players
    
    bool game_over = false;
    int turns = 0;
    char ans, winner;
    intro();
    printf("do you want me to go first?\n");
    ans = get_yes_no();
    printf("OH u said: %c\n", ans);
    turns += ans =='y' || ans=='Y';
    while(!game_over){
        system("clear");
        sort_game_pos(game_pos);
        print_game_pos(game_pos, turns);
        if(turns&1){
            computer_turn(game_pos);
        }else{
            human_turn(game_pos);
        }
        winner = check_for_win(game_pos, turns);
        game_over = winner != 'N';
        turns++;
    }
    if(winner=='C'){
        printf("COMPUTER WON\n");
        return 0;
    }
    printf("HUMAN WON\n");
    return 0;
}

//------HELPER FUNCTION-------------------------------//
bool is_valid_confirm(char *c){
    return *c == 'Y' || *c == 'y' || *c == 'n' || *c=='N';
}

bool is_valid_hand(char input, uint8_t hand_valid){
    // printf("valid hand is: %d\n", hand_valid);
    return (input=='B' || input=='b') || ((input=='a' || input=='A') && hand_valid==2);
}

bool is_valid_target(char input, uint8_t target_valid){
    return (input=='D' || input=='d') || ((input=='c' || input=='C') && target_valid==2);
}

void lower_case_char(char* c){
    if(*c >= 'a' && *c <= 'z'){
        return;
    }
    *c -= 'A';
    *c += 'a';
}

void swap(uint8_t *a, uint8_t *b){
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}

char get_yes_no(){
    bool is_done = false;
    char ans;
    while(!is_done){
        printf("[y/n]: ");
        scanf("\n%c", &ans);
        is_done = is_valid_confirm(&ans);
    }
    lower_case_char(&ans);
    return ans;
}

void menu(){
    printf("Which move are you going to choose?\n");
    printf("1. Attack!\n");
    printf("2. Divide!\n");
    printf("3. Transfer!\n");
    printf("4. Surrender :(\n");
}

void intro(){
    printf("HEY INTRO GOES HERE CERITANYA AZEK\n");
}
//----------------------------------------------------//
//----------MOVE FUNCTION-----------------------------//
//--------------------------------ATK MOVE-----------------------------------------------//
void show_attack_menu(uint8_t game_pos[4], uint8_t* hand_valid, uint8_t* target_valid){
    printf("Which hand do you want to use as attack, and which hand do you want to attack?\n");
    char curhand = 'A'-1;
    int i;
    *hand_valid = 0;
    *target_valid = 0;
    
    for(i = 0;i<2;i++){
        curhand++;
        if(game_pos[i]==0){
            continue;
        }
        printf("%c: %d\n", curhand, game_pos[i]);
        (*hand_valid)++;
    }
    
    printf("-----TARGET-----\n");
    curhand='C'-1;
    for(i = 2;i<4;i++){
        curhand++;
        if(game_pos[i]==0){
            continue;
        }
        printf("%c: %d\n", curhand, game_pos[i]);
        (*target_valid)++;
    }
    // printf("hand valid sir: %d\n", *hand_valid);
    // printf("target valid sir: %d\n", *target_valid);
}

void attack_util(uint8_t game_pos[4], char attacker, char target){
    int att_id = attacker - 'a';//lower sir
    int tgt_id = target - 'a';

    (game_pos[tgt_id]) += (game_pos[att_id]);
    (game_pos[tgt_id]) %=5;
}

bool attack(uint8_t game_pos[4]){
    uint8_t hand_valid, target_valid;
    show_attack_menu(game_pos, &hand_valid, &target_valid);
    char hand, target;
    uint8_t step = 0;
    while(step < 2){
        if(step==0){
            printf("Hand: ");
            scanf("\n%c", &hand);
            step += is_valid_hand(hand, hand_valid);
        }
        else{
            printf("Target: ");
            scanf("\n%c", &target);
            step += is_valid_target(target, target_valid);
        }
    }
    lower_case_char(&hand);
    lower_case_char(&target);
    //attack!
    attack_util(game_pos, hand, target);
    return true;
}

//----------------------------------------DIV MOVE------------------------------//
//for computer and human syr
void divide_util(uint8_t game_pos[4], bool four_even){
    //harusnya valid sir
    switch (game_pos[1])
    {
    case 2:
        game_pos[0] = 1;
        game_pos[1] = 1;
        break;
    case 3:
        game_pos[0] = 1;
        game_pos[1] = 2;
        break;
    case 4: 
        if(four_even){
            game_pos[0] = 2;
            game_pos[1] = 2;
        }else{
            game_pos[0] = 1;
            game_pos[1] = 3;
        }
        break;
    default:
        printf("This dividing should've not happen\n");
        printf("We got value: %d, but we expect in range 2 - 4\n", game_pos[1]);
        break;
    }
}

bool can_divide(uint8_t game_pos[4]){
    //harusnya udah tersort
    if(game_pos[0] != 0 || game_pos[1] < 2){
        return false;
    }
    return true;
}

bool divide(uint8_t game_pos[4]){
    if(!can_divide(game_pos)){
        printf("You can't divide in this situation!\n");
        return false;
    }
    //bagi sir
    bool four_even = false;
    if(game_pos[1]==4){
        printf("Do you want an even split?\n");
        char ans;
        ans = get_yes_no();
        four_even = ans =='y';
    }
    divide_util(game_pos,four_even);
    return true;
}

//-------------------------------------TRANSFER MOVE---------------------------//
void transfer_util(uint8_t game_pos[4]){
    int config = game_pos[0]*4 + game_pos[1];
    switch (config)
    {
    case 7://13-22
        game_pos[0] = 2;
        game_pos[1] = 2;
        break;
    case 8://14-23
        game_pos[0] = 2;
        game_pos[1] = 3;
        break;
    case 10://22-13
        game_pos[0] = 1;
        game_pos[1] = 3;
        break;
    case 11://23-14
        game_pos[0] = 1;
        game_pos[1] = 4;
        break;
    case 12://24-33
        game_pos[0] = 3;
        game_pos[1] = 3;
        break;
    case 15://33-24
        game_pos[0] = 2;
        game_pos[1] = 4;
        break;
    default:
        printf("Seems like an error in configuration of game pos, this case is not supposed possible\n");
        printf("Value that got here is %d\n", config);
        printf("With source: 4*%d + %d\n", game_pos[0], game_pos[1]);
        break;
    }
}

bool can_transfer(uint8_t game_pos[4]){
    int config = game_pos[0]*4 + game_pos[1];
    if(config < 7 || config > 15 || config==9 || config==13 || config==14){
        return false;
    }
    return true;
}

bool transfer(uint8_t game_pos[4]){
    if(!can_transfer(game_pos)){
        printf("You can't transfer in this situation\n");
        return false;
    }
    transfer_util(game_pos);
    return true;
}

//-------------------------------------MAN, JUST SURRENDER---------------------//
bool pencet_tombol_surrender(uint8_t game_pos[4]){
    printf("Very well, you understand how impossible it is to win against me\nWise Choice human\n");
    game_pos[0] = 0;
    game_pos[1] = 0;//losing con
    char ans;printf("type any char then enter\n");
    scanf("\n%c", &ans);
    return true;
}

//---------------------mini max bby--------------------------//
void get_available_move(vector<int>&vec, uint8_t game_pos[4]){
    //check attacking {(1,1),(1,2),(2,1),(2,2)} ==> [3, 4, 5, 6]
    for(int i = 0;i<2;i++){
        if(game_pos[i] == 0){
            continue;
        }//berarti tidak null and bisa nyerang
        for(int j = 2;j<4;j++){
            if(game_pos[j]==0){
                continue;
            }//berarti bisa diserang
            vec.push_back(2*(i+1) + j-1);
        }
    }
    //check dividing
    if(can_divide(game_pos)){
        //cek utk 4, normal divide is as even as it could
        vec.push_back(7);
        if(game_pos[1]==4) vec.push_back(8);//bisa uneven
    }
    //check transfer
    if(can_transfer(game_pos)){
        vec.push_back(9);//bisa transfer sir
    }
}

void do_move(int moveId, uint8_t game_pos[4]){
    switch (moveId)
    {
    case 3:
        attack_util(game_pos, 'a', 'c');
        break;
    case 4:
        attack_util(game_pos, 'a', 'd');
        break;
    case 5:
        attack_util(game_pos, 'b', 'c');
        break;
    case 6:
        attack_util(game_pos, 'b', 'd');
        break;
    case 7:
        divide_util(game_pos, true);
        break;
    case 8:
        divide_util(game_pos, false);
        break;
    case 9:
        transfer_util(game_pos);
        break;
    default:
        printf("Seems like an error in computer turn in moveId\n");
        printf("We got value: %d, but expected in range 3 - 9\n", moveId);
        break;
    }
}

void computer_turn(uint8_t game_pos[4]){
    //check all computer available move
    vector<int>availableMove;
    get_available_move(availableMove, game_pos);
    //intelegence way to decide best move TBI (to be implemented)
    // int idMove = rand()%availableMove.size();
    int savedMove = availableMove[0];
    int curMaxScore = NEG_INF;
    int depth = 3;//thinking 3 moves ahead
    uint8_t copy_of_game_pos[4] = {game_pos[0], game_pos[1], game_pos[2], game_pos[3]};
    for(auto move : availableMove){
        do_move(move, copy_of_game_pos);
        int move_score = minimax(copy_of_game_pos, depth, false, 0);
        if(move_score > curMaxScore){
            curMaxScore = move_score;
            savedMove = move;
        }
        //reset
        reset_game_pos(copy_of_game_pos, game_pos);
    }
    do_move(savedMove, game_pos);
    availableMove.clear();
}

void human_turn(uint8_t game_pos[4]){
    int cmd = -1;
    bool end_turn = false;
    printf("Human turn\n");
    while(!end_turn){
        menu();
        scanf("%d", &cmd);
        printf("You just pick this move: %d\n", cmd);
        switch (cmd)
        {
        case 1:
            end_turn = attack(game_pos);
            break;
        case 2:
            end_turn = divide(game_pos);
            break;
        case 3:
            end_turn = transfer(game_pos);
            break;
        case 4:
            end_turn = pencet_tombol_surrender(game_pos);
            break;
        default:
            printf("It seems you have inputted the wrong command\n");
            // end_turn=true;
            while (getchar() != '\n');
            break;
        }
    }
}

void print_game_pos(uint8_t game_pos[4], int turns){
    if(turns&1){
        printf("C:[%d, %d]\nH:[%d, %d]\n", game_pos[0], game_pos[1], game_pos[2], game_pos[3]);
        return;
    }
    printf("H:[%d, %d]\nC:[%d, %d]\n", game_pos[0], game_pos[1], game_pos[2], game_pos[3]);
    
}

void sort_game_pos(uint8_t game_pos[4]){
    //cur turn (not yet swap)
    if(game_pos[0] > game_pos[1]){
        swap(&game_pos[0], &game_pos[1]);
    }
    //next turn
    if(game_pos[2] > game_pos[3]){
        swap(&game_pos[2], &game_pos[3]);      
    }
    //swap cur turn and next turn
    swap(&game_pos[0], &game_pos[2]);
    swap(&game_pos[1], &game_pos[3]);
}

char check_for_win(uint8_t game_pos[4], int turn){
    //harusnya kalo giliran sekarang yang perlu di check adalah lawan, karena kalau masih bisa jalan, pasti masih hidup
    char winner = 'N';
    if(game_pos[2] != 0 || game_pos[3] != 0){
        //nope no one win, unless human surrender
        if(game_pos[0]==0 && game_pos[1]==0){
            //surrender human
            winner='C';
        }
        return winner;
    }
    if(turn&1){
        //computer win
        winner='C';
    }else{
        winner='H';
    }
    return winner;
}

int evalute_game_pos(uint8_t game_pos[4], char winner, int turns){
    if(winner != 'N'){
        return WIN_CONDITION[winner];
    }
    //so none of em is winner huh
    int score = game_pos[0]*5 + game_pos[1];//score 2 tangan is better
    score -= (game_pos[1]*5 + game_pos[2]);//score lawan
    if(turns&1){
        //current turn is computer's turn, so want to max
        return score;
    }
    //human turn, make it negative
    return -score;
}

void reset_game_pos(uint8_t game_pos[4], uint8_t reset_pos[4]){
    for(int i = 0;i<4;i++){
        game_pos[i] = reset_pos[i];
    }
}

int minimax(uint8_t game_pos[4], int depth, bool maximizingPlayer, int turns){
    /*
    comp is maximizing, turns&1 (a.k.a turns%2 == 1)
    human is minimizing, turns%2==0
    */
    //sepertinya perlu di swap-sort dahulu everytime masuk sini
    sort_game_pos(game_pos);

    char winner = check_for_win(game_pos, turns);
    if(depth==0 || winner != 'N'){
        return evalute_game_pos(game_pos, winner, turns);
    }
    if(maximizingPlayer){
        //computer's turn
        int value = NEG_INF;
        vector<int>available_compMove;
        get_available_move(available_compMove, game_pos);
        uint8_t temp_pos[4] = {game_pos[0], game_pos[1], game_pos[2], game_pos[3]};
        for(auto move: available_compMove){
            do_move(move, game_pos);
            value = max(value, minimax(game_pos, depth-1, false, turns+1));
            reset_game_pos(game_pos, temp_pos);
        }
        available_compMove.clear();
        return value;
    }
    //minimizing (human)
    int value = INF;
    vector<int>available_humanMove;
    get_available_move(available_humanMove, game_pos);
    uint8_t temp_pos[4] = {game_pos[0], game_pos[1], game_pos[2], game_pos[3]};
    for(auto move : available_humanMove){
        do_move(move, game_pos);
        value = min(value, minimax(game_pos, depth-1, true, turns+1));
        reset_game_pos(game_pos, temp_pos);
    }
    available_humanMove.clear();
    return value;
}