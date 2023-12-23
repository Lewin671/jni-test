//
// Created by qingyingliu on 2023/12/22.
//

#include "ResourceItem.h"

void ResourceItem::remove() {
    if (filePath != nullptr) {
        filePath->remove();
    }
}
