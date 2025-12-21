#pragma once
#include "../../unit/pad.hpp"

namespace ViPlugsEngine {
class QueuePad : public Pad {
 public:
    explicit QueuePad(PadDirection dir);
    ~QueuePad();

    void push(BufferShrPtr buf) override;

    void set_next(std::function<void(BufferShrPtr)> fn);

    bool accept_caps(const Caps&) override;

 private:
    void run();

    std::queue<BufferShrPtr> q;
    std::mutex               mtx;
    std::condition_variable  cv;
    std::thread              worker;
    std::atomic<bool>        running{true};

    std::function<void(BufferShrPtr)> next;
    std::size_t                       max_size = 3;

 public:
    std::string debug_element;
};

}  // namespace ViPlugsEngine