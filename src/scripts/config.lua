dir = os.getenv("PWD") or io.open("cd"):read()
home = os.getenv("HOME")
--print(dir)
--print(package.path)

local ok, mod = pcall(require, 'ozone')
if not ok then
  print("Failed")
end

local eok, edit = pcall(require, "editor.editor")
if not eok then
  print("Editor require failed")
  
else
  edit.init();
end
