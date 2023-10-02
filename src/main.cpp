#include <cstring>
#include <iostream>

constexpr const char *DEMOS[] = {"simple"};

namespace simple {
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
    simple::main();
  } else {
    print_usage(invoc);
    return 1;
  }
  return 0;
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
