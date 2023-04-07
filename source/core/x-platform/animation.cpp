#include "animation.h"

Animation::Animation()
{
}

Animation::~Animation()
{
}

/*int Animation::GetNodeIndex(cgltf_node *target, cgltf_node *allNodes, unsigned int numNodes)
{
    if (target == 0)
    {
        return -1;
    }

    for (unsigned int i = 0; i < numNodes; i++)
    {
        if (target == &allNodes[i])
        {
            return int(i);
        }
    }

    return -1;
}

void Animation::GetScalarValues(std::vector<float> &outScalars, unsigned int inComponentCount, const cgltf_accessor &inAccessor)
{
    outScalars.resize(inAccessor.count * inComponentCount);

    for (cgltf_size i = 0; i < inAccessor.count; i++)
    {
        cgltf_accessor_read_float(&inAccessor, i, &outScalars[i * inComponentCount], inComponentCount);
    }
}

glm::quat Normalize(glm::quat q)
{
    float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;

    if (lenSq < QUAT_EPSILON)
    {
        return glm::quat();
    }

    float i_len = 1.0f / sqrtf(lenSq);

    return glm::quat(
                q.w * i_len,
                q.x * i_len,
                q.y * i_len,
                q.z * i_len
                );

    return q;
}

template<typename T, int N>
void Animation::TrackFromChannel(Array<KeyFrame*> &keyframes, const cgltf_animation_channel &inChannel, int nodeId)
{
    cgltf_animation_sampler &sampler = *inChannel.sampler;

    int interpolation = Interpolation::Constant;

    if (inChannel.sampler->interpolation == cgltf_interpolation_type_linear)
    {
        interpolation = Interpolation::Linear;
    }
    else if (inChannel.sampler->interpolation == cgltf_interpolation_type_cubic_spline)
    {
        interpolation = Interpolation::Cubic;
    }

    bool isSamplerCubic = interpolation == Interpolation::Cubic;

    std::vector<float> timelineFloats;
    GetScalarValues(timelineFloats, 1, *sampler.input);

    std::vector<float> valueFloats;
    GetScalarValues(valueFloats, N, *sampler.output);

    unsigned int numFrames = sampler.input->count;

    if (keyframes.Size() != numFrames)
    {
        for (unsigned int i = 0; i < numFrames; i++)
        {
            keyframes.Add(NULL);
        }
    }

    unsigned int numberOfValuesPerFrame = valueFloats.size() / timelineFloats.size();

    for (unsigned int i = 0; i < numFrames; i++)
    {
        if (keyframes[i] == NULL)
        {
            keyframes[i] = new KeyFrame(interpolation, timelineFloats[i]); // ERROR: Each channel can have unique interpolation, probably...
        }

        int baseIndex = i * numberOfValuesPerFrame;
        int offset = 0;

        float in_keyframeTimes[N];
        float out_keyframeValues[N];

        float values[N];

        for (int component = 0; component < N; component++)
        {
            in_keyframeTimes[component] = isSamplerCubic ? valueFloats[baseIndex + offset++] : 0.0f;
        }

        for (int component = 0; component < N; component++)
        {
            values[component] = valueFloats[baseIndex + offset++];
        }

        for (int component = 0; component < N; component++)
        {
            out_keyframeValues[component] = isSamplerCubic ? valueFloats[baseIndex + offset++] : 0.0f;
        }

        if (inChannel.target_path == cgltf_animation_path_type_translation)
        {
            keyframes[i]->transforms[nodeId].position = glm::vec3(values[0], values[1], values[2]);
        }
        else if (inChannel.target_path == cgltf_animation_path_type_rotation)
        {
            float x = values[0];
            float y = values[1];
            float z = values[2];
            float w = values[3];

            glm::quat rotation(w, x, y, z);
            rotation = Normalize(rotation);

            keyframes[i]->transforms[nodeId].rotation = rotation;
        }
        else if (inChannel.target_path == cgltf_animation_path_type_scale)
        {
            keyframes[i]->transforms[nodeId].scale = glm::vec3(values[0], values[1], values[2]);
        }
    }
}*/

