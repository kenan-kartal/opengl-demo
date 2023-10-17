#include <cstring>
#include <iostream>

constexpr const char *DEMOS[] = {"simple", "texture"};

namespace simple {
extern int main();
}
namespace texture {
extern int main();
}

void print_usage(const char *invoc);

int main(int argc, char *argv[]) {
  const char *invoc = argv[0];
  if (argc != 2) {
    print_usage(invoc);
    return 1;
  }
  const char *prog = argv[1];
  if (std::strcmp(prog, "simple") == 0) {
    return simple::main();
  }
  if (std::strcmp(prog, "texture") == 0) {
    return texture::main();
  }
  print_usage(invoc);
  return 1;
}

void print_usage(const char *invoc) {
  std::cout << invoc << " [";
  for (const auto *demo : DEMOS) {
    if (demo != *std::begin(DEMOS)) {
      std::cout << '|';
    }
    std::cout << demo;
  }
  std::cout << "]\n";
}
