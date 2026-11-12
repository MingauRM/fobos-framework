#include "engine.hpp"
const Engine& engine = Engine::GetInstance();

std::string getFileContent(std::filesystem::path scriptPath)
{
    std::ifstream file(scriptPath, std::ios::in | std::ios::binary);
    if (!file)
    {
        std::cerr << "Could not open script file: " << scriptPath << "\n";
        return "";
    }

    std::string scriptContent(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    file.close();

    return scriptContent;
}

std::string compile(std::filesystem::path scriptPath)
{
    Luau::CompileOptions options;
    options.optimizationLevel = 2;
    options.typeInfoLevel = 1;

    std::string bytecode = engine.CompileScript(scriptPath.generic_string(), options);
    GAME_ROOT = scriptPath.parent_path();

    return bytecode;
}

int outputBytecode(std::filesystem::path outputPath, std::string bytecode)
{
    std::ofstream file(outputPath, std::ios::out | std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Could not open output file file: " << outputPath << "\n";
        return 1;
    }

    file << bytecode;
    
    file.close();
    return 0;
}

int main(int argc, char* argv[])
{
    std::string arg1 = argv[1];
    std::string arg2 = argv[2];

    if (!argv[1])
    {
        std::cerr << "Expected an argument.\n";
        return 1;
    }

    if (arg1 == "run")
    {
        if (std::filesystem::path(arg2).has_extension())
        {
            std::string bytecode = compile(arg2);
            return engine.ExecuteBytecode(arg2, bytecode);;
        }
        else
        {
            std::string bytecode = getFileContent(arg2);
            return engine.ExecuteBytecode(arg2, bytecode);;
        }
    }
    else if (arg1 == "setup")
    {
        std::cout << "Setup command not implemented yet.\n";
    }
    else if (arg1 == "compile")
    {
        std::string bytecode = compile(arg2);
        std::filesystem::path scriptNoExtension = std::filesystem::path(arg2).replace_extension("");

        outputBytecode(scriptNoExtension, bytecode);

        return 0;
    }

    return 0;
}