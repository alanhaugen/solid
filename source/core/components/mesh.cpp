#include "mesh.h"
#include <core/application.h>

#include <core/x-platform/locator.h>
#include <core/x-platform/typedefs.h>

#include <core/x-platform/parserjson.h>

#define FAST_OBJ_IMPLEMENTATION	
#include "3rdparty/fast_obj.h"

//#define CGLTF_IMPLEMENTATION
//#include "3rdparty/cgltf.h"

static void addQuadIndices( Array<unsigned int> &indices, int i0, int i1, int i2, int i3 )	
{	
    indices.Add(i0); indices.Add(i1); indices.Add(i2);	
    indices.Add(i2); indices.Add(i3); indices.Add(i0);	
}	

Mesh::Mesh(const String& type, glm::vec4 vertexColor /*so we dont conflict with ctor that loads from file*/)	
{	
    assert( type == "cube" );	
    //Log(String("Create mesh of type " + type));	
    Array<IDrawable::Vertex> vertices;	
    Array<unsigned int> indices;	
    Array<String> shaders(2);	

    IFile *simpleVertShader = filesystem->Open(URL("data/simple.vert"), PLAIN_TEXT);
    IFile *simpleFragShader = filesystem->Open(URL("data/simple.frag"), PLAIN_TEXT);

    shaders.Insert(simpleVertShader->Read(), VERTEX_SHADER);
    shaders.Insert(simpleFragShader->Read(), FRAGMENT_SHADER);

    delete simpleVertShader;
    delete simpleFragShader;

    // front face	
    // v0 ---- v1	
    //  |      |	
    //  |      |	
    // v3-----v2	
    // back face	
    // v4 ---- v5	
    //  |      |	
    //  |      |	
    // v7-----v6	

    typedef IDrawable::Vertex Vertex;	

    Vertex v0( .5f*glm::vec3(-1, +1, -1), vertexColor);	
    Vertex v1( .5f*glm::vec3(+1, +1, -1), vertexColor);	
    Vertex v2( .5f*glm::vec3(+1, -1, -1), vertexColor);	
    Vertex v3( .5f*glm::vec3(-1, -1, -1), vertexColor);	
    Vertex v4( .5f*glm::vec3(-1, +1, +1), vertexColor);	
    Vertex v5( .5f*glm::vec3(+1, +1, +1), vertexColor);	
    Vertex v6( .5f*glm::vec3(+1, -1, +1), vertexColor);	
    Vertex v7( .5f*glm::vec3(-1, -1, +1), vertexColor);	

    vertices.Add(v0);	
    vertices.Add(v1);	
    vertices.Add(v2);	
    vertices.Add(v3);	
    vertices.Add(v4);	
    vertices.Add(v5);	
    vertices.Add(v6);	
    vertices.Add(v7);	

    addQuadIndices(indices, 0,1,2,3); // front	
    addQuadIndices(indices, 4,7,6,5); // back	
    addQuadIndices(indices, 4,0,3,7); // left	
    addQuadIndices(indices, 1,5,6,2); // right	
    addQuadIndices(indices, 0,4,5,1); // top	
    addQuadIndices(indices, 3,2,6,7); // bottom	

    drawables.Add(Locator::renderer->CreateDrawable(vertices, indices, shaders, NULL));
}

// For animation, consider https://www.youtube.com/watch?v=z0jb1OBw45I

Mesh::Mesh(Array<IDrawable::Vertex> &vertices, Array<unsigned int> &indices, Actor *actor)
{
    (void)vertices;
    (void)indices;
}

Mesh::Mesh()
{
    //emptyAnimation = NULL;
    rootJoint = NULL;
}

Mesh::~Mesh()
{
    //delete emptyAnimation;

    // TODO: Think about and fix
    for (unsigned int i = 0; i < drawables.Size(); i++)
    {
        IDrawable *drawable = drawables[i];
        //renderer->RemoveDrawable(drawable);
        // TEMP: hack
        drawable->visible = false;
    }

    // TODO: delete textures
    // TODO: Recursively delete joints. ...
}

