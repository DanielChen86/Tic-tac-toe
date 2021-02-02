//
//  main.cpp
//  initializing
//
//  Created by 陳柏廷 on 2020/9/19.
//  Copyright © 2020 陳柏廷. All rights reserved.
//

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <time.h>
using namespace std;


const int boardsize=9;
const int initial=30;

string write_file="/Users/chenboting/Desktop/OOXX.csv";

// return true if integer is in vec
bool in_vec(vector<int> vec,int integer)
{
    bool result=false;
    for (int i=0;i<vec.size();i++)
    {
        if (vec[i]==integer){result=true;}
    }
    return result;
}

// show a vector
void show_vec(vector<int> vec)
{
    for (int i=0;i<vec.size();i++)
    {
        if (i<vec.size()-1)
        {cout<<vec[i]<<",";}
        else
        {cout<<vec[i]<<endl;}
    }
}


// show a map:
// key: 1,2,3,4,5
// value: 6,7,8,9,10
void show_map(map<vector<int>,vector<int>> Map)
{
    map<vector<int>,vector<int>>::iterator iter=Map.begin();
    while (iter!=Map.end())
    {
        show_vec(iter->first);
        show_vec(iter->second);
        iter++;
    }

}

// if condition= vec(1,3,7)
// then initial= (x0,0,x2,0,x4,x5,x6,0)
vector<int> put_vec_init(vector<int> conditions,vector<int> initial)
{
    vector<int> result=initial;
    for (int i=0;i<conditions.size();i++)
    {
        result[conditions[i]]=0;
    }
    return result;
}


int main(void)
{
    
    clock_t starting,start_writing_csv,ending;
    
    starting=clock();
    
    // map<the vector input, vec[i]=win rate of i given input vec>
    map<vector<int>,vector<int>> all_learning;
    
    
    // auxiliary
    vector<int> vec_temp;
    map<vector<int>,vector<int>> map_temp;
    map<vector<int>,vector<int>>::iterator it_temp; // an iterator of map_temp
    vector<int> vec_rate_init(boardsize,initial);
    int Index;
    int length;
    
    // set up the map elements with key= cleared chessboard
    vec_temp.push_back(boardsize);
    for (Index=0;Index<boardsize;Index++)
    {
        all_learning[vec_temp]=vec_rate_init;
    }
    vec_temp.clear();
    
    // Iterate to make all_learning contain keys with length longer and longer
    map<vector<int>,vector<int>>::iterator it=all_learning.begin();
    for (length=1;length<boardsize-1;length++)
        {
        while (it!=all_learning.end())
        {
            if (it->first.size()==length)
            {
                for (Index=0;Index<boardsize;Index++)
                {
                    vec_temp=it->first;
                    if (!in_vec(vec_temp, Index))
                    {
                        vec_temp.push_back(Index);
                        // store the key-value pairs in map_temp temporarily
                        map_temp[vec_temp]=put_vec_init(vec_temp,vec_rate_init);
                        vec_temp.clear();
                    }
                }
            }
            it++;
        }
        // append map_temp to all_learning
        it_temp=map_temp.begin();
            while (it_temp!=map_temp.end())
            {
                all_learning[it_temp->first]=it_temp->second;
                it_temp++;
            }
        map_temp.clear();
        it=all_learning.begin();
    }
    
    
    start_writing_csv=clock();
    cout<<"Time consumed for constructing all_learning: "<<double(start_writing_csv-starting)/CLOCKS_PER_SEC<<" (sec)"<<endl;
    
    cout<<"The size of all_learning is: "<<all_learning.size()<<endl;
    
    // write in csv file
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
    
    ending=clock();
    cout<<"Time consumed for writing csv file: "<<double(ending-start_writing_csv)/CLOCKS_PER_SEC<<" (sec)"<<endl;
    
    
    return 0;
}

