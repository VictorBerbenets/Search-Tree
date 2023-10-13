# Search tree implementation (AVL tree)
### About
This program finds a range of values in a balanced search tree in O(log(n)) complexity,  
thereby overtaking std::distance() function which works for O(n).
## Requirements
**cmake** version must be 3.15 or higher
## How to build
```bash
git clone git@github.com:VictorBerbenets/Search-Tree.git
cd SearchTree/
git submodule init
git submodule update
cmake -S ./ -B build/ -DCMAKE_BUILD_TYPE=Release
cd build/
cmake --build .
```
## To Run the program do
```bash
./search_tree
```
The programm will be waiting for input data in stdin in such way:  
```bash
<k> key1 <k> key2 ... <q> begin end ...
```
**k** - integer key attribute  
**q** - range attribute. After **q** we get the beginning and end of the search range.  
At the end, the program will output a list of distances for each query **q**.
## How to run tests:
### You can run unit tests:
```bash
./tests/unit
```
### Or you can run end2end tests:
```bash
bash ../tests/end2end/test_runner.sh <tests_number> <keys_number> <queries_number>
```
**tests_number** - the number of tests you want to generate.  
**keys_number**  - number of keys to be generated.  
**max_determinant** - number of queries to be generated.  
After running you can see a generated directory - tests/end2end/resources/.  
There will be the tests themselves and the answers to them.
## How to run compare programm:
```bash
bash ../compare/compare.sh
```
This program will output to a file compare/compare.txt comparison of the performance time  
of the distance method in the AVL tree and std::distance().  
Make sure that you have generated end2end tests before, so they are taken for comparison.
