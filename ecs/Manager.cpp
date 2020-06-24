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
//
//void Manager::setupEntity(const EntityRef &e){
//
//
//    printf("created entity: %u, %u\n", entitiesCreated, e->getId() );
//
//    e->mEntityId = entitiesCreated;
//    mEntities.push_back( e );
//
//    printf("created entity: %u, %u\n", entitiesCreated, e->getId() );
//
//    e->mManager = this;
//    e->mComponentPool = &mComponents;
//
//    e->setup();
//
//    if( e->onLateSetup ){
//        e->onLateSetup();
//    }
//
//    printf("created entity: %u, %u\n", entitiesCreated, e->getId() );
//
//    entitiesCreated += 1;
//}


void Manager::addComponent(uint64_t entityId, ComponentID id, const ComponentRef component ){

//        auto& componentVector = mEntityPool.mComponents[id];
//        auto& componentVectorByType = mComponentsByType[id];
}

void Manager::refresh(){

    /* 

    entities id| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | ...> grows at runtime
    
    ComponentA | 1 | 1 | 1 | 1 | 0 | 1 | 1 | 1 | ...>
    ComponentB | 0 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | ...>
    ComponentC | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 1 | ...>
    .
    .
    ... Goes up to ecs::MaxComponents
    */

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
    
    printf("removed entity: %s\n", e->getName().c_str());
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
