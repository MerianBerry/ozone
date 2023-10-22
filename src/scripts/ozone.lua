local M = {
  env = __OZENV,
  fun = function(self, str)
    __ozapi_fun(self.env, str)
  end,
  setstyle = function(self, tab)
    __ozapi_setstyle(self.env, tab)
  end
}

return M;