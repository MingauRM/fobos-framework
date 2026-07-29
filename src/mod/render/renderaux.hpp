#include "lua.h"
#include "lualib.h"
#include "luacode.h"

#include "classes.hpp"

#include <GL/gl.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Constant expressions

constexpr int WindowTag = 3;
constexpr int VertexTag = 4;
constexpr int ColorTag = 5;

// Window

GLFWwindow* lua_checkwindow(lua_State* L, int index)
{
    GLFWwindow* w = (GLFWwindow*)lua_touserdatatagged(L, index, WindowTag);
    if (!w)
        luaL_typeerror(L, index, "Window");
    
    return w;
}

GLFWwindow* lua_pushwindow(lua_State* L, const GLFWwindow* window)
{
    GLFWwindow* udata = (GLFWwindow*)lua_newuserdatatagged(L, sizeof(GLFWwindow*), WindowTag);
    lua_pushlightuserdatatagged(L, udata, WindowTag);
    return udata;
}

// Vertex

Vertex* lua_checkvertex(lua_State* L, int index)
{
    Vertex* v = (Vertex*)lua_touserdatatagged(L, index, VertexTag);
    if (!v)
        luaL_typeerror(L, index, "Vertex");
    
    return v;
}

Vertex* lua_pushvertex(lua_State* L, const Vertex vertex)
{
    Vertex* udata = (Vertex*)lua_newuserdatatagged(L, sizeof(Vertex*), VertexTag);
    new (udata) Vertex(vertex);
    return udata;
}

// Color

Color* lua_checkcolor(lua_State* L, int index)
{
    Color* v = (Color*)lua_touserdatatagged(L, index, ColorTag);
    if (!v)
        luaL_typeerror(L, index, "Color");
    
    return v;
}

Color* lua_pushcolor(lua_State* L, const Color color)
{
    Color* udata = (Color*)lua_newuserdatatagged(L, sizeof(Color*), ColorTag);
    new (udata) Color(color);
    return udata;
}