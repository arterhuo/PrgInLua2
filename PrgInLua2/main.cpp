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

#include "header.h"

// main
int main()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	//interpreteStdin(L);	// 24.1
	//stackTest(L);			// 24.2.3
	//testWHOnly(L);		// 25.1
	//testTable(L);			// 25.2
	//testCCallLuaFunc(L);	// 25.3
	//testcall_va(L);		// 25.4
	//testCFunc(L);			// 26.1
	//testMapFunc(L);		// 27.1
	//testSplitFunc(L);		// 27.2
	test28_1(L);


	lua_close(L);
	return 0;
}