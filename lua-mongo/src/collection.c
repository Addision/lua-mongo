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
#include "mongonew.h"
#include "log.h"
#include <sys/select.h>
#include <sys/time.h>

static int m_aggregate(lua_State *L) {
	mongoc_collection_t *collection = checkCollection(L, 1);
	bson_t *pipeline = castBSON(L, 2);
	bson_t *options = toBSON(L, 3);
    
	MongoCmd *cmd = mongo_cmd_init(L, collection);
	cmd->reqstr = pipeline;
	cmd->reqopts = options;
	cmd->opcode = OP_AGGR;
	create_req(cmd);
	//return lum_pause(L, 0, NULL, NULL);
    return 1;
}

static int m_createBulkOperation(lua_State *L) {
	mongoc_collection_t *collection = checkCollection(L, 1);
	bool ordered = lua_isnone(L, 2) || lua_toboolean(L, 2); /* 'true' if omitted */

	MongoCmd *cmd = mongo_cmd_init(L, collection);
	cmd->opcode = OP_BULKOPERA;
	cmd->bl = ordered;
	create_req(cmd);
	//return lum_pause(L, 0, NULL, NULL);
    return 1;
}

static int m_count(lua_State *L) {
	mongoc_collection_t *collection = checkCollection(L, 1);
	bson_t *query = toBSON(L, 2);
	bson_t *options = toBSON(L, 3);
	
	MongoCmd *cmd = mongo_cmd_init(L, collection);
	cmd->reqstr = query;
	cmd->reqopts = options;
	cmd->opcode = OP_COUNT;
	create_req(cmd);
	//return lum_pause(L, 0, NULL, NULL);
    return 1;
}

static int m_drop(lua_State *L) {
	mongoc_collection_t *collection = checkCollection(L, 1);
	bson_t *options = toBSON(L, 2);

	MongoCmd *cmd = mongo_cmd_init(L, collection);
	cmd->reqopts = options;
	cmd->opcode = OP_DROP;
	create_req(cmd);
	//return lum_pause(L, 0, NULL, NULL);
    return 1;
}

static int m_find(lua_State *L) {
	mongoc_collection_t *collection = checkCollection(L, 1);
	bson_t *query = castBSON(L, 2);
	bson_t *options = toBSON(L, 3);

    write_log("create find cmd");
	MongoCmd *cmd = mongo_cmd_init(L, collection);
	cmd->reqstr = query;
	cmd->reqopts = options;
	cmd->opcode = OP_FIND;
	create_req(cmd);

	//return lum_pause(L, 0, NULL, NULL);
    struct timeval delay;
    delay.tv_sec = 0;
    delay.tv_usec = 10000; //10ms
    select(0, NULL,NULL, NULL, &delay);
    return 1;
}

static int m_findAndModify(lua_State *L) {
	mongoc_collection_t *collection = checkCollection(L, 1);
	bson_t *query = castBSON(L, 2);
	bson_t *options = castBSON(L, 3);

    MongoCmd *cmd = (MongoCmd *)mongo_cmd_init(L, collection);
    cmd->reqstr = query;
    cmd->reqopts = options;
    cmd->opcode = OP_FINDMODIFY;
    create_req(cmd);
    //return lum_pause(L, 0, NULL, NULL);
    return 1;
}

static int m_findOne(lua_State *L) {
	mongoc_collection_t *collection = checkCollection(L, 1);
	bson_t *query = castBSON(L, 2);
	bson_t *options = toBSON(L, 3);
	bson_t opts;
	bson_init(&opts);
	if (options) bson_copy_to_excluding_noinit(options, &opts, "limit", "singleBatch", (char *)0);
	BSON_APPEND_INT32(&opts, "limit", 1);
	BSON_APPEND_BOOL(&opts, "singleBatch", true);

    MongoCmd *cmd = mongo_cmd_init(L, collection);
    cmd->reqstr = query;
    cmd->reqopts = &opts;
    cmd->opcode = OP_FINDONE;
    create_req(cmd);
    //return lum_pause(L, 0, NULL, NULL);
        //return nres;
    return 1;
}

static int m_getName(lua_State *L) {
	mongoc_collection_t *collection = checkCollection(L, 1);
	MongoCmd *cmd = mongo_cmd_init(L, collection);
	cmd->opcode = OP_GETNAME;
	create_req(cmd);
	//return lum_pause(L, 0, NULL, NULL);
    return 1;
}

