local M = {
  env = __OZENV,
  addplug = function(self, plugin)
    __ozapi_addplug(self.env, plugin)
  end

}

return M
