/*
 * Copyright (C) 2016-2017 Arseny Vakhrushev <arseny.vakhrushev@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "common.h"
#include "log.h"

static int m_poolPush(lua_State *L){
    mongoc_client_pool_t *pool = checkClientPool(L, 1); 
    mongoc_client_t *client = checkClient(L, 2);
    mongoc_client_pool_push(pool, client);
    return 1;
}

static int m_poolPop(lua_State *L){
   mongoc_client_pool_t *pool = checkClientPool(L, 1);
   mongoc_client_t *client = mongoc_client_pool_pop(pool);
   luaL_argcheck(L, client, 1, "no client in the pool");
   pushClient(L, client, 1);

   return 1;
}

//if we use this function, need to change mongo-c-driver api
/* static int m_poolSize(lua_State *L){ */
/*     mongoc_client_pool_t *pool = checkClientPool(L, 1); */
/*     lua_Number sz = mongoc_client_pool_get_size(pool); */
/*     lua_pushnumber(L, sz); */
/* 	return 1; */
/* } */

static int m_poolSetMaxSize(lua_State *L){
    mongoc_client_pool_t *pool = checkClientPool(L, 1);
    int max_size = luaL_checkinteger(L,2);
    mongoc_client_pool_max_size(pool, max_size);
	return 1;
}

static int m_poolSetMinSize(lua_State *L){
    mongoc_client_pool_t *pool = checkClientPool(L, 1);
    int min_size = luaL_checkinteger(L,2);
    mongoc_client_pool_min_size(pool, min_size);
    return 1;
}


static int m__gc(lua_State *L) {
    mongoc_client_pool_t *pool = checkClientPool(L, 1);
	if (getHandleMode(L, 1)) return 0; /* Reference handle */
    write_log("destroy mongo pool");
	//mongoc_client_pool_destroy(checkClientPool(L, 1)); //no need to call this function
	unsetType(L);
	return 0;
}

static const luaL_Reg funcs[] = {
	{ "poolPush", m_poolPush },
	{ "poolPop", m_poolPop },
	/* { "poolSize", m_poolSize}, */
	{ "poolMaxSize", m_poolSetMaxSize},
	{ "poolMinSize", m_poolSetMinSize},
	{ "__gc", m__gc },
	{ 0, 0 }
};


int newPool(lua_State *L){
	mongoc_client_pool_t *pool = mongoc_client_pool_new(checkUri(L, 1));
	luaL_argcheck(L, pool, 1, "invalid pool format");
	pushHandle(L, pool, 0, 0);
	setType(L, TYPE_POOL, funcs);
	return 1;
}

mongoc_client_pool_t *checkClientPool(lua_State *L, int idx) {
	return *(mongoc_client_pool_t **)luaL_checkudata(L, idx, TYPE_POOL);
}