static int m_insert(lua_State *L) {
	mongoc_collection_t *collection = checkCollection(L, 1);
	bson_t *document = castBSON(L, 2);
	int flags = toInsertFlags(L, 3);

	MongoCmd *cmd = mongo_cmd_init(L, collection);
	cmd->reqstr = document;
	cmd->flag = flags;
	cmd->opcode = OP_INSERT;
	create_req(cmd);
	//return lum_pause(L, 0, NULL, NULL);
    struct timeval delay;
    delay.tv_sec = 0;
    delay.tv_usec = 10000; //10ms
    select(0, NULL,NULL, NULL, &delay);
    return 1;
}

static int m_remove(lua_State *L) {
	mongoc_collection_t *collection = checkCollection(L, 1);
	bson_t *query = castBSON(L, 2);
	int flags = toRemoveFlags(L, 3);

	MongoCmd *cmd = mongo_cmd_init(L, collection);
	cmd->opcode = OP_REMOVE;
	cmd->reqstr = query;
	cmd->flag = flags;
	create_req(cmd);
	//return lum_pause(L, 0, NULL, NULL);
    return 1;
}

static int m_rename(lua_State *L) {
	mongoc_collection_t *collection = checkCollection(L, 1);
	const char *dbname = luaL_checkstring(L, 2);
	const char *collname = luaL_checkstring(L, 3);
	bool force = lua_toboolean(L, 4);
	bson_t *options = toBSON(L, 5);

	MongoCmd *cmd = mongo_cmd_init(L, collection);
	cmd->opcode = OP_RENAME;
	cmd->dbname = dbname;
	cmd->collname = collname;
	cmd->bl = force;
	cmd->reqopts = options;
	create_req(cmd);
	//return lum_pause(L, 0, NULL, NULL);
    return 1;
}

static int m_stats(lua_State *L) {
	mongoc_collection_t *collection = checkCollection(L, 1);
	bson_t *options = toBSON(L, 2);

	MongoCmd *cmd = mongo_cmd_init(L, collection);
	cmd->opcode = OP_STATS;
	cmd->reqopts = options;
	create_req(cmd);
	//return lum_pause(L, 0, NULL, NULL);
    return 1;
}

static int m_update(lua_State *L) {
	mongoc_collection_t *collection = checkCollection(L, 1);
	bson_t *query = castBSON(L, 2);
	bson_t *update = castBSON(L, 3);
	int flags = toUpdateFlags(L, 4);

	MongoCmd *cmd = mongo_cmd_init(L, collection);
	cmd->opcode = OP_UPDATE;
	cmd->reqstr = query;
	cmd->update = update;
	cmd->flag = flags;
	create_req(cmd);
	//return lum_pause(L, 0, NULL, NULL);
    return 1;
}

static int m_validate(lua_State *L) {
	mongoc_collection_t *collection = checkCollection(L, 1);
	bson_t *options = toBSON(L, 2);

	MongoCmd *cmd = mongo_cmd_init(L, collection);
	cmd->opcode = OP_VALIDATE;
	cmd->reqopts = options;
	create_req(cmd);
	//return lum_pause(L, 0, NULL, NULL);
    return 1;
}

static int m__gc(lua_State *L) {
	mongoc_collection_t *collection = checkCollection(L, 1);
	if (getHandleMode(L, 1)) return 0; /* Reference handle */
	mongoc_collection_destroy(collection);
	unsetType(L);
	return 0;
}

static const luaL_Reg funcs[] = {
	{ "aggregate", m_aggregate },
	{ "createBulkOperation", m_createBulkOperation },
	{ "count", m_count },
	{ "drop", m_drop },
	{ "find", m_find },
	{ "findAndModify", m_findAndModify },
	{ "findOne", m_findOne },
	{ "getName", m_getName },
	{ "insert", m_insert },
	{ "remove", m_remove },
	{ "rename", m_rename },
	{ "stats", m_stats },
	{ "update", m_update },
	{ "validate", m_validate },
	{ "__gc", m__gc },
	{ 0, 0 }
};

void pushCollection(lua_State *L, mongoc_collection_t *collection, bool ref, int pidx) {
	pushHandle(L, collection, ref ? 1 : -1, pidx);
	setType(L, TYPE_COLLECTION, funcs);
}

mongoc_collection_t *checkCollection(lua_State *L, int idx) {
	return *(mongoc_collection_t **)luaL_checkudata(L, idx, TYPE_COLLECTION);
}
