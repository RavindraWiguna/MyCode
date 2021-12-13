#include <cstdio>
#include <cmath>
#include <algorithm>
#define ll long long
using namespace std;
typedef struct{
    ll row;
    ll col;
}pose;

ll formula(ll n){
    //1,2,3,6,7,10,11,14,15,18,19,22
    //0,1,2,3,4,5 ,6 ,7, 8
    if(n > 0){
        return 2*n - (1 + 1 * (1- 2*(n%2)))/2;
    }
    if(n==0)return 1;
    return 0;
}

//mencari berapa banyak gerakan untuk thief menuju ujung grid yang berlawanan terhadap
//posisi nya dan cop
ll opositeRelativeCop(ll col, ll row, pose *Cop, pose *Thief){
    if(Thief->col < Cop->col){
        //Thief ada di kiri police
        if(Thief->row < Cop->row){
            //thief ada di atas or sama
            //gerak ke pojok kiri atas
            ll diffC = (Thief->col - 1);
            ll diffR = (Thief->row -1);
            Thief->col -= diffC;
            Thief->row -= diffR;
            Cop->col -= diffC;
            Cop->row -= diffR;
            return 2*(diffC + diffR);
        }
        //berarti dibawah, pojok kiri bawah
        ll diffC = (Thief->col - 1);
        ll diffR = (row - Thief->row);
        Thief->col -= diffC;
        Thief->row += diffR;
        Cop->col -= diffC;
        Cop->row += diffR;
        return 2*(diffC + diffR);
    }
    //berarti di kanan
    if(Thief->row < Cop->row){
        //thief ada di atas atau sama, gerak ke pojok kanan atas
        ll diffC = (col - Thief->col);
        ll diffR = (Thief->row - 1);
        Thief->col += diffC;
        Thief->row -= diffR;
        Cop->col += diffC;
        Cop->row -= diffR;
        return 2*(diffC + diffR);
    }
    //berarti thief ada dibawah kanan, gerak ke pojok kanan bawah
    ll diffC = (col - Thief->col);
    ll diffR = (row - Thief->row);
    Thief->col += diffC;
    Thief->row += diffR;
    Cop->col += diffC;
    Cop->row += diffR;
    return 2*(diffC + diffR);
}

