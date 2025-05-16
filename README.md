# c-linux

## CSCB09 System Programming Projects

This repository contains three system-level tools developed in C for a Linux environment as part of the CSCB09: Software Tools and Systems Programming course. Each assignment progressively builds knowledge of the /proc filesystem, Linux system calls, and real-time process monitoring.

### Assignment 1 – **/proc File Parser & System Info Tool**

**Overview:**
Built a command-line tool that parses the `/proc` filesystem to retrieve and display system information such as:

- Process IDs (PIDs)
- Open file descriptors (FDs)
- File names and inodes
  The tool outputs this information based on provided **CLI flags** and supports positional arguments to filter output.

**Key Features:**

- Parses `/proc/[pid]/fd` and related entries to show how Linux tracks open files
- Custom flag parser for user-defined output
- Designed for modular extension in future assignments

---

### Assignment 2 – **Real-Time System Monitor (memwatch & cpuwatch)**

**Overview:**
Created a real-time system monitor that displays live updates for:

- **Memory utilization** (in GB) with graphical bar output
- **CPU utilization** as a percentage with a dynamic graph
- **CPU core info** including number of cores and maximum frequency
  The tool repeatedly polls `/proc/meminfo` and `/proc/stat` to gather and render this data in a terminal-friendly format.

**Key Features:**

- Text-based **graphical plots** for memory and CPU usage
- Interval-based updates over a set number of periods
- CPU core frequency and count visualization
- Modular CLI for selecting desired stats

---

### Assignment 3 – **Signals, Pipes, and Process Management Enhancements**

**Overview:**
Extended the previous tools to handle advanced Linux system behavior:

- Implemented **pipes** to manage concurrent system metric processes (memory, CPU, core info)
- Added support for **SIGINT (Ctrl+C)** and **SIGTSTP (Ctrl+Z)** to gracefully handle termination and pausing
- Refactored codebase for better modularity and integration across monitoring tools

**Key Features:**

- Forked subprocesses for isolated metric collection
- Inter-process communication using pipes
- Signal handlers to support pause/resume/exit behavior
