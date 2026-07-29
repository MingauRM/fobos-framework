#include "lua.h"
#include "lualib.h"
#include "luacode.h"

#include "renderaux.hpp"

#include <GL/gl.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Library functions

static int render_createwindow(lua_State* L)
{
    int width = luaL_checkinteger(L, 1);
    int height = luaL_checkinteger(L, 2);

    const char* title = luaL_checkstring(L, 3);

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    lua_pushwindow(L, window);

    return 1;
}

static int render_clearcolor(lua_State* L)
{
    Color* color = lua_checkcolor(L, 1);

    glClearColor(color->R, color->G, color->B, color->A);
    return 0;
}

static int render_createvertex(lua_State* L)
{
    const float* posVec = luaL_checkvector(L, 1);

    Color* color = lua_checkcolor(L, 2);
    Vec3 position = Vec3(posVec[0], posVec[1], posVec[2]);

    Vertex v = Vertex(position, *color);
    lua_pushvertex(L, v);

    return 1;
}

static int render_drawtriangle(lua_State* L)
{
    Vertex* v1 = lua_checkvertex(L, 1);
    Vertex* v2 = lua_checkvertex(L, 2);
    Vertex* v3 = lua_checkvertex(L, 3);


}