//
//  Tweeter.cpp
//  Twitter
//
//  Created by Surya Mattu on 23/11/13.
//
//

#include "Tweeter.h"

Tweeter::Tweeter(){
    name = " ";
    ID = -1 ;
    position = ofVec3f(-1, -1,-1);
}

Tweeter::Tweeter(string _name, int _id){
    name = _name;
    ID = _id;
    position = ofVec3f(-1, -1,-1);
}


vector<Tweet> Tweeter::getTweetsByDate(Date d){
    vector<Tweet> dateTweets;
    for(int i =0; i < tweets.size(); i++){
        if(tweets[i].tweetDate.day == d.day &&
           tweets[i].tweetDate.month == d.month &&
           tweets[i].tweetDate.year== d.year){
            dateTweets.push_back(tweets[i]);
        }
    }
    
    return dateTweets;
}

bool Tweeter::hasTweetOnDate(Date d){
    for(int i =0; i< tweets.size(); i++){
        if(tweets[i].tweetDate.day == d.day &&
           tweets[i].tweetDate.month == d.month &&
           tweets[i].tweetDate.year== d.year){
//      cout<<name <<" has tweets on date "<< d.day<< " , "<<d.month<<" , "<<d.year<<endl;      
            return true;
        }
    }
    
//    cout<<name <<" has no tweets on date "<< d.day<< " , "<<d.month<<" , "<<d.year<<endl;
    return false;
}




