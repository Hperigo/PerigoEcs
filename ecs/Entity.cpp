//
// Created by Henrique on 5/24/18.
//

#include "Entity.h"
#include "Manager.h"

#include <iostream>

using namespace ecs;


void Entity::setupComponent( void* input){
    
    Component* component = (Component*) input;
    component->mEntityId = mEntityId;
    component->mManager = mManager;

}

void Entity::destroy(){
    mIsAlive = false;
    for( size_t i = 0; i < internal::getLastID(); i++ ){
        if(mComponentBitset[i] == true) {
            mComponentPool->mComponents[i]->onEntityDestroy( this );
        }
    }
    markRefresh();
}

void Entity::markRefresh(){
    mManager->needsRefresh = true;
}
