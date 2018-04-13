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

static int m__gc(lua_State *L) {
	mongoc_uri_destroy(checkUri(L, 1));
	unsetType(L);
	return 0;
}

static const luaL_Reg funcs[] = {
	{ "__gc", m__gc },
	{ 0, 0 }
};


int newUri(lua_State *L){
	mongoc_uri_t *uri = mongoc_uri_new(luaL_checkstring(L, 1));
	luaL_argcheck(L, uri, 1, "invalid uristr format");
	pushHandle(L, uri, 0, 0);
	setType(L, TYPE_URI, funcs);
	return 1;
}

mongoc_uri_t *checkUri(lua_State *L, int idx){
    return *(mongoc_uri_t **)luaL_checkudata(L, idx, TYPE_URI);
}
