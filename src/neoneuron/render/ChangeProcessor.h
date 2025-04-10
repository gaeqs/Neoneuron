//
// Created by gaeqs on 10/04/2025.
//

#ifndef CHANGEPROCESSOR_H
#define CHANGEPROCESSOR_H

#include <neon/util/task/TaskRunner.h>
#include <neoneuron/structure/GID.h>
#include <hey/Hey.h>

namespace neoneuron
{

    template<typename Input, typename Result>
    class ChangeProcessor
    {
        neon::TaskRunner* _runner;
        std::unordered_map<GID, std::shared_ptr<neon::Task<Result>>> _tasks;
        std::function<Result(Input input)> _processor;

        hey::Observable<GID> _processFinishedEvent;

      public:
        ChangeProcessor(neon::TaskRunner* runner, std::function<Result(Input input)> processor) :
            _runner(runner),
            _processor(processor)
        {
        }

        void process(GID gid, Input&& input)
        {
            // Search for tasks with the same id.
            auto it = _tasks.find(gid);
            if (it != _tasks.end()) {
                it->second->cancel();
                _tasks.erase(it);
            }

            auto task = _runner->executeAsync(_processor, std::move(input));
            task->then(false, true, [this, gid] { _processFinishedEvent.invoke(gid); });

            _tasks.insert({gid, task});
        }

        std::vector<std::pair<GID, Result>> getResults()
        {
            std::vector<std::pair<GID, Result>> results;

            for (auto it = _tasks.begin(); it != _tasks.end();) {
                auto& [gid, task] = *it;
                if (auto result = task->moveResult()) {
                    result.emplace_back(gid, std::move(result.value()));
                    it = _tasks.erase(it);
                } else {
                    ++it;
                }
            }

            return std::move(results);
        }

        /**
         * Event always invoked on the main thread.
         */
        hey::Observable<GID>& onProcessedEvent()
        {
            return _processFinishedEvent;
        }
    };

} // namespace neoneuron

#endif // CHANGEPROCESSOR_H
