# TPX3Cam
Software for working with TPX3Cam data

bin2root: Converts binary event files (exported with eventExporter) into root format
If there are a large number of exported binary files it may be helpful to concatenate them together to form larger files
e.g. for i in 2024-08-08T171914_00*exported; do cat $i >> 2024-08-08T171914.bin; done
