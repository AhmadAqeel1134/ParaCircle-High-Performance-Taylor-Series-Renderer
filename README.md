# ParaCircle-High-Performance-Taylor-Series-Renderer
Parallel vs. Scalar Circle Rendering Using Taylor Series
# Overview
This project investigates the performance difference between parallel and scalar implementations of a circle rendering algorithm using the Taylor series approximation. The comparison aims to analyze execution time, efficiency, and accuracy when using multi-threading versus a single-threaded approach.
# Methodology
Approach: The Taylor series expansion is used to approximate a circle's shape. The implementation is tested in two versions: a scalar (single-threaded) approach and a parallelized (multi-threaded) version using OpenMP.
# Tools/Techniques Used:
C++ programming language
OpenMP for parallel processing
SDL2 for graphical rendering
# Performance benchmarking via execution time measurement
Data Sources: The dataset consists of computed values based on the mathematical Taylor series representation of a circle.
# Key Findings
Scalar Implementation: The single-threaded version accurately renders the circle but suffers from longer execution times due to sequential computation.
Parallel Implementation: The multi-threaded approach significantly reduces rendering time by distributing computations across multiple cores while maintaining accuracy.
Performance Analysis: A clear performance gain is observed in the parallel approach, with execution time improvements proportional to the number of threads utilized.
# Conclusion
The study demonstrates the benefits of parallelizing computational tasks, especially in graphics-related applications. The parallel implementation shows a notable reduction in execution time while maintaining accuracy, making it a more efficient solution for real-time rendering applications.
# How to Use This Information
This comparison serves as a benchmark for choosing between single-threaded and multi-threaded implementations in real-time systems.
The findings can be extended to other mathematical approximations and graphical computations.
