#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <ctime>
#define NO_THRES 7 //basically no threshold (max == 3)
using namespace std;

//numbers of possibility
vector<string> numbers;

void generate(){
    int i, j, k;
    string str = "";
    for(i = 0;i<10;i++){
        str += i + '0';
        for(j =0;j<10;j++){
            if(j == i){
                continue;
            }
            str += j +'0';
            for(k = 0;k<10;k++){
                if(k == j || k == i){
                    continue;
                }
                str += k +'0';
                numbers.push_back(str);
                str.erase(2, 1);
            }
            str.erase(1, 1);
        }
        str.erase(0, 1);
    }
}

//-------------------------------helper function--------------------------------------------//
void delete_index(int id){
    //delete element at index id
    numbers.erase( remove(numbers.begin(), numbers.end(), numbers[id]), numbers.end());
}

/*
guess ==> the guessed 3 digit password/number
id ==> index of possible guess/numbers/digits combination in the numbers vector
threshold ==> for early pruning, ex: need to delete > 0 position, if 1 position correct, stop, return 1
*/
int check_correct_pos(string& guess, int id, int threshold){
    int position_correct = 0;
    for(int i = 0;i<3 && position_correct < threshold;i++){
        if(guess[i] != numbers[id][i]){
            continue;
        }
        position_correct++;
    }
    if(position_correct < threshold)return position_correct;
    return threshold;
}

int check_correct_digit(string& guess, int id, int threshold){
    int digit_correct = 0;
    for(int i = 0;i<3 && digit_correct < threshold;i++){
        if(guess[i] == numbers[id][0]){
            digit_correct++;
            continue;
        }
        if(guess[i] == numbers[id][1]){
            digit_correct++;
            continue;
        }
        if(guess[i] == numbers[id][2]){
            digit_correct++;
            continue;
        }
        //aman
    }
    if(digit_correct < threshold)return digit_correct;
    return threshold;
}

int get_case(string& guess, int id){
    int digit_correct = 0;
    int position_correct = 0;
	for(int i = 0;i<3;i++){
		for(int j = 0;j<3;j++){
			if(guess[j] == numbers[id][i]){
				digit_correct++;
				if(i == j){
					position_correct++;
				}
			}
		}
	}
    return 3*digit_correct + position_correct;
}

//got it from 3blue1brown
double count_information(double p){
    if(p == 0)return 0;
    return p*(-log2(p));
}

bool compare_info(pair<double, int> a, pair<double, int> b){
    if(a.first == b.first){
        return a.second > b.second;
    }
    return a.first > b.first;

}

