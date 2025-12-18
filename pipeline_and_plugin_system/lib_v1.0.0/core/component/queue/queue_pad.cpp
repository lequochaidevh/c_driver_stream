#include "queue_pad.hpp"
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
        std::cout << "[QueuePad::push] sleep_for --- s --- " << debug_element << "\n";
    }
    cv.notify_one();
}

void QueuePad::set_next(std::function<void(BufferShrPtr)> fn) { next = std::move(fn); }

bool QueuePad::accept_caps(const Caps&) { return true; }

void QueuePad::run() {
    std::cout << "[QueuePad::run] thread start\n";
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
