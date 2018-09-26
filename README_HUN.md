# Nerd Accessory Number 1 (NaN1) Official Repository
Üdvözöljük a NaN1 hivatalos repository-ján. Itt megtalálod a kezdéshez szükséges dolgokat és néhány példakódot.
For English instructions, open [README.md](README.md).

# Értesítés
A projekt az [STM32 Arduino](https://github.com/rogerclarkmelbourne/Arduino_STM32) alapján készült, manuális telepítéshez keresd fel az oldalukat.

This software is experimental and a work in progress. Under no circumstances should these files be used in relation to any critical system(s). Use of these files is at your own risk.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# Előfeltételek
* NaN1 Alaplap
* Arduino Szoftver

# Telepítés
Az alaplapi fájlok feltelepíthetők az Arduino Alaplap kezelőjével, de ha a lentebb leírt módszer nem működik, próbálja meg a manuális telepítést a fentiek szerint.

* Nyissa meg az Arduino szoftvert
* Kattintson a Fájl menüre és válassza a Beállítások lehetőséget.
* Másolja ki és illessze be a következő linket a További Alaplap kezelő URL-ek mezőjébe (amennyiben nem üres, válassza el vesszővel a többitől):
```
https://raw.githubusercontent.com/balux33/BoardManagerFiles/master/package_Arduino_STM32_balux33_index.json
```
* Kattintson az Eszközök menüre és az válassza ki az Alap-on belül az Alaplap kezelő menüpontot.
* Keresse meg HADDPHI NaN1 Board-ot és kattintson a telepítés gombra.

# Vázlat fordítása
Szokásos módon fordítsa le és töltse fel a vázlatot az Arduino szoftverből.

* Alaplap: HADDPHI NaN1 board
* Variant: STM32F103C8 (20K RAM. 64K Flash)
* CPU Speed: 72MHz

# Szerzők
* [HADDPHI](https://www.facebook.com/haddphi.de.3):
  - Ujvári Balázs
  - Gyöngyösi Balázs
  - Korcsmáros Berta
  - Baranyai Dávid