Mesh::Mesh(const Mesh& other)
{
    (void)other;
    assert(0); // TODO get other meshes drawable and increase ref count to it? or find a different solution to instancing
}

/*
Mat LoadLocalTransform(cgltf_node *node)
{
    Mat result;

    if (node->has_matrix)
    {
        result = Mat( glm::mat4(
                node->matrix[0],
                node->matrix[1],
                node->matrix[2],
                node->matrix[3],
                node->matrix[4],
                node->matrix[5],
                node->matrix[6],
                node->matrix[7],
                node->matrix[8],
                node->matrix[9],
                node->matrix[10],
                node->matrix[11],
                node->matrix[12],
                node->matrix[13],
                node->matrix[14],
                node->matrix[15]));
    }

    if (node->has_translation)
    {
        result.Translate(glm::vec3(node->translation[0], node->translation[1], node->translation[2]));
    }

    if (node->has_rotation)
    {
        result.Rotate(glm::vec4(node->rotation[2], node->rotation[3], node->rotation[0], node->rotation[1]));
    }

    if (node->has_scale)
    {
        result.Scale(glm::vec3(node->scale[0], node->scale[1], node->scale[2]));
    }

    return result;
}

void GetScalarValues(std::vector<float> &outScalars, unsigned int inComponentCount, const cgltf_accessor &inAccessor)
{
    outScalars.resize(inAccessor.count * inComponentCount);

    for (cgltf_size i = 0; i < inAccessor.count; ++i)
    {
        cgltf_accessor_read_float(&inAccessor, i, &outScalars[i * inComponentCount], inComponentCount);
    }
}

int GetNodeIndex(cgltf_node *target, cgltf_node *allNodes, unsigned int numNodes)
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

glm::mat4 LoadBindTransform(
        unsigned int index,
        std::vector<float> &bindMatrices)
{
    glm::mat4 inverseBindMatrix(
            bindMatrices[(index*16) + 0],
            bindMatrices[(index*16) + 1],
            bindMatrices[(index*16) + 2],
            bindMatrices[(index*16) + 3],
            bindMatrices[(index*16) + 4],
            bindMatrices[(index*16) + 5],
            bindMatrices[(index*16) + 6],
            bindMatrices[(index*16) + 7],
            bindMatrices[(index*16) + 8],
            bindMatrices[(index*16) + 9],
            bindMatrices[(index*16) + 10],
            bindMatrices[(index*16) + 11],
            bindMatrices[(index*16) + 12],
            bindMatrices[(index*16) + 13],
            bindMatrices[(index*16) + 14],
            bindMatrices[(index*16) + 15]);

    glm::mat4 bindTransform = glm::inverse(inverseBindMatrix);

    return bindTransform;
}

//! The original position of the joints, without any animation/transform applied.
Joint *LoadBindPose(cgltf_data *data)
{
    Joint *rootJoint;

    unsigned int skinsQuantity = data->skins_count;

    for (unsigned int i = 0; i < skinsQuantity; i++)
    {
        cgltf_skin *skin = &data->skins[i];

        std::vector<float> bindMatrices;

        GetScalarValues(bindMatrices, 16, *skin->inverse_bind_matrices);

        unsigned int jointsQuantity = skin->joints_count;

        Array<Joint*> joints(jointsQuantity);
        Array<cgltf_node*> nodes(jointsQuantity);

        for (unsigned int j = 0; j < jointsQuantity; j++)
        {
            glm::mat4 bindTransform = LoadBindTransform(j, bindMatrices);

            cgltf_node *jointNode = skin->joints[j];

            int jointIndex = GetNodeIndex(jointNode, data->nodes, MAX_JOINTS);//jointsQuantity);//bonesQuantity);

            Joint *joint = new Joint(jointIndex, jointNode->name, bindTransform); //LoadLocalTransform(jointNode).matrix);

            joints[joint->id] = joint;
            nodes[joint->id] = jointNode;

            if (j == 0)
            {
                rootJoint = joint;
            }
        }

        for (unsigned int j = 0; j < jointsQuantity; j++)
        {
            Joint *joint = joints[j];
            cgltf_node *jointNode = nodes[j];

            for (unsigned int k = 0; k < jointNode->children_count; k++)
            {
                cgltf_node *childNode = jointNode->children[k];
                int jointIndex = GetNodeIndex(childNode, data->nodes, MAX_JOINTS);//jointsQuantity);//bonesQuantity);

                joint->children.Add(joints[jointIndex]);
            }
        }
    }

    return rootJoint;
}

void SkeletonIndices(Joint *joint, Array<unsigned int> &indices, unsigned int parentId = 0)
{
    indices.Add(parentId);
    indices.Add(joint->id);

    for (unsigned int i = 0; i < joint->children.Size(); i++)
    {
        SkeletonIndices(joint->children[i], indices, joint->id);
    }
}
*/

