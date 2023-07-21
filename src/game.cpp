#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "text_renderer.h"

SpriteRenderer *Renderer;
SpriteRenderer *Zapper;
SpriteRenderer *Barry;
GameObject *Character;
vector<GameObject *> Coins(1000);
vector<GameObject *> Zappers(1000);
TextRenderer *Text;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
    delete Renderer;
    // delete Character;
}

bool CheckCollision(GameObject *one, GameObject *two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one->Position.x + one->Size.x >= two->Position.x &&
                      two->Position.x + two->Size.x >= one->Position.x;
    // collision y-axis?
    bool collisionY = one->Position.y + one->Size.y >= two->Position.y &&
                      two->Position.y + two->Size.y >= one->Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

void Game::Init()
{
    ResourceManager::LoadShader("/home/gb2385/Desktop/IIIT-H/2-2/CG/Assignments/Ass_1/code/src/shaders/sprite.vs", "/home/gb2385/Desktop/IIIT-H/2-2/CG/Assignments/Ass_1/code/src/shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::LoadShader("/home/gb2385/Desktop/IIIT-H/2-2/CG/Assignments/Ass_1/code/src/shaders/zapper.vs", "/home/gb2385/Desktop/IIIT-H/2-2/CG/Assignments/Ass_1/code/src/shaders/zapper.fs", nullptr, "zapper");
    ResourceManager::LoadShader("/home/gb2385/Desktop/IIIT-H/2-2/CG/Assignments/Ass_1/code/src/shaders/character.vs", "/home/gb2385/Desktop/IIIT-H/2-2/CG/Assignments/Ass_1/code/src/shaders/character.fs", nullptr, "character");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("zapper").Use().SetInteger("image", 0);
    ResourceManager::GetShader("zapper").SetMatrix4("projection", projection);
    ResourceManager::GetShader("character").Use().SetInteger("image", 0);
    ResourceManager::GetShader("character").SetMatrix4("projection", projection);

    Shader myShader;
    myShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myShader);
    myShader = ResourceManager::GetShader("zapper");
    Zapper = new SpriteRenderer(myShader);
    myShader = ResourceManager::GetShader("character");
    Barry = new SpriteRenderer(myShader);

    ResourceManager::LoadTexture("/home/gb2385/Desktop/IIIT-H/2-2/CG/Assignments/Ass_1/code/src/textures/background.png", true, "background");
    ResourceManager::LoadTexture("/home/gb2385/Desktop/IIIT-H/2-2/CG/Assignments/Ass_1/code/src/textures/character.png", true, "character");
    ResourceManager::LoadTexture("/home/gb2385/Desktop/IIIT-H/2-2/CG/Assignments/Ass_1/code/src/textures/coin.png", true, "coin");
    ResourceManager::LoadTexture("/home/gb2385/Desktop/IIIT-H/2-2/CG/Assignments/Ass_1/code/src/textures/zapper.png", true, "zapper");

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("/home/gb2385/Desktop/IIIT-H/2-2/CG/Assignments/Ass_1/code/src/fonts/OCRAEXT.TTF", 24);

    Texture2D myTexture;
    glm::vec2 characterPos = glm::vec2(50.0f, 500.0f);
    myTexture = ResourceManager::GetTexture("character");
    Character = new GameObject(characterPos, CHARACTER_SIZE, myTexture);
    myTexture = ResourceManager::GetTexture("coin");
    float x = 200.0f;
    float y = 300.0f;
    for (int i = 0; i < 1000; i++)
    {
        float y = rand() % 500;
        glm::vec2 coinPos = glm::vec2(x * i, y);
        Coins[i] = new GameObject(coinPos, COIN_SIZE, myTexture);
    }
    myTexture = ResourceManager::GetTexture("zapper");
    for (int i = 0; i < 1000; i++)
    {
        x = (rand() % 400) + (400 * i);
        y = rand() % 300;
        glm::vec2 zapperPos = glm::vec2(x, y);
        Zappers[i] = new GameObject(zapperPos, ZAPPER_SIZE, myTexture);
    }
}

void Game::DoCollisionsCoins()
{
    for (int i = 0; i < 1000; i++)
    {
        if (!Coins[i]->Destroyed)
        {
            if (CheckCollision(Character, Coins[i]))
            {
                Coins[i]->Destroyed = true;
                CoinsCollected++;
            }
        }
    }
}

void Game::DoCollisionsZappers()
{
    for (int i = 0; i < 1000; i++)
    {
        if (!Zappers[i]->Destroyed)
        {
            if (CheckCollision(Character, Zappers[i]))
            {
                this->State = GAME_LOSE;
            }
        }
    }
}

void Game::Update(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        BG_OFFSET = (BG_OFFSET + 1) % 800;

        if (this->Keys[GLFW_KEY_SPACE])
        {
            CHARACTER_ACCELERATION_UP = CHARACTER_ACCELERATION_UP + 0.1f;
            CHARACTER_ACCELERATION_DOWN = 1.0f;
        }
        else if (!this->Keys[GLFW_KEY_SPACE])
        {
            CHARACTER_ACCELERATION_DOWN = CHARACTER_ACCELERATION_DOWN + 0.1f;
            CHARACTER_ACCELERATION_UP = 1.0f;
        }

        for (int i = 0; i < 1000; i++)
        {
            Coins[i]->Position.x -= COIN_VELOCITY;
        }

        for (int i = 0; i < 1000; i++)
        {
            Zappers[i]->Position.x -= COIN_VELOCITY;
            if (Level == 3)
            {
                int flag = rand() % 2;
                if (flag == 0)
                {
                    Zappers[i]->Position.y -= (ZAPPER_VELOCITY * 5);
                }
                else
                {
                    Zappers[i]->Position.y += (ZAPPER_VELOCITY * 5);
                }
            }
        }

        this->DoCollisionsCoins();
        this->DoCollisionsZappers();
        DistanceTravelled++;

        if (DistanceTravelled == 1000 && Level == 3)
        {
            this->State = GAME_WIN;
        }

        if (DistanceTravelled == 1500 && Level == 2)
        {
            Level++;
            DistanceTravelled = 0;
            CoinsCollected = 0;
        }

        if (DistanceTravelled == 2000 && Level == 1)
        {
            Level++;
            DistanceTravelled = 0;
            CoinsCollected = 0;
        }

        if (Level != 1)
        {
            COIN_VELOCITY = 1.5f;
        }
    }
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        if (this->Keys[GLFW_KEY_SPACE])
        {
            ResourceManager::GetShader("character").Use().SetInteger("Space", 1);
            if (Character->Position.y >= 0.5f)
            {
                Character->Position.y -= (CHARACTER_VELOCITY_UP + CHARACTER_ACCELERATION_UP);
            }
        }
        else if (!this->Keys[GLFW_KEY_SPACE])
        {
            ResourceManager::GetShader("character").Use().SetInteger("Space", 0);
            if (Character->Position.y < 490.0f)
            {
                Character->Position.y += (CHARACTER_VELOCITY_DOWN + CHARACTER_ACCELERATION_DOWN);
            }
        }
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        Texture2D myTexture;
        myTexture = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(myTexture, glm::vec2(-BG_OFFSET, 0.0f), glm::vec2(1100.0f, 600.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        Renderer->DrawSprite(myTexture, glm::vec2(800 - BG_OFFSET, 0.0f), glm::vec2(1100.0f, 600.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        Character->Draw(*Barry);
        for (int i = 0; i < 1000; i++)
        {
            Coins[i]->Draw(*Renderer);
        }
        for (int i = 0; i < 1000; i++)
        {
            Zappers[i]->Draw(*Zapper);
        }

        stringstream ss1, ss2, ss3;
        ss1 << CoinsCollected;
        Text->RenderText("Coins:" + ss1.str(), 5.0f, 5.0f, 1.0f);
        ss2 << DistanceTravelled;
        Text->RenderText("Distance:" + ss2.str(), 200.0f, 5.0f, 1.0f);

        if (Level == 1)
        {
            ss3 << Level;
            Text->RenderText("Level:" + ss3.str() + "/2000", 500.0f, 5.0f, 1.0f);
        }
        else if (Level == 2)
        {
            ss3 << Level;
            Text->RenderText("Level:" + ss3.str() + "/1500", 500.0f, 5.0f, 1.0f);
        }
        else if (Level == 3)
        {
            ss3 << Level;
            Text->RenderText("Level:" + ss3.str() + "/1000", 500.0f, 5.0f, 1.0f);
        }
    }

    if (this->State == GAME_LOSE)
    {
        Text->RenderText("YOU LOSE", 200.0f, 200.0f, 4.0f);
        stringstream ss1;
        ss1 << CoinsCollected;
        Text->RenderText("Coins:" + ss1.str(), 5.0f, 5.0f, 1.0f);
    }

    if (this->State == GAME_WIN)
    {
        Text->RenderText("YOU WIN", 200.0f, 200.0f, 4.0f);
        stringstream ss1;
        ss1 << CoinsCollected;
        Text->RenderText("Coins:" + ss1.str(), 5.0f, 5.0f, 1.0f);
    }
}