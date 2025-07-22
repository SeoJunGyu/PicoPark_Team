#include "stdafx.h"

bool loadLevel_(const std::string& path, Level& out)
{
	std::ifstream f(path); //���� ����
	if (!f) { std::cerr << "file open fail\n"; return false; } //���� ���� -> false ��ȯ

	nlohmann::json j;
	try { f >> j; } //���� ������ JSON ��ü j �� �Ľ�
	catch (std::exception& e) {
		std::cerr << "parse error: " << e.what() << '\n'; //���ܳ��� �����޼���
		return false;
	}

	if (j["version"] != 1) { //version Ȯ�� / 1�� �ƴϸ� �������� �ʴ� ����
		std::cerr << "unsupported version\n";
		return false;
	}

	out.gridWidth = j["gridWidth"];
	out.gridHeight = j["gridHeight"];
	out.tileSize = j["tileSize"];
	out.tiles = j["layers"][0]["data"].get<std::vector<int>>(); //ù���� ���̾��� data �迭�� �޾� ����
	out.entities = j["entities"];
	return true;
}
