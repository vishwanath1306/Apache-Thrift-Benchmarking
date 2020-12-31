local _M = {}

function _M.HelloWorld()
    local GenericObjectPool = require "GenericObjectPool"
    local HelloWorldClient = require "hello_HelloWorld"
    local ngx = ngx
    local client = GenericObjectPool:connection(HelloWorldClient, "echoservice", 3062)
    local status, ret = pcall(client.hello_world, client)
    GenericObjectPool:returnConnection(client)
    
    if not status then
        ngx.status = ngx.HTTP_INTERNAL_SERVER_ERROR
        ngx.say("Nothing works and no one knows why")
        ngx.log("Nothing works and no one knows why")
        ngx.exit(ngx.HTTP_OK);
    else
        ngx.header["Content-Type"] = "text/plain";
        ngx.say(ret.message)
    end
end

return _M