int calculate_best_guess(){
    vector<pair<double, int>> Information;
    pair<double, int> temp;
    double case_counter[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int valid_case[9] = {0, 3, 4, 6, 7, 8, 9, 10, 12};
    int num_size = numbers.size();
    for(int i = 0;i<num_size;i++){
        double sum_info = 0;
        temp.second = i;
        //get all case distribution 
        for(int j = 0;j<num_size;j++){
            case_counter[get_case(numbers[i], j)]+=1.0;
        }
        //count information of each valid case
        for(int k = 0;k<9;k++){
            // cout << "case: " << valid_case[k] << " has " << case_counter[valid_case[k]] << " times\n"; 
            sum_info += count_information(case_counter[valid_case[k]]/(num_size*1.0));
        }
        temp.first = sum_info;
        Information.push_back(temp);
    }
    //sort dulu baru nanti ganti jadi max
    sort(Information.begin(), Information.end(), compare_info);
    //print information
    int total = 5 < num_size ? 5: num_size;
    cout << "Computer best: " << total << "/" << num_size << " guesses\n";
    for(int i = 0;i<total;i++){
        cout << "[" <<Information[i].second<< "]:" << numbers[Information[i].second] << "= " <<Information[i].first<< "\n"; ;
    }
    int idMax = Information[0].second;
    Information.clear();
    return idMax;
}

//--------------------------------eliminae function (to delete based on certain condition) --------------------------------------------//

//destroy guesses(numbers) that has > 0 position correct
void destroy_non_0_position(string& guess){
	int i,j;
	//destroy if posisi the same ==> destroy > 0 position correct
	for(j = 0;j<numbers.size();j++){
		for(i = 0;i<3;i++){
			if(guess[i] == numbers[j][i]){
                delete_index(j);
				j--;
				break;
			}
		}
	}
}

//not at least 1 position ==> destroy 0 position correct
void destroy_0_position(string& guess){
	//destroy anything without atleast 1 position correct ==> destroy 0 position correct
	for(int j = 0;j<numbers.size();j++){
        //kalau posisi bener >= 1, skip
        if( check_correct_pos(guess, j, 1) == 1){
            continue;
        }
        delete_index(j);
        j--;
	}
}

//destroy numbers that has != 1 position correct
void destroy_non_1_position(string& guess){
    for(int i = 0;i<numbers.size();i++){
        if( check_correct_pos(guess, i, NO_THRES) == 1){
            continue;
        }
        //if not 1 pos correct delete it
        delete_index(i);
        i--;
    }
}

//2 position (auto 2 angka) ==> destroy != 2 position correct
void destroy_non_2_position(string& guess){
	for(int i = 0;i<numbers.size();i++){
        //kalau posisi bener == 2 skip
        if( check_correct_pos(guess, i, NO_THRES) == 2){
            continue;
        }
        //sisanya hapus
        delete_index(i);
        i--;
    }
}

//untuk 0 angka (auto 0 posisi) ==> destroy yang ada at least 1 angka bener ==> destroy >0 angka
void destroy_all(string& guess){
    for(int i = 0;i<numbers.size();i++){
        //kalau 0 angka bener, keep
        if( check_correct_digit(guess, i, 1) == 0){
            continue;
        }
        //sisanya delete
        delete_index(i);
        i--;
    }
}

//for not at least 1 angka ==> destroy 0 digit guess
void destroy_0_digit(string& guess){
    for(int i = 0;i<numbers.size();i++){
        //kalau digit bener >= 1 keep
        if( check_correct_digit(guess, i, 1) == 1){
            continue;
        }
        //sisa delete
        delete_index(i);
        i--;
    }
}

//keep 1 digit
void destroy_non_1_digit(string& guess){
    for(int i = 0;i<numbers.size();i++){
        //kalau digit bener == 1 skip
        if( check_correct_digit(guess, i, NO_THRES) == 1){
            continue;
        }
        //sisa hapus syr
        delete_index(i);
        i--;
    }
}

//keep 2 digit
void destroy_non_2_digit(string& guess){
    for(int i = 0;i<numbers.size();i++){
        if( check_correct_digit(guess, i, NO_THRES) == 2){
            continue;
        }
        delete_index(i);
        i--;
    }
}

//keep 3 angka
void destroy_non_3_digit(string& guess){
    for(int i = 0;i<numbers.size();i++){
        if( check_correct_digit(guess, i, NO_THRES) == 3){
            continue;
        }
        delete_index(i);
        i--;
    }
}
//---------------------------------------------------END OF ELIMINATE FUNCTION------------------------
//--------------------------------------------------COMBINATION OF DESTROY BASED ON GAME OUTPUT--------//
//0 angka 0 posisi
void destroy_0_0(string& guess){
	destroy_all(guess);
}

//untuk 1 angka 0 posisi
void destroy_1_0(string& guess){
	destroy_non_1_digit(guess);
	destroy_non_0_position(guess);
}

//1 angka 1 posisi
void destroy_1_1(string& guess){
	destroy_non_1_digit(guess);
	destroy_non_1_position(guess);
}

//2 angka 0 posisi
void destroy_2_0(string& guess){
	destroy_non_2_digit(guess);
	destroy_non_0_position(guess);
}

//2 angka 1  posisi
void destroy_2_1(string& guess){
	destroy_non_2_digit(guess);
	destroy_non_1_position(guess);
}

//2 angka 2 posisi
void destroy_2_2(string& guess){
	destroy_non_2_position(guess);
}

//3 angka 0 posisi
void destroy_3_0(string& guess){
	destroy_non_3_digit(guess);
	destroy_non_0_position(guess);
}

//3 angka 1 posisi
void destroy_3_1(string& guess){
	destroy_non_3_digit(guess);
	destroy_non_1_position(guess);
}
//-------------------------------------------------------------------SWITCH FOR ELIMINATION----------------//
void eliminate(int cases, int index_guess){
	string guessed = numbers[index_guess];
    delete_index(index_guess);
	switch(cases){
		case 0:
			//all wrong
			destroy_0_0(guessed);
			break;
		
		case 3:
			// 1 angka benar 0 posisi
			destroy_1_0(guessed);
			break;
			
		case 4:
			//1 angka benar 1 posisi
			destroy_1_1(guessed);
			break;
			
		case 6:
			//2 angka 0 pos
			destroy_2_0(guessed);
			break;
			
		case 7:
			//2 angka 1 pos
			destroy_2_1(guessed);
			break;
			
		case 8:
			//2 angka 2 poss
			destroy_2_2(guessed);
			break;
			
		case 9:
			//3 angka 0 poss
			destroy_3_0(guessed);
			break;
			
		case 10:
			//3 angka 1 pos
			destroy_3_1(guessed);
			break;
	}
}

//bot generate numberss
void generate_secretNum(string& str){
	char tmp1 = rand()%10 + '0';
	str += tmp1;
	char tmp2 = rand()%10 + '0';
	while(tmp2 == tmp1){
		tmp2 = rand()%10 + '0';
	}
	str += tmp2;
	char tmp3 = rand()%10 + '0';
	while(tmp3 == tmp1 || tmp3 == tmp2){
		tmp3 = rand()%10 + '0';
	}
	str += tmp3;
}

//bot chekin
bool check(string& needcheck, string& guessin){
	int i, j;
	int angka = 0, pos = 0;
	for(i = 0;i<3;i++){
		for(j = 0;j<3;j++){
			if(guessin[j] == needcheck[i]){
				angka++;
				if(i == j){
					pos++;
				}
			}
		}
	}
	cout << "[" << angka << ", " << pos  << "]"<< endl;
	return pos == 3;
}

void computer_algorithm(string& comp_secret, bool& game_over){
    int angka, pos;
    int cases = 0;
    if(numbers.empty()){
        cout << "It seemed that you made an input mistake before\nThere is no valid guess left\nShame\n";
        return;
    }
    
    int index_guess = calculate_best_guess();
    cout << "Is this your number? "<< numbers[index_guess] << endl;
    cin >> angka >> pos;
    cases = 3*angka + pos;
    if(cases != 12){
        eliminate(cases, index_guess);
    }else{
        cout << "so your number is : "<< numbers[index_guess] << endl;
        cout << "and my number is " << comp_secret << endl;
        game_over = true;
    }
}

void intro(){
	cout << "Hey welcome to my guess the number game!\n";
	cout << "This time, comp is able to deducc your secret number :D\n";
	cout << "\n~How to play~:\nPick 3 differents number between 0-9 (all must different) ex. 293\n";
	cout << "And the computer will try to guess it! and u too are free to guess comp secret!\n";
	cout << "\n~How to guess~:\nType 3 numbers (with no spaces) ex. 496\n";
	cout << "\n~Comp Response~:\n";
	cout << "[a, b] means there are a number/s got correct and b position/s got correct\n";
	cout << "Your response should be the same, a number/s and b position/s\n";
	cout << "ex 2 1 -> the computer will recognize this as 2 numbers correct and 1 position correct\n";
	cout << "\n~Now let's play!~\n\n";
}


int main(){
	intro();
    srand((unsigned) time(0));
	int turns = 0;
	bool game_over = false;
	string comp_secret = "";
	string human_guess = "";
	int i;
    //prepare numbers
    numbers.clear();
    generate();
	generate_secretNum(comp_secret);

	while(!game_over){
        printf("===MOVE [%d]===\n", turns/2);
		//comp turn
		if(turns % 2 == 1){	
		    //begin guess
            computer_algorithm(comp_secret, game_over);
		}
		
		//human turn
		if(turns % 2 == 0){
			cout << "Your guess?\n";
			cin >> human_guess;
			
			//check
			if(check(comp_secret, human_guess)){
                game_over = true;
				cout << "damn u are right, i lost\n";
				cout << "lemme guess one more time\n";
				//bot guessin again for fair play
                computer_algorithm(comp_secret, game_over);
				cout <<"this is my left over guess" << endl;
				for(i=0;i<numbers.size();i++){
					cout << numbers[i] <<endl;
				}
			}
		}
		turns++;
	}
    return 0;
}