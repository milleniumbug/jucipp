#include "utility.h"

ScopeGuard::~ScopeGuard() {
  if(on_exit)
    on_exit();
}
