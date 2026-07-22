#include "lua.h"
#include "lualib.h"
#include "luacode.h"

#include "signal.hpp"

constexpr int ScriptSignalTag = 1;
constexpr int ScriptConnectionTag = 2;

// Helpers

static ScriptSignal* lua_pushsignal(lua_State* L, const ScriptSignal& signal)
{
    ScriptSignal* udata = (ScriptSignal*)lua_newuserdatatagged(L, sizeof(ScriptSignal), ScriptSignalTag);
    new (udata) ScriptSignal(signal);
    return udata;
}

static ScriptSignal* lua_checksignal(lua_State* L, int idx)
{
    ScriptSignal* s = (ScriptSignal*)lua_touserdatatagged(L, idx, ScriptSignalTag);
    if (!s)
        luaL_typeerrorL(L, idx, "ScriptSignal");
    return s;
}

static ScriptConnection* lua_pushconnection(lua_State* L, const ScriptConnection& conn)
{
    ScriptConnection* udata = (ScriptConnection*)lua_newuserdatatagged(L, sizeof(ScriptConnection), ScriptConnectionTag);
    new (udata) ScriptConnection(conn);
    return udata;
}

static ScriptConnection* lua_checkconnection(lua_State* L, int idx)
{
    ScriptConnection* conn = (ScriptConnection*)lua_touserdatatagged(L, idx, ScriptConnectionTag);
    if (!conn)
        luaL_typeerrorL(L, idx, "ScriptConnection");
    return conn;
}

// Library functions

static int signal_create(lua_State* L)
{
    lua_pushsignal(L, ScriptSignal());
    return 1;
}

static int signal_destroy(lua_State* L)
{
    ScriptSignal* signal = lua_checksignal(L, 1);
    auto it = std::find(SignalRegistry.begin(), SignalRegistry.end(), signal);
    SignalRegistry.erase(it);

    signal->~ScriptSignal();

    return 0;
}

static int signal_fire(lua_State* L)
{
    ScriptSignal* signal = lua_checksignal(L, 1);
    
    int n = lua_gettop(L);
    int nArgs = n - 1;

    std::vector<ScriptConnection> connections = signal->GetConnections();

    lua_checkstack(L, nArgs + 1);

    for (size_t i = 0; i < connections.size(); i++)
    {
        int callbackRef = connections.at(i).GetCallback();
        lua_rawgeti(L, LUA_REGISTRYINDEX, callbackRef);

        for (int j = 2; j <= n; j++)
        {
            lua_pushvalue(L, j);
        }

        int status = lua_pcall(L, nArgs, 0, 0);
        if (status != LUA_OK)
        {
            const char* err = lua_tostring(L, -1);
            std::cerr << (err ? err : "Unknown") << "\n";
            lua_pop(L, 1);
        }
    }

    return 0;
}

static int signal_connect(lua_State* L)
{
    ScriptSignal* signal = lua_checksignal(L, 1);
    luaL_checktype(L, 2, LUA_TFUNCTION);
    lua_pushvalue(L, 2);

    int callbackRef = lua_ref(L, -1);
    signal->Connect(callbackRef);

    lua_pushconnection(L, ScriptConnection(signal, callbackRef));

    return 1;
}

static int signal_disconnect(lua_State* L)
{
    ScriptConnection* conn = lua_checkconnection(L, 1);

    conn->GetSignal()->Disconnect(*conn);
    lua_unref(L, conn->GetCallback());

    return 0;
}

static const luaL_Reg signallib[] = {
    {"create", signal_create},
    {"destroy", signal_destroy},
    {"connect", signal_connect},
    {"disconnect", signal_disconnect},
    {"fire", signal_fire},
    {NULL, NULL}
};

int luaopen_signal(lua_State* L)
{
    luaL_register(L, LUA_SIGNALLIBNAME, signallib);
    return 1;
}