//menghitung berapa lama gerakan yang diperlukan thief mencapai ujung grid yang optimal
ll moveToEnd(ll row, ll col, pose *Cop, pose *Thief, char who){
    //cek same col dulu
    if(Thief->col == Cop->col){
        //cek same row?
        if(Cop->row == Thief->row)return 0;
        //waypoint
        ll distoLeft = Thief->col - 1, distoRight = col - Thief->col;
        ll diffC;
        if(distoLeft < distoRight){
            //chose right, terjauh
            Thief->col += distoRight;
            Cop->col += distoRight;
            diffC = distoRight;
        }else{
            //chose left terjauh
            diffC = distoLeft;
            Thief->col -= distoLeft;
            Cop->col -= distoLeft;
        }
        bool flag = (abs(Thief->row - Cop->row) != 1);
        if(Thief->row < Cop->row){
            //thief ada di atas cop
            //gerak ke pojok atas, way point
            ll diffR = Thief->row - 1;
            Thief->row -= diffR;
            Cop->row -= diffR;
            return  2*(diffR + diffC) * flag;
        }
        //berarti thief ada di bawah cop
        ll diffR = row - Thief->row;
        Thief->row += diffR;
        Cop->row += diffR;
        return 2*(diffR + diffC) * flag;
    }
    //not on the same col, cek on the same row
    if(Thief->row == Cop->row){
        //waypoint
        ll distoTop = Thief->row - 1, distoBottom = row - Thief->row;
        ll diffR;
        if(distoTop < distoBottom){
            //chose bottom, terjauh
            diffR = distoBottom;
            Thief->row += distoBottom;
            Cop->row += distoBottom;
        }else{
            //chose top terjauh
            diffR = distoTop;
            Thief->row -= distoTop;
            Cop->row -= distoTop;
        }
        bool flag = (abs(Thief->col - Cop->col) != 1);
        if(Thief->col < Cop->col){
            //thief ada di kiri cop
            //gerak ke pojok kiri, way point
            ll diffC = (Thief->col -  1);
            Thief->col -= diffC;
            Cop->col -= diffC;
            return  2*(diffR + diffC) * flag;
        }
        //berarti thief ada di kanan cop, ke pojok kanan waypoint
        ll diffC = (col - Thief->col);
        Cop->col += diffC;
        Thief->col += diffC;
        return 2*(diffR + diffC) * flag;
    }    
    
    //so not on the same col, and not on the same row
    ll dCol = abs(Cop->col - Thief->col);
    ll dRow = abs(Thief->row - Cop->row);
    if(abs(dRow - dCol) <= 1){
        //giliran thief
        return opositeRelativeCop(col, row, Cop, Thief);
    }

    if(dCol > dRow){
        //kalau jarak col lebih besar
        //thief gerak ROW
        //giliran si thief, jalan nyamain si cop
        if(Cop->col < Thief->col){
                //cop di kiri thief
            Cop->col += dRow;
        }else{
            Cop->col -= dRow;
        }
        if(Cop->row < Thief->row){
            //cop di atas thief
            Thief->row -= dRow;
        }else{
            Thief->row += dRow;
        }
        ll ans = 2*dRow + moveToEnd(row, col, Cop, Thief, who);
        return ans;
    }
    //berarti thief gerak Col karena dCol < dRow
    //giliran si thief, jalan nyamain si cop
    if(Cop->row < Thief->row){
        //cop di atas thief
        Cop->row += dCol;
    }else{
        Cop->row -= dCol;
    }
    //thief gerak col, cop gerak row
    if(Cop->col < Thief->col){
        //cop di kiri thief
        Thief->col -= dCol;
    }else{
        Thief->col += dCol;
    }
    ll ans = 2*dCol + moveToEnd(row, col, Cop, Thief, who);
    return ans;      
}

