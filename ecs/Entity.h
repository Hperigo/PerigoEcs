//
// Created by Henrique on 5/23/18.
//

#ifndef LEKSAPP_ENTITY_H
#define LEKSAPP_ENTITY_H


#include <bitset>
#include <array>
#include <vector>
#include <functional>
#include <assert.h>
#include "Component.h"


#include <iostream>

namespace ecs{

    class Manager;
    class System;

    using EntityRef = Entity*;
//    using EntityWeakRef = std::weak_ptr<Entity>;
    class Entity {

    public:
        Entity() {  }
        virtual ~Entity(){ }

        bool isAlive() const { return mIsAlive; }
        virtual void destroy();
        virtual void drawUi(){};

        std::string getName(){ return mName; }
        void setName(const std::string& name ){ mName = name; }
        
        unsigned int getId() {
            return mEntityId;
        }

        template < typename T>
        bool hasComponent() const{
            return  hasComponentBitset( getComponentTypeID<T>() ) ;
        }

        bool hasComponentBitset( const size_t i ) const {
             return  mComponentBitset[ i ];
        }

        template <class T>
        T* addComponent(){
            
            assert(!hasComponent<T>() && "entity already has that component!");
            
            mComponentBitset[ getComponentTypeID<T>() ] = true;
            std::shared_ptr<ComponentContainer<T>> container = mComponentPool->getContainer<T>();
            T* comp = (T*)container->create(this);

            if ( std::is_base_of<Component, T>::value == true ){
                setupComponent((void*)comp);
            }
            
            return comp;
        }
        
        template <class T, typename... TArgs>
        T* addComponent(TArgs&&... _Args) {
            
            assert(!hasComponent<T>());
            
            std::shared_ptr<ComponentContainer<T>> container = mComponentPool->getContainer<T>();
            mComponentBitset[ getComponentTypeID<T>() ] = true;
            auto comp = container->create( this,  std::forward<TArgs>(_Args)...  );
            
            if( std::is_base_of<Component, T>::value == true ){
                setupComponent((void*)comp);
            }
            
            return comp;
        }
        
        template <class T>
        T* getComponent(){
            assert(hasComponent<T>() && "entity does not have that component!");
            auto container = mComponentPool->getContainer<T>();
            return container->get( this );
        }

        template <class T>
        void removeComponent(){
            assert(hasComponent<T>() && "Entity does not have that component");
            std::shared_ptr<ComponentContainer<T>> container = mComponentPool->getContainer<T>();
            mComponentBitset[ getComponentTypeID<T>() ] = false;
            container->remove( this );
        }
        
        inline std::bitset<MaxComponents> getComponentBitset(){ return mComponentBitset; }

        Manager* getManager() { return mManager; }
        
        void setOnLateSetupFn( const std::function<void()>& fn ){ onLateSetup = fn; }
        
        
    protected:
        
        void setupComponent(void* input);
        void markRefresh();

        Manager* mManager;
        ComponentPool* mComponentPool;
        
        bool mIsAlive{ true };
        
        std::bitset<MaxComponents> mComponentBitset;
        unsigned int mEntityId;
        
        std::string mName;
        friend class Manager;
        
        //use this to initialize components in the entity constructor
        std::function<void()> onLateSetup;
    };
}

#endif //LEKSAPP_ENTITY_H
