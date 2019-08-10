# Apertium Anaphora Resolution
## Google Summer of Code 2019

### How to Run
- `./cmake.sh`
- `make -j4`
- `sudo make install`
- Usage: `apertium-anaphora [-z] ref_file.xml`

- Usage in Pipeline:
`apertium-deshtml < input.txt | lt-proc spa-eng.automorf.bin | apertium-tagger -g $2 spa-eng.prob | apertium-pretransfer | lt-proc -b spa-eng.autobil.bin | lrx-proc -m spa-eng.autolex.bin | apertium-anaphora ref_file.xml | apertium-transfer -b apertium-eng-spa.spa-eng.t1x spa-eng.t1x.bin | apertium-interchunk apertium-eng-spa.spa-eng.t2x spa-eng.t2x.bin | apertium-postchunk apertium-eng-spa.spa-eng.t3x spa-eng.t3x.bin | lt-proc -g spa-eng.autogen.bin | lt-proc -p spa-eng.autopgen.bin | apertium-retxt`

### Flags
- -z : Null Flush

### Files
- `src/`: 		C++ Code for Anaphora Module
