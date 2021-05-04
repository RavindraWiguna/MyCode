#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

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

//--------------------------------eliminae func --------------------------------------------\\

//for 0 position
void destroy_0_posisi(string& guess){
	int i,j;
	//destroy if posisi the same
	for(j = 0;j<numbers.size();j++){
		for(i = 0;i<3;i++){
			if(guess[i] == numbers[j][i]){
				numbers.erase( remove(numbers.begin(), numbers.end(), numbers[j]), numbers.end());
				j--;
				break;
			}
		}
	}
}

//not at least 1 position
void destroy_not_at_least_1_pos(string& guess){
	int j;
	//phase 1, destroy anything without atleast 1 position correct
	for(j = 0;j<numbers.size();j++){
		if( (numbers[j][0] != guess[0]) && (numbers[j][1] != guess[1]) && (numbers[j][2] != guess[2]) ){
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[j]), numbers.end());
			j--;
		}
	}
}


//for 1 position
void destroy_1_posisi(string& guess){
	//phase 1
	destroy_not_at_least_1_pos(guess);
	
	int j;
	//phase 2, destroy anything with 2 position correct or 3 in a way
	for(j = 0;j<numbers.size();j++){
		// 0 and 1
		if( (numbers[j][0] == guess[0]) && (numbers[j][1] == guess[1]) ){
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[j]), numbers.end());
			j--;
		}else if(  (numbers[j][0] == guess[0]) && (numbers[j][2] == guess[2]) ){
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[j]), numbers.end());
			j--;
		}else if(  (numbers[j][1] == guess[1]) && (numbers[j][2] == guess[2]) ){
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[j]), numbers.end());
			j--;
		}
	}
}

//2 position (auto 2 angka)
void destroy_2_posisi(string& guess){
	//phase 1
	destroy_not_at_least_1_pos(guess);
	
	//phase 2, destroy if solo or trio
	int i;
	for(i = 0;i<numbers.size();i++){
		if( (numbers[i][0] == guess[0]) && (numbers[i][1] != guess[1]) && (numbers[i][2] != guess[2]) ){
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[i]), numbers.end());
			i--;
		}else if( (numbers[i][1] == guess[1]) && (numbers[i][0] != guess[0]) && (numbers[i][2] != guess[2]) ){
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[i]), numbers.end());
			i--;
		}else if( (numbers[i][2] == guess[2]) && (numbers[i][1] != guess[1]) && (numbers[i][0] != guess[0]) ){
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[i]), numbers.end());
			i--;
		//trio case
		}else if( (numbers[i][0] == guess[0]) && (numbers[i][1] == guess[1]) && (numbers[i][2] == guess[2]) ){
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[i]), numbers.end());
			i--;
		}
	}
	
}

//untuk 0 angka (auto 0 posisi)
void destroy_all(string& guess){
	int i,j;
	for(j = 0;j<numbers.size();j++){
		for(i = 0;i<3;i++){
			if(numbers[j].find(guess[i]) != string::npos){
				numbers.erase( remove(numbers.begin(), numbers.end(), numbers[j]), numbers.end());
				j--;
				break;
			}
		}
	}
}

//for not at least 1 angka
void destroy_not_at_least_1_angka(string& guess){
	int j;
	//phase 1, destroy anything without guess 0 and 1 and 2
	for(j = 0;j<numbers.size();j++){
		if( (numbers[j].find(guess[0]) == string::npos) && (numbers[j].find(guess[1]) == string::npos) && (numbers[j].find(guess[2]) == string::npos) ){
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[j]), numbers.end());
			j--;
		}
	}
}


// 1 angka
void destroy_1_angka(string& guess){
	int j;
	
	//phase 1
	destroy_not_at_least_1_angka(guess);
	
	//phase 2, destroy any with 0 and 1 or 0 and 2 or 1 and 2 or 0 == 1== 2 ultimately trio
	for(j = 0;j<numbers.size();j++){
		// 0 and 1
		if( (numbers[j].find(guess[0]) != string::npos) && (numbers[j].find(guess[1]) != string::npos) ){
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[j]), numbers.end());
			j--;
		}else if( (numbers[j].find(guess[0]) != string::npos) && (numbers[j].find(guess[2]) != string::npos) ){
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[j]), numbers.end());
			j--;
		}else if( (numbers[j].find(guess[1]) != string::npos) && (numbers[j].find(guess[2]) != string::npos) ){
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[j]), numbers.end());
			j--;
		}
	}	
}

