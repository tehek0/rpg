#include "../header/item_requirements.h"

namespace item_requirements {
~item_requirements() {
    for (size_t i = 0; i < item_requirements_ptrs.size(); ++i) {
        delete item_requirements_ptrs[i];
    }
}
}
