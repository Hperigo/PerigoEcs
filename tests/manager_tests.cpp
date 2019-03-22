
#include "catch2/catch.hpp"
#include "Manager.h"

#include <iostream>
using namespace std;
TEST_CASE( "1: Manager tests, create and delete entities" ) {
    
    auto mManager = ecs::Manager::create();
    
    
    {
        ecs::EntityRef mEntity = mManager->createEntity();
        cout << "entity is alive!" <<  mEntity->getName() << endl;
    }
    
    mManager->update();
    
    REQUIRE( mManager->getEntities().size() == 1 );

    // REQUIRE( mManager->mEntityPool.idPool.size() == 1 );
    // REQUIRE(mManager->getEntities().size() == 0);
}

