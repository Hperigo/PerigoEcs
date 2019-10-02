//
//  Transform.hpp
//  EcsTest
//
//  Created by Henrique on 11/30/18.
//

#ifndef Transform_hpp
#define Transform_hpp

#include "ecs/Component.h"
#include "ecs/System.h"

#include <functional>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/mat4x2.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Entity.h"

class Transform : public ecs::Component {
    
public:
    Transform();
    ~Transform();
    void onDestroy() override;
    
    void clearChildren();
    
    void updateMatrices();

    void setParent( ecs::Entity* e );
    void removeParent();
    
    std::vector<ecs::Entity*> getChildren();
    void descendChildren( const std::function<void (ecs::Entity* parent)> &fn);
    
    //rotation
    glm::quat getOrientation();
    glm::vec3 getRotation();
    void setRotation( const glm::vec3& value );
    void setRotationZ( const float value );
    float getRotationZ(){ rotation.z; };
    
    glm::vec3 anchorPoint;
    glm::vec3 position;
    glm::vec3 scale { 1.0f };
    glm::vec3 rotation;

    glm::mat4 transformMatrix;
    glm::mat4 worldTransformMatrix;
    
    std::vector<ecs::Entity*> children;
    ecs::Entity* parent = nullptr;
};

#endif /* Transform_hpp */
