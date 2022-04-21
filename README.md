# Math Lab Readme

## Purpose
This repository is a project I created to refresh my C++ skills.  This project was started around the time that Visual Studio 2022 came out, and I was curious to see how the first 64 bit implementation of Visual Studio compares to g++.

One of the first projects I tackle whenever I want to get a handle on new versions of languages, whether it be Java, Python or C++. I choose this project due fairly well documented methods of parallelization.

In this case it's was to try out some of the changes for the C++20 standard.

I have been working on this since mid November 2021, and decided I might as well put my changes up as I tried things, so that others can hopefully see what to do, or what not to do.

## Wiki

I also will place a Wiki up with both information on how to build, as well as hopefully document how I setup the project.

### Branch naming conventions

Each branch will be named **&lt;Version>.Description** With 00.Template as the initial checkin as well as template build, and master being my most recent version.

The Wiki will have a page describing what occured as each step is made.

Initial Branches

00.Template - Initial checkin.
01.Configuration - Initial project configuration in Visual Studio

## Timings

The following test was done calculating a dot product of two 100,000,000 column vectors run 100 times.

This test isn't designed to be a 

<table>
<tr>
    <th colspan = 2></th>
    <th colspan = 2>Ryzen 5900X</th>
    <th colspan = 2>Ryzen 5900X</th>
    <th colspan = 2>M1 pro</th>
</tr>
<tr>
    <th colspan = 2></th>
    <th colspan = 2>64 GB Memory</th>
    <th colspan = 2>64 GB Memory Linux WSL</th>
    <th colspan = 2>16 GB Memory</th>
</tr>
<tr>
    <th>Item</th><th># Runs</th>
    <th>Total Time</th><th>Average</th>
    <th>Total Time</th><th>Average</th>
    <th>Total Time</th><th>Average</th>
</tr>

<tr><td>vector</td><td>100</td>
    <td style="text-align:right">2000 nansec</td><td style="text-align:right">20 nsec</td>
    <td style="text-align:right">1863 nansec</td><td style="text-align:right">18.630 nsec</td>
    <td style="text-align:right">2000 nansec</td><td style="text-align:right">20 nsec</td>
</tr>
<tr><td>thdvector</td><td>100</td>
    <td style="text-align:right">3.772 seconds</td><td style="text-align:right">37 ms</td>
    <td style="text-align:right">3.931 seconds</td><td style="text-align:right">39 ms</td>
    <td style="text-align:right">1719 ms</td><td style="text-align:right">17 ms</td>
</tr>
<tr><td>lambdavector</td><td>100</td>
    <td style="text-align:right">3.771 seconds</td><td style="text-align:right">37 ms</td>
    <td style="text-align:right">3.902 seconds</td><td style="text-align:right">39 ms</td>
    <td style="text-align:right">1708 ms</td><td style="text-align:right">17 ms</td>
</tr>
<td>ompvector</td><td>100</td>
    <td style="text-align:right">3.714 seconds</td><td style="text-align:right">37 ms</td>
    <td style="text-align:right">3.898 seconds</td><td style="text-align:right">39 ms</td>
    <td style="text-align:right">1721 ms</td><td style="text-align:right">17 ms</td>
</tr>
</table>

## To compile on mac run
```bash
brew install gcc
exit terminal
cd /opt/homebrew/bin
ln -s g++-11 g++
ln -s gcc-11 gcc
brew install libomp
```