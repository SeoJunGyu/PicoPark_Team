#include "stdafx.h"

bool loadLevel_(const std::string& path, Level& out)
{
	std::ifstream f(path); //파일 열기
	if (!f) { std::cerr << "file open fail\n"; return false; } //열기 실패 -> false 반환

	nlohmann::json j;
	try { f >> j; } //파일 내용을 JSON 객체 j 로 파싱
	catch (std::exception& e) {
		std::cerr << "parse error: " << e.what() << '\n'; //예외나면 에러메세지
		return false;
	}

	if (j["version"] != 1) { //version 확인 / 1이 아니면 지원하지 않는 포멧
		std::cerr << "unsupported version\n";
		return false;
	}

	out.gridWidth = j["gridWidth"];
	out.gridHeight = j["gridHeight"];
	out.tileSize = j["tileSize"];
	out.tiles = j["layers"][0]["data"].get<std::vector<int>>(); //첫번쨰 레이어의 data 배열을 받아 저장
	out.entities = j["entities"];
	return true;
}
