#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <map>
#define NO_THRES 7 //basically no threshold (max == 3)

//number of possibility
std::vector<std::string> comp_pool;//computer
std::vector<std::string> human_pool;//human
//map vector code
std::map<std::string, int> enc_vector;
std::map<int, std::string> decr_vector;
void generate(){
    int i, j, k;
    std::string str = "000";
    for(i = 0;i<10;i++){
        str[0] = i + '0';
        for(j =0;j<10;j++){
            if(j == i){
                continue;
            }
            str[1] = j +'0';
            for(k = 0;k<10;k++){
                if(k == j || k == i){
                    continue;
                }
                str[2] = k +'0';
                comp_pool.push_back(str);
                human_pool.push_back(str);
            }
        }
    }
}

//-------------------------------helper function--------------------------------------------//
void delete_index(int id, int vector_code){
    //delete element at index id
    if(decr_vector[vector_code] == "computer"){
        comp_pool.erase( comp_pool.begin() + id);
    }else{
        human_pool.erase( human_pool.begin() + id);
    }
}
/*
guess ==> the guessed 3 digit password/number
id ==> index of possible guess/comp_pool/digits combination in the comp_pool vector
threshold ==> for early pruning, ex: need to delete > 0 position, if 1 position correct, stop, return 1
*/
int get_case(std::string& guess, std::string& truth, int thres_d, int thres_p, bool verbose){
    int digit_correct = 0;
    int position_correct = 0;
    bool isRun = true;
	for(int i = 0;i<3 && isRun;i++){
		for(int j = 0;j<3 && isRun;j++){
			if(guess[j] == truth[i]){
				digit_correct++;
				if(i == j){
					position_correct++;
				}
			}
            isRun = digit_correct < thres_d && position_correct < thres_p;
		}
	}
    // cout << "scanning: " << guess << " ~ " << truth << " completed\n";
    if(!isRun)return 3*thres_d + thres_p;
    if(verbose)std::cout<<"["<<  digit_correct << "," << position_correct << "]\n";
    return 3*digit_correct + position_correct; 
}

//di pisah because reference is dangerous (cape debug, ternyata reference ter delete, and so on)
void computer_filter(std::string guess, int thres_d, int thres_p, int& safe_case){
    for(int i = 0;i<comp_pool.size();i++){
        if( get_case(guess, comp_pool[i], thres_d, thres_p, false) == safe_case){
            continue;
        }
        delete_index(i, enc_vector["computer"]);
        i--;
    }
}

void human_filter(std::string guess, int thres_d, int thres_p, int& safe_case){
    for(int i = 0;i<human_pool.size();i++){
        if( get_case(guess, human_pool[i], thres_d, thres_p, false) == safe_case){
            continue;
        }
        delete_index(i, enc_vector["human"]);
        i--;
    }  
}

void filter_util(std::string guess, int thres_d, int thres_p, int safe_case, int vector_code){
    if(decr_vector[vector_code]=="computer"){
        computer_filter(guess, thres_d, thres_p, safe_case);
    }else{
        human_filter(guess, thres_d, thres_p, safe_case);
    }
}

//--------------------------------------------------COMBINATION OF DESTROY BASED ON GAME OUTPUT--------//
//0 angka 0 posisi
void keep_0_0(std::string& guess, int vector_code){
    int threshold_pos = 1;
    int threshold_dig = 1;
    int keeped_case = 0;
    filter_util(guess, threshold_dig, threshold_pos, keeped_case, vector_code);
}

//untuk 1 angka 0 posisi
void keep_1_0(std::string& guess, int vector_code){
    int threshold_pos = 1;
    int threshold_dig = 2;
    int keeped_case = 3;
    filter_util(guess, threshold_dig, threshold_pos, keeped_case, vector_code);
}

//1 angka 1 posisi
void keep_1_1(std::string& guess, int vector_code){
    int threshold_pos = 2;
    int threshold_dig = 2;
    int keeped_case = 4;
    filter_util(guess, threshold_dig, threshold_pos, keeped_case, vector_code);
}

//2 angka 0 posisi
void keep_2_0(std::string& guess, int vector_code){
    int threshold_pos = 1;
    int threshold_dig = 3;
    int keeped_case = 6;
    filter_util(guess, threshold_dig, threshold_pos, keeped_case, vector_code);
}

//2 angka 1  posisi
void keep_2_1(std::string& guess, int vector_code){
    int threshold_pos = 2;
    int threshold_dig = 3;
    int keeped_case = 7;
    filter_util(guess, threshold_dig, threshold_pos, keeped_case, vector_code);
}

//2 angka 2 posisi
void keep_2_2(std::string& guess, int vector_code){
    int threshold_pos = 3;
    int threshold_dig = 3;
    int keeped_case = 8;
    filter_util(guess, threshold_dig, threshold_pos, keeped_case, vector_code);
}

//3 angka 0 posisi
void keep_3_0(std::string& guess, int vector_code){
    int threshold_pos = 1;
    int threshold_dig = NO_THRES;
    int keeped_case = 9;
    filter_util(guess, threshold_dig, threshold_pos, keeped_case, vector_code);
}

