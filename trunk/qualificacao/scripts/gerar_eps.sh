#!/bin/bash

for f in *.svg
do
    nome=`echo $f | sed 's/svg/eps/'`
    echo $nome
    inkscape -D -z --file=$f --export-eps=$nome --export-latex
done
