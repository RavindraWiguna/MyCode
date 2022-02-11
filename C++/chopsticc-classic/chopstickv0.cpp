#include <cstdio>
#include <cstdlib>
#include <vector>
typedef unsigned char uint8_t;
using std::vector;

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

void get_comp_available_move(vector<int>&vec, uint8_t game_pos[4]){
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

void computer_turn(uint8_t game_pos[4]){
    printf("Comp's turn\n");
    //check all computer available move
    vector<int>availableMove;
    get_comp_available_move(availableMove, game_pos);

    //intelegence way to decide best move TBI (to be implemented)
    int idMove = rand()%availableMove.size();
    switch (availableMove[idMove])
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
        printf("Seems like an error in computer turn in idMove\n");
        printf("We got value: %d, but expected in range 3 - 9\n", idMove);
        printf("checker: available move 0 over %lu is = %d\n", availableMove.size(), availableMove[0]);
        break;
    }
    
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

void check_for_win(uint8_t game_pos[4], int turn, bool* game_over){
    //harusnya kalo giliran sekarang yang perlu di check adalah lawan, karena kalau masih bisa jalan, pasti masih hidup
    if(game_pos[2] != 0 || game_pos[3] != 0){
        //nope no one win, unless human surrender
        if(game_pos[0]==0 && game_pos[1]==0){
            printf("Game ended by surrender\nGGWEZ\n");
            *game_over = true;
        }
        return;
    }
    *game_over=true;
    if(turn&1){
        //computer win
        printf("Hahaha, computer win, u idot\nGGWEZ\n");
    }else{
        printf("God dang..., how coud you win against computer?\nGGWP\n");
    }
}

void intro(){
    printf("HEY INTRO GOES HERE CERITANYA AZEK\n");
}

int main(){
    uint8_t game_pos[4] = {1, 1, 1, 1};//A, B, C, D (A, B) are curr turn hand players, (C, D) are next turn hand players
    
    bool game_over = false;
    int turns = 0;
    char ans;
    intro();
    printf("do you want me to go first?\n");
    ans = get_yes_no();
    printf("OH u said: %c\n", ans);
    turns += ans =='y' || ans=='Y';
    while(!game_over){
        // system("clear");
        sort_game_pos(game_pos);
        print_game_pos(game_pos, turns);
        if(turns&1){
            computer_turn(game_pos);
        }else{
            human_turn(game_pos);
        }
        check_for_win(game_pos, turns, &game_over);
        turns++;
    }
    return 0;
}