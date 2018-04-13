//mongo获取操作处理接口

#ifndef __MONGOWORK_H
#define __MONGOWORK_H

#include "mongonew.h"

void do_work(ReqList *req);
void destroy_cmd(MongoCmd *cmd);
void finish_event(lua_State *L, void *args);

#endif
