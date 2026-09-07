#include "engine.hpp"

int main(int argc, char* argv[])
{
    Engine& engine = Engine::GetInstance();

    if (!argv[1])
    {
        std::cerr << "The first call argument cannot be NULL." << "\n";
        return 1;
    }

    std::string compilePath(argv[1]);
    std::string bytecode = engine.CompileScript(compilePath);

    GAME_ROOT = std::filesystem::path(compilePath).parent_path();

    int res = engine.ExecuteBytecode(compilePath, bytecode);

    lua_close(engine.GetLuaState());
    return res;
}