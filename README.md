# Graphics Shadow Mapping

## Overview

This project implements shadow mapping using OpenGL, C++, and C (GLSL shaders) to achieve realistic lighting and shadows in a 3D scene. 
It renders the scene from the light’s perspective, generating a depth map to accurately shade objects and improve visual realism.

## Features

- [x] Realistic Shadow Mapping – Generates shadows based on light sources.
- [x] Depth Map Rendering – Captures scene depth from the light's view.
- [x] Shadow Acne Reduction – Implements bias techniques to minimize artifacts.
- [x] PCF (Percentage-Closer Filtering) Support – Enhances shadow smoothness.
- [x] Optimized Rendering – Ensures efficiency and high performance.

## Technologies Used

- C++ – Core programming language.
- C (GLSL) – Shader programming for vertex and fragment processing.
- OpenGL – Graphics API for rendering.
- GLFW & GLEW – Window and OpenGL extension handling.
- GLM – Math library for vector and matrix operations.
- Microsoft Visual Studio – Development environment.
