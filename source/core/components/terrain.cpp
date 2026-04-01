#include "terrain.h"
#define STB_IMAGE_IMPLEMENTATION
#include "3rdparty/stb_image.h"

void Terrain::Init()
{
    /*vertices.push_back(Vertex(0, 0, 0, glm::vec3(1,0.5,0))); // 0  bottom left
    vertices.push_back(Vertex(0, -0.2,  1, glm::vec3(0.5,1,0))); // 1  bottom left
    vertices.push_back(Vertex(1, -0.5, 0, glm::vec3(0,0.5,1))); // 2  top right
    vertices.push_back(Vertex(1, -0.4,  1, glm::vec3(0.7,0,0))); // 3  top left - B

    vertices.push_back(Vertex(2, 0.3,  1, glm::vec3(1,0,0))); // 4 bottom right
    vertices.push_back(Vertex(2, 0.8,  0, glm::vec3(0,1,0)));  //5 C

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(2);

    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(4);

    indices.push_back(2);
    indices.push_back(4);
    indices.push_back(5);*/
}

Terrain::Terrain()
{
    tag = "Terrain";

    Init();

    //drawable = renderer->CreateDrawable(vertices, indices, "shaders/color.vert.spv", "shaders/color.frag.spv");
}

Terrain::Terrain(const char *filePath,
                 const char* texturePath,
                 const char* vertexShaderPath,
                 const char* fragmentShaderPath)
{
    tag = "Terrain";
    int n;
    unsigned char *data = stbi_load(filePath, &width, &height, &n, 0);

    if (!data)
    {
        LogError("Failed to load heightmap");
    }

    if (n != 1)
    {
        LogError("The texture used for the terrain has to many colour channles. Only images with one 8-bit grayscale channel are supported");
    }

    /*for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int idx = x + y * width;

            float h = data[idx]; // height

            vertices[idx].pos = glm::vec3(x, h, y);
            vertices[idx].texCoord = glm::vec2(x / float(width), y / float(height));
            vertices[idx].normal = glm::vec3(0,0,0);

            if (x > width / 2.0f)
            {
                vertices[idx].color = glm::vec3(1,0,0);
                vertices[idx].friction = 0.1f;
            }
        }
    }

    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {

            int i0 = x + y * width;
            int i1 = (x+1) + y * width;
            int i2 = x + (y+1) * width;
            int i3 = (x+1) + (y+1) * width;

            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i2);

            indices.push_back(i1);
            indices.push_back(i3);
            indices.push_back(i2);
        }
    }

    stbi_image_free(data);

    // For each triangle, accumulate face normals
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        unsigned int i0 = indices[i];
        unsigned int i1 = indices[i + 1];
        unsigned int i2 = indices[i + 2];

        glm::vec3& p0 = vertices[i0].pos;
        glm::vec3& p1 = vertices[i1].pos;
        glm::vec3& p2 = vertices[i2].pos;

        glm::vec3 edge1 = p1 - p0;
        glm::vec3 edge2 = p2 - p0;

        glm::vec3 faceNormal = glm::normalize(glm::cross(edge2, edge1));

        vertices[i0].normal += faceNormal;
        vertices[i1].normal += faceNormal;
        vertices[i2].normal += faceNormal;
    }

    // Normalize vertices
    for (auto& v : vertices)
    {
        v.normal = glm::normalize(v.normal);
    }

    drawable = renderer->CreateDrawable(vertices, indices, vertexShaderPath, fragmentShaderPath, Renderer::TRIANGLES, texturePath);*/
}

void Terrain::Update(float deltaTime)
{
}

float Terrain::GetHeightAt(float x, float z) const
{
    return GetHeightAt(glm::vec3(x, 0, z));
}

float Terrain::GetHeightAt(glm::vec3 positionXZ) const
{
    /*float x = positionXZ.x;
    float z = positionXZ.z;

    // Convert to grid coordinates
    int ix = int(floor(x));
    int iz = int(floor(z));

    // Bounds check
    if (ix < 0 || iz < 0 || ix >= width - 1 || iz >= height - 1)
        return 0.0f;

    // Get vertex indices in the heightmap grid
    const Vertex& v00 = vertices[iz * width + ix];
    const Vertex& v10 = vertices[iz * width + (ix + 1)];
    const Vertex& v01 = vertices[(iz + 1) * width + ix];
    const Vertex& v11 = vertices[(iz + 1) * width + (ix + 1)];

    // Local coords in the cell
    float dx = x - ix;
    float dz = z - iz;

    float h;

    // Determine which of the two triangles we're in
    if (dx + dz < 1.0f) {
        // Triangle 1: v00 - v10 - v01
        float u = 1 - dx - dz;
        float v = dx;
        float w = dz;
        h = v00.pos.y * u + v10.pos.y * v + v01.pos.y * w;
    } else {
        // Triangle 2: v10 - v11 - v01
        float u = 1 - (1 - dx) - (1 - dz);
        float v = 1 - dz;
        float w = 1 - dx;
        h = v10.pos.y * u + v11.pos.y * v + v01.pos.y * w;
    }

    return h;*/
    return 0;
}

glm::vec3 Terrain::GetNormal(glm::vec3 position) const
{
    /*float x = position.x;
    float z = position.z;

    int ix = int(floor(x));
    int iz = int(floor(z));

    if (ix < 0 || iz < 0 || ix >= width - 1 || iz >= height - 1)
        return glm::vec3(0, 1, 0);

    const Vertex& v00 = vertices[iz * width + ix];
    const Vertex& v10 = vertices[iz * width + (ix + 1)];
    const Vertex& v01 = vertices[(iz + 1) * width + ix];
    const Vertex& v11 = vertices[(iz + 1) * width + (ix + 1)];

    float dx = x - ix;
    float dz = z - iz;

    glm::vec3 n;

    if (dx + dz < 1.0f) {
        float u = 1 - dx - dz;
        float v = dx;
        float w = dz;
        n = v00.normal * u + v10.normal * v + v01.normal * w;
    } else {
        float u = dx + dz - 1.0f; // derived to match continuity
        float v = 1 - dz;
        float w = 1 - dx;
        n = v10.normal * u + v11.normal * v + v01.normal * w;
    }

    return glm::normalize(n);*/
    return glm::vec3();
}
