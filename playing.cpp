//
//  main.cpp
//  playing
//
//  Created by 陳柏廷 on 2020/9/19.
//  Copyright © 2020 陳柏廷. All rights reserved.
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



// tunable parameters
string read_file="/Users/chenboting/Desktop/OOXX/OOXX8.csv";


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



int main()
{
    clock_t start_time, after_reading_csv;
    start_time=clock();
    
    // random seed
    srand((unsigned) time(NULL));
    
    
    // map<the vector input, vec[i]=win rate of i given input vec>
    map<vector<int>,vector<int>> all_learning;
    
    
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
    
    
    after_reading_csv=clock();
    cout<<"Time consumed for reading csv file: "<<double(after_reading_csv-start_time)/CLOCKS_PER_SEC<<" (sec)"<<endl;
    cout<<"--------------------------------------------"<<endl;
    
    
    vector<int> allmoves_temp;
    chessboard Game;
    string yn;
    int FirstOrSecond;
    
    while (true)
    {
        cin.clear();
        cout<<"Do you want to play first? (y/n): ";
        cin>>yn;
        if (yn=="y"){FirstOrSecond=0;}
        else if (yn=="n"){FirstOrSecond=1;}
        else {cout<<"Please enter y or n. ";continue;}
        cin.clear();
        
        
        Game.initiate();
        Game.show_chess();
        cout<<"--------------------------------------------"<<endl;
        int moving;
        while (!Game.winning().first and Game.remaining_int()!=1)
        {
            
            
            if (Game.get_turn()%2==FirstOrSecond)
            {
                cout<<"player: ";
                cin>>moving;
                moving--;   // can be removed
                Game.move(moving);
                Game.show_chess();
                cin.clear();
//                cout<<"all moves: ";
//                Game.show_allmoves();
                cout<<"--------------------------------------------"<<endl;
                
            }
            else
            {
                cout<<"computer: ";
                allmoves_temp=Game.get_allmoves();
                moving=give_move(all_learning[allmoves_temp]);
                cout<<moving+1<<endl;   // +1 can be removed
                Game.move(moving);
                allmoves_temp.clear();
                Game.show_chess();
//                cout<<"all moves: ";
//                Game.show_allmoves();
                cout<<"--------------------------------------------"<<endl;
            }
            
            
            
        }
        
        
        if (!Game.winning().first and Game.remaining_int()==1)
        {
            Game.move(Game.the_remaining_one());
            Game.show_chess();
//            cout<<"all moves: ";
//            Game.show_allmoves();
            cout<<"--------------------------------------------"<<endl;
        }
        
        
        if (Game.winning().second==p1)
        {
            if (FirstOrSecond==0){cout<<"player wins"<<endl;}
            else {cout<<"computer wins"<<endl;}
            
        }
        else if (Game.winning().second==p2)
        {
            if (FirstOrSecond==0){cout<<"computer wins"<<endl;}
            else {cout<<"player wins"<<endl;}
        }
        else {cout<<"draw game"<<endl;}
        cout<<"--------------------------------------------"<<endl;
        
        
    }
    
    
    return 0;
}
