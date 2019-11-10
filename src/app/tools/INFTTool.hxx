#pragma once

#include "plugins/traits.hxx"

#include <iostream>

class INFTTool {
public:
  virtual bool Run(std::vector<std::string> const &args) = 0;
  virtual ~INFTTool() {}
};

DECLARE_PLUGIN_INTERFACE(INFTTool);
