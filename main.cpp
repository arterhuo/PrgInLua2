/**lua�������2ndԴ����
*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

// �򵥵�lua���������������ж���ִ��
void interpreteStdin(lua_State *L)
{
	int a = _MSC_VER;
	char buff[256];
	int error;
	while(fgets(buff, sizeof(buff), stdin)!=NULL)
	{
		error = luaL_loadbuffer(L, buff, strlen(buff), "line") || lua_pcall(L, 0, 0, 0);
		if(error)
		{
			fprintf(stderr, "%s", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}
}

// ��ӡ��ջ
void stackDump(lua_State *L)
{
	int i;
	int top = lua_gettop(L);
	for (i=1; i<=top; i++)
	{
		int t = lua_type(L, i);
		switch(t)
		{
		case LUA_TSTRING:
			{
				printf("'%s'", lua_tostring(L, i));
				break;
			}
		case LUA_TBOOLEAN:
			{
				printf(lua_toboolean(L, i) ? "true" : "false");
				break;
			}
		case LUA_TNUMBER:
			{
				printf("%g", lua_tonumber(L, i));
				break;
			}
		default:
			{
				printf("%s", lua_typename(L, t));
				break;
			}
			printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}

// ���Դ�ӡ��ջ
void stackTest(lua_State *L)
{	
	lua_pushboolean(L, 1);
	lua_pushnumber(L, 10);
	lua_pushnil(L);
	lua_pushstring(L, "hello");
	stackDump(L);

	lua_pushvalue(L, -4);
	stackDump(L);

	lua_replace(L, 3);
	stackDump(L);

	lua_settop(L, 6);
	stackDump(L);

	lua_remove(L, -3);
	stackDump(L);

	lua_settop(L, -5);
	stackDump(L);

}

// c����lua������װ
double f(lua_State *L, double x, double y)
{
	double z;

	lua_getglobal(L, "f");
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);

	if(lua_pcall(L, 2, 1, 0)!=0)
		printf("error running function 'f':%s", lua_tostring(L, -1));

	if(!lua_isnumber(L, -1))
		printf("function 'f' must return a number");
	z = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return z;
}

// c����lua��������
void testCCallLuaFunc(lua_State *L)
{
	if(luaL_loadfile(L, "func.lua") || lua_pcall(L, 0, 0, 0))
		printf("run func.lua error");
	int z = f(L, 3, 2);
	printf("%d\n", z);
}

// ͨ�õ�C����lua����
// ����: call_va("f", "dd>d", x, y, &z)
void call_va(lua_State *L, const char *func, const char *sig, ...)
{
	va_list vl;
	int narg, nres;
	va_start(vl, sig);
	lua_getglobal(L, func);

	// ѹ�����
	bool bEnd = false;
	for(narg=0; *sig; narg++)
	{
		luaL_checkstack(L, 1, "too many arguments");
		switch (*sig++)
		{
		case 'd':
			lua_pushnumber(L, va_arg(vl, double));
			break;
		case 'i':
			lua_pushinteger(L, va_arg(vl, int));
			break;
		case 's':
			lua_pushstring(L, va_arg(vl, char*));
			break;
		case '>':
			bEnd = true;
			break;
		default:
			printf("invalid option (%c)", *(sig-1));
		}
		if(bEnd)
			break;
	}

	nres = strlen(sig);
	if(lua_pcall(L, narg, nres, 0)!=0)
	{
		printf("error calling %s:%s", func, lua_tostring(L, -1));
	}

	// �����
	nres = -nres;
	while(*sig)
	{
		switch(*sig++)
		{
		case 'd':
			if(!lua_isnumber(L, nres))
				printf("wrong result type");
			*va_arg(vl, double*) = lua_tonumber(L, nres);
			break;
		case 'i':
			if(!lua_isnumber(L, nres))
				printf("wrong result type");
			*va_arg(vl, int*) = lua_tointeger(L, nres);
			break;
		case 's':
			if(!lua_isstring(L, nres))
				printf("wrong result type");
			*va_arg(vl, const char**) = lua_tostring(L, nres);
			break;
		default:
			printf("invalid option (%c)", *(sig-1));
		}
		nres++;
	}

	va_end(vl);
}

void testcall_va(lua_State *L)
{
	if(luaL_loadfile(L, "func.lua") || lua_pcall(L, 0, 0, 0))
		printf("run func.lua error");
	double x = 3;
	double y = 2;
	double z;
	call_va(L, "f", "dd>d", x, y, &z);
	printf("%f\n", z);
}

// main
int main()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	//interpreteStdin();
	//stackTest(L);
	//testCCallLuaFunc(L);
	testcall_va(L);

	lua_close(L);
	return 0;
}