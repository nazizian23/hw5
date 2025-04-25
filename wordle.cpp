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


void generateWords(string& attempt, int pos,map<char,int>& floatLetters, int floatsLeft, const set<string>& dict, set<string>& results, const set<char>& allowedLetters);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    //store valid words
    set<string> results;
    //start with given pattern
    string attempt=in;
    //map to track required floating floating letters and how many of each
    map<char,int> floatLetters;
    int totalFloat=0;
    //count the frequency of floating letters
    for(size_t i=0;i<floating.size();++i){
        char c=floating[i];
        floatLetters[c]++;
        totalFloat++;
    }
    int numBlanks=0;
    //Adjust the floating letter counts based on already fixed letters in the pattern
    for(size_t i=0;i<attempt.size();++i){
      char c=attempt[i];
      if(c=='-'){
        numBlanks++;
      }
      else if(floatLetters.find(c) != floatLetters.end()){
        if(--floatLetters[c]==0){
          floatLetters.erase(c);
        }
        totalFloat--;
      }
    }
    //Determine the set of allowed letters that can fill blanks
    set<char>allowedLetters(floating.begin(),floating.end());
    if(numBlanks>floating.size()){
      //If there are extra blanks allow any letter a through z
      char c;
      for(c='a';c<='z';++c){
        allowedLetters.insert(c);
      }
    }
    // begin recursion
    generateWords(attempt,0,floatLetters,totalFloat,dict,results,allowedLetters);
    
    return results;

}

// Define any helper functions here
//Recursive helper function
void generateWords(string& attempt, int pos,map<char,int>& floatLetters, int floatsLeft, const set<string>& dict, set<string>& results, const set<char>& allowedLetters){
  //If the word is fully filled only add if no floating letters and the word is valid
  if(pos==attempt.size()){
      if(floatLetters.empty() && dict.count(attempt)){
          results.insert(attempt);
      }
      return;
  }
  //Count how many blanks remain after current position
  int blanksAhead=0;
  for(size_t j=pos;j<attempt.size();++j){
    if(attempt[j]=='-'){
      blanksAhead++;
    }

  }

  if(blanksAhead<floatsLeft){
    return;
  }
  //If current position is already filled move to next
  if(attempt[pos]!='-'){
    generateWords(attempt,pos+1,floatLetters,floatsLeft,dict,results,allowedLetters);
    return;
  }
  else{
    //Try all allowed letters at the blank position
    set<char>::const_iterator it;
    for(it=allowedLetters.begin();it!=allowedLetters.end();++it){
      char c=*it;
      attempt[pos]=c;
      //copy floating map and update it if its using a floating letter
      map<char,int> newFloatLetters=floatLetters;
      int newFloatsLeft=floatsLeft;
      if(newFloatLetters.find(c)!=newFloatLetters.end()){
        if(--newFloatLetters[c]==0){
          newFloatLetters.erase(c);
        }
        newFloatsLeft--;
      }
      //recurse for next position
      generateWords(attempt,pos+1,newFloatLetters,newFloatsLeft,dict,results,allowedLetters);
      attempt[pos]='-';
    }
  }
}

