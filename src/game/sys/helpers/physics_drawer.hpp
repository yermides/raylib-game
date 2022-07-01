#pragma once
#include <vector>
#include <bullet3/LinearMath/btIDebugDraw.h> 
#include "game/cmp/helpers/line.hpp"

// struct PhysicsDebugLine_t {
//     explicit PhysicsDebugLine_t(const btVector3& p_from, const btVector3& p_to) 
//     : from(p_from), to(p_to)
//     {}

//     Vector3f_t from {}, to {};
// };

struct Bullet3PhysicsDrawer_t final : public btIDebugDraw {
    explicit Bullet3PhysicsDrawer_t(std::size_t capacity = 2048, int pflags = -1);

    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) final;
    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) final;
    void reportErrorWarning(const char* warningString) final;
    void draw3dText(const btVector3& location, const char* textString) final;
    void setDebugMode(int debugMode) final;
    int getDebugMode(void) const final;

    // clear DOES get called inside world->DebugDrawWorld, no need for explicit call
    void clearLines() final;
    const std::vector<DebugLine3D_t>& getLinesVector() const;
private:
    std::vector<DebugLine3D_t> lines {};
    int flags {};
};
