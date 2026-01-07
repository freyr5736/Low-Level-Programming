# Low-Level Programming Experiments in C++

This repository contains a collection of low-level C++ implementations and experiments, created to better understand how fundamental systems concepts work beneath higher-level abstractions.
Each file focuses on a specific low-level concept, implemented from scratch or explored in isolation to study behavior, performance implications, and design trade-offs.

---

## Overview

The implementations in this repository explore areas such as:

* Custom memory management
* Data structure internals
* Iterator design
* Object layout and polymorphism
* Compile-time techniques
* Serialization and reflection concepts
---

## Implemented Components

* **Custom Vector Implementation**
  A handcrafted dynamic array to understand memory growth, allocation, and element management.

* **Arena Allocator**
  A fast allocation strategy using pre-allocated memory blocks.

* **Circular Queue**
  Fixed-size queue implementation with wrap-around indexing.

* **CRTP Usage**
  Exploration of the Curiously Recurring Template Pattern for static polymorphism.

* **Market Data Tick Structure**
  A low-level data representation focused on performance and layout.

* **Custom Iterator**
  Iterator implementation to understand traversal and STL compatibility.

* **Reflection and Serialization Concepts**
  Experiments with compile-time and runtime metadata handling.

* **Tag Dispatching**
  Use of tag types to influence overload resolution and behavior.

* **Tracking Allocator** 
  Custom allocator used to monitor memory usage and allocation patterns.

* **Trie Data Structure**
  Tree-based structure optimized for prefix-based lookups.

* **VTable Stripping / Polymorphism Exploration**
  Investigation into virtual tables, object layout, and polymorphic overhead.

---

## Purpose

This repository exists to:

* Strengthen understanding of how C++ works at a low level
* Explore memory, performance, and object layout
* Experiment with language features beyond standard usage
* Build intuition for systems programming and performance-critical code

---
