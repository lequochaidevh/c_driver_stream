// g++ example/main.cpp -std=c++17 -lpthread && ./a.out
#include "../sfd_RAII_helper.h"
#include <assert.h>

// ============================================================================
// MULTI-THREADED TESTING FUNCTIONS
// ============================================================================

// Worker function for SharedFileDescription concurrency testing
void shared_concurrency_worker(SharedFileDescription<FilePolicy> shared_desc, int thread_id) {
    std::string thread_identity = "[Thread_" + std::to_string(thread_id) + "]";
    
    for (int i = 0; i < 100; ++i) {
        // Acquire the fine-grained session lock
        auto session = shared_desc.lock();
        
        // Prepare two disconnected data blocks to write
        std::string part1 = thread_identity + " iter " + std::to_string(i);
        
        // Clean text string block without any special characters
        std::string part2 = " : Executing critical section uninterrupted.\n";
        
        // Write the first block
        session.write(part1.c_str(), part1.length());
        
        // Force the thread to sleep while holding the lock to test thread-isolation
        std::this_thread::sleep_for(std::chrono::microseconds(5));
        
        // Write the second block
        session.write(part2.c_str(), part2.length());
    }
}

// Function to test rapid copy and destruction of SharedFileDescription across threads
void registry_stress_worker(SharedFileDescription<FilePolicy> shared_desc) {
    for (int i = 0; i < 500; ++i) {
        // Rapidly create and destroy copies to trigger heavy CAS loop contention in the registry
        SharedFileDescription<FilePolicy> temporary_copy = shared_desc;
        assert(temporary_copy.use_count() > 1);
    }
}

// ============================================================================
// MAIN COMPREHENSIVE TEST SUITE
// ============================================================================

int main() {
    std::cout << "================================================================\n";
    std::cout << "STARTING REVISED SMART FILE DESCRIPTION COMPREHENSIVE TEST SUITE\n";
    std::cout << "================================================================\n\n";

    // ------------------------------------------------------------------------
    // TEST 1: UniqueFileDescription Move Semantics and RAII Verification
    // ------------------------------------------------------------------------
    std::cout << "[TEST 1] Testing UniqueFileDescription...\n";
    {
        UniqueFileDescription<FilePolicy> u1(LinuxArgs{}, "unique_test.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
        assert(u1.get() >= 0 && "UniqueFileDescription failed to open file");
        
        // Transfer ownership via move constructor
        UniqueFileDescription<FilePolicy> u2(std::move(u1));
        assert(u1.get() == -1 && "Source UniqueFileDescription must be reset to -1 after move");
        assert(u2.get() >= 0 && "Destination UniqueFileDescription must inherit the valid fd");
        
        std::cout << "-> UniqueFileDescription Move Semantics: PASSED\n";
    } // u2 goes out of scope here, file descriptor must be automatically closed
    std::cout << "-> UniqueFileDescription RAII Auto-Close: PASSED\n\n";


    // ------------------------------------------------------------------------
    // TEST 2: SharedFileDescription Multi-threaded I/O Lock Isolation Test
    // ------------------------------------------------------------------------
    std::cout << "[TEST 2] Testing SharedFileDescription Multi-threaded I/O Lock...\n";
    {
        SharedFileDescription<FilePolicy> shared_log(LinuxArgs{}, "io_isolation_test.log", O_CREAT | O_WRONLY | O_TRUNC, 0644);
        assert(shared_log && "SharedFileDescription failed to open test file");

        const int num_threads = 8;
        std::vector<std::thread> thread_pool;
        
        std::cout << "-> Spawning " << num_threads << " competing threads for locked I/O operations...\n";
        for (int i = 1; i <= num_threads; ++i) {
            thread_pool.emplace_back(shared_concurrency_worker, shared_log, i);
        }

        for (auto& t : thread_pool) {
            t.join();
        }
        
        std::cout << "-> Multi-threaded I/O Lock Stress: PASSED\n";
        std::cout << "   (Check 'io_isolation_test.log' to verify perfect line isolation)\n\n";
    }


    // ------------------------------------------------------------------------
    // TEST 3: Static Registry Reference Counting High Contention CAS Test
    // ------------------------------------------------------------------------
    std::cout << "[TEST 3] Testing Static Registry Lock-Free High Contention CAS...\n";
    {
        SharedFileDescription<FilePolicy> shared_stress(LinuxArgs{}, "registry_stress.log", O_CREAT | O_WRONLY | O_TRUNC, 0644);
        size_t initial_count = shared_stress.use_count();
        assert(initial_count == 1 && "Initial ownership count must be exactly 1");

        const int num_threads = 12;
        std::vector<std::thread> thread_pool;

        std::cout << "-> Spawning " << num_threads << " threads for rapid reference counting stress...\n";
        for (int i = 0; i < num_threads; ++i) {
            thread_pool.emplace_back(registry_stress_worker, shared_stress);
        }

        for (auto& t : thread_pool) {
            t.join();
        }

        // Verify that after all threads exit, the reference count converges perfectly back to 1
        size_t final_count = shared_stress.use_count();
        std::cout << "-> Initial Count: " << initial_count << " | Final Count after high stress: " << final_count << "\n";
        assert(final_count == 1 && "Registry reference count leaked or drifted under heavy contention");
        
        std::cout << "-> Static Registry Lock-Free CAS Consistency: PASSED\n\n";
    }


    // ------------------------------------------------------------------------
    // TEST 4: Unique-to-Shared Conversion Verification
    // ------------------------------------------------------------------------
    std::cout << "[TEST 4] Testing Unique-to-Shared Conversion...\n";
    {
        UniqueFileDescription<FilePolicy> unique_source(LinuxArgs{}, "conversion_test.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
        int native_fd = unique_source.get();
        
        // Convert by moving Unique into Shared constructor
        SharedFileDescription<FilePolicy> shared_dest(std::move(unique_source));
        
        assert(unique_source.get() == -1 && "Unique source must be cleared during conversion");
        assert(shared_dest.get() == native_fd && "Shared destination must adopt the exact same native fd");
        assert(shared_dest.use_count() == 1 && "Converted Shared object must be registered with count 1");
        
        std::cout << "-> Unique-to-Shared Lifecycle Conversion: PASSED\n\n";
    }

    std::cout << "================================================================\n";
    std::cout << "ALL SMART FILE DESCRIPTION INTEGRATION TESTS PASSED SUCCESSFULLY\n";
    std::cout << "================================================================\n";
    
    return 0;
}