#include "EntityManager.h"



GameEngine::EntityManager* GameEngine::EntityManager::m_Instance = NULL;

 GameEngine::EntityManager* GameEngine::EntityManager::CreateInstance() {
	if (m_Instance == NULL) {m_Instance = new EntityManager();}
	return m_Instance;
}

 void GameEngine::EntityManager::Terminate() {
	 delete m_Instance;
	 m_Instance = NULL;
 }

GameEngine::EntityManager::EntityManager(){
    m_IdPool = GameEngine::IdGenerationPool::CreateInstance();

    /*componentLists["UnitDamage"].push_back(new UnitDamage(5.0f));

    UnitDamage* child = dynamic_cast<UnitDamage*>(componentLists.at("UnitDamage")[0]);
    std::cout << "Damage value:" << child->GetDamage() << std::endl;*/
}

GameEngine::EntityManager::~EntityManager() {
    delete m_IdPool;
    m_IdPool = NULL;
}


int GameEngine::EntityManager::CreateNewEntity(char* form, float x_pos, float y_pos , float z_pos) {
	std::cout << "New entity created " << std::endl;
    GameObject* entity = new GameObject();
    EntityList.insert({ entity->GetID(), entity});
    
    /*--------Load the json file---------*/
    std::ifstream testData("Data/EntityData.json");
    Json::Value actualJson;
    Json::Reader reader;

    // using the reader we parse the json 
    reader.parse(testData, actualJson);

    entity->GetTransform()->SetTransform(x_pos, y_pos, z_pos);

    /*------Take out the the info regarding which components to add.--------*/
    for (Json::Value::const_iterator itr = actualJson[form]["Components"].begin(); itr != actualJson[form]["Components"].end(); itr++) {
        BaseComponent* comp;
        int componentID = 0x00000000;
        if (itr->asCString() == (std::string) "UnitDamage") {
            comp = new UnitDamage(entity, actualJson[form]["Template"]["Damage"].asFloat());
            componentID = 0x00000001;
        }

        else if (itr->asCString() == (std::string)"UnitHealth") {
            comp = new UnitHealth(entity, actualJson[form]["Template"]["Health"].asFloat());
            componentID = 0x00000002;

        }

        else if (itr->asCString() == (std::string)"UnitMovement") {
            comp = new UnitMovement(entity, actualJson[form]["Template"]["Speed"].asFloat());
            componentID = 0x00000003;
        }

        else if (itr->asCString() == (std::string)"PathFinding") {
            comp = new PathFinding(entity);
            componentID = 0x00000004;
        }


        else if (itr->asCString() == (std::string)"Renderable") {
            std::string meshData = actualJson[form]["Template"]["Mesh"].asCString();
            const char* meshDir = meshData.c_str();
            std::string fragData = actualJson[form]["Template"]["fs_Shader"].asCString();
            const char* fragDir = fragData.c_str();
            std::string vertData = actualJson[form]["Template"]["vs_Shader"].asCString();
            const char* vertDir = vertData.c_str();

            comp = new Renderable(entity, meshDir, fragDir, vertDir);
            componentID = 0x00000006;
        }


        else if (itr->asCString() == (std::string)"Sound") {
            std::string data = actualJson[form]["Template"]["Sound"].asCString();
            const char* directory = data.c_str();
            comp = new Sound(entity);
            componentID = 0x00000007;
        }
        if (componentID != 0x00000000) {
            entity->AddComponent(componentID, comp);
        }
        
    }
    return entity->GetID();

}

void GameEngine::EntityManager::playMusicTest(int id) {
    //Sound* child = dynamic_cast<Sound*>(EntityList.at(id));
    //Mix_Music* music = child->Sound::loadMusic("Audio/Music/test_Seq06.wav");
    //std::map<std::string, Mix_Chunk*> audio = child->Sound::loadChunk("Audio/SoundEffect/");
    //child->Sound::playMusic(music, 52, -1);
    //child->Sound::playChunk(audio.at("test_Seq08"), 52, 0);
    
    //std::cout << "Sound:" << child->loadMusic("Audio/Music/") << std::endl;
}

void GameEngine::EntityManager::TerminateEnity(int entityID) {
    /*--Delete the entity--*/
    delete EntityList.begin()->second;
    EntityList.begin()->second = NULL;
    EntityList.erase(EntityList.begin());
    /*---------------------*/
    
}


void GameEngine::EntityManager::EarlyUpdate() {
    for (auto entity : EntityList) {
        entity.second->EarlyUpdate();
    }
}

void GameEngine::EntityManager::Update() {
    for (auto entity : EntityList) {
        entity.second->Update();
    }
}

void GameEngine::EntityManager::LateUpdate() {
    for (auto entity : EntityList) {
        entity.second->LateUpdate();
    }
}

int GameEngine::EntityManager::GetNewID() {
    currentID += 1;
    return currentID;
}

GameEngine::GameObject* GameEngine::EntityManager::GetEntity(int id) {
    if (EntityList.count(id) == 0) {
        return nullptr;
    }
    return EntityList.at(id);

}


std::map<int, GameEngine::GameObject*>* GameEngine::EntityManager::GetList() {
    return &EntityList;
}