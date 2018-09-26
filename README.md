# Nerd Accessory Number 1 (NaN1) Official Repository
Welcome to NaN1 official repository. Here you can find how to get started with the board and some example codes.
Magyar leíráshoz nyissa meg a []README_HUN.md(README_HUN.md) fájlt.

# Notice
Our project is based on [STM32 Arduino](https://github.com/rogerclarkmelbourne/Arduino_STM32), for manual installation check their page. 

This software is experimental and a work in progress. Under no circumstances should these files be used in relation to any critical system(s). Use of these files is at your own risk.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# Prerequisities
* NaN1 Board
* Arduino Software

# Installation
You can install the board files with Arduino Board Manager, but if the following steps not working, try manual installation as stated above.

* Open Arduino Software
* Click on Files and open Preferences
* Copy and paste the following link into the Additional Boards Manager URLs field (if it's not empty, separate the URLs with commas):
```
https://raw.githubusercontent.com/balux33/BoardManagerFiles/master/package_Arduino_STM32_balux33_index.json
```
* Click on Tools, then select Boards Manager from Boards menu
* Search for HADDPHI NaN1 Board and select Install

# Building your sketch
Just as with normal Arduino codes, build and flash it from the Arduino Software.

* Board: HADDPHI NaN1 board
* Variant: STM32F103C8 (20K RAM. 64K Flash)
* CPU Speed: 72MHz

# Authors
* [HADDPHI](https://www.facebook.com/haddphi.de.3):
  - Ujvári Balázs
  - Gyöngyösi Balázs
  - Korcsmáros Berta
  - Baranyai Dávid
