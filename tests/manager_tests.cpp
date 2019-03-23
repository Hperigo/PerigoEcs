
#include "catch2/catch.hpp"
#include "Manager.h"

#include <iostream>
using namespace std;

auto mManager = ecs::Manager::create();

struct CustomEntity : public ecs::Entity{ 

    void setup() override{
        setName("custom");
    }

  ~CustomEntity(){ 
        cout << "CustomEntity deconstructor!" << endl;
    }    
};

struct Dummy : public ecs::Component {
    ~Dummy(){ 
        cout << "Dummy deconstructor!" << endl;
    }    
};

TEST_CASE( "1: Manager tests" ) {
    

    SECTION( "resizing bigger changes size and capacity" ) {

        {
            ecs::EntityRef mEntity = mManager->createEntity();
            mEntity->setName("test");

            {
                auto e = mManager->getEntities()[0];
                REQUIRE( e->getName() == "test" );
                REQUIRE( mManager->getEntities().size() == 1 );
            }
            mManager->update();
            REQUIRE( mManager->getEntities().size() == 1 );
        }

        mManager->update(); // actually delete the entity
 
        //entity lost it's scope, so we can't have anything else in the manager
        REQUIRE( mManager->getEntities().size() == 0 );

        //but we should have something in the poo
        cout << "idPool: " <<  mManager->mEntityPool.idPool.size() << endl;
        // REQUIRE( mManager->mEntityPool.idPool.size() == 1 );
    } 


    SECTION( "Same but with custom Entity" ) {
        {
            ecs::EntityRef mEntity = mManager->createEntity<CustomEntity>();
            {
                auto e = mManager->getEntities()[0];
                REQUIRE( e->getName() == "custom" );
                REQUIRE( mManager->getEntities().size() == 1 );
            }
            mManager->update();
            REQUIRE( mManager->getEntities().size() == 1 );
        }

        mManager->update(); // actually delete the entity

        //entity lost it's scope, so we can't have anything else in the manager
        REQUIRE( mManager->getEntities().size() == 0 );
    } 

}

    TEST_CASE( "Add and remove components" ) {

        SECTION( "destroy component when entity is destroyed" ){
            {
                ecs::EntityRef mEntity = mManager->createEntity();
                mEntity->setName("test");
                mEntity->addComponent<Dummy>();

                auto dummies = mManager->getComponentsArray<Dummy>();
                REQUIRE( dummies.size() == 1 );
            }

            // check if dummy was deleted!
            mManager->update();            
            {
                auto dummies = mManager->getComponentsArray<Dummy>();
                REQUIRE( dummies.size() == 0 );
            }
        }

        SECTION("Remove component"){

                ecs::EntityRef mEntity = mManager->createEntity<CustomEntity>();
                mEntity->setName("test");
                mEntity->addComponent<Dummy>();

                REQUIRE( mEntity->hasComponent<Dummy>() == true );
                
                mEntity->removeComponent<Dummy>();

                REQUIRE( mEntity->hasComponent<Dummy>() == false ); 

                {
                    auto dummies = mManager->getComponentsArray<Dummy>();
                    REQUIRE( dummies.size() == 0 );
                }
        }

}



