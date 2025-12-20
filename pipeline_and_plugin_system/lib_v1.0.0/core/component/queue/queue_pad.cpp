#include "queue_pad.hpp"
#include "../../logger/logger.hpp"
namespace ViPlugsEngine {

QueuePad::QueuePad(PadDirection dir) : Pad(dir) { worker = std::thread(&QueuePad::run, this); }

QueuePad::~QueuePad() {
    running = false;
    cv.notify_all();
    if (worker.joinable()) worker.join();
}

void QueuePad::push(BufferShrPtr buf) {
    {
        std::lock_guard<std::mutex> lk(mtx);
        q.push(buf);
        CORE_LOG_DEBUG("[QueuePad::push] {} ", debug_element);
    }
    cv.notify_one();
}

void QueuePad::set_next(std::function<void(BufferShrPtr)> fn) { next = std::move(fn); }

bool QueuePad::accept_caps(const Caps&) { return true; }

void QueuePad::run() {
    CORE_LOG_DEBUG("[QueuePad::run] thread start");
    while (running) {
        BufferShrPtr buf;
        {
            std::unique_lock<std::mutex> lk(mtx);
            cv.wait(lk, [&] { return !q.empty() || !running; });
            if (!running && q.empty()) break;
            buf = q.front();
            q.pop();
        }
        if (next) next(std::move(buf));
    }
}

}  // namespace ViPlugsEngine