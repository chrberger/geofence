How to contribute to the development of geofence
================================================

Thank you for taking a moment to read these continuously evolving instructions how to contribute to geofence.
To maintain a high quality of our code base, we are very strict in terms of reviewing and accepting pull requests (PR).

## Getting Started
- Make sure you have a GitHub account.
- Make sure you use an up-to-date C++11 compiler; we are usually developing with GNU GCC.
- Register a report about your issue but check beforehand [our issue list](https://github.com/chrberger/geofence/issues) for similar problems.

## Making Changes
- Create a named branch where you want to base your work.
    - We are using the YYYYQN.Z.topic scheme with small letters, where YYYY is the year, N is the current quarter, Z is one of `feature` or `fix`, and Z is the topic. Example: 2020Q2.feature.awesome-feature. This scheme allows us to quickly list and sort branches.
    - Make sure that your topic branched off from master.
- Make changes and commits.

## Coding Guidelines
- Keep the highest possible warning level.
- We treat warnings as errors.
- Keep C++11-only.
- Keep dependency-free.
- Keep platform-independent (i.e., prefer C++ standard library instead of Windows/POSIX dependent API).

