
#include "plugins/Instantiate.hxx"

#include "app/tools/INFTTool.hxx"

// Run like valgrind --tool=<tool_name>
// Tools:
//  predict-flux
//  crunch-uncertainties
//  place-events
int main(int argc, char const *argv[]) {

  if (std::string(argv[1]).find("--tool=") != 0) {
    std::cout << "[ERROR]: Expected to find tool declaration as the first "
                 "argument, e.g. --tool=Convert."
              << std::endl;
    return 1;
  }

  std::vector<std::string> args;
  for (int i = 0; i < argc; ++i) {
    if (i == 1) {
      continue;
    }
    args.push_back(argv[i]);
  }

  nft::plugins::Instantiate<INFTTool>(&argv[1][7])->Run(args);
}
