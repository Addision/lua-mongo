local json = require 'json'
local mongo = require 'mongo'
-- local client = mongo.Client 'mongodb://127.0.0.1'
-- local collection = client:getCollection('test2', 'test_haha')
-- if not collection then
--   print("get coll error")
-- else
--   print("get coll ok")
-- end

-- --test find
-- local query1 = mongo.BSON '{ "age" : { "$gt" : 10 }}'
-- for i in collection:find( mongo.BSON '{ "age" : { "$gt" : 10 } }', {sort={ age=1}}):iterator() do
--   print(""..i.age.." "..i.item)
-- end



--test pool
local uri_str = "mongodb://127.0.0.1"
local uri = mongo.Uri(uri_str)
local mongo_pool = mongo.Pool(uri)

if not mongo_pool then
   print("get pool clients error")
else
   print("get pool clients ok")
end

local client = mongo_pool:poolPop();
if not client then
   print("get client error")
end

local collection = client:getCollection('test2', 'test_haha')
if not collection then
  print("get coll error")
else
  print("get coll ok")
end

-- --test find
-- local query1 = mongo.BSON '{ "age" : { "$gt" : 10 }}'
-- for i in collection:find( mongo.BSON '{ "age" : { "$gt" : 10 } }', {sort={ age=1}}):iterator() do
--   print(""..i.age.." "..i.item)
-- end

--test insert
-- local tb = {}
-- tb.age = 105
-- tb.item = "sdsdsaaaaa"
-- collection:insert(json.encode(tb))



--mongo_pool:poolPush(client)
-- local size = mongo_pool:poolSize()
-- print("size=="..size)
-- local tb_client = {}
-- for i=1, 50 do
--    tb_client[i] = mongo_pool:poolPop()
-- end

-- local size2 = mongo_pool:poolSize()
-- print("size2=="..size2)

-- local size3   = mongo_pool:poolSize()
-- print("size3=="..size3)

-- for i=1, 30 do   
--    mongo_pool:poolPush(tb_client[i])
-- end

-- local size4   = mongo_pool:poolSize()
-- print("size4=="..size4)

--test remove
--collection:remove({age=105})

--test update

collection:update({ age = 11 }, { item="AA", age = 11 }, { upsert = true })

os.execute("sleep 1")


   




   