//2 angka but could be 0,1,2 pos
void destroy_2_angka(string& guess){
	//phase 1
	destroy_not_at_least_1_angka(guess);
	
	//phase 2, solo or trio
	int i;
	for(i = 0;i<numbers.size();i++){
		if( (numbers[i].find(guess[0]) != string::npos) && (numbers[i].find(guess[1]) == string::npos) && (numbers[i].find(guess[2]) == string::npos) ){
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[i]), numbers.end());
			i--;
		}else if( (numbers[i].find(guess[1]) != string::npos) && (numbers[i].find(guess[0]) == string::npos) && (numbers[i].find(guess[2]) == string::npos) ){
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[i]), numbers.end());
			i--;
		}else if ( (numbers[i].find(guess[2]) != string::npos) && (numbers[i].find(guess[0]) == string::npos) && (numbers[i].find(guess[1]) == string::npos) ){
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[i]), numbers.end());
			i--;
		}else if( (numbers[i].find(guess[1]) != string::npos) && (numbers[i].find(guess[0]) != string::npos) && (numbers[i].find(guess[2]) != string::npos) ){
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[i]), numbers.end());
			i--;
		}
	}
}

//3 angka with possibility
void destroy_3_angka(string& guess){
	//phase 1
	destroy_not_at_least_1_angka(guess);
	
	//destroy solo or duo
	int i;
	for(i = 0;i<numbers.size();i++){
		if( (numbers[i].find(guess[0]) != string::npos) && (numbers[i].find(guess[1]) != string::npos) && (numbers[i].find(guess[2]) != string::npos) ){
			continue;
		}else{
			numbers.erase( remove(numbers.begin(), numbers.end(), numbers[i]), numbers.end());
			i--;
		}
	}
}


//0 angka 0 posisi
void destroy_0_0(string& guess){
	destroy_all(guess);
}

//untuk 1 angka 0 posisi
void destroy_1_0(string& guess){
	destroy_1_angka(guess);
	destroy_0_posisi(guess);
}

//1 angka 1 posisi
void destroy_1_1(string& guess){
	destroy_1_angka(guess);
	destroy_1_posisi(guess);
}

//2 angka 0 posisi
void destroy_2_0(string& guess){
	destroy_2_angka(guess);
	destroy_0_posisi(guess);
}

//2 angka 1  posisi
void destroy_2_1(string& guess){
	destroy_2_angka(guess);
	destroy_1_posisi(guess);
}

//2 angka 2 posisi
void destroy_2_2(string& guess){
	destroy_2_posisi(guess);
}

//3 angka 0 posisi
void destroy_3_0(string& guess){
	destroy_3_angka(guess);
	destroy_0_posisi(guess);
}

//3 angka 1 posisi
void destroy_3_1(string& guess){
	destroy_3_angka(guess);
	destroy_1_posisi(guess);
}

void eliminate(int cases, int index_guess){
	string guessed = numbers[index_guess];
	numbers.erase( remove(numbers.begin(), numbers.end(), numbers[index_guess]), numbers.end());
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
	bool first_time_bot = true;
	bool first_time_human = true;
	bool game_over = false;
	int angka=0, pos=0;
	string comp_secret = "";
	string human_guess = "";
	int i;
	while(!game_over){
		//comp turn
		if(turns % 2 == 1){
			if(first_time_bot){
				numbers.clear();
				generate();
				first_time_bot = false;
			}
	
		    angka = 0; 
			pos = 0;
		    //begin guess
		    int cases = 0;
		    int index_guess = rand()%numbers.size();
	    	cout << "Is this your number? "<< numbers[index_guess] << endl;
			cin >> angka >> pos;
			cases = 3*angka + pos;
			if(cases != 12){
				eliminate(cases, index_guess);
			}else if(cases == 12){
				cout << "ur number is : "<< numbers[index_guess] << endl;
				cout << "and my number is " << comp_secret << endl;
				game_over = true;
				first_time_human = true;
				first_time_bot = true;
			}	
		}
		
		//human turn
		if(turns % 2 == 0){
			if(first_time_human){
				comp_secret = "";
				generate_secretNum(comp_secret);
				first_time_human = false;
			}
			//guess
			cout << "Your guess?\n";
			cin >> human_guess;
			
			//check
			if(check(comp_secret, human_guess)){
				cout << "damn u are right, i lost\n";
				cout << "lemme guess one more time\n";
				//bot guessin again for fair play
				int cases = 0;
				int index_guess = rand()%numbers.size();
		    	cout << "Is this your number? "<< numbers[index_guess] << endl;
				cin >> angka >> pos;
				cases = 3*angka + pos;
				if(cases != 12){
					eliminate(cases, index_guess);
				}else if(cases == 12){
					cout << "what a tough battle, ur number is : "<< numbers[index_guess] << endl;
					cout << "and my number is " << comp_secret << endl;
					game_over = true;
					first_time_human = true;
					first_time_bot = true;
				}
				
				game_over = true;
				first_time_human = true;
				first_time_bot = true;
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
