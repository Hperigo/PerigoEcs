//
// Created by Henrique on 5/24/18.
//

#ifndef LEKSAPP_MANAGER_H
#define LEKSAPP_MANAGER_H


#include "Entity.h"
#include "System.h"

#include <vector>
#include <queue>


#include <iostream>
using namespace std;

namespace  ecs{
    using ManagerRef = std::shared_ptr<class Manager>;

class Manager {

public:
    Manager(){
    
    }
    
    ~Manager(){ }

    template<typename... Args>
    static ManagerRef create(Args&&... args){
        return std::make_shared<Manager>( std::forward<Args>(args)...  );
    }

    EntityRef createEntity(){
        Entity* e = new ecs::Entity();
        auto sharedEntity = std::shared_ptr<ecs::Entity >( e );
        setupEntity(sharedEntity);
        return sharedEntity;
    }
    
    
    ScopedEntity<Entity> createScopedEntity() {
        return ScopedEntity<Entity>(createEntity());
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> createEntity(Args&&... args){
        
        T* e = new T(std::forward<Args>(args)... );
        std::shared_ptr<T> sharedEntity = std::shared_ptr<T>( e );
        
        setupEntity(sharedEntity);
        return sharedEntity;
    }

    template<typename T, typename... Args>
    ScopedEntity<T> createScopedEntity(Args&&... args){
        return ScopedEntity<T>( createEntity<T>( std::forward<Args>( args ) ...  ) );
    }

    
    template<typename T, typename... TArgs>
    std::shared_ptr<T> createSystem(TArgs&&... _Args) {

        std::shared_ptr<T> rawSystem( new T(std::forward<TArgs>(_Args)... ));

        SystemRef systemPtr{ rawSystem };
        rawSystem->mManager = this;
        
        mSystems.push_back( rawSystem );
        
        if( isManagerInitialized == true ){
            rawSystem->setup();
        }
        
        return  rawSystem;
    }

    void removeSystem(SystemRef iSystem){
        
        auto sys = std::find( mSystems.begin(), mSystems.end(), iSystem );

        if( sys != mSystems.end() ){
            mSystems.erase(sys);
        }
    }

    void setup();
    void update();
    void draw();
    
    void refresh();

    void addComponent(uint64_t entityId, ComponentID id, const ComponentRef component);


    
//    template <class T,
//    typename std::enable_if< std::is_base_of<ecs::Component, T>::value, T>::type* = nullptr>
//    const std::vector<T*>& getComponentsArray() {
//
//        if( needsRefresh ){
//            refresh();
//        }
//
//        auto _id = getComponentTypeID<T>();
//        return  (std::vector<T*>&) mComponentsByType[_id];
//    }
    
//    template <class T,
//    typename std::enable_if< ! std::is_base_of<ecs::Component, T>::value, T>::type* = nullptr>
//    const std::vector< WrapperComponent<T>* >& getComponentsArray() {
//
//        if( needsRefresh ){
//            refresh();
//        }
//
//        auto _id = getComponentTypeID< WrapperComponent<T> >();
//        return  (std::vector< WrapperComponent<T>* >&) mComponentsByType[_id];
//    }
//

    template <typename T>
    std::vector<T>* getComponentsArray(){
        auto container = mComponents.getContainer<T>();
        return container->getDataPtr();
    }
    
    template<class T>
    void setBitset(std::bitset<MaxComponents>* bitset, T head)const {
        bitset->set( head, 1 );
    }
    
    
    template <class T, class ...Args>
    void  setBitset(std::bitset<MaxComponents>* bitset, T head ,  Args ... args) const {
        setBitset( bitset, head );
        setBitset( bitset, args ... );
    }
    
    template <class ...Args>
    std::vector<std::shared_ptr<Entity>> getEntitiesWithComponents() const {
        
        std::bitset<MaxComponents> bitsetMask;
        setBitset( &bitsetMask, getComponentTypeID<Args>()... );
        std::vector<std::shared_ptr<Entity>> entities;
        for( auto &e : mEntities ){
            
            if(auto shared = e ){
                bool b = ( shared->getComponentBitset() | bitsetMask  ) == shared->getComponentBitset(); // check if entity has all the bits in the bitset mask
                if( b ){
                    entities.push_back( shared );
                }
            }
         }
        
        return entities;
    };

    
    // EntityRef copyEntity( const EntityRef& iEntity ){
    //     return copyEntity(iEntity.get());
    // }
    
    
    std::vector<EntityRef> getEntities() {
        
//        std::vector<EntityRef> output;
//        for( auto &e : mEntities ){
//
//            if( auto shared = e ){
//                output.push_back(shared);
//            }
//        }
//
//        return output;
        return mEntities;
    }
    std::vector<SystemRef>& getSystems() { return mSystems; }
    
    
    void printCheck();

    
protected:
    
    //  ---- general manager vars -------
    std::vector<SystemRef> mSystems;
    
    
    std::vector< EntityRef > mEntities;
    ComponentPool mComponents;
    
    bool needsRefresh{false};
    bool isManagerInitialized = false;

    unsigned int entitiesCreated = 0;
    // setup entity after it's creation
    void setupEntity(const EntityRef& e ){

        e->mEntityId = entitiesCreated;
        mEntities.push_back( e );

        e->mManager = this;
        e->mComponentPool = &mComponents;

        e->setup();
        
        if( e->onLateSetup ){
            e->onLateSetup();
        }
        
        
        entitiesCreated += 1;
    }
    void entityDeleter( ecs::EntityRef e );

    friend class Entity;
};
}
#endif //LEKSAPP_MANAGER_H
