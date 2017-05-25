
#include "all.h"

#include <spdlog/spdlog.h>
#include <bandit/bandit.h>
using namespace bandit;

// Tell bandit there are tests here.
go_bandit([](){
  describe("base:", [](){
    it("adds things", [&](){
        int four = craft::cmake::base::add(2, 2);
        AssertThat(four, Equals(4));
      });
  });
});

int main(int argc, char const *argv[]) {
  return bandit::run(argc, (char**)argv);
}
