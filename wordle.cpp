#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here

bool containsAllFloating(const string& word, const string& floating);

void recurseWordle(string& curr, int index,string floating, const set<string>& dict, set<string>& results);
// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    set<string> results;
    string curr=in;
    string adjustedFloating=floating;
    //removes the fixed position chars from the floating list
    for(size_t i=0;i<curr.size();++i){
        char c=curr[i];
        size_t pos=adjustedFloating.find(c);
        if(c!='-'&& pos!=string::npos){
            adjustedFloating.erase(pos,1);//removes the first occurrence
        }
    }
    // begin recursion
    recurseWordle(curr,0,adjustedFloating,dict,results);
    
    return results;

}

// Define any helper functions here

//Checks if all floating letters exist in the final word
bool containsAllFloating(const string& word, const string& floating){
    //make a temp copy of word so we can find used letters and mark them
    string temp=word;

    //For each floating letter check if its in the temp word
    for(size_t i=0;i<floating.size();++i){
        char c=floating[i];
        //find first occurrence of the char
        size_t pos=temp.find(c);
        //if its not found the word is missing a required floating letter
        if(pos==string::npos){
            return false;
        }
        temp[pos]='*';
    }

    return true;




}


//Recursive helper to generate words char by char
void recurseWordle(string& curr, int index,string floating, const set<string>& dict, set<string>& results){

    //Base case when the entire word is formed
    if(index==curr.size()){
        //Check if all the letters are used and word is correct
        if(containsAllFloating(curr,floating) && dict.find(curr)!=dict.end()){
            results.insert(curr);
        }
        return;
    }
    //Counts the remaining blank spaces from the current index on
    int blanksLeft=0;
    for(int i=index;i<curr.size();++i){
        if(curr[i]=='-'){
            ++blanksLeft;
        }
    }
    //If not enough blanks left to place the letters stop
    if(blanksLeft<floating.size()){
        return;
    }
    //If the current char is already fixed continue to next index
    if(curr[index]!='-'){
        recurseWordle(curr,index+1,floating,dict,results);
    }
    else{
        //Try lowercase letters
        for(char c= 'a';c<='z';++c){
            curr[index]=c;
            //copy the floating list to update if we use the letter
            size_t pos=floating.find(c);
            string nextFloating=floating;
            //if the letter is in the floating list remove one instance of it
            if(pos!=string::npos){
                nextFloating.erase(pos,1);
                recurseWordle(curr,index+1,nextFloating,dict,results);
            }
            //if not in floating, only continue if there are enough blanks left
            else if(floating.size()<blanksLeft){
                recurseWordle(curr,index+1,floating,dict,results);

            }
            
        }

        curr[index]='-';
    }


}




