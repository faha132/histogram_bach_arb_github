# Histogramm_Bach_Arb

This Programm was written for my bachelor thesis in computer science.
It implements a routing scheme for simple histogramms.
Simple Histogramms are polygons, that can be converted into general graphs.
Only a subset of all graphs can be converted to histogramms.
It is written in C++, Files from generated Documentation are also included.

## Authors and acknowledgment
Based on the PHD thesis of Max Willert.

## License
GPL(v3), due to QT being availible with GPLv3 License,
the programm has to be published with GPLv3 as well.

## Project status
finished

## Compiling for Windows users

Download the QT-Framework, select MinGW and QT-Creator packages for installation.
Also download the Boost Library and change the path in CMakefile.txt.
Start QT-Creator and open the project, then press the compile button.
If there are errors, with the newest version of the framework. Make sure to install any optional
Windows Updates.

## Running the programm

The Programm can be run from QT-Creator.
There is a user manual, in german, in "algorithm_describtion/ba_ausarbeitung.pdf".
When running the programm, selecting the "gen_data" option, will take some time,
during which the programm is not responsonding.
