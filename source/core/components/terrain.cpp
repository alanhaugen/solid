#include "terrain.h"
#include "3rdparty/stb_image.h"

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
        LogError("The texture used for the terrain has too many colour channels. Only images with a single 8-bit channel are supported");
    }

    vertices.Resize(width * height);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int idx = x + y * width;

            float h = data[idx]; // height

            vertices[idx].position = glm::vec3(x, h, y);
            vertices[idx].textureCoordinates = glm::vec2(x / float(width), y / float(height));
            vertices[idx].normal = glm::vec3(0,0,0);
        }
    }

    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {

            int i0 = x + y * width;
            int i1 = (x+1) + y * width;
            int i2 = x + (y+1) * width;
            int i3 = (x+1) + (y+1) * width;

            indices.Add(i0);
            indices.Add(i1);
            indices.Add(i2);

            indices.Add(i1);
            indices.Add(i3);
            indices.Add(i2);
        }
    }

    stbi_image_free(data);

    // For each triangle, accumulate face normals
    for (size_t i = 0; i < indices.Size(); i += 3)
    {
        unsigned int i0 = indices[i];
        unsigned int i1 = indices[i + 1];
        unsigned int i2 = indices[i + 2];

        glm::vec3& p0 = vertices[i0].position;
        glm::vec3& p1 = vertices[i1].position;
        glm::vec3& p2 = vertices[i2].position;

        glm::vec3 edge1 = p1 - p0;
        glm::vec3 edge2 = p2 - p0;

        glm::vec3 faceNormal = glm::normalize(glm::cross(edge2, edge1));

        vertices[i0].normal += faceNormal;
        vertices[i1].normal += faceNormal;
        vertices[i2].normal += faceNormal;
    }

    // Normalize vertices
    for (size_t i = 0; i < vertices.Size(); i += 3)
    {
        vertices[i].normal = glm::normalize(vertices[i].normal);
    }

    Array<String> shaders(2);
    shaders.Insert(vertexShaderPath, VERTEX_SHADER);
    shaders.Insert(fragmentShaderPath, FRAGMENT_SHADER);

    drawable = renderer->CreateDrawable(vertices, indices, shaders);
}

void Terrain::Update(float deltaTime)
{
}

void Terrain::UpdateAfterPhysics()
{
}

float Terrain::GetHeightAt(float x, float z) const
{
    return GetHeightAt(glm::vec3(x, 0, z));
}

float Terrain::GetHeightAt(glm::vec3 positionXZ) const
{
    float x = positionXZ.x;
    float z = positionXZ.z;

    // Convert to grid coordinates
    int ix = int(floor(x));
    int iz = int(floor(z));

    // Bounds check
    if (ix < 0 || iz < 0 || ix >= width - 1 || iz >= height - 1)
        return 0.0f;

    // Get vertex indices in the heightmap grid
    const IDrawable::Vertex& v00 = vertices[iz * width + ix];
    const IDrawable::Vertex& v10 = vertices[iz * width + (ix + 1)];
    const IDrawable::Vertex& v01 = vertices[(iz + 1) * width + ix];
    const IDrawable::Vertex& v11 = vertices[(iz + 1) * width + (ix + 1)];

    // Local coords in the cell
    float dx = x - ix;
    float dz = z - iz;

    float h;

    // Determine which of the two triangles we're in
    if (dx + dz < 1.0f)
    {
        // Triangle 1: v00 - v10 - v01
        float u = 1 - dx - dz;
        float v = dx;
        float w = dz;
        h = v00.position.y * u + v10.position.y * v + v01.position.y * w;
    }
    else
    {
        // Triangle 2: v10 - v11 - v01
        float u = 1 - (1 - dx) - (1 - dz);
        float v = 1 - dz;
        float w = 1 - dx;
        h = v10.position.y * u + v11.position.y * v + v01.position.y * w;
    }

    return h;
}

glm::vec3 Terrain::GetNormal(glm::vec3 position) const
{
    float x = position.x;
    float z = position.z;

    int ix = int(floor(x));
    int iz = int(floor(z));

    if (ix < 0 || iz < 0 || ix >= width - 1 || iz >= height - 1)
        return glm::vec3(0, 1, 0);

    const IDrawable::Vertex& v00 = vertices[iz * width + ix];
    const IDrawable::Vertex& v10 = vertices[iz * width + (ix + 1)];
    const IDrawable::Vertex& v01 = vertices[(iz + 1) * width + ix];
    const IDrawable::Vertex& v11 = vertices[(iz + 1) * width + (ix + 1)];

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

    return glm::normalize(n);
}
