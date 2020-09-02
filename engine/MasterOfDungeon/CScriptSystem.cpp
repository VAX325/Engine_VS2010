#include "CScriptSystem.h"

#include "FileSystem.h"
#include "CLogManager.h"

extern "C" void CScriptSystem::init()
{
    using namespace luabind;

    L = lua_open();

    luaopen_base(L);

    open(L);

    module(L)
        [
            class_<CLogManager>("CLogManager")
                .def("LogMsg", &CLogManager::LogMsg)
                .def("LogError", &CLogManager::LogError)
        ];
}

void CScriptSystem::deinit()
{
    lua_close(L);
}

int CScriptSystem::ExecuteFile(char* scriptFile)
{
    luaL_dofile(L, scriptFile);
    return lua_tointeger(L, lua_gettop(L));
}

template<class T, typename T2>
void CScriptSystem::Add(char* name, T pointer, T2 pointer2)
{
    if (pointer == NULL && pointer2 != NULL)
    {
        module(L)
            [
                def(name, pointer2)
            ];
    }
    else
    {
        module(L)
            [
                def(name, pointer)
            ];
    }
}