#ifndef ANIMATION_H
#define ANIMATION_H

#include "core/x-platform/keyframe.h"
#include "core/x-platform/locator.h"
#include "core/x-platform/url.h"

#include <glm/gtx/transform.hpp>
#include <vector>

#include <cstring>
#include "3rdparty/cgltf.h"

#define QUAT_EPSILON 0.000001f

class Animation
{
private:
    enum Interpolation
    {
        Constant,
        Linear,
        Cubic
    };

    float length;
    Array<KeyFrame*> keyframes; // Alternatively Map<String, glm::mat4>
    int GetNodeIndex(cgltf_node *target, cgltf_node *allNodes, unsigned int numNodes);
    template<typename T, int N>
    void TrackFromChannel(Array<KeyFrame*> &keyframes, const cgltf_animation_channel& inChannel, int nodeId);
    void GetScalarValues(std::vector<float> &outScalars, unsigned int inComponentCount, const cgltf_accessor &inAccessor);

    void ApplyTransform(Joint *joint, glm::mat4 parentTransform);
    String name;

    //void SkinningCPU();

public:
    Animation();
    ~Animation();
    Animation(const URL filename);
    void Update(Joint *rootJoint);

    float animationTime;
    glm::mat4 transforms[MAX_JOINTS];
    glm::mat4 transformsSkeleton[MAX_JOINTS];
};

#endif // ANIMATION_H
