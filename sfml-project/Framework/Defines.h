#pragma once

#define DEF_DEV

enum class SortingLayers
{
	Background,	
	Foreground,
	Default,
	UI,
};

enum class Origins
{
	TL, TC, TR,	// 0 1 2
	ML, MC, MR,	// 3 4 5
	BL, BC, BR, // 6 7 8
	Custom
};

enum class SceneIds
{
	None = -1,
	Dev1,
	Dev2,
	Title,
	Game,
	Editor,
	Count,
};

enum class Sides
{
	Left,
	Right,
	None,
};

enum class PlayerState
{
	Idle,
	Walk,
	Jump,
	MagnetHold,
	Absorbed, //흡수 당한 상태
	AbsorbCharge, //다른 캐릭터 흡수 한 상태
	AbsorbThrow, //흡수 공 쏜 상태
	Dead,
	Count,
};

enum class GimmickType
{
	//움직임 X
	Key,
	Door,
	Button,
	BouncePad,
	WeightPad,
	KillLine,
	//움직임 -> Update 필요
	MovingPlatform,
	FallingPlatform,
	RoundPlatform,
	PushBlock,
	ColorPushBlock,
	Rope,
	//플레이어 관련
	PlayerSpawn,
	Count,
};

enum class StandType
{
	None,
	Platform,
	Player,
	Count,
};