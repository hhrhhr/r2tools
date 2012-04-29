#r2tools

tools for Risen 2

#usage

`r2tool command input output`

* `command`
 - `tab2csv` - convert tab file to CSV (semicolon delimeter)
 - `tab2xml` - convert tab file to XML
 - `img2dds` - convert _ximg file to DDS
* `input`
 - path to input file
* `output`
 - path to output directory

#example

`r2tool tab2xml "d:\games\risen 2\strings\mapinfo.tab" .`

convert mapinfo.tab to mapinfo.xml and write it in current directory
