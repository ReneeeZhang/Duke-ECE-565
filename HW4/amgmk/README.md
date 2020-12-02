Use perf to tell which lines/instructions are taking time by:
1. Compile with –ggdb to enable source code viewing.
2. run sudo perf record ./AMGMk.
3. run sudo perf report.
4. Pick the function you are insterested in.