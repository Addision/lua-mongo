#include "mongonew.h"
#include "mongothread.h"

void mongo_init()
{
	init_threads();
}

void create_req(MongoCmd *cmd)
{
	ReqList *req = (ReqList *)malloc(sizeof(struct _ReqList));
    req->cmd = cmd;
	push_list(req);
}

MongoCmd* mongo_cmd_init(lua_State *L, mongoc_collection_t *coll)
{
	MongoCmd *cmd = (MongoCmd*)malloc(sizeof(struct _MongoCmd));
	cmd->reqstr = NULL;
	cmd->reqopts = NULL;
    cmd->update = NULL;
	cmd->coll = coll;
	cmd->opcode = OP_NULL;
	cmd->m_L = L;
	cmd->bl = true;
	cmd->flag = 0;
	cmd->ret = 0;
	cmd->dbname = NULL;
	cmd->collname = NULL;

    return cmd;
}
