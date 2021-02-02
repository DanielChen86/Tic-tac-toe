#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;


const int boardsize=9;
const int p1=1;
const int p2=-1;

class chessboard
{
    private:
    // chess[i]=0: no chess at i
    // chess[i]=1: chess p1 at i / chess[i]=-1: chess p2 at i
    int chess[boardsize];
    
    // turn=even: p1 play / turn=odd: p2 play
    int turn;
    
    // record all moves
    vector<int> allmoves;

public:
    // remaining=[0,1,9,3,4,9,6,7,8]: spaces 2, 5 are filled
    vector<int> remaining;
    
    // game start
    void initiate()
    {
        // no chess at all
        for (int i=0;i<boardsize;i++)
        {chess[i]=0;}
        // p1 plays first
        turn=0;
        // no moves at first
        allmoves.clear();
        allmoves.push_back(9);
        // all spaces remain
        remaining.clear();
        for (int j=0;j<boardsize;j++)
        {remaining.push_back(j);}
    }
    
    // put a chess at position: block
    void move(int block)
    {
        if (chess[block]==0)
        {
            if (turn%2==0)
            {
                chess[block]=p1;
                allmoves.push_back(block);
                turn++;
                remaining[block]=boardsize;
            }
            else
            {
                chess[block]=p2;
                allmoves.push_back(block);
                turn++;
                remaining[block]=boardsize;
            }
        }
        else
        {cout<<"space taken"<<endl;}
    }
    
    // return turn
    int get_turn()
    {return turn;}
    
    // return allmoves
    vector<int> get_allmoves()
    {return allmoves;}
    
    void show_allmoves()
    {
        for (int i=1;i<allmoves.size();i++)
        {cout<<allmoves[i]<<" ";}
        cout<<endl;
    }


    // first: true if someone wins ;second(p1/p2): who wins
    pair<bool,int> winning()
    {
        pair<bool,int> result;
        if((chess[0]==chess[1]) and (chess[0]==chess[2]) and (chess[0]!=0))
        {result.first=true;result.second=chess[0];}
        else if((chess[3]==chess[4]) and (chess[3]==chess[5]) and (chess[3]!=0))
        {result.first=true;result.second=chess[3];}
        else if((chess[6]==chess[7]) and (chess[6]==chess[8]) and (chess[6]!=0))
        {result.first=true;result.second=chess[6];}
        else if((chess[0]==chess[3]) and (chess[0]==chess[6]) and (chess[0]!=0))
        {result.first=true;result.second=chess[0];}
        else if((chess[1]==chess[4]) and (chess[1]==chess[7]) and (chess[1]!=0))
        {result.first=true;result.second=chess[1];}
        else if((chess[2]==chess[5]) and (chess[2]==chess[8]) and (chess[2]!=0))
        {result.first=true;result.second=chess[2];}
        else if((chess[0]==chess[4]) and (chess[0]==chess[8]) and (chess[0]!=0))
        {result.first=true;result.second=chess[0];}
        else if((chess[2]==chess[4]) and (chess[2]==chess[6]) and (chess[2]!=0))
        {result.first=true;result.second=chess[2];}
        else {result.first=false;result.second=0;}
        return result;
    }
    

    // count the number of remaining space on the chessboard
    int remaining_int()
    {
        int result=0;
        for (int i=0;i<remaining.size();i++)
        {
            if (remaining[i]!=boardsize)
            {result++;}
        }
        return result;
    }

    // if there is only one remaining space on chess[9], return that space
    int the_remaining_one()
    {
        int sum=0;
        int result=boardsize;
        for (int i=0;i<boardsize;i++)
        {
            if (chess[i]==0)
            {
                sum+=1;
                result=i;
            }
        }
        if (sum!=1){cout<<"something's wrong"<<endl;}
        return result;
    }


    string num_to_str(int num)
    {
        if (num==0){return "o";}
        else if (num==p1){return "a";}
        else if (num==p2){return "b";}
        else {cout<<"error2"<<endl;return "error";}
    }

    void show_chess()
    {
        int i;
        for (i=0;i<3;i++)
        {cout.width(3);cout<<num_to_str(chess[i]);}
        cout<<endl;
        for (i=3;i<6;i++)
        {cout.width(3);cout<<num_to_str(chess[i]);}
        cout<<endl;
        for (i=6;i<9;i++)
        {cout.width(3);cout<<num_to_str(chess[i]);}
        cout<<endl;
    }

};



