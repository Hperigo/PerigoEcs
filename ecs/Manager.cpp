//
//  Manager.cpp
//  EcsCereal
//
//  Created by Henrique on 6/21/18.
//

#include <map>
#include "Manager.h"

using namespace ecs;

void Manager::setup(){
    
    if( needsRefresh == true ){
        refresh();
    }
    
    for(auto& sys  : mSystems){
        sys->setup();
    }
    
    update();
    isManagerInitialized = true;
}

void Manager::update(){
    refresh();
    
    for(auto& sys  : mSystems){
        if( sys->updatable ){
            sys->update();
        }
    }
}

void Manager::refresh(){

    //erase dead Entities
    for( auto eIt = mEntities.begin(); eIt != mEntities.end(); ){

        if( ! (*eIt)->isAlive() )
        {
            auto e = *eIt;
            entityDeleter( e );
            eIt = mEntities.erase( eIt );
            
        }else{
            ++eIt;
        }
    }
}

void Manager::draw(){

    for(auto& sys  : mSystems){
        if( sys->drawable ){
            sys->draw();
        }
    }
}


void Manager::entityDeleter(ecs::EntityRef e){


    for( auto& components : mComponents.mComponents ){

        if( components ){
            components->remove(e);
        }
    }
    delete e;
//    e = nullptr;
}

// prints a csv with the components in a row, and entities as collumns
void Manager::printCheck(){
    // print entities id's
    
#if 0
    cout << "enti,";
    for( int i = 0; i < mEntities.size(); i++){

        if( mEntities[i] == nullptr ){
            cout << "*" << "," ;
        }else{
            cout << i << ",";
        }
    }
    cout << endl;

    // print valid components
    for( int i = 0; i < 10; i++){
        cout << "id: " << i << ",";

        for( int j = 0; j < mComponents[i].size(); j++){
            auto c = mEntityPool.mComponents[i][j];
            bool valid = c != nullptr;
            cout << valid << ",";
        }
        cout << "_" << endl;
    }
#endif
}
