#include "mongowork.h"
#include "common.h"

void do_work(ReqList *req)
{
    write_log("start to deal cmd...");
    MongoCmd *cmd = req->cmd;
	switch((mongo_op)cmd->opcode)
		{
		case OP_AGGR:
            {
                mongoc_cursor_t * pcursor = mongoc_collection_aggregate(cmd->coll, MONGOC_QUERY_NONE, cmd->reqstr, cmd->reqopts, 0);
                pushCursor(cmd->m_L, pcursor, 1);
            }
			break;
		case OP_BULKOPERA:
            {
                mongoc_bulk_operation_t * pbulkopera = mongoc_collection_create_bulk_operation(cmd->coll, cmd->bl, 0);
                pushBulkOperation(cmd->m_L,pbulkopera, 1); 
            }
			break;
		case OP_COUNT:
            {
                bson_error_t error;
                int64_t n = mongoc_collection_count_with_opts(cmd->coll, MONGOC_QUERY_NONE, cmd->reqstr, 0, 0, cmd->reqopts, 0, &error);
                if (n == -1)
                    commandError(cmd->m_L, &error);
                else
                    pushInt64(cmd->m_L, n);
            }
			break;
		case OP_DROP:
            {
                bson_error_t error;
                commandStatus(cmd->m_L, mongoc_collection_drop_with_opts(cmd->coll, cmd->reqopts, &error), &error);
            }
			break;
		case OP_FIND:
            {
                pushCursor(cmd->m_L, mongoc_collection_find_with_opts(cmd->coll, cmd->reqstr, cmd->reqopts, 0), 1);
            }
			break;
		case OP_FINDMODIFY:
            {
                bson_t reply;
                bson_error_t error;
                int nres = 1;
                mongoc_find_and_modify_opts_t *opts = mongoc_find_and_modify_opts_new();
                mongoc_find_and_modify_opts_append(opts, cmd->reqopts);
                if (!mongoc_collection_find_and_modify_with_opts(cmd->coll, cmd->reqstr, opts, &reply, &error))
                    nres = commandError(cmd->m_L, &error);
                else
                    pushBSONField(cmd->m_L, &reply, "value", false);
                mongoc_find_and_modify_opts_destroy(opts);
                bson_destroy(&reply);
            }
			break;
		case OP_FINDONE:
            {
                mongoc_cursor_t *cursor;
                int nres;
                cursor = mongoc_collection_find_with_opts(cmd->coll, cmd->reqstr, cmd->reqopts, 0);
                nres = iterateCursor(cmd->m_L, cursor, 0);
                mongoc_cursor_destroy(cursor);
                bson_destroy(cmd->reqopts);
            }
			break;
		case OP_GETNAME:
            {
                lua_pushstring(cmd->m_L, mongoc_collection_get_name(cmd->coll));
            }
			break;
		case OP_INSERT:
            {
                bson_error_t error;
                commandStatus(cmd->m_L, mongoc_collection_insert(cmd->coll, cmd->flag, cmd->reqstr, 0, &error), &error);
            }
			break;
		case OP_REMOVE:
            {
                bson_error_t error;
                commandStatus(cmd->m_L, mongoc_collection_remove(cmd->coll, cmd->flag, cmd->reqstr, 0, &error), &error);
            }
			break;
		case OP_RENAME:
            {
                bson_error_t error;
                commandStatus(cmd->m_L, mongoc_collection_rename_with_opts(cmd->coll, cmd->dbname, cmd->collname, cmd->bl, cmd->reqopts, &error), &error);
            }
			break;
		case OP_STATS:
            {
                bson_t reply;
                bson_error_t error;
                commandReply(cmd->m_L, mongoc_collection_stats(cmd->coll, cmd->reqopts, &reply, &error), &reply, &error);
            }
			break;
		case OP_UPDATE:
            {
                bson_error_t error;
                commandStatus(cmd->m_L, mongoc_collection_update(cmd->coll, cmd->flag, cmd->reqstr, cmd->update, 0, &error), &error);
            }
			break;
		case OP_VALIDATE:
            {
                bson_t reply;
                bson_error_t error;
                commandReply(cmd->m_L, mongoc_collection_validate(cmd->coll, cmd->reqopts, &reply, &error), &reply, &error);
            }
			break;
		default:
			write_log("the cmd opcode is error");
			break;
		}

    char str[100];
    sprintf(str, "finish excute cmd:%d", cmd->opcode);
    write_log(str);

    cmd->ret = 1;
    finish_event(cmd->m_L, req);
	//lum_event_add((EVTHANDLER)finish_event, list);
}

void finish_event(lua_State *L, void *args)
{
	ReqList *req = (ReqList*)args;
	if(req->cmd->ret)
		{
			//lum_continue(L, cmd->m_L, 1, 0);
		}
    free(req);
    req = NULL;
}
