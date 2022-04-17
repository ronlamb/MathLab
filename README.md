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

**Ryzen 5900X with 64 GB Memory**

<table>
<tr><th>Item</th><th>Total Time</th><th># Runs</th><th>Average</th></th>
<tr><td>vector 100000000</td><td style="text-align:right">2000 nanosec</td><td>100</td><td style="text-align:right">20 nsec</td></tr>
<tr><td>thdvector 100000000</td><td style="text-align:right">3.772 seconds</td><td>100</td><td style="text-align:right">37 ms</td></tr>
<tr><td>lambdavector 100000000</td><td style="text-align:right">3.771 seconds</td><td>100</td><td style="text-align:right">37 ms</td></tr>
<td>ompvector 100000000</td><td style="text-align:right">3.714 seconds</td><td>100</td><td style="text-align:right">37 ms</td></tr>
</table>


**Ryzen 5900X with 64 GB Memory WSL**

<table>
<tr><th>Item</th><th>Total Time</th><th># Runs</th><th>Average</th></th>
<tr><td>vector 100000000</td><td style="text-align:right">1863 nansec</td><td>100</td><td style="text-align:right">18.630 nsec</td></tr>
<tr><td>thdvector 100000000</td><td style="text-align:right">3.931 seconds</td><td>100</td><td style="text-align:right">39 ms</td></tr>
<tr><td>lambdavector 100000000</td><td style="text-align:right">3.902 seconds</td><td>100</td><td style="text-align:right">39 ms</td></tr>
<td>ompvector 100000000</td><td style="text-align:right">3.898 seconds</td><td>100</td><td style="text-align:right">39 ms</td></tr>
</table>

**Macbook 16' M1 Pro with 16 GB Memory**

<table>
<tr><th>Item</th><th>Total Time</th><th># Runs</th><th>Average</th></th>
<tr><td>vector 100000000</td><td style="text-align:right">2000 nansec</td><td>100</td><td style="text-align:right">20 nsec</td></tr>
<tr><td>thdvector 100000000</td><td style="text-align:right">1719 ms</td><td>100</td><td style="text-align:right">17 ms</td></tr>
<tr><td>lambdavector 100000000</td><td style="text-align:right">1708x ms</td><td>100</td><td style="text-align:right">17 ms</td></tr>
<td>ompvector 100000000</td><td style="text-align:right">1721 ms</td><td>100</td><td style="text-align:right">17 ms</td></tr>
</table>

Note: to compile on mac run
brew install gcc
exit terminal
cd /opt/homebrew/bin
ln -s g++-11 g++
ln -s gcc-11 gcc
brew install libomp