This is the project for the Keepit task.

It counts unique words from the input text file.

Usage: cmake + build
and ./UniqueWordsCounter ../sample.txt

where ../sample.txt - path to the input file.

It was implemented with the intent to spend least possible amount of time:)
without any performance measuring.

Please, note: in the requirements it was mentioned that the input file could be up to 32GB.
This program tries to read a small chunks and process it right away to avoid huge memory consumption,
however it depends on the system very highly.

Also, basics like std::thread::hardware_concurrency() and std::thread were used to make it run in parallel which
is not good for prod, but custom thread pool or good concurrent container requires a lot of time for implementation.
