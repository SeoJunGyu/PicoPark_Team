#include "stdafx.h"
#include "JsonSerializer.hpp"

bool loadLevel_(const std::string& path, Level& out)
{
    std::ifstream f(path);
    if (!f) { std::cerr << "file open fail\n"; return false; }

    nlohmann::json j;
    try { f >> j; }
    catch (const std::exception& e) {
        std::cerr << "parse error: " << e.what() << '\n';
        return false;
    }

    if (j.value("version", 0) != 1) {           
        std::cerr << "unsupported version\n";
        return false;
    }

    try {                                        
        out = j.get<Level>();                    // from_json(Level) ȣ��
    }
    catch (const std::exception& e) {            // ���� �ʵ� �� ó��
        std::cerr << "schema mismatch: " << e.what() << '\n';
        return false;
    }
    return true;
}
