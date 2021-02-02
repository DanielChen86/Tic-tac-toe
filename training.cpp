//
//  main.cpp
//  training
//
//  Created by 陳柏廷 on 2020/11/11.
//

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include "./chessboard.h"
using namespace std;


// appears in the last-step-win keys
const int absolute_big_num=10000000;

// tunable parameters
// prize and punishment
const int win_score=100;
const int lose_score=-3;
const int draw_score=5;
// the number to start after reviving
const int revive_int=30;
// the number of games to train
const int playing_times=100000;
// the maximum and minimum length of key whose value can vary
// min_training_length<=length<max_training_length
const int max_training_length=9; // Warning: should not be greater than 9
const int min_training_length=8;
// the read in file and the write out file
string read_file="/Users/chenboting/Desktop/OOXX.csv";
string write_file="/Users/chenboting/Desktop/OOXX1.csv";

vector<int> vec_norm(boardsize,0);



int sum_vec(vector<int> vec,int start, int end)
{
    int sum=0;
    for (int i=start;i<end;i++)
    {sum+=vec[i];}
    return sum;
}

// give a move base on [0,0,3,3,3,3,0,0]= the relative weight of next move
int give_move(vector<int> vec)
{
    int result=-1;
    vector<int> ints;
    for (int i=0;i<boardsize;i++)
    {ints.push_back(sum_vec(vec, 0, i+1));}
    int random=rand()%ints[boardsize-1];
    for (int j=0;j<boardsize;j++)
    {
        if (ints[j]>random)
        {
            result=j;
            break;
        }
    }
    ints.clear();
    return result;
}



// prize the winner and punish the loser
int score(int WL,int player)
{
    if (WL==p1)
    {
        if (player==p1)
        {return win_score;}
        else {return lose_score;}
    }
    else if (WL==p2)
    {
        if (player==p1)
        {return lose_score;}
        else {return win_score;}
    }
    else {return draw_score;}
}


// return the smaller one, input integer or boardsize
long min_boardsize(long integer)
{
    if (integer<boardsize){return integer;}
    else if (integer>boardsize){return boardsize;}
    else {return boardsize;}
}


// to avoid elements in all_learning.second be smaller than zero
// to revive all_learning.second if all_learning.second=[0,0,0,0,0,0,0,0]
void simplify(map<vector<int>,vector<int>> *Map, vector<int> key, int element)
{
    if ((*Map)[key][element]<0)
    {(*Map)[key][element]=0;}
//    if ((*Map)[key][element]>1000)
//    {(*Map)[key][element]=1000;}
    if ((*Map)[key]==vec_norm)
    {
        (*Map)[key].clear();
        for (int j=0;j<boardsize;j++)
        {(*Map)[key].push_back(revive_int);}
        for (int k=0;k<key.size();k++)
        {
            if (key[k]!=boardsize)
            {
                (*Map)[key][key[k]]=0;
            }
        }
    }
}


// devide AllMoves into sub-vectors, and prize/punish the winner/loser
void devide(map<vector<int>,vector<int>> *Map, vector<int> AllMoves, int WL)
{
    int Index;
    int length; // length-1=length of the key=Index.max
    vector<int> vector_auxi;    // vector_auxi=key
    int int_auxi;
    for (length=min_training_length;length<AllMoves.size();length++)
    {
        for (Index=0;Index<length;Index++)
        {vector_auxi.push_back(AllMoves[Index]);}
        if (length==AllMoves.size()-1 and length<boardsize)
        {
            for (int k=0;k<boardsize;k++)
            {
                if (k==AllMoves[AllMoves.size()-1])
                {(*Map)[vector_auxi][k]=absolute_big_num;}
                else {(*Map)[vector_auxi][k]=0;}
            }
        }
        else
        {
            if (length<max_training_length)
            {
                int_auxi=AllMoves[length];
                if ((length%2)==0)      // player1's turn
                {
                    (*Map)[vector_auxi][int_auxi]+=score(WL,p2);
                    simplify(Map, vector_auxi, int_auxi);
                }
                else        //player2's turn
                {
                    (*Map)[vector_auxi][int_auxi]+=score(WL,p1);
                    simplify(Map, vector_auxi, int_auxi);
                }
            }
            vector_auxi.clear();
        }
    }
}




