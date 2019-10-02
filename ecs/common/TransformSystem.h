//
//  Transform.h
//
//  Created by Henrique on 8/21/17.
//
//

#ifndef TransformSystem_h
#define TransformSystem_h

#include "ecs/Manager.h"
#include "Transform.h"
#include "Drawables.h"

struct TransformSystem : ecs::System{
    
    void update() override{
        
        auto transforms = getManager()->getComponentsArray<Transform>();
        for(auto& t : *transforms){
            // TODO: only update leaf nodes or do some type of cache
            if( t.parent == nullptr ){
                t.updateMatrices();
            }
        }
    }
};


#endif /* TransformSystem_h */
