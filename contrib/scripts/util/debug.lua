--提供打印Table的功能

function dump(name, value, indent)
  if value == _G then return end
  local tname = type(name)
  local tvalue = type(value)
  if not indent then indent = 0 end
  if tvalue ~= "table" then
    local s = string.format("%s(%s)%s = (%s)%s", string.rep("  ", indent), tname, tostring(name), tvalue, tostring(value))
   
  else
    local s = string.format("%s(%s)%s = {", string.rep("  ", indent), tname, tostring(name))
    
    for k, v in pairs(value) do
      if k ~= name and v ~= value then
        dump(k, v, indent+1)
      end
    end
    s = string.format("%s}", string.rep("  ", indent))
    
  end
end

function pack(...)
  arg.n = nil
  return arg
end

function repr(obj)
  local tobj = type(obj)
  local s = ""
  if tobj == "string" then
    s = string.format("%q", obj)
  elseif tobj == "number" or tobj == "boolean" then
    s = tostring(obj)
  elseif tobj == "nil" then
    s = "nil"
  elseif tobj == "table" then
    s = s  .. "{"
    for k,v in pairs(obj) do
      s = s .. "[" .. repr(k) .. "] = " .. repr(v) .. ", "
    end
    s = s  .. "}"
  else
    error("can't repr this object.")
  end
  return s
end

function eval(str)
  if type(str) ~= "string" then return end
  local s = string.format("return %s", str)
  return exec(s)
end

function exec(str)
  if type(str) ~= "string" then return end
  return loadstring(str)()
end

--t = {a=1,b=2,c=3,d=4}
--t2 = {1,2,3,4}
--dump("t",t,8)

