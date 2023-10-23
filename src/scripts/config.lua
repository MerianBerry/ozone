local dir = os.getenv("PWD") or io.open("cd"):read()
local home = os.getenv("HOME")
--print(dir)
--print(package.path)

local ok, oz = pcall(require, 'ozone')
if not ok then
  error("Failed")
end

oz:addplug(require "theme.sulfur")

local eok, edit = pcall(require, "editor.editor")
if not eok then
  error("Editor require failed")
else
  edit.init()
end
