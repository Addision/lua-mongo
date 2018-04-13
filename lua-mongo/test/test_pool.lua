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


--local client = mongo.Client 'mongodb://127.0.0.1'
-- mongo_pool:poolPush(client)

local client = mongo_pool:poolPop()
if not client then
   print("get client error")
end

local sz = mongo_pool:poolSize()
print("size=="..sz)

mongo_pool:poolPush(client)

while true do
   os.execute("sleep " .. n)
end




