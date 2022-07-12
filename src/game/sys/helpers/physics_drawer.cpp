#include "physics_drawer.hpp"
#include <iostream>

Bullet3PhysicsDrawer_t::Bullet3PhysicsDrawer_t(std::size_t capacity, int pflags) { 
    lines.reserve(capacity);
    setDebugMode(pflags);
}

void Bullet3PhysicsDrawer_t::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)  { 
    // basically, the world->debugDraw will call this line multiple times and I will save the results in a SingletonComponent
    // I'm saving the line in the array to collect it later in the RenderSystem
    // lines.emplace_back(from, to);
    lines.emplace_back(from, to, color);
}

void Bullet3PhysicsDrawer_t::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) { 
    const btVector3 startPoint = PointOnB;
	const btVector3 endPoint = PointOnB + normalOnB * distance;
	drawLine(startPoint, endPoint, color);
}

void Bullet3PhysicsDrawer_t::reportErrorWarning(const char* warningString) { 
    std::cout << warningString << "\n";
}

void Bullet3PhysicsDrawer_t::draw3dText(const btVector3& location, const char* textString) { 
    // TODO: although will never use
}

void Bullet3PhysicsDrawer_t::setDebugMode(int debugMode) { 
    flags = debugMode;
}

int Bullet3PhysicsDrawer_t::getDebugMode(void) const { 
    return flags;
}

void Bullet3PhysicsDrawer_t::clearLines() {
    lines.clear();
}

const std::vector<DebugLine3D_t>& Bullet3PhysicsDrawer_t::getLinesVector() const {
    return lines;
}

