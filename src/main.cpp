#include <cstring>
#include <iostream>

constexpr const char *DEMOS[] = {"simple", "texture", "transformation",
                                 "camera"};

namespace simple {
extern int main();
}
namespace texture {
extern int main();
}
namespace transformation {
extern int main();
}
namespace camera {
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
  if (std::strcmp(prog, DEMOS[0]) == 0) {
    return simple::main();
  }
  if (std::strcmp(prog, DEMOS[1]) == 0) {
    return texture::main();
  }
  if (std::strcmp(prog, DEMOS[2]) == 0) {
    return transformation::main();
  }
  if (std::strcmp(prog, DEMOS[3]) == 0) {
    return camera::main();
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
