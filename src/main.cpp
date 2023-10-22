#include <cstring>
#include <iostream>

constexpr const char *DEMOS[] = {"simple", "texture", "transformation",
                                 "coordinates", "camera"};

namespace demo {
namespace simple {
extern int main();
}
namespace texture {
extern int main();
}
namespace transformation {
extern int main();
}
namespace coordinates {
extern int main();
}
namespace camera {
extern int main();
}
} // namespace demo

void print_usage(const char *invoc);

int main(int argc, char *argv[]) {
  const char *invoc = argv[0];
  if (argc != 2) {
    print_usage(invoc);
    return 1;
  }
  const char *prog = argv[1];
  if (std::strcmp(prog, DEMOS[0]) == 0) {
    return demo::simple::main();
  }
  if (std::strcmp(prog, DEMOS[1]) == 0) {
    return demo::texture::main();
  }
  if (std::strcmp(prog, DEMOS[2]) == 0) {
    return demo::transformation::main();
  }
  if (std::strcmp(prog, DEMOS[3]) == 0) {
    return demo::coordinates::main();
  }
  if (std::strcmp(prog, DEMOS[4]) == 0) {
    return demo::camera::main();
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
