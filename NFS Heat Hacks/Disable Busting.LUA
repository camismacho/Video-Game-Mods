{$lua}
if syntaxcheck then return end
[ENABLE]
local pattern = "47 61 6D 65 70 6C 61 79 2F 54 77 65 61 6B 61 62 6C 65 73 2F 43 68 61 73 65 5F 44 69 73 61 62 6C 65 42 75 73 74 69 6E 67 00 66 61 6C 73 65"
local replace = "47 61 6D 65 70 6C 61 79 2F 54 77 65 61 6B 61 62 6C 65 73 2F 43 68 61 73 65 5F 44 69 73 61 62 6C 65 42 75 73 74 69 6E 67 00 74 72 75 65 00"
local scans = AOBScan(pattern)
if scans == nil then
  showMessage("Unable to find pattern:\n"..pattern)
else
  local saved = {}
  local length = (#replace + 1) / 3
  for i = 0, scans.Count - 1 do
    local backup = readBytes(scans[i], length, true)
    local bytes = {}
    for hex in string.gmatch(replace, "%S+") do
      local size = #bytes + 1
      if hex == "??" then
        bytes[size] = backup[size]
      else
        bytes[size] = tonumber(hex, 16)
      end
    end
    saved[i] = backup
    writeBytes(scans[i], bytes)
  end
  _G[cheat_name] = {
    ["scans"] = scans,
    ["saved"] = saved
  }
end
[DISABLE]
local vars = _G[cheat_name]
if vars ~= nil then
  local scans = vars.scans
  local saved = vars.saved
  for i = 0, scans.Count - 1 do
    writeBytes(scans[i], saved[i])
  end
  scans.Destroy()
  vars.scans = nil
  vars.saved = nil
  vars = nil
  _G[cheat_name] = nil
end