int main()
{
    clock_t start_time, after_reading_csv, after_playing_chess, end_time;
    
    // random seed
    srand((unsigned) time(NULL));
    
    
    // map<the vector input, vec[i]=win rate of i given input vec>
    map<vector<int>,vector<int>> all_learning;
    
    start_time=clock();
    
    // auxiliary (to read csv)
    vector<int> vec_first;
    vector<int> vec_second;
    int temp_int=0;
    
    // read csv file
    fstream file;
    file.open(read_file);
    string line;
    string data;
    while (getline( file, line,'\n'))  // read until "\n"
    {
        if (temp_int==0)
        {
            istringstream templine(line); // string -> stream
            while (getline( templine, data,',')) // read until ","
            {
                vec_first.push_back(atoi(data.c_str()));  //string -> int
            }
            temp_int=1;
        }
        else
        {
            istringstream templine(line); // string -> stream
            while (getline( templine, data,',')) // read until ","
            {
                vec_second.push_back(atoi(data.c_str()));  //string -> int
            }
            temp_int=2;
        }
        if (temp_int==2)
        {
            all_learning[vec_first]=vec_second;
            vec_first.clear();
            vec_second.clear();
            temp_int=0;
        }
    }
    file.close();
    
    // to create a copy of all_learning
    // all_learning: varies while learning
    // all_learning_copy: a copy of the original write-in csv, a constant
    map<vector<int>,vector<int>> all_learning_copy=all_learning;
    
    after_reading_csv=clock();
    cout<<"Time consumed for reading csv file: "<<double(after_reading_csv-start_time)/CLOCKS_PER_SEC<<" (sec)"<<endl;
    
    
    vector<int> allmoves_temp;
    chessboard Game;
    int playgames=0;
//    int total_training_times=0;
    while (playgames<playing_times)
    {
        Game.initiate();
        while (!Game.winning().first and Game.remaining_int()!=1)
        {
            allmoves_temp=Game.get_allmoves();
            Game.move(give_move(all_learning_copy[allmoves_temp]));
            allmoves_temp.clear();
        }
        
        
        if (!Game.winning().first and Game.remaining_int()==1)
        {Game.move(Game.the_remaining_one());}
        
        
        if (Game.get_allmoves().size()>=min_training_length)
        {
            devide(&all_learning, Game.get_allmoves(), Game.winning().second);
        }
        playgames++;
    }
    
    
    after_playing_chess=clock();
    cout<<"Time consumed for playing "<<playing_times<<" times: "<<double(after_playing_chess-after_reading_csv)/CLOCKS_PER_SEC<<" (sec)"<<endl;
//    cout<<"total training times= "<<total_training_times<<endl;
    
    // write csv file
    std::ofstream OOXX(write_file);
    map<vector<int>,vector<int>>::iterator write=all_learning.begin();
    int writing;

    while (write!=all_learning.end())
    {

        for (writing=0;writing<(write->first.size()-1);writing++)
        {
            OOXX<<write->first[writing]<<",";
        }
        OOXX<<write->first[write->first.size()-1]<<"\n";

        for (writing=0;writing<(write->second.size()-1);writing++)
        {
            OOXX<<write->second[writing]<<",";
        }
        OOXX<<write->second[write->second.size()-1]<<"\n";



        write++;
    }
    OOXX.close();
    
    end_time=clock();
    cout<<"Time consumed for writing csv file: "<<double(end_time-after_playing_chess)/CLOCKS_PER_SEC<<" (sec)"<<endl;
    
    
    return 0;
}

