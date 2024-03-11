-- DNS Parser

local protocol = '998'

local function Parse(strm, dataitem)
  print('From Lua: strm: ' .. strm)
  dataitem['strm'] = strm
  dataitem['app_type'] = '123456'
end

parsers[protocol] = {
  protocol = protocol,
  Parse = Parse
}