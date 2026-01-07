# Concurrency and Optimization Projects in C++

This repository contains a set of C++ projects focused on implementing and optimizing programs using different concurrency concepts. Each project explores multiple approaches to parallelism, synchronization, and performance optimization, progressing from basic implementations to more optimized designs.

The goal of this repository is to gain a deeper understanding of multithreading, synchronization primitives, atomic operations, and thread management in C++.

---

## Projects Overview

### P01 – Coin Toss Simulation

A multithreaded coin toss simulation designed to compare different synchronization strategies and their performance impact.

**Concepts explored:**

* Mutex-based synchronization
* Scoped locking and reduced lock contention
* Atomic variables and lock-free updates
* Thread-safe output using `std::osyncstream`
* Performance measurement using `std::chrono`

The project evolves through multiple levels, starting with basic locking and progressing toward a fully atomic-based implementation to reduce overhead.

---

### P02 – Matrix Multiplication

A performance-focused matrix multiplication program implemented using different concurrency models.

**Concepts explored:**

* Work distribution across multiple threads
* Thread pool–based task management
* Tiled (blocked) matrix multiplication for cache efficiency
* Measuring execution time and comparing optimization strategies

The project demonstrates how algorithmic changes combined with proper thread management can significantly improve performance.

---

### P03 – Producer–Consumer Problem

An implementation of the classic producer–consumer problem using modern C++ concurrency tools.

**Concepts explored:**

* `std::mutex` and `std::condition_variable`
* Atomic flags for completion signaling
* Shared task queues
* Thread coordination and synchronization
* Performance comparison between direct threading and thread-pool-based execution

---

## Thread Pool Implementation

The repository includes a custom **thread pool implementation** used across projects.

**Key features:**

* Task queue with condition-variable-based synchronization
* Support for asynchronous task execution using `std::future`
* Tracking of active tasks using atomic counters
* Graceful shutdown and thread cleanup
* Ability to wait until all queued tasks are completed

This implementation is designed for learning purposes and emphasizes correctness, clarity, and practical understanding of thread pool internals.

---

## Purpose

This repository exists to:

* Explore core concurrency patterns in C++
* Understand performance trade-offs between different synchronization mechanisms
* Practice writing safe and efficient multithreaded code
* Build intuition for parallel programming and optimization

---
