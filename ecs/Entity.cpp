//
// Created by Henrique on 5/24/18.
//

#include "Entity.h"
#include "Manager.h"

#include <iostream>

using namespace ecs;


void Entity::setupComponent( void* input){

//    mManager->addComponent(getId(), cId, component );
    
    Component* component = (Component*) input;
    component->mEntity = this;
    component->mManager = mManager;
}

void Entity::destroy(){
    mIsAlive = false;
    markRefresh();
}

void Entity::markRefresh(){
    mManager->needsRefresh = true;
}
