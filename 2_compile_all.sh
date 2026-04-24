#!/bin/bash
cd sequence_generator/sources
make
mv sequence_generator ../
cd ../../time_series_generator/sources
make
mv complete_anesthesia ../
cd ../../
