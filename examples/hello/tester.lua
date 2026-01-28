package.path = package.path .. ';./_install/lib/lua/?.lua'
package.cpath = package.cpath .. ';./_install/lib/lua/?.so'
local helloendec = require("helloendec")
local base64 = require("base64")

local function tbldump(o)
  if type(o) == 'table' then
    local s = '{ '
    local k2
    for k,v in pairs(o) do
      if type(k) ~= 'number' then k2 = '"'..k..'"' end
      s = s .. '['..k..'] = ' .. dump(v) .. ','
    end
    return s .. '} '
  else
    return tostring(o)
  end
end

local function hexdump(str)
  local len = string.len( str )
  local dump = ""
  local hex = ""
  local asc = ""
  
  for i = 1, len do
    if 1 == i % 8 then
      dump = dump .. hex .. asc .. "\n"
      hex = string.format( "%04x: ", i - 1 )
      asc = ""
    end
    
    local ord = string.byte( str, i )
    hex = hex .. string.format( "%02x ", ord )
    if ord >= 32 and ord <= 126 then
      asc = asc .. string.char( ord )
    else
      asc = asc .. "."
    end
  end
  return dump .. hex .. string.rep( "   ", 8 - len % 8 ) .. asc
end

local function test_t1(cb64)
  data = base64.decode(cb64)
  print(hexdump(data))
  local tbl = helloendec.hello_decode(data)
  print("a: ", tbl.a)
  print("world1: ", tbl.world1)
  print("c: ", tbl.world1.c)
  print(hexdump(tbl.world1.data))
  data2 = helloendec.hello_encode(tbl)
  print(hexdump(data2))
end

test_t1("KgAAAGUAAAAEdGVzdA==")
