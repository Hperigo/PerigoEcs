//
// Created by Henrique on 5/23/18.
//

#include <memory>
#include <bitset>
#include <iostream>       // std::cout

#ifndef ECSSAPP_COMPONENT_H
#define ECSSAPP_COMPONENT_H

#ifndef ECS_MAX_COMPONENTS
    #define ECS_MAX_COMPONENTS 200
#endif


#include <array>
#include <unordered_map>

using namespace std;

namespace ecs{

    // forward decls...
    struct Component;

    class Entity;
    class Manager;

    using ComponentID = std::size_t;
    using EntityID = std::size_t;
    
    using ComponentRef = std::shared_ptr<Component>;
    constexpr std::size_t MaxComponents{ECS_MAX_COMPONENTS};
    using ComponentBitset = std::bitset<MaxComponents>;
    
    namespace internal{

        //@TODO: maybe move this to std:: typeinfo?
        
        ComponentID getLastID();
        ComponentID getUniqueComponentID();
            
        template <typename T>
        ComponentID getComponentTypeID() noexcept {
            
            static ComponentID typeID { internal::getUniqueComponentID() };
//            std::cout << typeid(T).name() << "  " << typeID << std::endl;
            return typeID;
            
        }
    }

    struct Component {

    public:
        virtual ~Component();
        virtual bool drawUi() { return false; };
        virtual void onDestroy(){ };

        Entity* getEntity(){ return mEntity; }
        Entity* getEntity() const { return mEntity; }
        Manager* getManager(){ return mManager; }

    protected:
            
        Entity* mEntity;
        uint64_t mEntityId;
        Manager* mManager;
    
        std::size_t mComponentId;

        friend class Entity;
        friend class Manager;
    };

    template <class T>
    inline ComponentID getComponentTypeID(){
            return internal::getComponentTypeID<T>();
    }

    
    
    class ComponentContainerBase {
    public:
        virtual ~ComponentContainerBase(){ };
        virtual void remove( Entity* e ) = 0;

//       we might not need theese
//        virtual Component* get( Entity* e ) = 0;
//        virtual Component* create(Entity* e) = 0;
    };
    typedef std::shared_ptr<ComponentContainerBase> ComponentContainerBaseRef;
    
    template <typename T>
    class ComponentContainer : public ComponentContainerBase {
    public:
        
        ~ComponentContainer(){};
        
        T* get( Entity* e ) {
            size_t index = entityKeys[e];
            return &data[index];
        }
        
        void remove( Entity* e ) override {

            if( data.size() == 0 ){
                return;
            }
            
            auto it = entityKeys.find(e);
            if( it == entityKeys.end() ){
                return;
            }
        
            //TODO: we can make this constexpr
             if( std::is_base_of<Component, T>::value == true ){
                
                T* typePointer = &data[it->second];
                Component* component = (Component*) typePointer;
                component->onDestroy();
            }

            for( auto& k : entityKeys ){
                
                if( k.second > it->second && k.second > 0 ){
                    k.second = k.second - 1;
                }
            }
            data.erase( data.begin() + it->second );
            entityKeys.erase( it );

        }
        
        template<typename... Args>
        T* create(Entity* e, Args&&... args){
            int index = data.size();
            entityKeys[e] = index;
            data.emplace_back( std::forward<Args>(args)... );

            return &data[index];
        }
        
        T* create(Entity* e) {
            
            size_t index = data.size();
            data.emplace_back();
            entityKeys[e] = index;
            
            return &data[index];
        }

        std::vector<T>* getDataPtr(){
            return &data;
        }
        
    private:
        std::vector<T> data;
        std::unordered_map< Entity* , size_t> entityKeys;
    };

    struct ComponentPool {
        std::array< ComponentContainerBaseRef, MaxComponents> mComponents;
        
        //get a component with the cointainer key
        template <typename T>
        std::shared_ptr< ComponentContainer<T> > getContainer(){
            std::size_t componentId = getComponentTypeID<T>();

            if( mComponents[componentId] == nullptr ){
                mComponents[componentId] = std::make_shared< ComponentContainer<T>>();
            }
            
            return std::static_pointer_cast< ComponentContainer<T> >( mComponents[componentId] );
        }
    };
    
}//end of namespace



#endif //ECSSAPP_COMPONENT_H