/*void Mesh::LoadFromGLTF(const URL filename)
{
    emptyAnimation = new Animation();
    animation = emptyAnimation;

    cgltf_options options;
    memset(&options, 0, sizeof(cgltf_options));
    cgltf_data *data = NULL;
    cgltf_result result = cgltf_parse_file(&options, filename.ToChar(), &data);

    if (result != cgltf_result_success)
    {
        LogError("Failed to load mesh: " + filename);
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

    Array<IDrawable::Vertex> vertices;
    Array<unsigned int> indices;
    Array<String> shaders(2);
    Array<Pixmap*> textures;

    cgltf_node *nodes = data->nodes;
    unsigned int nodeCount = data->nodes_count;

    if (data->images != NULL)
    {
        Pixmap *texture = new Pixmap(data->images->name);
        textures.Add(texture);
    }

    for (unsigned int i = 0; i < nodeCount; i++)
    {
        cgltf_node *node = &nodes[i];

        if (node->mesh == 0)
        {
            continue;
        }

        unsigned int numPrims = node->mesh->primitives_count;

        for (unsigned int j = 0; j < numPrims; j++)
        {
            cgltf_primitive *primitive = &node->mesh->primitives[j];

            unsigned int numAttributes = primitive->attributes_count;

            for (unsigned int k = 0; k < numAttributes; k++)
            {
                cgltf_attribute *attribute = &primitive->attributes[k];
                cgltf_attribute_type attribType = attribute->type;
                cgltf_accessor &accessor = *attribute->data;

                unsigned int componentCount = 0;

                if (accessor.type == cgltf_type_vec2)
                {
                    componentCount = 2;
                }
                else if (accessor.type == cgltf_type_vec3)
                {
                    componentCount = 3;
                }
                else if (accessor.type == cgltf_type_vec4)
                {
                    componentCount = 4;
                }

                std::vector<float> values;
                GetScalarValues(values, componentCount, accessor);
                unsigned int acessorCount = accessor.count;

                IDrawable::Vertex vertex;

                for (unsigned int i = 0; i < acessorCount; i++)
                {
                    int index = i * componentCount;

                    switch (attribType)
                    {
                    case cgltf_attribute_type_position:
                        vertices.Add(glm::vec3(values[index + 0], values[index + 1], values[index + 2])); // TODO: Resize array before loading
                        break;

                    case cgltf_attribute_type_joints:
                        // These indices are skin relative. This function has no information about the
                        // skin that is being parsed. Add +0.5f to round, since we can't read ints
                        vertices[i].joints = glm::ivec4(
                                    int(values[index + 0] + 0.5f),
                                    int(values[index + 1] + 0.5f),
                                    int(values[index + 2] + 0.5f),
                                    int(values[index + 3] + 0.5f));

                        if (node->skin)
                        {
                            vertices[i].joints.x = std::max(0, GetNodeIndex(node->skin->joints[vertices[i].joints.x], nodes, nodeCount));
                            vertices[i].joints.y = std::max(0, GetNodeIndex(node->skin->joints[vertices[i].joints.y], nodes, nodeCount));
                            vertices[i].joints.z = std::max(0, GetNodeIndex(node->skin->joints[vertices[i].joints.z], nodes, nodeCount));
                            vertices[i].joints.w = std::max(0, GetNodeIndex(node->skin->joints[vertices[i].joints.w], nodes, nodeCount));
                        }
                        break;

                    case cgltf_attribute_type_texcoord:
                        vertices[i].textureCoordinates = glm::vec2(values[index + 0], values[index + 1]);
                        break;

                    case cgltf_attribute_type_weights:
                        vertices[i].weights = glm::vec4(values[index + 0], values[index + 1], values[index + 2], values[index + 3]);
                        break;

                    case cgltf_attribute_type_normal:
                        glm::vec3 normal(values[index + 0], values[index + 1], values[index + 2]);

                        if (normal.length() < 0.000001f)
                        {
                            normal = glm::vec3(0, 1, 0);
                        }

                        vertices[i].normal = glm::normalize(normal);
                        break;
                    }
                }

                if (primitive->indices != 0)
                {
                    unsigned int indexCount = primitive->indices->count;

                    for (unsigned int k = 0; k < indexCount; k++)
                    {
                        indices.Add(cgltf_accessor_read_index(primitive->indices, k));
                    }
                }
            }

            //drawable = renderer->CreateDrawable(vertices, indices, shaders, textures.Size() == 0 ? NULL : &textures);
        }

        drawable2 = NULL;
        if (node->skin == 0)
        {
            shaders.Insert(filesystem->Open(URL("data/simple.vert"), PLAIN_TEXT)->Read(), VERTEX_SHADER);
            shaders.Insert(filesystem->Open(URL("data/simple.frag"), PLAIN_TEXT)->Read(), FRAGMENT_SHADER);

            drawable = renderer->CreateDrawable(vertices, indices, shaders, textures.Size() == 0 ? NULL : &textures);
        }
        else
        {
            rootJoint = LoadBindPose(data);
            rootJoint->CalculateInverseTransform(glm::mat4(1.0f));

            shaders.Insert(filesystem->Open(URL("data/skinning-lines.vert"), PLAIN_TEXT)->Read(), VERTEX_SHADER);
            shaders.Insert(filesystem->Open(URL("data/skinning-lines.frag"), PLAIN_TEXT)->Read(), FRAGMENT_SHADER);

            Array<unsigned int> indicesSkeleton;
            SkeletonIndices(rootJoint, indicesSkeleton);

            Array<IDrawable::Vertex> lines(MAX_JOINTS);
            drawable = renderer->CreateDrawable(lines, indicesSkeleton, shaders, textures.Size() == 0 ? NULL : &textures);
            drawable->type = DRAW_LINES;

            shaders.Insert(filesystem->Open(URL("data/skinning.vert"), PLAIN_TEXT)->Read(), VERTEX_SHADER);
            shaders.Insert(filesystem->Open(URL("data/skinning.frag"), PLAIN_TEXT)->Read(), FRAGMENT_SHADER);

            drawable2 = renderer->CreateDrawable(vertices, indices, shaders, textures.Size() == 0 ? NULL : &textures);
            drawable2->matrix[3].x = 10.0f;

            drawable->animatedMatrices = emptyAnimation->transformsSkeleton;
            drawable2->animatedMatrices = emptyAnimation->transforms;
        }
    }

    cgltf_free(data);
}*/

