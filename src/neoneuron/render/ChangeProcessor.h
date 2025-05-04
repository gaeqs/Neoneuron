// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#ifndef CHANGEPROCESSOR_H
#define CHANGEPROCESSOR_H

#include <ranges>

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
        std::mutex _mutex;

        hey::Observable<GID> _processFinishedEvent;

      public:
        ChangeProcessor(neon::TaskRunner* runner, std::function<Result(Input input)> processor) :
            _runner(runner),
            _processor(processor)
        {
        }

        void process(GID gid, Input&& input)
        {
            cancel(gid);
            std::lock_guard lock(_mutex);
            auto task = _runner->executeAsync(_processor, std::move(input));
            task->then(false, true, [this, gid] { _processFinishedEvent.invoke(gid); });
            _tasks.insert({gid, task});
        }

        void cancel(GID gid)
        {
            std::lock_guard lock(_mutex);
            auto it = _tasks.find(gid);
            if (it != _tasks.end()) {
                it->second->cancel();
                _tasks.erase(it);
            }
        }

        void cancelAll()
        {
            std::lock_guard lock(_mutex);
            for (auto& task : _tasks | std::views::values) {
                task->cancel();
            }
            _tasks.clear();
        }

        std::vector<std::pair<GID, Result>> getResults()
        {
            std::lock_guard lock(_mutex);

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

        template<typename F>
        void fetchResults(F&& consumer)
        {
            std::lock_guard lock(_mutex);

            for (auto it = _tasks.begin(); it != _tasks.end();) {
                auto& [gid, task] = *it;
                if (auto result = task->moveResult()) {
                    std::invoke(std::forward<F>(consumer), std::move(result.value()));
                    it = _tasks.erase(it);
                } else {
                    ++it;
                }
            }
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
