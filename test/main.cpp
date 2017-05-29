#include <stack>
#include <queue>

#include <spdlog/spdlog.h>
#include <bandit/bandit.h>

#include "types/all.h"

#undef CRAFT_TYPE_EXPORTED_MINE
#define CRAFT_TYPE_EXPORTED_MINE CRAFT_TYPE_EXPORTED_EXPORT

using namespace bandit;


// Tell bandit there are tests here.
go_bandit([](){
  describe("base:", [](){
    it("adds things", [&](){
        
      });
  });
});

int main(int argc, char const *argv[]) {
  return bandit::run(argc, (char**)argv);
}
