//
//  Manager.cpp
//  EcsCereal
//
//  Created by Henrique on 6/21/18.
//

#include <map>
#include "Manager.h"

using namespace ecs;
//std::map< std::string , std::shared_ptr<ecs::internal::ComponentFactoryInterface>> Manager::typeFactory = std::map< std::string, std::shared_ptr<ecs::internal::ComponentFactoryInterface > >();

//bool Manager::EntityPool::fetchId(uint64_t* outputID){
//
//    if( idPool.size() > 0 ){
//        *outputID = idPool.front();
//        idPool.pop();
//        return true;
//    }
//    return false;
//}

// Manager::EntityPool Manager::EntityPool::duplicate(){
    
//     // Manager::EntityPool returnPool;
    
//     //     cout << "copied!" << endl;
//     //     returnPool.mEntities.reserve(mEntities.size());
        
//     //     for( auto& entity : mEntities){
            
//     //         EntityRef e;
//     //         entity->getFactory()->copyInto(entity.get(), e);
            
//     //         returnPool.mEntities.push_back( e );
//     //         cout << "---------" << endl;
//     //         cout << "e : " << e->getId() << endl;
            
//     //         for(size_t i = 0; i < e->mComponentBitset.size(); ++i){
                
//     //             if(  e->mComponentBitset[i] == true ){
                    
//     //                 cout << "c : " << i << endl;
                    
//     //                 auto sourceComponent = mComponents[i][entity->getId()].get();
//     //                 ComponentRef targetComponent;
//     //                 auto factory = sourceComponent->getFactory();
                    
//     //                 factory->copyInto( sourceComponent, targetComponent );
//     //                 targetComponent->mEntity = e.get();
                    
//     //                 auto entityId = e->getId();
//     //                 auto id = i;
                    
//     //                 // Todo add addComponent function to entityPool ----
//     //                 auto& componentVector = returnPool.mComponents[id];
                    
//     //                 if( entityId >= componentVector.size() ){
//     //                     returnPool.resizeComponentVector();
//     //                     componentVector[entityId] = targetComponent;
//     //                 }else{
//     //                     componentVector[entityId] = targetComponent;
//     //                 }
//     //             }
//     //         }
//     //     }
//     //     return returnPool;
// }

// void Manager::EntityPool::setPool(const EntityPool &otherPool){
// //         for( int i = 0; i < otherPool.mComponents.size(); i++ ){
            
// //             mComponents[i].clear();
            
// //             for( int j = 0; j < otherPool.mComponents[i].size(); j++ ){
                
// //                 cout << "---j: " << j << endl;

// //                 ComponentRef sourceComponent = otherPool.mComponents[i][j];
// //                 ComponentRef targetComponent;
                
// //                 sourceComponent->getFactory()->copyInto( sourceComponent.get(), targetComponent );
// // //        /        mEntities[j]->mComponentArray[i] = otherPool.mComponents[i][j].get();
                
// //                 mComponents[i].push_back(targetComponent);
                
// //                 cout << " --- " << endl;
                
// //             }
// //         }
    

//     // TODO: maybe we also need to deep copy the entities here?
//     // for( auto& sourceEntity : otherPool.mEntities ){
        
//     //     EntityRef e;
//     //     sourceEntity->getFactory()->copyInto(sourceEntity.get(), e);
//     //     for( auto& c : mComponents[e->getId()] ){
//     //         if( c != nullptr  ){
//     //             c->mEntity = e.get();
//     //         }
//     //         mEntities[e->getId()] = e;
//     //     }
//     // }
// }


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
            components->remove(e.get());
        }
    }
    
    e.reset();

}

// prints a csv with the components in a row, and entities as collumns
void Manager::printCheck(){
    // print entities id's
//    cout << "enti,";
//    for( int i = 0; i < mEntities.size(); i++){
//
//        if( mEntities[i] == nullptr ){
//            cout << "*" << "," ;
//        }else{
//            cout << i << ",";
//        }
//    }
//    cout << endl;
//
//    // print valid components
//    for( int i = 0; i < 10; i++){
//        cout << "id: " << i << ",";
//
//        for( int j = 0; j < mComponents[i].size(); j++){
//            auto c = mEntityPool.mComponents[i][j];
//            bool valid = c != nullptr;
//            cout << valid << ",";
//        }
//        cout << "_" << endl;
//    }
//}

}
