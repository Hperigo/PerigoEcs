
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
    }
};

struct Dummy : public ecs::Component {
    ~Dummy(){ 
    }

	float value;
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

            mEntity->destroy();
        }

        mManager->update(); // actually delete the entity
 
        //entity lost it's scope, so we can't have anything else in the manager
        REQUIRE( mManager->getEntities().size() == 0 );

        //but we should have some available ID in the pool
        REQUIRE( mManager->mEntityPool.idPool.size() == 1 );

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
            mEntity->destroy();
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
                mEntity->addComponent<Dummy>();

                auto dummies = mManager->getComponentsArray<Dummy>();
                REQUIRE( dummies.size() == 1 );

                mEntity->destroy();
            }
            // check if dummy was deleted!

            mManager->update();

            {
                auto dummies = mManager->getComponentsArray<Dummy>();
                REQUIRE( dummies.size() == 0 );
            }
        }
	

		SECTION("Create multiple entities"){


                ecs::EntityRef A = mManager->createEntity<CustomEntity>();
                A->setName("A");
                auto a = A->addComponent<Dummy>();
				a->value = 100;

				mManager->update();

                ecs::EntityRef B = mManager->createEntity<CustomEntity>();
                B->setName("B");
                auto b = B->addComponent<Dummy>();
				b->value = 200;

				
				mManager->update();


				ecs::EntityRef C = mManager->createEntity<CustomEntity>();
                C->setName("C");
                auto c = C->addComponent<Dummy>();
				c->value = 300;

				mManager->update();

				REQUIRE( A->getComponent<Dummy>()->value == 100 );  
				REQUIRE( B->getComponent<Dummy>()->value == 200 );
				REQUIRE( C->getComponent<Dummy>()->value == 300 );
		
				B->removeComponent<Dummy>();
				mManager->update();

				REQUIRE( A->getComponent<Dummy>()->value == 100 );
				REQUIRE( B->hasComponent<Dummy>() == false );
				REQUIRE( C->getComponent<Dummy>()->value == 300 );


				A->destroy();
				B->destroy();
				C->destroy();

				mManager->update();
		}




        SECTION("Remove component"){

                ecs::EntityRef mEntity = mManager->createEntity<CustomEntity>();
                mEntity->setName("test");
                mEntity->addComponent<Dummy>();

                REQUIRE( mEntity->hasComponent<Dummy>() == true );
                REQUIRE( mManager->getEntitiesWithComponents<Dummy>().size() == 1 );
                
                mEntity->removeComponent<Dummy>();

                REQUIRE( mEntity->hasComponent<Dummy>() == false ); 

                {
                    auto dummies = mManager->getComponentsArray<Dummy>();
                    REQUIRE( dummies.size() == 0 );
                    
                    REQUIRE( mManager->getEntitiesWithComponents<std::string>().size() == 0 );
                }
        }


    SECTION( "Wrapper components" ){

        {           
            ecs::EntityRef mEntity = mManager->createEntity<CustomEntity>();
            mEntity->addComponent<std::string>( "string!" );

            REQUIRE( mEntity->hasComponent<std::string>() == true );

            REQUIRE( mManager->getEntitiesWithComponents<std::string>().size() == 1 );

            mEntity->removeComponent<std::string>();
            REQUIRE( mEntity->hasComponent<std::string>() == false ); 
        }

        {
            auto strings = mManager->getComponentsArray<std::string>();
            REQUIRE( strings.size() == 0 );

            REQUIRE( mManager->getEntitiesWithComponents<std::string>().size() == 0 );
        }
    }

      SECTION( "Scoped Entities" ){

        {           
            auto mEntity = mManager->createScopedEntity<CustomEntity>();
            mEntity->addComponent<std::string>( "string!" );

            REQUIRE( mEntity->hasComponent<std::string>() == true );

            REQUIRE( mManager->getEntitiesWithComponents<std::string>().size() == 1 );

            mEntity->removeComponent<std::string>();
            REQUIRE( mEntity->hasComponent<std::string>() == false ); 
        }

        {
            auto strings = mManager->getComponentsArray<std::string>();
            REQUIRE( strings.size() == 0 );

            REQUIRE( mManager->getEntitiesWithComponents<std::string>().size() == 0 );
        }
    
    }
}


TEST_CASE("Benchmarks"){

		// now let's benchmark:
    BENCHMARK("Normal Entity") {
		ecs::EntityRef mEntity = mManager->createEntity();
    };
	
	BENCHMARK("Scoped Entity") {
       auto mEntity = mManager->createScopedEntity();
    };

	BENCHMARK("Add Component to Entity") {
       auto mEntity = mManager->createEntity<CustomEntity>();
	   mEntity->addComponent<Dummy>();
    };

	BENCHMARK("Remove Component to Entity") {
       auto mEntity = mManager->createEntity<CustomEntity>();
	   mEntity->removeComponent<Dummy>();
    };
}
