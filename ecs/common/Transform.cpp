//
//  Transform.cpp
//  Created by Henrique on 10/10/17.
//

#include "Transform.h"
#include "ecs/Manager.h"

#include  <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "ofMain.h"

using namespace glm;
using namespace ecs;

Transform::Transform(){
}

Transform::~Transform(){

}


void Transform::onDestroy(){
    removeParent();
    clearChildren();
}

void Transform::clearChildren(){
    auto children = getChildren();
    for( auto& child : children ){
        child->getComponent<Transform>()->removeParent();
    }
}


void Transform::setParent( ecs::Entity* e ){

    if( parent != nullptr ){
        removeParent();
    }
    
    e->getComponent<Transform>()->children.push_back( getEntity() );
    parent = e; 
}


void Transform::removeParent(){
    
    if( parent == nullptr ){
        return;
    }
    
    auto parentTransform = parent->getComponent<Transform>();
    auto it = std::remove_if( parentTransform->children.begin(), parentTransform->children.end(), [&]( ecs::Entity* e ){
        return getEntity() == e;
    });
    
    parentTransform->children.erase(it);
    parent = nullptr;
}

void Transform::descendChildren( const std::function<void (ecs::Entity* entity)> &fn ){

    fn( getEntity() );
    for( auto& child : children ){
        child->getComponent<Transform>()->descendChildren( fn );
    }
    
}

std::vector<Entity*> Transform::getChildren(){
    return children;
}

glm::vec3 Transform::getRotation(){
    return rotation;
}
void Transform::setRotation( const glm::vec3& value ){
    rotation = value;
}

void Transform::setRotationZ( const float value ){
    rotation.z = value;
}

glm::quat Transform::getOrientation(){
    return glm::quat(glm::vec3(rotation.x, rotation.y, rotation.z));
}

void Transform::updateMatrices(){
    
    glm::mat4 transform;
    transform *= glm::translate<float>(mat4(), position + anchorPoint);
    transform *= glm::toMat4<float>( getOrientation() ); //glm::rotate(localRotation, vec3(0,0,1));
    transform *= glm::scale<float>(mat4(), scale );
    transform *= glm::translate<float>(mat4(), -anchorPoint );
    transformMatrix = transform;

    if( parent == nullptr ){
        worldTransformMatrix = transform;
    }

    for( auto& child : children ){
        
        auto transform = child->getComponent<Transform>();
        transform->updateMatrices();
        transform->worldTransformMatrix = worldTransformMatrix * transform->transformMatrix;
        
    }

}