int main(){
    ll r, c;
    pose Cop, Thief;
    scanf("%lld %lld", &r, &c);
    scanf("%lld %lld", &Cop.row, &Cop.col);
    scanf("%lld %lld", &Thief.row, &Thief.col);
    char who;
    scanf("\n%c", &who);
    //0 = C, 1 =T
    ll ans;
    ll sum = abs(Cop.col - Thief.col) + abs(Thief.row - Cop.row);
    //1 row atau 1 col pasti cop menang
    if(r == 1){
        if(sum == 1 && who == 'C'){
            puts("1");
            return 0;
        }
        if(Thief.col > Cop.col){
            //pindahin thief and cop ke mentok kanan
            ll diff = c - Thief.col;
            Thief.col += diff;
            Cop.col += diff;
            int turn = 0;
            if(who =='T'){
                //jika sudah dipinggir tapi giliran Thief, jadi perlu gerak dulu
                //simulasiin pergerakannya
                turn = 1;
                bool copWin = false;
                while(1){
                    if(turn %2== 1){
                        Thief.col = Thief.col - 1 + 2*(turn % 4 == 3);
                    }else{
                        Cop.col+=1;
                    }

                    if(Thief.col == Cop.col || Thief.col == c){
                        copWin = Thief.col == Cop.col;
                        break;
                    }

                    turn +=1;
                }
                if(copWin){
                    printf("%lld\n", (ll)turn + 2*diff);
                    return 0;
                }
            }
            ll d = Thief.col - Cop.col;
            ans = 2*diff + formula(d-1) + turn;
            printf("%lld\n", ans);
        
        }else{
            //bearti thief di kiri cop
            //pindahinn thief, cop ke mmentok kiri
            ll diff = Thief.col - 1;
            Thief.col -= diff;
            Cop.col -= diff;
            int turn = 0;
            if(who =='T'){
                //jika sudah dipinggir tapi giliran Thief, jadi perlu gerak dulu
                //simulasiin pergerakannya
                turn = 1;
                bool copWin = false;
                while(1){
                    if(turn %2== 1){
                        Thief.col =Thief.col + 1 - 2*(turn % 4 == 3);
                    }else{
                        //cop
                        Cop.col-=1;
                    }

                    if(Thief.col == Cop.col || Thief.col == 1){
                        copWin = Thief.col == Cop.col;
                        break;
                    }

                    turn +=1;
                }
                if(copWin){
                    printf("%lld\n", (ll)turn+2*diff);
                    return 0;
                }
            }         
            ll d = Cop.col - Thief.col;
            ans = 2*diff + formula(d-1) + turn;
            printf("%lld\n", ans);            
        }
        return 0;
    }
    if(c == 1){
        if(sum == 1 && who == 'C'){
            puts("1");
            return 0;
        }
        if(Thief.row > Cop.row){
            //pindahin thief and cop ke mentok bawah
            ll diff = r - Thief.row;
            Thief.row += diff;
            Cop.row += diff;
            int turn = 0;
            if(who =='T'){
                //jika sudah dipinggir tapi giliran Thief, jadi perlu gerak dulu
                //simulasiin pergerakannya
                turn = 1;
                bool copWin = false;
                while(1){
                    if(turn %2== 1){
                        Thief.row = Thief.row - 1 + 2*(turn % 4 == 3);
                    }else{
                        Cop.row+=1;
                    }

                    if(Thief.row == Cop.row || Thief.row == r){
                        copWin = Thief.row == Cop.row;
                        break;
                    }
                    turn +=1;
                }
                if(copWin){
                    printf("%lld\n", (ll)turn + 2*diff);
                    return 0;
                }
            }
            ll d =   Thief.row - Cop.row;
            ans = 2*diff + formula(d-1) + turn;
            printf("%lld\n", ans);
        }else{
            //pindahinn thief, cop ke mmentok atas
            ll diff = Thief.row - 1;
            Thief.row -= diff;
            Cop.row -= diff;
            int turn = 0;
            if(who =='T'){
                //jika sudah dipinggir tapi giliran Thief, jadi perlu gerak dulu
                //simulasiin pergerakannya
                turn = 1;
                bool copWin = false;
                while(1){
                    if(turn %2== 1){
                        Thief.row = Thief.row + 1 - 2*(turn % 4 == 3);
                    }else{
                        Cop.row-=1;
                    }

                    if(Thief.row == Cop.row || Thief.row == 1){
                        copWin = Thief.row == Cop.row;
                        break;
                    }

                    turn +=1;
                }
                if(copWin){
                    printf("%lld\n", (ll)turn + 2*diff);
                    return 0;
                }
            }
            ll d = Cop.row - Thief.row;
            ans = 2*diff + formula(d-1) + turn;
            printf("%lld\n", ans);            
        }
        return 0;        
    }
    //kalau tidak 1 row or 1 col
    if(sum%2 == 0){
        //genap, first kalah kecuali 1 row atau 1 col
        if(who == 'C'){
            puts("0");
        }else{
            //T, berarti T kalah a.ka cop won
            //tambahkan banyak gerakan untuk sampai pojok
            ans = moveToEnd(r, c, &Cop, &Thief, who);
            sum = abs(Cop.col - Thief.col) + abs(Thief.row - Cop.row);
            ans += formula(sum-1);
            printf("%lld\n", ans);
        }
    }else{
        //ganjil, first menang
        if(who == 'C'){
            ans = moveToEnd(r, c, &Cop, &Thief, who);
            sum = abs(Cop.col - Thief.col) + abs(Thief.row - Cop.row);
            ans += formula(sum-1);
            printf("%lld\n", ans);
        }else{
            puts("0");
        }
    }
    return 0;
}