//3 angka 1 posisi
void keep_3_1(std::string& guess, int vector_code){
    int threshold_pos = 2;
    int threshold_dig = NO_THRES;
    int keeped_case = 10;
    filter_util(guess, threshold_dig, threshold_pos, keeped_case, vector_code);
}
//-------------------------------------------------------------------SWITCH FOR ELIMINATION----------------//
void eliminate(int cases, std::string guessed, int vector_code){
	switch(cases){
		case 0:
			//all wrong
			keep_0_0(guessed, vector_code);
			break;
		
		case 3:
			// 1 angka benar 0 posisi
			keep_1_0(guessed, vector_code);
			break;
			
		case 4:
			//1 angka benar 1 posisi
			keep_1_1(guessed, vector_code);
			break;
			
		case 6:
			//2 angka 0 pos
			keep_2_0(guessed, vector_code);
			break;
			
		case 7:
			//2 angka 1 pos
			keep_2_1(guessed, vector_code);
			break;
			
		case 8:
			//2 angka 2 poss
			keep_2_2(guessed, vector_code);
			break;
			
		case 9:
			//3 angka 0 poss
			keep_3_0(guessed, vector_code);
			break;
			
		case 10:
			//3 angka 1 pos
			keep_3_1(guessed, vector_code);
			break;
	}
}

void generate_secret_num(std::string& str){
    int id = rand()%720;
    str = human_pool[id];
}
/*
comp_secret biar bisa nunjukin kalo gamenya selesai
*/
void computer_algorithm(std::string comp_secret, bool& game_over){
    int angka, pos;
    int cases = 0;
    if(comp_pool.empty()){
        std::cout << "It seemed that you made an input mistake before\nThere is no valid guess left\nShame\n";
        game_over=true;
        return;
    }
    int index_guess = rand()%comp_pool.size();
    std::cout << "Is this your number? "<< comp_pool[index_guess] << "\n";
    std::cin >> angka >> pos;
    cases = 3*angka + pos;
    if(cases != 12){
        eliminate(cases, comp_pool[index_guess], enc_vector["computer"]);
    }else{
        std::cout << "so your number is : "<< comp_pool[index_guess] << "\n";
        std::cout << "and my number is " << comp_secret << "\n";
        std::cout << "Chance i got this right is 1 out of " << comp_pool.size() << "\n";
        if(!game_over)std::cout << "You are " << human_pool.size() << " guesses away from beating me!\n"; 
        game_over = true;
    }
}

void human_algorithm(std::string human_guess, std::string comp_secret, bool& game_over){
    std::cout << "Your guess?\n";
    std::cin >> human_guess;
    int result_case = get_case(human_guess, comp_secret, NO_THRES, NO_THRES, true);
    if(result_case == 12){
        game_over = true;
        std::cout << "damn u are right, i lost\n";
        std::cout << "lemme guess one more time\n";
        //bot guessin again for fair play
        computer_algorithm(comp_secret, game_over);
        std::cout <<"this is my left over guess" <<"\n";
        for(int i=0;i<comp_pool.size();i++){
            std::cout << comp_pool[i] <<"\n";
        }
    }else{
        eliminate(result_case, human_guess, enc_vector["human"]);
    }
}

float calc_prob_comp_win(){
    float comp_poss_guess = comp_pool.size()*1.0;
    float user_poss_guess = human_pool.size()*1.0;
    return 100.0 - (comp_poss_guess)*100.0/(comp_poss_guess + user_poss_guess);
}

void intro(){
	std::cout << "Hey welcome to my guess the number game!\n";
	std::cout << "This time, comp is able to deducc your secret number :D\n";
	std::cout << "\n~How to play~:\nPick 3 differents number between 0-9 (all must different) ex. 293\n";
	std::cout << "And the computer will try to guess it! and u too are free to guess comp secret!\n";
	std::cout << "\n~How to guess~:\nType 3 comp_pool (with no spaces) ex. 496\n";
	std::cout << "\n~Comp Response~:\n";
	std::cout << "[a, b] means there are a number/s got correct and b position/s got correct\n";
	std::cout << "Your response should be the same, a number/s and b position/s\n";
	std::cout << "ex 2 1 -> the computer will recognize this as 2 comp_pool correct and 1 position correct\n";
	std::cout << "\n~Now let's play!~\n\n";
}

int main(){
    //Game preparation
	srand((unsigned) time(0));
    enc_vector["computer"] = 0; enc_vector["human"] = 1;
    decr_vector[0] = "computer"; decr_vector[1] = "human";
    intro();
	int turns = 1;
	bool game_over = false;
	std::string comp_secret = "";
	std::string human_guess = "";
	int i, result_case;
    //prepare vector of posibility
    generate();
	generate_secret_num(comp_secret);
    std::cout << "PICK YOUR SECRET NUMBERS | type any char if you have made up your mind. Remember to remember your secret number\n";
    system("read -p press_any_key local_temporal");
    //Game loop
	while(!game_over){
        std::cout<<"===MOVE [" << (turns+1)/2 << "]====";
        std::cout<<"Probability Computer Wins: " << calc_prob_comp_win() << "%\n";
		//comp turn
		if(turns % 2 == 1){
            std::cout <<"comp's turn\n";
            computer_algorithm(comp_secret, game_over);
		}
        //human turn
        else{
            std::cout <<"human's turn\n";
            human_algorithm(human_guess, comp_secret, game_over);
        }
        std::cout << "end of turn\n";
		turns++;
	}

    std::cout << "type any character to end program\n";
    system("read -p press_any_key local_temporal");
    return 0;
}
