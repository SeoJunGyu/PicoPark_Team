#include "stdafx.h"
#include "SceneMgr.h"
#include "SceneDev1.h"
#include "SceneDev2.h"
#include "SceneGame.h"

void SceneMgr::Init()
{
	scenes.insert({ SceneIds::Dev1, new SceneDev1() });
	scenes.insert({ SceneIds::Dev2, new SceneDev2() });
	scenes.insert({ SceneIds::Game, new SceneGame()});

	for (auto pair : scenes)
	{
		pair.second->Init();
	}

	currentScene = startScene;
	scenes[currentScene]->Enter();
}

void SceneMgr::Release()
{
	for (auto pair : scenes)
	{
		if (pair.first == currentScene)
		{
			pair.second->Exit();
		}
		pair.second->Release();
		delete pair.second;
	}
	scenes.clear();
}

void SceneMgr::ChangeScene(SceneIds id)
{
	nextScene = id;
}

void SceneMgr::Update(float dt)
{
	if (nextScene != SceneIds::None)
	{
		scenes[currentScene]->Exit();
		currentScene = nextScene;
		nextScene = SceneIds::None;
		scenes[currentScene]->Enter();
	}
	scenes[currentScene]->Update(dt);
}

void SceneMgr::Draw(sf::RenderWindow& window)
{
	scenes[currentScene]->Draw(window);
}
