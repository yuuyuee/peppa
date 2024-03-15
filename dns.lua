-- DNS Parser

local bit = require("bit")
local os = require("os")
local string = require("string")

local function bkdr_hash(s)
  local hash = 0
  for i = 1, #s do
    hash = hash * 131 + string.byte(s, i, i)
    -- hash = bit.tobit(hash * 131 + string.byte(s, i, i))
  end
  return bit.band(hash, 0x7FFFFFFF)
end

return {
  protocol = "998",
  parse = function (strm, dataitem)
    -- assert(#strm ~= 0, "Invalid arguments")
    -- if #strm == 0 then
    --   return
    -- end
    -- print("From Lua: strm: " .. strm)
    -- dataitem["strm"] = strm
    -- dataitem["app_type"] = "123456"

    local b = os.clock() * 1000

    local v = 0
    for i = 1, 10000000 do
      v = bkdr_hash("1234567890")
    end
    print("hash value: " .. v)

    local e = os.clock() * 1000
    print("Cost: " ..  math.floor(e - b) .. "ms")
  end
}