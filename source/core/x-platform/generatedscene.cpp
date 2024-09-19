#include "generatedscene.h"
#include "core/application.h"
#include "core/components/pointlight.h"
#include "core/components/directionallight.h"
#include "core/components/sphere.h"
#include "core/components/triangle.h"
#include "core/components/fpscamera.h"
#include "core/components/snowglobecamera.h"
#include "core/components/tankcontrols.h"
#include "core/components/laddercontrols.h"
#include "core/components/videocapture.h"
#include "core/components/skipcutscene.h"

GeneratedScene::GeneratedScene(String &sceneJSON, int sceneID)
    : jsonScene(sceneJSON.ToChar()),
      animations(DEFAULT_HASH_SIZE, Hash<String, Animation*>::StringHash)
{
    // Set scene ID
    /*if (sceneID == -1)
    {*/
        id = Application::scenes.Size();
    /*}
    else
    {
        id = sceneID;
    }*/

    activeActor = NULL;

    // Setup parser
    jsonScene.AddObject("camera");
    jsonScene.AddObject("point");
    jsonScene.AddObject("emission");
    jsonScene.AddObject("diffuse");
    jsonScene.AddObject("shininess");
    jsonScene.AddObject("specular");
    jsonScene.AddObject("vertices");
    jsonScene.AddObject("actor");
    jsonScene.AddObject("scale");
    jsonScene.AddObject("tri");
    jsonScene.AddObject("ambient");
    jsonScene.AddObject("translate");
    jsonScene.AddObject("rotate");
    jsonScene.AddObject("sphere");
    jsonScene.AddObject("mesh");
    jsonScene.AddObject("animation");
    jsonScene.AddObject("video");
    jsonScene.AddObject("audio");
}

void GeneratedScene::Init()
{
    Log("Init scene " + String(id));
    for (String object = jsonScene.GetNextObject(); (object == "Empty") == false; object = jsonScene.GetNextObject())
    {
        if (object == "camera")
        {
            AddCamera();
        }
        else if (object == "actor")
        {
            AddActor();
        }
        // Lights
        else if (object == "directional")
        {
            AddDirectional();
        }
        else if (object == "point")
        {
            AddPointLight();
        }
        else if (object == "attenuation")
        {
            SetAttenuation();
        }
        else if (object == "ambient")
        {
            SetAmbient();
        }
        // Materials
        else if (object == "emission")
        {
            SetEmission();
        }
        else if (object == "diffuse")
        {
            SetDiffuse();
        }
        else if (object == "shininess")
        {
            SetShininess();
        }
        else if (object == "specular")
        {
            SetSpecular();
        }
        // Geometry
        else if (object == "vertices")
        {
            AddVertex();
        }
        else if (object == "tri")
        {
            AddTriangles();
        }
        else if (object == "sphere")
        {
            AddSphere();
        }
        // Transformations
        else if (object == "scale")
        {
            SetScale();
        }
        else if (object == "translate")
        {
            SetTranslation();
        }
        else if (object == "rotate")
        {
            SetRotation();
        }
        //
        else if (object == "mesh")
        {
            AddMesh();
        }
        else if (object == "video")
        {
            PlayVideo();
        }
        else if (object == "audio")
        {
            AddAudio();
        }
        else if (object == "animation")
        {
            AddAnimation();
        }
        else if (object == "snowglobecamera")
        {
            AddSnowBallCamera();
        }
        else if (object == "skipcutscene")
        {
            //AddSkipCutscene();
        }
    }
}

GeneratedScene::~GeneratedScene()
{
    /*for (unsigned int i = 0; i < components.Size(); i++)
    {
        delete components[i];
    }*/ // Handled by Ptr

    audio->Stop();
}

void GeneratedScene::Update()
{
}

void GeneratedScene::AddCamera()
{
    float x = jsonScene.GetNumber("lookfromx");
    float y = jsonScene.GetNumber("lookfromy");
    float z = jsonScene.GetNumber("lookfromz");

    float atx = jsonScene.GetNumber("lookatx");
    float aty = jsonScene.GetNumber("lookaty");
    float atz = jsonScene.GetNumber("lookatz");

    float upx = jsonScene.GetNumber("upx");
    float upy = jsonScene.GetNumber("upy");
    float upz = jsonScene.GetNumber("upz");

    float fov = jsonScene.GetNumber("fov");

    components.Add(activeCamera = new Camera(glm::vec3(x, y, z), glm::vec3(upx, upy, upz), glm::vec3(atx, aty, atz), fov));
}

void GeneratedScene::AddPointLight()
{
    float x = jsonScene.GetNumber("x");
    float y = jsonScene.GetNumber("y");
    float z = jsonScene.GetNumber("z");
    float r = jsonScene.GetNumber("r");
    float g = jsonScene.GetNumber("g");
    float b = jsonScene.GetNumber("b");

    components.Add(new PointLight(glm::vec3(x, y, z), glm::vec3(r, g, b)));
}

void GeneratedScene::AddActor()
{
    activeActor = new Actor();
    components.Add(activeActor);

//    components.Add(new SnowGlobeCamera(activeActor, activeCamera));
    components.Add(new TankControls(activeActor));
//    components.Add(new LadderControls(activeActor));
}

