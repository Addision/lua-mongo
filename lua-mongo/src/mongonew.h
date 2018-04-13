//slg项目mongo处理接口

#ifndef __MONGO_H
#define __MONGO_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <mongoc.h>
#include <bson.h>
#include "log.h"


struct ReqList;

//define opera cmd
typedef enum {
    OP_AGGR=1,//aggregate
	OP_BULKOPERA=2,//
	OP_COUNT=3,//count
	OP_DROP=4,//drop
	OP_FIND=5,//find
   	OP_FINDMODIFY=6,//findandmodify
   	OP_FINDONE=7,//findone
   	OP_GETNAME=8,//getname
   	OP_INSERT=9,//insert
   	OP_REMOVE=10,//remove
   	OP_RENAME=11,//rename
   	OP_STATS=12,//stats
   	OP_UPDATE=13,//update
   	OP_VALIDATE=14,//validate
	OP_NULL=15,//null
}mongo_op;

typedef struct _MongoCmd{
	bson_t *reqstr;   //请求字符串
    bson_t *reqopts;//请求可选参数
    bson_t *update;
    mongoc_collection_t *coll;
	mongo_op opcode;  //操作指令
	lua_State *m_L;	  //当前用户协程
	bool bl;
    int flag;
    const char *dbname;
    const char *collname;
	int ret;
}MongoCmd;

typedef struct _ReqList{
    MongoCmd *cmd;
	struct _ReqList *next;
}ReqList;

void mongo_init();
void create_req(MongoCmd *cmd);

MongoCmd* mongo_cmd_init(lua_State *L, mongoc_collection_t *coll);
#endif