Animation::Animation(const URL filename)
{
    animationTime = 0.0f;

    /*cgltf_options options;
    memset(&options, 0, sizeof(cgltf_options));
    cgltf_data *data = NULL;
    cgltf_result result = cgltf_parse_file(&options, filename.ToChar(), &data);

    if (result != cgltf_result_success)
    {
        LogError("Failed to load animation: " + filename);
    }

    result = cgltf_load_buffers(&options, data, filename.ToChar());

    if (result != cgltf_result_success)
    {
        cgltf_free(data);
        LogError("Failed to load buffers for mesh: " + filename);
    }

    result = cgltf_validate(data);

    if (result != cgltf_result_success)
    {
        cgltf_free(data);
        LogError("Invalid gltf file: " + filename);
    }

    unsigned int animationQuantity = data->animations_count;
    unsigned int nodeQuantity = data->nodes_count;

    if (animationQuantity != 1)
    {
        cgltf_free(data);
        LogError("Animation does not have excactly one animation: " + filename);
    }

    for (unsigned int i = 0; i < animationQuantity; i++)
    {
        unsigned int numChannels = data->animations[i].channels_count;
        name = data->animations[i].name;

        for (unsigned int j = 0; j < numChannels; j++)
        {
            cgltf_animation_channel &channel = data->animations[i].channels[j];
            cgltf_node *target = channel.target_node;

            int nodeId = GetNodeIndex(target, data->nodes, nodeQuantity);

            if (channel.target_path == cgltf_animation_path_type_translation)
            {
                TrackFromChannel<glm::vec3, 3>(keyframes, channel, nodeId);
            }
            else if (channel.target_path == cgltf_animation_path_type_scale)
            {
                TrackFromChannel<glm::vec3, 3>(keyframes, channel, nodeId);
            }
            else if (channel.target_path == cgltf_animation_path_type_rotation)
            {
                TrackFromChannel<glm::mat4, 4>(keyframes, channel, nodeId);
            }
        }

        //result[i].RecalculateDuration();
    }

    for (unsigned int i = 0; i < keyframes.Size(); i++)
    {
        keyframes[i]->CalculateTransform();
    }

    cgltf_free(data);*/
}

/*void Animation::SkinningCPU(
  //Skeleton& skeleton, Pose& pose
)
{
    // Move the vertices on the CPU
    //unsigned int numVerts = mPosition.size();
    //if (numVerts == 0){
    //    return;
    //}

    //mSkinnedPosition.resize(numVerts);
    //mSkinnedNormal.resize(numVerts);

    //pose.GetMatrixPalette(mPosePalette);

    //std::vector<mat4> invPosePalette = skeleton.GetInvBindPose();

    //for (unsigned int i = 0; i < numVerts; ++i)
    //{
    //    ivec4& j = mInfluences[i];
    //    vec4& w = mWeights[i];

    //    mat4 m0 = (mPosePalette[j.x] * invPosePalette[j.x]) * w.x;
    //    mat4 m1 = (mPosePalette[j.y] * invPosePalette[j.y]) * w.y;
    //    mat4 m2 = (mPosePalette[j.z] * invPosePalette[j.z]) * w.z;
    //    mat4 m3 = (mPosePalette[j.w] * invPosePalette[j.w]) * w.w;

    //    mat4 skin = m0 + m1 + m2 + m3;

    //    mSkinnedPosition[i] = transformPoint(skin, mPosition[i]);
    //    mSkinnedNormal[i] = transformVector(skin, mNormal[i]);
    //}
    for (unsigned int i = 0; i < MAX_JOINTS; i++)
    {
        glm::mat4 transform(1.0f);

        transform += glm::mat4(0.0f);
        transform += glm::mat4(0.0f);
        transform += glm::mat4(0.0f);
        transform += glm::mat4(0.0f);

        transforms[i] = transform;
    }
}*/

void Animation::ApplyTransform(Joint *joint, glm::mat4 parentTransform)
{
    glm::mat4 animatedTransform = joint->worldBindTransform;

    if (keyframes.Size() > 0)
    {
        int keyframe = int(animationTime) % keyframes.Size();
        glm::mat4 animation = keyframes[keyframe]->transforms[joint->id].matrix;

        animatedTransform = parentTransform * animation;
    }

    for (unsigned int i = 0; i < joint->children.Size(); i++)
    {
        ApplyTransform(joint->children[i], animatedTransform);
    }

    transforms[joint->id] = animatedTransform * joint->inverseBindTransform;
    transformsSkeleton[joint->id] = animatedTransform;
}

void Animation::Update(Joint *rootJoint)
{
    if (rootJoint != NULL)
    {
        ApplyTransform(rootJoint, rootJoint->worldBindTransform);

        animationTime += 0.02f * Locator::time->TimeSinceStarted();
    }
}