void GeneratedScene::AddDirectional()
{
    // directional x y z r g b
    float x = jsonScene.GetNumber("x");
    float y = jsonScene.GetNumber("y");
    float z = jsonScene.GetNumber("z");
    float r = jsonScene.GetNumber("r");
    float g = jsonScene.GetNumber("g");
    float b = jsonScene.GetNumber("b");

    components.Add(new DirectionalLight());
}

void GeneratedScene::SetAttenuation()
{
    // attenuation const linear quadratic
    float constant = jsonScene.GetNumber("const");
    float linear = jsonScene.GetNumber("linear");
    float quadratic = jsonScene.GetNumber("quadratic");
}

void GeneratedScene::SetAmbient()
{
    // ambient r g b
    float r = jsonScene.GetNumber("r");
    float g = jsonScene.GetNumber("g");
    float b = jsonScene.GetNumber("b");
}

void GeneratedScene::SetEmission()
{
    // emission r g b
    float r = jsonScene.GetNumber("r");
    float g = jsonScene.GetNumber("g");
    float b = jsonScene.GetNumber("b");
}

void GeneratedScene::SetDiffuse()
{
    // diffuse r g b
    float r = jsonScene.GetNumber("r");
    float g = jsonScene.GetNumber("g");
    float b = jsonScene.GetNumber("b");
}

void GeneratedScene::SetShininess()
{
    // shininess s
    float s = jsonScene.GetNumber("s");
}

void GeneratedScene::SetSpecular()
{
    // specular r g b
    float r = jsonScene.GetNumber("r");
    float g = jsonScene.GetNumber("g");
    float b = jsonScene.GetNumber("b");
}

void GeneratedScene::AddVertex()
{
    // vertex x y z
    float x = jsonScene.GetNumber("x");
    float y = jsonScene.GetNumber("y");
    float z = jsonScene.GetNumber("z");

    vertices.Add(glm::vec3(x, y, z));
}

void GeneratedScene::AddTriangles()
{
    // tri v1 v2 v3
    float v1 = jsonScene.GetNumber("v1");
    float v2 = jsonScene.GetNumber("v2");
    float v3 = jsonScene.GetNumber("v3");

    activeActor->Add(new Triangle(vertices[v1], vertices[v2], vertices[v3]));
}

void GeneratedScene::AddSphere()
{
    // sphere x y z radius
    float x = jsonScene.GetNumber("x");
    float y = jsonScene.GetNumber("y");
    float z = jsonScene.GetNumber("z");
    float radius = jsonScene.GetNumber("radius");

    activeActor->Add(new Sphere(glm::vec3(x, y, z), radius));
}

void GeneratedScene::SetScale()
{
    // scale x y z
    float x = jsonScene.GetNumber("x");
    float y = jsonScene.GetNumber("y");
    float z = jsonScene.GetNumber("z");
}

void GeneratedScene::SetTranslation()
{
    // translate x y z
    float x = jsonScene.GetNumber("x");
    float y = jsonScene.GetNumber("y");
    float z = jsonScene.GetNumber("z");

    activeActor->matrix.Translate(glm::vec3(x, y, z));
}

void GeneratedScene::SetRotation()
{
    // rotate x y z angle
    float x = jsonScene.GetNumber("x");
    float y = jsonScene.GetNumber("y");
    float z = jsonScene.GetNumber("z");
    float angle = jsonScene.GetNumber("angle");
}

void GeneratedScene::AddMesh()
{
    // mesh x y z filename
    float x = jsonScene.GetNumber("x");
    float y = jsonScene.GetNumber("y");
    float z = jsonScene.GetNumber("z");
    String tag = jsonScene.GetString("tag");
    String filename = jsonScene.GetString("filename");

    Mesh *mesh = new Mesh("data/" + filename);

    mesh->matrix.matrix[3] = glm::vec4(x, y, z, 1.0f);

    if (tag == String("Empty") == false)
    {
        mesh->tag = tag;
    }
    else
    {
        mesh->tag = "Mesh";
    }

    if (activeActor)
    {
        activeActor->Add(mesh);
    }
    else
    {
        components.Add(mesh);
    }
}

void GeneratedScene::PlayVideo()
{
    String filename = jsonScene.GetString("filename");
    VideoCapture *video = new VideoCapture(URL("data/" + filename).ToChar());
    SkipCutscene *skip  = new SkipCutscene(video);

    components.Add(video);
    components.Add(skip);
}

void GeneratedScene::AddAudio()
{
    String filename = jsonScene.GetString("filename");

    audio->PlaySound(filename.ToChar());
}

void GeneratedScene::AddAnimation()
{
    String filename = jsonScene.GetString("filename");
    String name = jsonScene.GetString("tag");

    Animation *anim = new Animation(URL(filename));
    animations.Insert(name, anim);
}

void GeneratedScene::AddSnowBallCamera()
{
    String target = jsonScene.GetString("target");
    String tag = jsonScene.GetString("tag");

    Actor *actor = dynamic_cast<Actor*>(FindComponent(target));

    if (actor == NULL)
    {
        LogError("Target " + target + " not found in scene.");
    }

    components.Add(new SnowGlobeCamera(actor, activeCamera));
}

Component *GeneratedScene::FindComponent(String tag)
{
    for (unsigned int i = 0; i < components.Size(); i++)
    {
        if ((*components[i])->tag == tag)
        {
            return *components[i];
        }
    }

    return NULL;
}
