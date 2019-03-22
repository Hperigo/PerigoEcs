
#include "catch2/catch.hpp"
#include "Manager.h"

TEST_CASE( "1: Manager tests, create and delete entities" ) {
    
    ecs::ManagerRef mManager = ecs::Manager::create();
    ecs::EntityRef mEntity = mManager->createEntity();

    REQUIRE( mManager->mEntityPool.mEntities.size() == 1 );

    mEntity->destroy();

    REQUIRE( mManager->getEntities().size() == 1 );

    mManager->update();

    REQUIRE( mManager->mEntityPool.idPool.size() == 1 );
    REQUIRE(mManager->getEntities().size() == 0);
}