void Mesh::Hide()
{
    for (unsigned int i = 0; i < drawables.Size(); i++)
    {
        drawables[i]->visible = false;
    }
}

void Mesh::Show()
{
    for (unsigned int i = 0; i < drawables.Size(); i++)
    {
        drawables[i]->visible = true;
    }
}

void Mesh::LoadFrom3DFile(const String &filePath, const String vertexShaderPath, const String fragmentShaderPath)
{
    IFile *file = filesystem->Open(filePath);

    enum
    {
        MESH = 0,
        TEXTURE,
        TRIANGLE,
        UV,
        FRAME,
        VERTEX,
        NORMAL
    };

    Array<IDrawable::Vertex> vertices;
    Array<unsigned int> indices;
    String texturePath;

    Array<String> shaders(2);

    shaders[VERTEX_SHADER]   = vertexShaderPath;
    shaders[FRAGMENT_SHADER] = fragmentShaderPath;

    Array<ITexture *> textures;

    int frame;
    int index = 0;
    int indexTriangles = 0;
    int framesQuantity = 0;

    int verticesQuantity = 0;
    int trianglesQuantity = 0;

    while(file->isEOF == false)
    {
        int type = file->ReadInteger();

        if (type == MESH)
        {
            if (index != 0)
            {
                textures.Add(renderer->CreateTexture(String("data/" + texturePath)));

                IDrawable *drawable = renderer->CreateDrawable(vertices, indices, shaders, textures);
                drawable->verticesPerFrameQuantity = trianglesQuantity * 3;//verticesQuantity;
                drawable->lastFrame = framesQuantity;

                drawables.Add(drawable);

                textures.Clear();
                vertices.Clear();
                indices.Clear();

                index = 0;
                indexTriangles = 0;
            }

            verticesQuantity = file->ReadInteger();
            trianglesQuantity = file->ReadInteger();
            framesQuantity = file->ReadInteger();

            vertices.Resize(verticesQuantity + (framesQuantity * verticesQuantity));
            indices.Resize((trianglesQuantity * 3) * framesQuantity);
        }
        else if (type == TEXTURE)
        {
            int stringLength = file->ReadInteger();
            texturePath = file->ReadString(stringLength);
        }
        else if (type == TRIANGLE)
        {
            int v0 = file->ReadInteger();
            int v1 = file->ReadInteger();
            int v2 = file->ReadInteger();

            for (int i = 0; i < framesQuantity; i++)
            {
                indices[indexTriangles++] = v0 + (i * verticesQuantity);
                indices[indexTriangles++] = v1 + (i * verticesQuantity);
                indices[indexTriangles++] = v2 + (i * verticesQuantity);
            }
        }
        else if (type == UV)
        {
            int index = file->ReadInteger();
            float u = file->ReadFloat();
            float v = file->ReadFloat();

            for (int i = 0; i < framesQuantity; i++)
            {
                vertices[index + (i * verticesQuantity)].textureCoordinates = glm::vec2(u, 1 - v); // Notice blender inverts the v co
            }
        }
        else if (type == FRAME)
        {
            frame = file->ReadInteger();
        }
        else if (type == VERTEX)
        {
            float x = file->ReadFloat();
            float y = file->ReadFloat();
            float z = file->ReadFloat();

            vertices[index++].position = glm::vec3(x, z, y); // Notice blender uses +Z as a up direction while we use Y
        }
        else if (type == NORMAL)
        {

            float x = file->ReadFloat();
            float y = file->ReadFloat();
            float z = file->ReadFloat();

            vertices[index].normal = glm::vec3(x, z, y); // Notice blender uses +Z as a up direction while we use Y
        }
    }

    if (texturePath.Empty() == false)
    {
        textures.Add(renderer->CreateTexture(String("data/" + texturePath)));
    }

    IDrawable *drawable = Locator::renderer->CreateDrawable(vertices, indices, shaders, textures);
    drawable->verticesPerFrameQuantity = trianglesQuantity * 3; //verticesQuantity;
    drawable->lastFrame = framesQuantity;

    drawables.Add(drawable);

    textures.Clear();
    vertices.Clear();
    indices.Clear();

    delete file;
}

