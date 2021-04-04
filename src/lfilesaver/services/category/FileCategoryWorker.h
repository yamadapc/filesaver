//
// Created by Pedro Tacla Yamada on 4/4/21.
//

#ifndef FILESAVER_FILECATEGORYWORKER_H
#define FILESAVER_FILECATEGORYWORKER_H

#include "../BackgroundQueueWorker.h"

namespace filesaver::services {

class FileCategoryWorker : BackgroundQueueWorker<std::shared_ptr<Record inMemoryRecord>>
{

};

}

#endif // FILESAVER_FILECATEGORYWORKER_H
