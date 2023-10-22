dir = os.getenv("PWD") or io.open("cd"):read()
home = os.getenv("HOME")
print(dir)
--print(package.path)

local ok, mod = pcall(require, 'ozone')
if not ok then
  error("Failed")
end
mod:fun("yayayy")
mod:setstyle({r = 45, g = 2, b = 32})

local eok, edit = pcall(require, "editor.editor")
if not eok then
  error("Editor require failed")
  
else
  edit.init()
end