void Mesh::LoadFromWavefront(const String &filePath, const String vertexShaderPath, const String fragmentShaderPath)
{
    const bool left_handed = false;
    const bool log_enable = false;

    fastObjMesh* m = fast_obj_read(filePath.ToChar());
    String textureFilePath("");
    if (!m)
    {
        LogWarning(String("Failed to load: " + filePath + " (" + filePath + ")"));
        abort();
    }

    if (log_enable)
    {
        Log(String("fast_obj: ")+String(m->group_count)+" groups" );
        Log(String("fast_obj: ")+String(m->material_count)+" materials" );
        Log(String("fast_obj: ")+String(m->face_count)+" faces" );
        Log(String("fast_obj: ")+String(m->position_count)+" positions" );
        Log(String("fast_obj: ")+String(m->normal_count)+" normals" );
        Log(String("fast_obj: ")+String(m->texcoord_count)+" texcoords" );
    }
    for(unsigned i=0; i<m->material_count; i++)
    {
        fastObjMaterial &mat = m->materials[i];
        if (mat.name == 0x0) continue;
        if (log_enable)
        {
            if ( mat.map_Ka.name )   Log(String("fast_obj material Ka ")+String(mat.name)+String(mat.map_Ka.name)+" at "+String(mat.map_Ka.path) );
            if ( mat.map_Kd.name )   Log(String("fast_obj material Kd ")+String(mat.name)+String(mat.map_Kd.name)+" at "+String(mat.map_Kd.path) );
            if ( mat.map_Ks.name )   Log(String("fast_obj material Ks ")+String(mat.name)+String(mat.map_Ks.name)+" at "+String(mat.map_Ks.path) );
            if ( mat.map_Ke.name )   Log(String("fast_obj material Ke ")+String(mat.name)+String(mat.map_Ke.name)+" at "+String(mat.map_Ke.path) );
            if ( mat.map_Kt.name )   Log(String("fast_obj material Kt ")+String(mat.name)+String(mat.map_Kt.name)+" at "+String(mat.map_Kt.path) );
            if ( mat.map_Ks.name )   Log(String("fast_obj material Ks ")+String(mat.name)+String(mat.map_Ks.name)+" at "+String(mat.map_Ns.path) );
            if ( mat.map_Ni.name )   Log(String("fast_obj material Ni ")+String(mat.name)+String(mat.map_Ni.name)+" at "+String(mat.map_Ni.path) );
            if ( mat.map_d.name )    Log(String("fast_obj material diffuse ")+String(mat.name)+String(mat.map_d.name)+" at "+String(mat.map_d.path) );
            if ( mat.map_bump.name ) Log(String("fast_obj material bump ")+String(mat.name)+String(mat.map_bump.name)+" at "+String(mat.map_bump.path) );
        }

        if ( mat.map_Kd.name && mat.map_Kd.path)
        {
            textureFilePath = String(mat.map_Kd.path);
        }
    }

    Array<IDrawable::Vertex> vertices;
    Array<unsigned int> indices;
    Array<String> shaders(2);

    shaders.Insert(vertexShaderPath, VERTEX_SHADER);
    shaders.Insert(fragmentShaderPath, FRAGMENT_SHADER);

    glm::vec4 color(255.0, 255.0, 255.0, 255.0);
    typedef IDrawable::Vertex Vertex;
    for(unsigned i=0; i<m->position_count; i++)
    {
        float x = m->positions[3*i+0];
        float y = m->positions[3*i+1];
        float z = left_handed ? -1.0f * m->positions[3*i+2] : m->positions[3*i+2];
        Vertex v( glm::vec3(x,y,z), color);
        vertices.Add(v);
    }
    assert(m->position_count == (unsigned)vertices.Size());

    for (unsigned int ii = 0; ii < m->group_count; ii++)
    {
        const fastObjGroup& grp = m->groups[ii];

        String grp_name;
        if (grp.name) {
            grp_name = String(grp.name);
        }

        int idx = 0;
        for (unsigned int jj = 0; jj < grp.face_count; jj++)
        {
            unsigned int fv = m->face_vertices[grp.face_offset + jj];
            if ( fv != 3 ) LogError("Must have obj files with 3 faces, this has " + String(fv) + " faces");
            assert(fv == 3);
            int indices_reorder[3] = {0,0,0};
            for (unsigned int kk = 0; kk < fv; kk++)
            {
                fastObjIndex mi = m->indices[grp.index_offset + idx];
                assert(mi.p); // Must have a position index
                indices_reorder[kk] = mi.p;
                idx++;
            }
            if (left_handed)
            {
                indices.Add(indices_reorder[2]);
                indices.Add(indices_reorder[1]);
                indices.Add(indices_reorder[0]);
            } else {
                indices.Add(indices_reorder[0]);
                indices.Add(indices_reorder[1]);
                indices.Add(indices_reorder[2]);
            }
        }
    }

    for (unsigned int ii = 0; ii < m->group_count; ii++)
    {
        const fastObjGroup& grp = m->groups[ii];
        int idx = 0;
        for (unsigned int jj = 0; jj < grp.face_count; jj++)
        {
            unsigned int fv = m->face_vertices[grp.face_offset + jj];
            assert(fv == 3);

            for (unsigned int kk = 0; kk < fv; kk++)
            {
                fastObjIndex mi = m->indices[grp.index_offset + idx];
                assert(mi.p); // Has position

                if (mi.t)
                {
                    float u = m->texcoords[2 * mi.t + 0];
                    float v = m->texcoords[2 * mi.t + 1];
                    v = 1.0f - v; // always flip v
                    //v = left_handed ? 1.0f - v : v;
                    vertices[mi.p].textureCoordinates = glm::vec2(u,v);
                }

                if (mi.n)
                {
                    float nx = m->normals[3 * mi.n + 0];
                    float ny = m->normals[3 * mi.n + 1];
                    float nz = left_handed ? -1.0f * m->normals[3 * mi.n + 2] : m->normals[3 * mi.n + 2];
                    vertices[mi.p].color = 0.5f + 0.5f * glm::vec4( nx, ny, nz, 1.0f);
                    vertices[mi.p].normal = glm::vec3(nx,ny,nz);
                }

                idx++;
            }
        }
    }

    fast_obj_destroy(m);


    if (log_enable)
    {
        Log(String("Mesh Ctor Load ")+String(vertices.Size())+" vertices, and "+String(indices.Size())+" indices from"+filePath );
    }

    /*Array<Pixmap* > textures;
    if ( !(textureFilePath == "") )
    {
        Log(String("Loaded texture filename:")+textureFilePath );
        textures.Add( Pixmap::LoadAndCache(textureFilePath) );
    }*/

    //textureFilePath = "data/player/player_colour.png";
    //textures.Add( Pixmap::LoadAndCache(textureFilePath) );

    drawables.Add(Locator::renderer->CreateDrawable(vertices, indices, shaders, NULL));

    time = -1.0f;
}

