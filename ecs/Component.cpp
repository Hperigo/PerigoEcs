#include "Component.h"
#include "Entity.h"
#include "Manager.h"

namespace ecs {
    
    static ComponentID lastID{0};
    
    namespace internal{
        
        ComponentID getLastID(){
            return lastID;
        }
        
        ComponentID getUniqueComponentID(){
                return lastID++;
        }
        
    }

    Component::~Component(){}

    Entity* Component::getEntity(){
        return mManager->getEntity( mEntityId );
    }
}

