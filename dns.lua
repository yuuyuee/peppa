-- DNS Parser

local ffi = require("ffi")

ffi.cdef[[
  int printf(const char* fmt, ...);
]]

ffi.C.printf("C printf: %s\n", "hello world")

return {
  protocol = "998",
  parse = function (strm, dataitem)
    assert(#strm ~= 0, "Invalid arguments")
    if #strm == 0 then
      return
    end
    print("From Lua: strm: " .. strm)
    dataitem["strm"] = strm
    dataitem["app_type"] = "123456"
  end
}