Mesh::Mesh(const String &filePath, const String vertexShaderPath, const String fragmentShaderPath)
{
    String file_ext = filePath.GetFileExtension();
    rootJoint = NULL;

    if (file_ext == ".gltf" || file_ext == ".glb")
    {
        //LoadFromGLTF(URL(filePath));
    }
    else if (file_ext == ".obj")
    {
        LoadFromWavefront(filePath, vertexShaderPath, fragmentShaderPath);
    }
    else if (file_ext == ".3dformat")
    {
        LoadFrom3DFile(filePath, vertexShaderPath, fragmentShaderPath);
    }
    else if (file_ext == ".blend")
    {
        LoadFrom3DFile(filePath, vertexShaderPath, fragmentShaderPath);
    }
    else
    {
        assert("file format not supported");
    }

    framerate = Application::GetTime();
}

void Mesh::UpdateAfterPhysics()
{

}

void Mesh::Update()
{
    //animation->Update(rootJoint);

    for (unsigned int i = 0; i < drawables.Size(); i++)
    {
        //drawables[i]->uniformData = uniforms;
        drawables[i]->matrix = matrix.subMatrix * matrix.matrix;
        renderer->Draw(drawables[i]);
    }

    if (framerate->TimeSinceStarted() >= 1000.0f / 28.0f)
    {
        for (unsigned int i = 0; i < drawables.Size(); i++)
        {
            if (drawables[i]->lastFrame != 0)
            {
                drawables[i]->frame = (drawables[i]->frame + 1) % drawables[i]->lastFrame;
            }
        }

        framerate->Reset();
    }


    // obj animation hack follows:
/*    if (time < 0.0f)
    {
        time = float(tag.ToInt());
    }

    if (int(time) % 12 == 0)
    {
        drawable->visible = true;
    }
    else
    {
        drawable->visible = false;
    }

    time += 0.5f;*/
    // obj animation hack ends.
}

void Mesh::PlayAnimation(Animation *anim, int blend, bool reset)
{
    //animation = anim;
    //drawable->animatedMatrices = animation->transformsSkeleton;
    //drawable2->animatedMatrices = animation->transforms;
}
