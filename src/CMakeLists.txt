add_executable(FobosFramework)
target_include_directories(FobosFramework PRIVATE
    ${SOURCES}
    ${INCLUDES}
)

target_link_libraries(FobosFramework PRIVATE
    Luau.Ast
    Luau.Compiler
    Luau.VM
    Luau.Common
)

set_target_properties(FobosFramework PROPERTIES OUTPUT_NAME fobos)
target_sources(FobosFramework PRIVATE main.cpp)